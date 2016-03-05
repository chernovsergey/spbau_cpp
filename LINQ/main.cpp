#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <math.h>
#include <algorithm>

template<typename T>
class enumerable
{
        typedef std::vector<T> vecT;

        vecT vec_;
    public:
        enumerable( vecT& vec )
            : vec_( vec )
        {}

        enumerable<T> where( std::function<bool( T )> func ) const
        {
            vecT tmp_vec;

            for( auto & elem : vec_ )
            {
                if( func( elem ) )
                {
                    tmp_vec.push_back( elem );
                }
            }

            return enumerable( tmp_vec );
        }

        template<typename V>
        enumerable<V> select( std::function<V( T )> transform ) const
        {
            std::vector<V> tmp_vec;

            for( auto & elem : vec_ )
            {
                tmp_vec.push_back( transform( elem ) );
            }

            return enumerable<V>( tmp_vec );
        }

        template<typename V>
        enumerable<T> sorted( std::function<V( T )> func )
        {
            std::sort( vec_.begin(), vec_.end(), [&]( T a, T b )
            {
                return func( a ) < func( b );
            } );

            return enumerable<T>( vec_ );
        }

        size_t count()
        {
            return vec_.size();
        }

        size_t count( std::function<bool( T )> predicate )
        {
            size_t cnt = 0;
            for( auto & x : vec_ )
            {
                if( predicate( x ) )
                {
                    cnt++;
                }
            }

            return cnt;
        }

        std::vector<T> toVector()
        {
            return vec_;
        }
};


template<typename T>
enumerable<T> from( std::vector<T> vec )
{
    return enumerable<T>( vec );
}

int main()
{
    using namespace std;

    vector<int> tst_vec( {1, 2, 3, 4, 5} );
    vector<int> result = from( tst_vec )
                         .where( []( int x )->bool {return x < 5;} )
                         .toVector();
    copy( result.begin(), result.end(), ostream_iterator<int>( cout, " " ) );
    cout << endl;



    vector<int> ints_vector = {1, 2, 3};
    vector<double> result_double = from( ints_vector )
                                   .select<double>( []( int i ){return sqrt( ( double )i);})
                                   .toVector();
    copy( result_double.begin(), result_double.end(), ostream_iterator<double>( cout, " " ) );
    cout << endl;



    vector<int> ints = {1, 2, 3, 4, 5};
    auto size = from( ints ).count();
    auto size_predicate = from( ints )
                          .count( []( int x )->bool {return x < 4;} );
    cout << size << " " << size_predicate << endl;



    vector<int> sorted = {5, 4, 3, 2, 1};
    auto result_sorted = from( sorted )
                         .sorted<int>( []( int i ){return sqrt(i);})
                         .toVector();
    copy( result_sorted.begin(), result_sorted.end(), ostream_iterator<int>( cout, " " ) );
    cout << endl;


    return 0;
}
