#ifndef SHARED_BUFFER_H
#define SHARED_BUFFER_H
#include <stdlib.h>

struct shared_buffer
{
    explicit shared_buffer(size_t size);
    shared_buffer(const shared_buffer& src);
    shared_buffer& operator=(shared_buffer src);
    ~shared_buffer();

    char* get_data();
    const char* get_data() const;
    size_t get_size() const;

private:
    size_t* buff_counter;
    char*   buff_data;
    size_t  buff_size;

    void copy(const shared_buffer& src);
};

#endif // SHARED_BUFFER_H
