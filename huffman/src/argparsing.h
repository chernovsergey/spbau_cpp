#pragma once

#include <cstring>

inline bool args_eq(const char* argument, const char* key)
{
    return strcmp(argument, key) == 0;
}

inline bool args_eq(const char* argument, const char* key1, const char* key2)
{
    return args_eq(argument, key1) || args_eq(argument, key2);
}

inline bool is_enough_args(const char* argument, const char* key1, const char* key2, const int index, const int argc)
{
    return args_eq(argument, key1, key2) && (index != argc - 1);
}

inline bool isnt_enough_args(const char* argument, const char* key1, const char* key2, const int index, const int argc)
{
    return args_eq(argument, key1, key2) && (index == argc - 1);
}
