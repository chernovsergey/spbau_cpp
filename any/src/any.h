#pragma once

#include <typeinfo>

namespace utils
{
    using std::type_info;

    class bad_any_cast: public std::exception
    {
            const std::string message_;

        public:
            explicit bad_any_cast( const std::string message )
                : message_( message )
            {}

            const char* what() const _GLIBCXX_USE_NOEXCEPT
            {
                return message_.c_str();
            }
    };

    class any
    {
            class wrapper_interface
            {
                public:
                    virtual wrapper_interface* get_copy() = 0;
                    virtual const type_info& type() = 0;
                    virtual ~wrapper_interface() {}
            };

            template<typename T>
            class wrapper: public wrapper_interface
            {
                    T object_;

                    template<typename U>
                    friend const U& any_cast( const any& );

                    template<typename U>
                    friend U& any_cast( any& );

                    template<typename U>
                    friend const U* any_cast( const any* );

                    template<typename U>
                    friend U* any_cast( any* );

                public:
                    wrapper( const T& object )
                        : object_( object )
                    {}

                    const type_info& type()
                    {
                        return typeid( T );
                    }

                    wrapper_interface* get_copy()
                    {
                        return new wrapper<T>( object_ );
                    }

            }; // class wrapper

            wrapper_interface* wrapper_;

            template<typename T>
            friend const T& any_cast( const any& );

            template<typename T>
            friend T& any_cast( any& );

            template<typename T>
            friend const T* any_cast( const any* );

            template<typename T>
            friend T* any_cast( any* );


        public:
            any()
                : wrapper_( nullptr )
            {}

            any( const any& rhs )
            {
                if( !rhs.empty() )
                {
                    wrapper_ = rhs.wrapper_->get_copy();
                }
                else
                {
                    wrapper_ = nullptr;
                }
            }

            template<typename T>
            any( const T& rhs )
            {
                wrapper_ = new wrapper<T>( rhs );
            }

            any& operator = ( const any& rhs )
            {
                any( rhs ).swap( *this );
                return *this;
            }

            template<typename T>
            any& operator = ( const T& rhs )
            {
                if( wrapper_ )
                {
                    delete wrapper_;
                }

                wrapper_ = new wrapper<T>( rhs );
                return *this;
            }

            bool empty() const
            {
                return wrapper_ == nullptr;
            }

            any& swap( any& rhs )
            {
                std::swap( wrapper_, rhs.wrapper_ );
                return *this;
            }

            ~any()
            {
                delete wrapper_;
            }

            const type_info& type() const
            {
                if( !empty() )
                {
                    return wrapper_->type();
                }

                return typeid( void );
            }

    }; // class any

    void swap( any& lhs, any& rhs )
    {
        lhs.swap( rhs );
    }


    template<typename T>
    T& any_cast( any& object )
    {
        using std::string;
        using std::remove_reference;

        typedef typename remove_reference<T>::type nonref;
        any::wrapper<nonref>* wrapper = static_cast<any::wrapper<nonref>*>( object.wrapper_ );
        if( wrapper )
        {
            if( object.wrapper_->type() != typeid( T ) )
            {
                throw bad_any_cast( string( "Bad cast to type: " ) + typeid( T ).name() );
            }

            return wrapper->object_;
        }

        throw bad_any_cast( string( "Bad cast to type: " ) + typeid( T ).name() );
    }

    template<typename T>
    T* any_cast( any* object )
    {
        if( object != nullptr && object->wrapper_->type() == typeid( T ) )
        {
            return &( ( static_cast<any::wrapper<T>*>( object->wrapper_ ) )->object_ );
        }

        return nullptr;
    }

    template<typename T>
    const T& any_cast( const any& object )
    {
        return any_cast<typename std::remove_reference<T>::type const&>( const_cast<any&>( object ) );
    }

    template<typename T>
    const T* any_cast( const any* object )
    {
        return any_cast<T>( const_cast<any*>( object ) );
    }

} // namespace utils
