#include "huffman.h"

node* build_tree(const std::vector<int> &freq_table)
{
    std::priority_queue<node*, std::vector<node*>, comparator> tree;

    for (int i = 0; i < alphabet; ++i)
    {
        if(freq_table[i] != 0)
        {
            tree.push(new leaf(freq_table[i], static_cast<char>(i)));
        }
    }

    // If encoded text contains the only one symbol
    // then add fictious leaf
    if(tree.size() == 1)
    {
        tree.push(new leaf(0, '.'));
    }

    while (tree.size() > 1)
    {
        node* lchild = tree.top();
        tree.pop();

        node* rchild = tree.top();
        tree.pop();

        node* parent = new tree_node(lchild, rchild);
        tree.push(parent);
    }

    return tree.top();
}


void construct_codes(const node* node, const code& prefix, v_codemap &codes)
{
    if (const leaf* lf = dynamic_cast<const leaf*>(node))
    {
        codes[static_cast<unsigned char>(lf->c)] = prefix;
    }
    else if (const tree_node* in = dynamic_cast<const tree_node*>(node))
    {
        code leftPrefix = prefix;
        leftPrefix.push_back(false);
        construct_codes(in->left, leftPrefix, codes);

        code rightPrefix = prefix;
        rightPrefix.push_back(true);
        construct_codes(in->right, rightPrefix, codes);
    }
}


char to_byte(std::vector<bool> &b)
{
    assert(b.size() == 8);
    unsigned char c = 0;
    for (int i = 0; i < 8; ++i)
    {
        if (b[i])
        {
            c |= 1 << i;
        }
    }
    return c;
}


void from_byte(char c, std::vector<bool> &b)
{
    for (int i = 0; i < 8; ++i)
    {
        bool val = (c & (1<<i)) != 0;
        b.push_back(val);
    }
}


std::vector<bool> encode(const v_codemap &codes, const std::string &origin_str)
{
    std::vector<bool> encoded_str;
    for(auto ch_ : origin_str)
    {
        auto symbol_code = codes[static_cast<unsigned char>(ch_)];
        copy(symbol_code.begin(), symbol_code.end(), back_inserter(encoded_str));
    }

    return encoded_str;
}


size_t compress(const char* out_fname, const std::vector<int> &freqtable, std::vector<bool> &encoded_str)
{
    std::ofstream printer(out_fname, std::ios_base::binary);

    // Header of compressed file contains freq. table
    if(encoded_str.size() != 0 && freqtable.size() != 0)
    {
        size_t header_len = freqtable.size();
        printer.write(reinterpret_cast<char*>(&header_len), sizeof(header_len));

        size_t encoded_str_len = encoded_str.size();
        printer.write(reinterpret_cast<char*>(&encoded_str_len), sizeof(encoded_str_len));

        for(auto record: freqtable)
        {
            printer.write(reinterpret_cast<char*>(&record), sizeof(record));
        }
    }

    int len_after_compression = 0;
    for(size_t i = 0; i < encoded_str.size(); i += 8)
    {
        std::vector<bool> next_8_bits{};

        auto k = encoded_str.size() - i;
        if(k >= 8){
            copy(encoded_str.begin() + i, encoded_str.begin() + i + 8, back_inserter(next_8_bits));
        }
        else {
            copy(encoded_str.begin() + i, encoded_str.begin() + i + k, back_inserter(next_8_bits));
            std::fill_n(back_inserter(next_8_bits), 8 - k, 0);
        }


        char ch_ = to_byte(next_8_bits);
        printer.write(&ch_, sizeof(ch_));

        len_after_compression++;
    }

    return len_after_compression;
}


size_t decompress(const char* fname, std::vector<int> &freqtable, std::vector<bool> &decompressed_text)
{

    std::ifstream binloader(fname, std::ios_base::binary);

    size_t num_codes = 0;
    binloader.read(reinterpret_cast<char*>(&num_codes), sizeof(num_codes));

    size_t code_len = 0;
    binloader.read(reinterpret_cast<char*>(&code_len), sizeof(code_len));

    // Reading header
    if(num_codes != 0)
    {
        for(size_t i = 0; i < num_codes; ++i)
        {
            int val = 0;
            binloader.read(reinterpret_cast<char*>(&val), sizeof(val));
            freqtable[i] = val;
        }
    }

    size_t len_before_decompression = -1;
    while (!binloader.eof())
    {
        char ch_ = 0;
        binloader.read(&ch_, sizeof(ch_));
        from_byte(ch_, decompressed_text);
        len_before_decompression++;
    }
    decompressed_text.resize(code_len);


    return len_before_decompression;
}


void decode(const std::vector<bool> &text, const std::vector<int> &freqtable, std::string &decoded_str)
{
    // Build tree from extracted header
    node* root = build_tree(freqtable);
    node* origin = root;

    size_t text_len = text.size();
    size_t i = 0;

    // Walk over tree and decode
    while( i < text_len)
    {

        if(text[i] == 0)
        {
            tree_node* lchild = dynamic_cast<tree_node*>(root);
            root = lchild->left;
        }
        else
        {
            tree_node* rchild = dynamic_cast<tree_node*>(root);
            root = rchild->right;
        }

        // If leaf is reached
        if (const leaf* lf = dynamic_cast<const leaf*>(root))
        {
            decoded_str += lf->c;
            root = origin;
        }
        ++i;
    }

    delete root;
}
