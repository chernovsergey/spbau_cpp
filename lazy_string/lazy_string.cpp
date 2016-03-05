#include "lazy_string.h"
#include <cstring>
#include <typeinfo>

lazy_string::lazy_string(const char *src)
    :buf_(std::strlen(src)+1)
{
    memcpy(buf_.get_data(), src, sizeof(char) * (strlen(src)+1));
}


lazy_string::lazy_string(const lazy_string &src)
    :buf_(src.buf_)
{
}


bool lazy_string::empty() const
{
    return buf_.get_size() == 0;
}


size_t lazy_string::get_size() const
{
    return buf_.get_size();
}


char lazy_string::get_at(size_t idx) const
{
    return buf_.get_data()[idx];
}

void lazy_string::set_at(size_t idx, char c)
{
    size_t len = buf_.get_size();
    shared_buffer tmp(buf_);


    buf_ = shared_buffer(len);
    memcpy(buf_.get_data(), tmp.get_data(), sizeof(char) * len);

    buf_.get_data()[idx] = c;
}


size_t lazy_string::find(const char* what, size_t pos = 0) const
{
    size_t len = strlen(what);

    size_t range = get_size() - len + 1;
    for(size_t i = pos; i < range; ++i)
    {
        bool match = true;
        for(size_t j = 0; j < len; ++j)
        {
            match &= this->get_at(i+j) == what[j];
            if(match)
            {
                return j;
            }
        }
    }
    return -1;
}


lazy_string& concat(lazy_string& lhs, const lazy_string& rhs)
{
    size_t lhs_size = lhs.get_size();
    size_t rhs_size = rhs.get_size();
    size_t extended_size = lhs_size + rhs_size;
    shared_buffer extended(extended_size);
    memcpy(extended.get_data(),  lhs.buf_.get_data(), sizeof(char) * lhs_size);
    memcpy(extended.get_data() + lhs_size, rhs.buf_.get_data(), sizeof(char) * rhs_size);
    lhs.buf_ = extended;

    return lhs;
}



std::ostream& operator<<(std::ostream& os, const lazy_string& str) {
    size_t str_size = str.get_size();
    for(size_t i = 0; i < str_size; ++i)
    {
        os << str.get_at(i);
    }
    return os;
}


lazy_string& lazy_string::operator=(const lazy_string str)
{
    buf_ = str.buf_;
    return *this;
}


char lazy_string::operator [](size_t idx) const
{
    return get_at(idx);
}

bool operator < (const lazy_string& str1, const lazy_string& str2)
{
    size_t l_str1 = str1.get_size();
    size_t l_str2 = str2.get_size();

    size_t idx;
    for(idx = 0; idx < std::min(l_str1, l_str2) && str1[idx] == str2[idx]; ++idx){}

    if(idx < std::min(l_str1, l_str2))
    {
        return str1[idx] < str2[idx];
    }

    return false;
}

bool operator > (const lazy_string& str1, const lazy_string& str2)
{
    return !(str1 < str2);
}

lazy_string& lazy_string::operator+=(const lazy_string& rhs)
{
    return concat(*this, rhs);
}

lazy_string lazy_string::operator+(const lazy_string& str2)
{
    return *this += str2;
}
