#ifndef LAZY_STRING
#define LAZY_STRING

#include <string>
#include <memory>

namespace aux
{

    struct lazy_string_traits : std::char_traits<char>
    {
        static int compare( char const* lhs, char const* rhs, size_t size )
        {
            while( !eq( lhs[size - 1], rhs[size - 1] ) )
                --size;

            if( size == 0 )
                return 0;
            else
                return lt( lhs[size - 1], rhs[size - 1] ) ? -1 : 1;
        }

        static bool eq( char lhs, char rhs )
        {
            return std::tolower( lhs ) == std::tolower( rhs );
        }

        static bool lt( char lhs, char rhs )
        {
            return std::tolower( lhs ) < std::tolower( rhs );
        }
    };

    template<typename T, typename traits_t>
    struct shared_buffer
    {
            shared_buffer( size_t size = 0 )
                : rawptr_( new T[size + 1] )
                , size_( size + 1 )
            {
                assign( size, T() );
            }

            shared_buffer( size_t size, T const& value )
                : shared_buffer( size )
            {
                while( size-- ) assign( size, value );
            }

            shared_buffer( T const* src )
                : shared_buffer( traits_t::length( src ) )
            {
                traits_t::copy( rawptr_, src, size_ );
            }

            shared_buffer( T const* src1, T const* src2 )
                : shared_buffer( traits_t::length( src1 ) +
                                 traits_t::length( src2 ) )
            {
                size_t size1 = traits_t::length( src1 );
                size_t size2 = traits_t::length( src2 );
                traits_t::copy( rawptr_, src1, size1 );
                traits_t::copy( rawptr_ + size1 * sizeof( T ), src2, size2 );
            }

            shared_buffer( shared_buffer const& rhs )
                : shared_buffer( rhs.rawptr_ )
            {}

            ~shared_buffer()
            {
                delete[] rawptr_;
            }

            T& operator[]( size_t pos )
            {
                return rawptr_[pos];
            }

            T const& operator[]( size_t pos ) const
            {
                return rawptr_[pos];
            }

            void assign( size_t pos, T const& value )
            {
                traits_t::assign( rawptr_[pos], value );
            }

            size_t size() const
            {
                return size_ - 1;
            }

        private:
            T* rawptr_;
            size_t size_;
    };


    template <typename str_t>
    struct proxy_modifier
    {
            typedef typename str_t::value_type T;

            proxy_modifier( str_t* str, size_t pos )
                : str_( str )
                , pos_( pos )
            {}

            operator T()
            {
                return str_->c_str()[pos_];
            }

            T& operator=( T const& sym )
            {
                str_->set_at( pos_, sym );
                return const_cast<T*>( str_->c_str() )[pos_];
            }

        private:
            str_t* str_;
            size_t pos_;
    };


    template<typename U, typename T>
    struct comparable
    {
        public:
            friend bool operator==( T const& a, T const& b )
            {
                return !( b < a ) && !( a < b );
            }

            friend bool operator!=( T const& a, T const& b )
            {
                return !( a == b );
            }

            friend bool operator>( T const& a, T const& b )
            {
                return ( b < a );
            }

            friend bool operator>=( T const& a, T const& b )
            {
                return !( a < b );
            }

            friend bool operator<=( T const& a, T const& b )
            {
                return !( b < a );
            }

            friend bool operator< ( U const* a, T const& b )
            {
                return b > a;
            }

            friend bool operator==( U const* a, T const& b )
            {
                return !( b < a ) && !( a < b );
            }

            friend bool operator!=( U const* a, T const& b )
            {
                return !( a == b );
            }

            friend bool operator>( U const* a, T const& b )
            {
                return b < a;
            }

            friend bool operator>=( U const* a, T const& b )
            {
                return !( a < b );
            }

            friend bool operator<=( U const* a, T const& b )
            {
                return !( b < a );
            }
    };


    template<typename U, typename T>
    struct addable
    {
        friend T operator+( T const& lhs, U const* rhs )
        {
            return lhs + T( rhs );
        }

        friend T operator+( U const* lhs, T const& rhs )
        {
            return T( lhs ) + rhs;
        }

