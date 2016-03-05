#ifndef HUFFMAN
#define HUFFMAN

#include <cassert>
#include <vector>
#include <queue>
#include <fstream>
#include <iostream>

#include "auxilarytypes.h"

const int alphabet = 256;

node* build_tree(const std::vector<int> &freq_table);


void construct_codes(const node* node, const code& prefix, v_codemap& codes);


char to_byte(std::vector<bool> &b);


void from_byte(char c, std::vector<bool> &b);


std::vector<bool> encode(const v_codemap &codes, const std::string &origin_str);


size_t compress(const char* out_fname, const std::vector<int> &freqtable, std::vector<bool> &encoded_str);


size_t decompress(const char* fname, std::vector<int> &freqtable, std::vector<bool> &decompressed_text);


void decode(const std::vector<bool> &text, const std::vector<int> &freqtable, std::string &decoded_str);

#endif // HUFFMAN

