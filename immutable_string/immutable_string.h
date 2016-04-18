#ifndef IMMUTABLE_STRING
#define IMMUTABLE_STRING

#include <string>
#include <cstring>
#include <memory>

struct buffer
{
        typedef std::char_traits<const char> traits;

        buffer( const char* src )
            : raw_string( nullptr )
            , size( strlen( src ) + 1 )
            , is_lazy( false )
        {
            raw_string = new char[size];
            memcpy(raw_string, src, size );
        }

        buffer( std::shared_ptr<buffer>const& str1, std::shared_ptr<buffer>const& str2 )
            : raw_string( nullptr )
            , size( 0 )
            , is_lazy( true )
            , left_( str1 )
            , rght_( str2 )
        {}

        ~buffer()
        {
            if( !is_lazy )
                delete[] raw_string;

        }

        size_t length()
        {
            if( !is_lazy )
                return size - 1;
            else
                return left_->length() + rght_->length();
        }

        const char* c_str()
        {
            if( is_lazy )
            {
                size_t new_size = left_->length() + rght_->length() + 1;
                this->raw_string = new char[new_size];
                this->size = new_size;
                this->is_lazy = false;
                memcpy( raw_string, left_->c_str(), left_->length() );
                memcpy( raw_string + left_->length(), rght_->c_str(), rght_->length() + 1 );
                left_.reset();
                rght_.reset();
            }

            return raw_string;
        }

        std::ostream& print( std::ostream& out )
        {
            if( is_lazy )
            {
                left_->print( out );
                rght_->print( out );
            }
            else
                out << raw_string;

            return out;
        }

    private:

        char* raw_string;
        size_t size;
        bool is_lazy;

        std::shared_ptr<buffer> left_;
        std::shared_ptr<buffer> rght_;
};

struct imstring
{
        imstring( imstring const& ) = default;
        imstring operator=( imstring const& ) = delete;

        imstring( const char* src )
            : buf_( new buffer( src ) )
        {}

        imstring( imstring const& src1, imstring const& src2 )
            :buf_(new buffer(src1.buf_, src2.buf_))
       {}

        char operator[]( size_t pos ) const
        {
            return buf_->c_str()[pos];
        }

        size_t size() const
        {
            return buf_->length();
        }

        const char* c_str() const
        {
            return buf_->c_str();
        }

        std::ostream& print( std::ostream& out ) const
        {
            return buf_->print( out );
        }

        const char* begin() const
        {
            return &c_str()[0];
        }

        const char* end() const
        {
            return &c_str()[size()];
        }

    private:
        std::shared_ptr<buffer> buf_;
};

    std::ostream& operator << (std::ostream& out, imstring str)
    {
        return str.print(out);
    }

    imstring operator+(const imstring &src1, const imstring &src2)
    {
        return imstring(src1, src2);
    }

#endif // IMMUTABLE_STRING

