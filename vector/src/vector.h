#pragma once

#include <stdexcept>
#include <string.h>
namespace my
{
    template <typename T>
    struct vector
    {
        private:
            T* array = nullptr;
            size_t reserved_size = 0;
            size_t used = 0;

            T* allocate( size_t size )
            {
                return new T[ size * sizeof( T ) ] ;
            }

            void release_buf( T* mem_ptr, size_t size )
            {
                for( size_t i = 0; i < size; ++i )
                    mem_ptr[i].~T();

                delete[] mem_ptr;
            }

            T* safe_move( T* src, size_t src_size, size_t new_size )
            {
                T* dst = allocate( new_size );
                try
                {
                    for( size_t i = 0; i < src_size; ++i )
                        new( dst + i ) T( std::move_if_noexcept( src[i] ) );
                }
                catch( ... )
                {
                    delete[] dst;
                    throw;
                }
                return dst;
            }

            T* safe_copy( T* src, size_t src_size, size_t new_size )
            {
                T* dst = allocate( new_size );
                try
                {
                    for( size_t i = 0; i < src_size; ++i )
                        new( dst + i ) T( src[i] );
                }
                catch( ... )
                {
                    delete[] dst;
                    throw;
                }
                return dst;
            }

        public:
            explicit vector( size_t capacity_ )
                : array( allocate( capacity_ ) )
                , reserved_size( capacity_ )
                , used( 0 )
            {
                // TODO: strog exception safety brokes here :)
                for( size_t i = 0; i < capacity_; ++i )
                    emplace_back( T() );
                //
            }

            vector()
                : array( allocate( 1 ) )
            {
                reserved_size = 1;
                used = 0;
            }

            vector( const vector<T>& rhs )
                : array( safe_copy( rhs.array, rhs.used, rhs.used ) )
                , reserved_size( rhs.reserved_size )
                , used( rhs.used )
            {}

            vector( vector<T>&& rhs )
                : array( std::move( rhs.array ) )
                , reserved_size( rhs.reserved_size )
                , used( rhs.used )
            {
                rhs.array = nullptr;
                rhs.reserved_size = 0;
                rhs.used = 0;
            }

            void swap( vector<T>& rhs )
            {
                if( this != &rhs )
                {
                    std::swap( array, rhs.array );
                    std::swap( reserved_size, rhs.reserved_size );
                    std::swap( used, rhs.used );
                }
            }

            vector<T>& operator=( vector<T> rhs )
            {
                swap( rhs );
                return *this;
            }

            ~vector()
            {
                release_buf( array, used );
            }

            T& operator[]( size_t pos )
            {
                return array[pos];
            }

            const T& operator[]( size_t pos ) const
            {
                return array[pos];
            }

            T& at( size_t pos )
            {
                if( pos >= size() )
                    throw std::out_of_range( "err" );

                return array[pos];
            }

            const T& at( size_t pos ) const
            {
                if( pos >= size() )
                    throw std::out_of_range( "err" );

                return array[pos];
            }

            size_t size() const
            {
                return  used;
            }

            size_t capacity() const
            {
                return reserved_size;
            }

            void reserve( size_t new_capacity )
            {
                if( new_capacity <= reserved_size )
                    return;

                auto new_buffer = safe_move( array, used, new_capacity );

                release_buf( array, used );
                array = new_buffer;
                reserved_size = new_capacity;
            }

            bool empty() const
            {
                return used == 0;
            }

            void push_back( T const& new_element ) // amortized O(1)
            {
                if( used == reserved_size )
                    reserve( reserved_size * 2 );
                new( array + used ) T( new_element );
                used++;
            }

            void push_back( T&& new_element )
            {
                if( used == reserved_size )
                    reserve( 2 * reserved_size );
                new( array + used ) T( std::move( new_element ) );
                used++;
            }

            template <typename... Args>
            void emplace_back( Args&& ... args )
            {
                push_back( T( std::forward<Args>( args )... ) );
            }

            typedef T* iterator;
            iterator begin()
            {
                return array;
            }

            iterator end()
            {
                return array + used;
            }
    };

    template <typename T>
    void swap( vector<T>& lhs, vector<T>& rhs )
    {
        lhs.swap( rhs );
    }
} // vector
