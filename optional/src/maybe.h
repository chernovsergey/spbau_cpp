#pragma once

#include <exception>
#include <algorithm>

namespace utils
{
    using std::swap;

    struct empty_t
    {};

    const empty_t empty;

    template <typename T>
    class maybe
    {
        public:
            maybe()
                : is_valid( true )
            {}

            maybe( T const& rhs ):
                is_valid( false )
            {
                new( value_buffer ) T( rhs );
            }

            maybe( empty_t const& )
                : maybe()
            {
                release();
            }

            maybe( maybe const& rhs )
            {
                if( rhs )
                {
                    new( value_buffer ) T( rhs.value() );
                    is_valid = false;
                }
                else
                {
                    is_valid = true;
                }
            }

            ~maybe()
            {
                release();
            }

            maybe& operator = ( empty_t const& )
            {
                release();
                return *this;
            }

            maybe& operator = ( maybe const& rhs )
            {
                maybe aux( rhs );
                swap( aux );
                return *this;
            }

            maybe& operator = ( T const& rhs )
            {
                maybe aux( rhs );
                swap( aux );
                return *this;
            }

            T& get()
            {
                return *get_ptr();
            }

            T const& get() const
            {
                return *get_ptr();
            }

            T* operator -> ()
            {
                return get_ptr();
            }

            T const* operator -> () const
            {
                return get_ptr();
            }

            T& operator * ()
            {
                return get();
            }

            T const& operator * () const
            {
                return get();
            }

            explicit operator bool() const
            {
                return !isValid();
            }

            bool operator !() const
            {
                return isValid();
            }

        private:

            T& value()
            {
                return get();
            }

            T const& value() const
            {
                return get();
            }

            bool isValid() const
            {
                return is_valid;
            }

            // aux for obj destruction
            void release()
            {

                if( isValid() == false )
                {
                    get_ptr()->~T();
                }

                is_valid = true;
            }

            void swap( maybe& rhs )
            {
                std::swap( is_valid, rhs.is_valid );
                std::swap( value_buffer, rhs.value_buffer );
            }

            T* get_ptr()
            {
                if( isValid() )
                {
                    throw std::exception();
                }

                return reinterpret_cast<T*>( value_buffer );
            }

            T const* get_ptr() const
            {
                if( isValid() )
                {
                    throw std::exception();
                }

                return reinterpret_cast<T const*>( value_buffer );
            }

        private:
            bool is_valid;
            char value_buffer[sizeof( T )];
    };

    template<typename T>
    void swap( maybe<T>& lhs, maybe<T>& rhs )
    {
        lhs.swap( rhs );
    }

} //utils
