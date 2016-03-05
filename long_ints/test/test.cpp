#include <iostream>
#include <sstream>
#include <assert.h>
#include <string>
#include <limits>
#include <random>
#include <functional>
#include <algorithm>
#include <vector>

#include "lint.h"

using apa::lint;

// #define RUN_FAILED_COMP_TEST
// #define RUN_ZERO_DIVISION

static inline void test_construction()
{
    lint x;
    const lint y(x);
    x = y;
    x = x;
    (void)test_construction;
}

static inline void test_conversions()
{
    int ordinary = 42;
    lint long_int(ordinary);
    long_int = ordinary;
    ordinary = static_cast<int>(long_int); // ok
    std::string s("-15");
    lint z(s);
    z.to_string() == s;
    (void)test_conversions;
}

static inline void failed_conversions_test()
{
#ifdef RUN_FAILED_COMP_TEST
    int ordinary = 42;
    std::string str = "-42";
    lint x = 5;
    ordinary = x; // must be compilation error!
    x = str; // must be compilation error!
    str = x; // must be compilation error!
#endif
    (void)failed_conversions_test;
}

static inline void test_comparison()
{
    lint long_int1 = 2;
    lint long_int2 = 3.; // constructing from double value
    lint neglint1 = -2;
    lint neglint2 = -13.;
    lint zero = 0;
    lint dzero = 0.0;

    bool check = true;
    // positive comparisons
    check &= long_int1 < 3.14;
    check &= !(long_int1 > 3.14);
    check &= long_int1 > 1;
    check &= !(long_int1 < 1);
    check &= long_int1 == 2;
    check &= 2 == long_int1;
    check &= !(long_int1 != 2);
    check &= !(2 != long_int1);
    check &= 5 > long_int1;
    check &= !(5 < long_int1);
    check &= !(long_int1 == long_int2);
    check &= long_int1 != long_int2;
    check &= 5 >= long_int2;

    // negative comparisons
    check &= neglint1 > neglint2;
    check &= neglint1 >= neglint2;
    check &= !(neglint1 < neglint2);
    check &= !(neglint1 <= neglint2);
    check &= !(neglint1 == neglint2);
    check &= neglint2 < neglint1;
    check &= neglint2 <= neglint1;
    check &= neglint1 != neglint2;

    // comparisons with different signs;
    check &= long_int1 > neglint1;
    check &= long_int1 > neglint2;
    check &= long_int2 > neglint1;
    check &= long_int2 > neglint2;
    check &= neglint1 < long_int1;
    check &= neglint2 < long_int2;
    check &= !(neglint1 == long_int1);
    check &= neglint1 != long_int1;

    // comparisons with zero
    check &= zero == dzero;
    check &= !(zero != dzero);
    check &= zero > neglint1;
    check &= zero < long_int2;
    check &= dzero > neglint2;
    check &= dzero < long_int1;
    check &= dzero != neglint1;
    check &= zero != long_int2;
    assert(check);
    (void)test_comparison;
}

static inline void test_check()
{
    lint value = 0;
    if (value) {
        std::cout << "non zero value" << std::endl;
    }
    (void)test_check;
}

static inline void test_inc_n_dec()
{
    lint x = 42;
    lint y = ++x;
    lint t = 1;
    assert(y == 43 && x == 43);
    //std::cout << "=======" << std::endl;
    y = x--;
    assert(y == 43 && x == 42);
    --t;
    assert(t == 0 && 1 > t && -5 < t);
    --t;
    assert(t == -1 && 2 > t && 0 > t && t > -2);
    (void)test_inc_n_dec;
}

static inline void test_op_n_eq()
{
    lint a = 123456;
    const lint b = 789012;
    lint nega = -123456;
    lint negb = -789012;
    lint c = a * b;
    // checking signs of mult
    assert(nega * a == -(a * a));
    assert(nega * nega == a * a);
    assert(nega * negb == a * b);

    // checking signs of div
    assert(nega / a == -1);
    assert(a / nega == -1);
    assert(negb / nega == 6);
    assert(negb / a == -6);

    // cheking zero
    assert(nega / negb == 0 && nega / negb < 1 && nega / negb > -1);
    assert(a / negb == 0 && a / negb < 1 && a / negb > -1);
    assert(nega / b == 0 && nega / b < 1 && nega / b > -1);

    // default tests
    a *= b;
    assert(a == c);
    assert(a / c == 1);
    assert(c / a == 1);
    a += b;
    assert(a == (lint(123456) + 1) * b);
    c /= a;
    assert(c == 0);
    c -= c;
    assert(c == 0);

    //a /= c; // division by zero
    (void)test_op_n_eq;
}

static inline void test_bin_op()
{
    const lint a = 123456789;
    const lint b = 987654321;
    lint t1 = 1234567890;
    lint t2 = 1234567890;
    lint t3 = 0.;
    assert(t1 - t2 == t3);
    lint c = a * b;
    assert(c == lint(121932631112635269));
    lint d = 3.14 + c;
    c = a * b;
    assert(c == lint(121932631112635269));
    c = a + b;
    assert(c == 1111111110);
    c = c / a;
    assert(c == 9);
    c = c + (-c);
    assert(c == 0);
#ifdef RUN_ZERO_DIVISION
    c = a / c; // division by zero
#endif
    c = +c;
    assert(d); //avoid warning
    (void)test_bin_op;
}