        friend T operator+( T const& lhs, U const& rhs )
        {
            return lhs + T( rhs );
        }

        friend T operator+( U const& lhs, T const& rhs )
        {
            return T( lhs ) + rhs;
        }
    };

} // aux

namespace std_utils
{
    using aux::shared_buffer;
    using aux::proxy_modifier;
    using aux::lazy_string_traits;
    using aux::comparable;
    using aux::addable;

    template<typename T, typename traits_t = std::char_traits<char>>
    struct lazy_basic_string;

    typedef lazy_basic_string<char> lazy_string;
    typedef lazy_basic_string<wchar_t, std::char_traits<wchar_t>> lazy_wstring;
    typedef lazy_basic_string<char, aux::lazy_string_traits> lazy_istring;

    template<typename T, typename traits_t>
    struct lazy_basic_string
            : comparable<T, lazy_basic_string<T, traits_t>>
            , addable<T, lazy_basic_string<T, traits_t>>
    {
            typedef T value_type;
            typedef traits_t traits_type;
            typedef size_t size_type;

            typedef T& reference;
            typedef T* pointer;
            typedef T const& const_reference;
            typedef T const* const_pointer;
            typedef T difference_type;

            typedef lazy_basic_string<T, traits_t> base;

            typedef shared_buffer<T, traits_t> buffer_t;
            typedef proxy_modifier<base> proxy_t;

            lazy_basic_string( size_t size, T const& src )
                : buffer_( std::make_shared<buffer_t>( size, src ) )
            {}

            lazy_basic_string()
                : lazy_basic_string( 0, T() )
            {}

            lazy_basic_string( base const& src )
                : buffer_( std::shared_ptr<buffer_t>( src.buffer_ ) )
            {}

            lazy_basic_string( base&& src )
                : buffer_( std::move( src.buffer_ ) )
            {
                src.clear();
            }

            lazy_basic_string( T const* src )
                : buffer_( std::make_shared<buffer_t>( src ) )
            {}

            lazy_basic_string( T const& src )
                : lazy_basic_string( 1, src )
            {}

            base& set_at( size_t pos, T const& value )
            {
                if( !buffer_.unique() )
                {
                    std::unique_ptr<buffer_t> tmp( new buffer_t( *buffer_ ) );
                    tmp->assign( pos, value );
                    buffer_ = std::shared_ptr<buffer_t>( tmp.get() );
                    return *this;
                }

                buffer_->assign( pos, value );
                return *this;
            }

            void swap( base& src )
            {
                std::swap( buffer_, src.buffer_ );
            }

            size_t size() const
            {
                return buffer_->size();
            }

            bool empty() const
            {
                return buffer_->size() == 0;
            }

            void clear()
            {
                buffer_ = std::make_shared<buffer_t>();
            }

            T const* c_str() const
            {
                return &( *buffer_ )[0];
            }

            base& operator=( base rhs )
            {
                if( *this != rhs )
                    swap( rhs );

                return *this;
            }

            base& operator+=( base const& rhs )
            {
                buffer_ = std::make_shared<buffer_t>( c_str(), rhs.c_str() );
                return *this;
            }

            T const& operator[]( size_t pos ) const
            {
                return ( *buffer_ )[pos];
            }

            proxy_t operator[]( size_t pos )
            {
                return proxy_t( this, pos );
            }

            bool operator< ( base const& rhs ) const
            {
                size_t size1 = this->size();
                size_t size2 = rhs.size();

                int tmp = traits_t::compare( c_str(), rhs.c_str(), std::min( size1, size2 ) + 1 );

                if( tmp < 0 )
                    return true;
                else if( tmp == 0 )
                    return size1 < size2;
                else
                    return false;
            }

        private:
            std::shared_ptr<buffer_t> buffer_;
    };

    template <typename T, typename traits_t>
    lazy_basic_string<T, traits_t> operator+( lazy_basic_string<T, traits_t> const& lhs,
            lazy_basic_string<T, traits_t> const& rhs )
    {
        auto tmp = lhs;
        tmp += rhs;
        return tmp;
    }
} // std_utils

#endif // LAZY_STRING
