#include "shared_buffer.h"
#include <algorithm>
#include <iostream>

shared_buffer::shared_buffer(size_t size)
    :buff_size(size)
{
    buff_data = new char[size];
    buff_counter = new size_t(1);
}

shared_buffer::shared_buffer(const shared_buffer &src)
{
    copy(src);
}


shared_buffer& shared_buffer::operator=(shared_buffer src)
{
    --(*buff_counter);
    if(*buff_counter == 0)
    {
        delete buff_data;
        delete buff_counter;
    }

    copy(src);
    return *this;
}


shared_buffer::~shared_buffer()
{
    if(*buff_counter == 1)
    {
        delete[] buff_data;
    }
    else
    {
        (*buff_counter)--;
    }
}

void shared_buffer::copy(const shared_buffer& src)
{
    buff_counter = src.buff_counter;
    buff_data = src.buff_data;
    buff_size = src.buff_size;

    ++(*buff_counter);
}

char* shared_buffer::get_data()
{
    return buff_data;
}


const char* shared_buffer::get_data() const
{
    return buff_data;
}


size_t shared_buffer::get_size() const
{
    return buff_size;
}
