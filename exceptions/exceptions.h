#ifndef EXCEPTIONS
#define EXCEPTIONS


struct bst_key_exists_exception : std::logic_error
{
    bst_key_exists_exception()
        : std::logic_error( "Key already exists" )
    {}

};

struct bst_key_absent_exception : std::logic_error
{
    bst_key_absent_exception()
        : std::logic_error( "Binary search tree key is absent" )
    {}

};

#endif // EXCEPTIONS

