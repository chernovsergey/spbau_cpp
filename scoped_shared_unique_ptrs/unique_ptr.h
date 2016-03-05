#ifndef UNIQUE_PTR
#define UNIQUE_PTR

template<class T>
struct unique_ptr
{
        unique_ptr( unique_ptr& rhs ) = delete;
        unique_ptr& operator=( unique_ptr& rhs ) = delete;

        unique_ptr( T* obj_ptr )
            : ptr_( obj_ptr )
        {}

        unique_ptr( unique_ptr&& rhs ):
            ptr_( std::move( rhs.ptr_ ) )
        {
            rhs.ptr_ = nullptr;
        }

        unique_ptr& operator=( unique_ptr && rhs )
        {
            delete ptr_;
            ptr_ = std::move( rhs.ptr_ );

            rhs.ptr_ = nullptr;
            return *this;
        }

        ~unique_ptr()
        {
            if( *this )
            {
                delete ptr_;
                ptr_ = nullptr;
            }
        }

        explicit operator bool() const
        {
            return ptr_ != nullptr;
        }

        T* operator->()
        {
            return get();
        }

        T& operator*()
        {
            return *ptr_;
        }

        T* get() const
        {
            return ptr_;
        }

        void reset( T* new_ptr = nullptr )
        {
            delete ptr_;
            ptr_ = new_ptr;
        }

    private:
        T* ptr_;
};


#endif // UNIQUE_PTR

