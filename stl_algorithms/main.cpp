#include <iostream>
#include <algorithm>
#include <assert.h>

#include <vector>
#include <set>

#include <functional>
#include <iterator>

using namespace std;


// TASK1
template<typename T, size_t N>
constexpr size_t ARRAY_SIZE( T(& )[N] )
{
    return N;
}

template<typename Iterator>
using ivt = typename std::iterator_traits<Iterator>::value_type;

template<typename fwd_it, typename comp = std::less<ivt<fwd_it>>>
int elem_num_in_sorted( fwd_it begin, fwd_it end, ivt<fwd_it> value, comp cmp = comp() )
{
    auto bounds = std::equal_range( begin, end, value, cmp );
    return distance( bounds.first, bounds.second );
}


// TASK2
template<class Container>
typename Container::iterator set_add( Container& cont, typename Container::value_type obj )
{
    if( binary_search( cont.begin(), cont.end(), obj ) )
    {
        return cont.end();
    }
    else
    {
        return cont.insert( lower_bound( cont.begin(), cont.end(), obj ), obj );
    }
}


// TASK3
template<typename Container, typename Function>
void erase_if( Container& cont, Function predicate )
{
    cont.erase( remove_if( cont.begin(), cont.end(), predicate ), cont.end() );
}


// TASK4
template<typename fwd_it>
void merge_sort( fwd_it begin, fwd_it end )
{
    if( distance( begin, end ) == 1 )
    {
        return;
    }

    fwd_it middle = begin;
    advance( middle, distance( begin, end ) / 2 );
    merge_sort( begin, middle );
    merge_sort( middle, end );
    inplace_merge( begin, middle, end );
}



int main()
{
    int sorted_ar[] = { 1, 2, 3, 4, 5, 6, 7, 7, 7, 8, 9 , 10, 11};
    assert( elem_num_in_sorted( sorted_ar, sorted_ar + ARRAY_SIZE( sorted_ar ), 7 ) == 3 );


    std::vector<int> set;
    assert( *set_add( set, 10 ) == 10 );
    assert( set_add( set, 10 ) == set.end() );
    assert( *set_add( set, 5 ) == 5 );
    assert( ( set[0] == 5 ) && ( set[1] == 10 ) );



    set = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    erase_if( set, []( int val )
    {
        return val > 6;
    } );
    copy( set.begin(), set.end(), ostream_iterator<int>( cout, "," ) );



    std::vector<int> vec =
    {
        10, 9, 4, 345, 54, 2, 4, 12, 4, 2, 34, 34, 56, 4, 67, 3465, 3, 4, 5, 34, 5
    };
    merge_sort( vec.begin(), vec.end() );
    return 0;
}

