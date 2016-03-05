#include "io.h"
#include "huffman.h"

std::string input(const char* fname)
{
    std::ifstream binloader(fname, std::ios::in | std::ios_base::binary);
    char byte;
    std::string input_str;
    while (binloader.get(byte))
    {
        input_str += byte;
    }

    return input_str;
}

size_t output(const char* fname, std::string &output_str)
{
    std::fstream dumper(fname, std::fstream::out);
    auto buffer = dumper.rdbuf();
    for(auto ch_ : output_str)
    {
        buffer->sputc(ch_);
    }

    return output_str.size();
}
