#pragma once

#include <cstddef>
#include <list>
#include <algorithm>

struct au_allocator
{
        au_allocator( const au_allocator& ) = default;
        au_allocator( au_allocator&& ) = default;
        virtual ~au_allocator() = default;

        explicit au_allocator( size_t max_order = 7 )
        {
            for( int i = max_order; i >= 0; --i )
                blocks.push_front( block_t( i ) );
        }

        void* allocate( size_t size )
        {
            size_t pow = log( size );
            for( auto b : blocks )
                if( pow == b.get_pow() )
                    return b.get();

            return new char[1 << pow];
        }

        template<typename T, class... CONSTR_PARAMS>
        T* allocate( CONSTR_PARAMS&& ... constr_params )
        {
            void* ptr = allocate( sizeof( T ) );
            return new( ptr ) T( std::forward<CONSTR_PARAMS>( constr_params )... ) ;
        }

        void deallocate( void* ptr, size_t size )
        {
            size_t pow = log( size );

            auto has_equal_size = []( std::list<block_t>::iterator a, size_t b )
            {
                return a->get_pow() == b;
            };

            auto last_or_pow_fits = [&]( std::list<block_t>::iterator a, size_t b )
            {
                return std::next( a ) == blocks.end() || ( a->get_pow() < b && b < std::next( a )->get_pow() );
            };

            for( auto block = blocks.begin(); block != blocks.end(); ++block )
            {
                if( has_equal_size( block, pow ) )
                {
                    block->add( ptr );
                    break;
                }
                else if( last_or_pow_fits( block, pow ) )
                {
                    block_t b( pow );
                    b.add( ptr );
                    blocks.insert( std::next( block ), std::move( b ) );
                    break;
                }
            }
        }

        template<class T>
        void deallocate( const T* ptr )
        {
            ptr->~T();
            deallocate( ( void* ) ptr, sizeof( T ) );
        }

    private:
        struct block_t
        {
                block_t( size_t pow, size_t size = 10 )
                    : pow( pow )
                    , size( size )
                {
                    size_t blocksize = 1 << pow;

                    for( size_t i = 0; i < size; ++i )
                        pool.push_front( new char[blocksize] );
                }

                ~block_t()
                {
                    for(auto &it : pool)
                        delete[]( it );
                }

                block_t( block_t&& other ) = default;

                block_t( const block_t& rhs )
                    : block_t( rhs.pow )
                {}

                void add( void* ptr )
                {
                    pool.push_front( ( char* ) ptr );
                    ++size;
                }

                void* get()
                {
                    if( !size )
                    {
                        size_t place_size = 1 << pow;
                        return new char[place_size];
                    }

                    void* ptr = pool.front();
                    pool.pop_front();
                    --size;

                    return ptr;
                }

                size_t get_pow() const
                {
                    return pow;
                }

            private:
                size_t pow;
                size_t size;
                std::list<char*> pool;
        };

        size_t log( size_t size )
        {
            size_t p = 0;
            while( ( size = size >> 1 ) )
                ++p;

            return p + 1;
        }

    private:
        std::list<block_t> blocks;

};

template<typename T>
struct au_stl_allocator : public au_allocator
{
    public:
        using value_type = T;
        using pointer = T* ;
        using reference = T& ;
        using const_pointer = const T* ;
        using const_reference = const T& ;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        template<typename U>
        struct rebind
        {
            using other = au_stl_allocator<U>;
        };

        au_stl_allocator()
            : au_allocator()
        {}

        au_stl_allocator( const au_allocator* alloc )
            : au_allocator( *alloc )
        {}

        template<typename U>
        au_stl_allocator( const au_stl_allocator<U>& rhs )
            : au_allocator( rhs )
        {}

        pointer address( reference x ) const
        {
            return &x;
        }

        const_pointer address( const_reference x ) const
        {
            return &x;
        }

        // hint is rarely used but required. Throws WARNING
        pointer allocate( size_type size, const void* hint = 0 )
        {
            return ( pointer ) au_allocator::allocate( sizeof( value_type ) * size );
        }

        void deallocate( pointer p, size_type n )
        {
            au_allocator::deallocate( p, n );
        }

        size_type max_size() const noexcept
        {
            size_t size = 0;
            for( auto b : blocks )
                size += b.size;

            return size;
        }

        void construct( pointer ptr, const_reference val )
        {
            new( ptr ) value_type( val );
        }

        void destroy( pointer ptr )
        {
            ptr->~value_type();
        }
};
