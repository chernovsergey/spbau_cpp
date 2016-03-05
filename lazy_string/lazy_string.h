#ifndef LAZY_STRING_H
#define LAZY_STRING_H
#include <stdlib.h>
#include <iostream>

#include "shared_buffer.h"

struct lazy_string
{
    lazy_string(const char *src);
    lazy_string(const lazy_string &src);

    bool    empty() const;
    char    get_at(size_t idx) const;
    size_t  get_size() const;
    size_t  find(const char* what, size_t pos) const;

    void           set_at(size_t idx, char c);

    char         operator [](size_t idx) const;
    lazy_string& operator=(lazy_string str);
    lazy_string& operator+=(const lazy_string& str);
    lazy_string  operator+(const lazy_string& str2);

    struct proxy_modifier
    {
        proxy_modifier(lazy_string& s, size_t i)
            : s(s), i(i)
        {}

        proxy_modifier& operator=(char val)
        {
            s.set_at(i, val);
            return *this;
        }

        operator char()
        {
            return s.get_at(i);
        }
    private:

        lazy_string& s;
        size_t i;
    };
    proxy_modifier operator[] (size_t idx)
    {
        return proxy_modifier(*this, idx);
    }

private:
    friend std::ostream& operator<<(std::ostream& os, const lazy_string& str);
    friend lazy_string&  concat(lazy_string& lhs, const lazy_string& rhs);

    shared_buffer buf_;
};

bool        operator<(const lazy_string& str1, const lazy_string& str2);
bool        operator>(const lazy_string& str1, const lazy_string& str2);

#endif // LAZY_STRING_H
