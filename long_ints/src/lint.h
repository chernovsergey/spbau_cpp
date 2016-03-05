#pragma once
#include <string>

namespace apa
{
    enum Sign
    {
        plus  =  1,
        minus = -1
    };

    class lint
    {
        public:
            lint( long number );
            lint( int number = 0 );
            lint( double number );
            explicit lint( const std::string& number );
            lint( const lint& rhs );
            ~lint();

            explicit operator int() const;
            explicit operator bool() const;
            std::string to_string() const;

            lint& operator=( lint tocopy );

            lint  operator -() const;
            lint& operator +=( const lint& rhs );
            lint& operator -=( const lint& rhs );
            lint& operator *=( const lint& rhs );
            lint& operator /=( const lint& rhs );

            friend bool operator <( const lint& lhs, const lint& rhs );

        private:
            static const size_t base        = 1e4;
            static const size_t base_length = 4;
            Sign   sign;
            size_t length;
            size_t* digits;

            size_t num_digits( long number );
            bool chk_greaters( size_t start_index ) const;
            void expand();
            void expand_up();
            void expand_to( size_t new_size );
            void swap( lint& rhs );
            Sign change_sign( Sign rhs_sgn ) const;
            Sign change_sign() const;
            size_t add_or_sub( const lint& rhs, Sign op );
    };

    lint  operator +( const lint& );

    lint  operator ++( lint&, int );
    lint& operator ++( lint& );
    lint  operator --( lint&, int );
    lint& operator --( lint& );

    lint operator +( const lint& lhs, const lint& rhs );
    lint operator -( const lint& lhs, const lint& rhs );
    lint operator *( const lint& lhs, const lint& rhs );
    lint operator /( const lint& lhs, const lint& rhs );

    bool operator  >( const lint& lhs, const lint& rhs );
    bool operator <=( const lint& lhs, const lint& rhs );
    bool operator >=( const lint& lhs, const lint& rhs );
    bool operator !=( const lint& lhs, const lint& rhs );
    bool operator ==( const lint& lhs, const lint& rhs );

    std::ostream& operator<<( std::ostream& out, const lint& number );
    std::istream& operator>>( std::istream& in, lint& number );

    lint pow( const lint& number, int power );
    lint abs( const lint& number );

}//namespace apa