static inline void test_free_func()
{
    apa::lint a = -42;
    bool check = (-a == apa::abs(a));
    apa::lint c = apa::pow(a, 0);
    check = check && (c == 1);
    assert(check);
    assert(apa::pow(a, 1) == -42);
    assert(apa::pow(a, 2) == 1764);
    assert(apa::pow(a, 3) == -74088);
    assert(apa::pow(a, 4) == 3111696);
    assert(apa::pow(2, 100) == lint("1267650600228229401496703205376"));

    // tests on abs(zero)
    assert(abs(0) == abs(0.) && abs(0) < abs(-1) && abs(0) < abs(1));
    (void)test_free_func;
}


static inline void check_io_test(const lint &l, const std::string &s) {
    if (s != l.to_string()) {
        std::cout << "FAIL: " << std::endl;
        std::cout << "expected: " << s << std::endl;
        std::cout << "received: " << l.to_string() << std::endl;
    }
    assert(l.to_string() == s);
}

static inline void test_io()
{
    std::stringstream s;
    const lint a = -123454321;
    s << a;
    lint b;
    s >> b;
    bool check = a == b;
    assert(check);

    std::stringstream s2("+00015");
    lint c;
    s2 >> c;
    assert(c == 15);

    const std::string s3 = "93999999999999993497917866799996609666086101968304347578056871877980872554851570753183154175";
    lint c3{ s3 };
    check_io_test(c3, s3);

    const std::string st = "-123456789";
    lint ct{ st };
    check_io_test(ct, st);

    const std::string s4 = "-100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001";
    lint c5{ s4 };

    c5 += lint(s4);

    const std::string s5 = "-200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002";
    check_io_test(c5, s5);

    std::stringstream s6("-000000000000000001876545678909876");
    lint c6;
    s6 >> c6;
    assert(c6 == -1876545678909876);

    lint minusZero, plusZero;
    std::stringstream s7("-0");
    std::stringstream s8("+0");
    s7 >> minusZero;
    s8 >> plusZero;
    assert(minusZero == plusZero && minusZero >= plusZero && minusZero <= plusZero);
    (void)test_io;
}


static inline void test_round()
{
    lint ten = 10;
    lint nine = 9;
    assert(-ten / nine == -1);
    assert(-ten / ++nine == -1);
    assert(-ten / ++nine == 0);
    (void)test_round;
}

static void test_additional_arith()
{
    lint x( "-100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001" );
    lint a = x;
    lint b( "9999999999999999931398190359470212947659194368" );
    a /= b;
    assert( a == lint( "-10000000000000000068601809640529787522961634227" ) );

    a = lint( "-10000000000000000068601809640529787522961634228" );
    b = lint( "9999999999999999931398190359470212947659194368" );
    lint c = a * b - x;
    assert( c == lint( "-4514337085437500726574968521656929879173627903" ) );
    assert( ( c < b ) == true );


    a = lint( "-10000000000000000000000000000000000000000001" );
    b = lint( "-10000000000000000000000000000000000000000001" );
    c = a / b;
    lint d = a / b;
    lint e = a - b;
    assert( c == 1 );
    assert( d == c );
    assert( e == 0 );


    d = abs( lint( "-56834758349532495723489572945" ) );
    d++;
    ++d;
    d--;
    --d;
    assert( d == lint( "56834758349532495723489572945" ) );


    assert( lint( "-2147483649" ).operator int()   != std::numeric_limits<int>::min() );
    assert( apa::pow( lint( "8584858" ), 5 ) == lint( "46630017666849743823403526775460768" ) );

    ( void )test_additional_arith;
}


typedef std::vector<char> char_array;
char_array charset()
{
    return char_array( {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'} );
}

std::string random_string( size_t length, std::function<char( void )> rand_char )
{
    std::string str( length, 0 );
    std::generate_n( str.begin(), length, rand_char );
    return str;
}

static inline void test_additional_random_strings()
{
    const auto ch_set = charset();
    std::default_random_engine rng( std::random_device {}() );
    std::uniform_int_distribution<> dist( 0, ch_set.size() - 1 );
    auto randchar = [ ch_set, &dist, &rng ]()
    {
        return ch_set[ dist( rng ) ];
    };

    size_t length = 200;
    for( size_t len = 0; len < length; ++len )
    {
        for( size_t n = 0; n < 200; ++n )
        {
            std::string s = random_string( length, randchar );
            if( s[0] != '0' )
            {
                std::string s_min = '-' + s;
                lint c_min {s_min};
                check_io_test( c_min, s_min );

                lint c {s};
                check_io_test( c, s );
            }

        }
    }
}

int main()
{
    test_bin_op();
    test_construction();
    test_conversions();
    failed_conversions_test();
    test_comparison();
    test_check();
    test_inc_n_dec();
    test_op_n_eq();
    test_free_func();
    test_io();

    test_additional_arith();
    test_additional_random_strings();
    return 0;
}
