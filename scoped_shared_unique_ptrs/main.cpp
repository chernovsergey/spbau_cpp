#include <iostream>
#include "scoped_ptr.h"
#include "unique_ptr.h"
#include "shared_ptr.h"

struct foo
{
    int   field_1;
    float field_2;
};


void bar()
{
    scoped_ptr<foo> foo_ptr( new foo {1, 2} );
    std::cout << ( *foo_ptr ).field_1 << " " << foo_ptr->field_1 << " " << foo_ptr.get() << std::endl;
    if( foo_ptr )
    {
        foo_ptr->field_1 += 1;
    }

    std::cout << foo_ptr->field_1 << std::endl;

    foo_ptr.reset();
    foo_ptr->field_1;
    foo_ptr.reset( new foo {0, 111} );
    scoped_ptr<foo> foo_ptr3();

    // provide useful types
    decltype( foo_ptr )::element_type foo_on_stack {999, 1000};
}


void baz()
{
    unique_ptr<foo> foo_uptr( new foo {666, 777} );

    //+ to scoped_ptr<T>:
    unique_ptr<foo>  foo_uptr2 = std::move( foo_uptr ); // - ok

    foo_uptr2 = std::move( foo_uptr2 ); // - ok

    //    std::cerr << foo_uptr->field_1 << std::endl; // - runtime fail
    //    auto foo_uptr2(foo_uptr); // - compilation fails
}


void fee()
{
    shared_ptr<foo> foo_shptr(new foo {888, 999});
    //+ to unique_ptr:
    auto foo_shptr2 = foo_shptr;
    assert(foo_shptr.get() == foo_shptr2.get());
    std::cout << foo_shptr->field_1 << " "
              << foo_shptr2->field_1 << std::endl;

    shared_ptr<foo> foo_shptr3(foo_shptr2.get()); // don't do that, leads to double delete
}

int main()
{
    bar();

    baz();

    fee();

    return 0;
}

