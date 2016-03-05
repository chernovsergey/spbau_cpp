#ifndef IO
#define IO

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>

#include "auxilarytypes.h"

std::string input(const char* fname);

size_t output(const char* fname, std::string &output_str);

#endif // IO

