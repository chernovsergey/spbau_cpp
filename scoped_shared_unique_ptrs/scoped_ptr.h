#ifndef SCOPED_PTR
#define SCOPED_PTR

template<class T>
struct scoped_ptr
{
        using element_type = T;

        scoped_ptr( const scoped_ptr<T>& ) = delete;
        scoped_ptr<T>& operator=( scoped_ptr<T> ) = delete;

        scoped_ptr( const scoped_ptr<T>&& ) = delete;
        scoped_ptr<T>& operator=( const scoped_ptr<T> && ) = delete;

        // Explicit because the folowing code mustn't work
        // int a = 5; scoped_ptr<foo>(&a)
        // then delete will be called for variable a
        explicit scoped_ptr( T* obj_ptr )
            : ptr_( obj_ptr )
        {}

        ~scoped_ptr()
        {
            if( *this )
            {
                delete ptr_;
                ptr_ = nullptr;
            }
        }

        T* operator->() const
        {
            return get();
        }

        T& operator*()
        {
            return *ptr_;
        }

        explicit operator bool() const
        {
            return ptr_ != nullptr;
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


#endif // SCOPED_PTR

