#ifndef SHARED_PTR
#define SHARED_PTR

#include<algorithm>
#include <assert.h>

struct shared_ref_counter
{
    shared_ref_counter()
        : ptr_n( nullptr )
    {}

    shared_ref_counter( const shared_ref_counter& rhs )
        : ptr_n( rhs.ptr_n )
    {}

    void swap( shared_ref_counter& lhs )
    {
        std::swap( ptr_n, lhs.ptr_n );
    }

    size_t get_count() const
    {
        size_t count = 0;
        if( ptr_n )
        {
            count = *ptr_n;
        }

        return count;
    }

    template<class U>
    void share( U* p )
    {
        if( p != nullptr )
        {
            if( ptr_n == nullptr )
            {
                try
                {
                    ptr_n = new size_t( 1 );
                }
                catch( std::bad_alloc& )
                {
                    delete p;
                    throw;
                }
            }
            else
            {
                ++( *ptr_n );
            }
        }
    }

    template<class U>
    void release( U* p )
    {
        if( ptr_n != nullptr )
        {
            --( *ptr_n );
            if( 0 == *ptr_n )
            {
                delete p;
                delete ptr_n;
            }
            ptr_n = NULL;
        }
    }

    size_t* ptr_n;
};

template<class T>
struct shared_ptr
{
        using element_type = T;

        shared_ptr()
            : raw_ptr( nullptr )
            , ref_counter()
        {}

        shared_ptr( T* ptr )
            : ref_counter()
        {
            share( ptr );
        }

        template<class U>
        shared_ptr( shared_ptr<U>& ptr, T* p )
            : ref_counter( ptr.ref_counter )
        {
            share( p );
        }

        template <class U>
        shared_ptr( const shared_ptr<U>& ptr )
            : ref_counter( ptr.ref_counter )
        {
            assert( ( NULL == ptr.raw_ptr ) || ( 0 != ptr.ref_counter.get_count() ) );
            share( static_cast<typename shared_ptr<T>::element_type*>( ptr.raw_ptr ) );
        }

        shared_ptr( const shared_ptr& rhs )
            : ref_counter( rhs.ref_counter )
        {
            assert( ( NULL == rhs.raw_ptr ) || ( rhs.ref_counter.get_count() > 0 ) );
            share( rhs.raw_ptr );
        }

        ~shared_ptr()
        {
            release();
        }

        shared_ptr& operator=( shared_ptr rhs )
        {
            swap( rhs );
            return *this;
        }

        void reset( T* p )
        {
            assert( ( NULL == p ) || ( raw_ptr != p ) );
            release();
            share( p );
        }

        void swap( shared_ptr& lhs )
        {
            std::swap( raw_ptr, lhs.raw_ptr );
            ref_counter.swap( lhs.ref_counter );
        }

        size_t get_count()
        {
            return ref_counter.get_count();
        }

        inline T& operator*()  const throw()
        {
            assert( NULL != raw_ptr );
            return *raw_ptr;
        }

        inline T* operator->() const throw()
        {
            assert( NULL != raw_ptr );
            return raw_ptr;
        }

        inline T* get( void )  const throw()
        {
            return raw_ptr;
        }

    private:
        inline void share( T* p )
        {
            ref_counter.share( p );
            raw_ptr = p;
        }

        inline void release( void ) throw()
        {
            ref_counter.release( raw_ptr );
            raw_ptr = NULL;
        }

    private:
        template<class U> friend class shared_ptr;

        T* raw_ptr;
        shared_ref_counter ref_counter;
};

// comparaison operators
template<class T, class U>
inline bool operator>( const shared_ptr<T>& l, const shared_ptr<U>& r ) throw()
{
    return ( l.get() > r.get() );
}

template<class T, class U>
inline bool operator==( const shared_ptr<T>& l, const shared_ptr<U>& r ) throw()
{
    return ( l.get() == r.get() );
}

template<class T, class U>
inline bool operator!=( const shared_ptr<T>& l, const shared_ptr<U>& r ) throw()
{
    return ( l.get() != r.get() );
}

template<class T, class U>
inline bool operator<=( const shared_ptr<T>& l, const shared_ptr<U>& r ) throw()
{
    return ( l.get() <= r.get() );
}

template<class T, class U>
inline bool operator<( const shared_ptr<T>& l, const shared_ptr<U>& r ) throw()
{
    return ( l.get() < r.get() );
}

template<class T, class U>
inline bool operator>=( const shared_ptr<T>& l, const shared_ptr<U>& r ) throw()
{
    return ( l.get() >= r.get() );
}


template<class T, class U>
shared_ptr<T> static_pointer_cast( const shared_ptr<U>& ptr )
{
    return shared_ptr<T>( ptr, static_cast<typename shared_ptr<T>::element_type*>( ptr.get() ) );
}

template<class T, class U>
shared_ptr<T> dynamic_pointer_cast( const shared_ptr<U>& ptr )
{
    T* p = dynamic_cast<typename shared_ptr<T>::element_type*>( ptr.get() );
    if( NULL != p )
    {
        return shared_ptr<T>( ptr, p );
    }
    else
    {
        return shared_ptr<T>();
    }
}

#endif // SHARED_PTR
