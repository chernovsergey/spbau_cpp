#include <stdio.h>
#include <string.h>

#include "auxilarytypes.h"
#include "io.h"
#include "huffman.h"
#include "argparsing.h"

int main(int argc, char *argv[])
{
    using namespace std;

    if(argc != 6)
    {
        cout << "Usage: " << argv[0] << " -c|-u -f <input filename> -o <output filename>" << endl;
        return 1;
    }

    bool encode_mode = false;
    bool decode_mode = false;

    char const* input_filename = "";
    char const* output_filename = "";

    for (int i = 1; i < argc; i++)
    {
        char const *argument = argv[i];

        if (args_eq(argument, "-c"))
        {
            encode_mode = true;
        }
        else if (args_eq(argument, "-u"))
        {
            decode_mode = true;
        }
        else if (is_enough_args(argument, "-f", "--file", i, argc))
        {
            i++;
            input_filename = argv[i];
        }
        else if (is_enough_args(argument, "-o", "--output", i, argc))
        {
            i++;
            output_filename = argv[i];
        }
        else if (isnt_enough_args(argument, "-f", "--file", i, argc) ||
                 isnt_enough_args(argument, "-o", "--output", i, argc))
        {
            std::cout << "Not enough arguments for option: " << argv[i];
            return 1;
        }
        else
        {
            std::cout << "Unknown argument: " << argument << std::endl;
            return 1;
        }
    }

    if (encode_mode != decode_mode)
    {

        if(encode_mode)
        {
            string main_text = input(input_filename);


            v_codemap codes(alphabet);
            vector<int> freq_table(alphabet);
            if(main_text != "")
            {
                for(auto sym : main_text)
                {
                    ++freq_table[static_cast<unsigned char>(sym)];
                }

                node* root = build_tree(freq_table);
                construct_codes(root, code(), codes);
                delete root;
            }


            vector<bool> encoded_text = encode(codes, main_text);
            size_t compressed_size = compress(output_filename, freq_table, encoded_text);


            cout << main_text.size() << endl;
            cout << compressed_size << endl;
            cout << freq_table.size()+codes.size() << endl;
        }

        if(decode_mode)
        {
            vector<bool> text;
            vector<int> freq_table_decode(alphabet);
            size_t compressed_size = decompress(input_filename, freq_table_decode, text);


            string decoded_str;
            if(text.size() != 0)
            {
                decode(text, freq_table_decode, decoded_str);
            }


            size_t decompressed_size = output(output_filename, decoded_str);


            cout << compressed_size << endl;
            cout << decompressed_size << endl;
            //freq. table size + number of records in it + encoded string bit length
            cout << freq_table_decode.size()+2*sizeof(size_t) << endl;
        }
    }
    else
    {
        std::cout << "Incompatible arguments: -c -u" << std::endl;
        return 1;
    }

    return 0;
}
