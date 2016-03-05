#include <iostream>
#include <typeinfo>
#include <exception>

#include <cassert>
#include <string>
#include <iostream>

#include "any.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

using utils::any;
using utils::any_cast;
using utils::bad_any_cast;

void contruct_test()
{
    any def;
    any copy_on_type( 42 );

    def = 3.14;
    std::cout << def.type().name() << std::endl;
    def = string( "2.71" );
    std::cout << def.type().name() << std::endl;

    any def_copy( def );
    std::cout << def_copy.type().name() << std::endl;
    def = copy_on_type;
    std::cout << def.type().name() << std::endl;

    any e;
    assert( e.empty() );
}

template<class T>
void check_cast( any& a, bool should_throw )
{
    bool thrown = false;
    try
    {
        double res = any_cast<T>( a );
        std::cout << res << std::endl;
    }
    catch( bad_any_cast const& err )
    {
        thrown = true;
        std::cerr << err.what() << std::endl;
    }

    assert( should_throw == thrown );
}

void retrieve_value_test()
{
    any ia( 42 );

    auto res = any_cast<double*>( &ia );
    assert( res == nullptr );

    check_cast<double>( ia, true );
    check_cast<int>( ia, false );
}

void swap_test( any& a, any& b )
{
    swap( a, b );
    std::cout << a.type().name() << " " << b.type().name() << std::endl;
}

int main()
{
    contruct_test();
    retrieve_value_test();

    any a( 5 ), b( string( "6" ) );
    swap_test( a, b );

    any empty;
    any str( std::string( "foobar" ) );
    empty = str;
    ( void ) any_cast<string&>( empty );

    return 0;
}
