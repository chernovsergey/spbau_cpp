#include <sstream>
#include <string.h>
#include <math.h>
#include <iomanip>

#include "lint.h"

namespace apa
{
    lint::lint( long number )
    {
        if( number < 0 )
        {
            sign = minus;
            number *= -1;
        }
        else
        {
            sign = plus;
        }

        length = num_digits( number ) / base_length + 1;
        digits = new size_t[length];
        for( size_t i = 0; i < length; ++i )
        {
            digits[i] = number % base;
            number /= base;
        }
    }

    lint::lint( int number )
        : lint( ( long )number )
    {}

    lint::lint( double number )
    {
        std::string str_number = std::to_string( number );
        size_t dot = str_number.find( "." );
        str_number = str_number.substr( 0, dot );

        digits = new size_t[1];

        *this = lint( str_number );
    }

    lint::lint( const std::string& number )
    {
        size_t str_len;
        if( number[0] == '-' )
        {
            sign = minus;
            str_len = number.size() - 1;
        }
        else if( number[0] == '+' )
        {
            sign = plus;
            str_len = number.size() - 1;
        }
        else
        {
            sign = plus;
            str_len = number.size();
        }

        length = str_len / base_length + 1;
        digits = new size_t[length];

        for( size_t i = 0; i < length; ++i )
        {
            size_t digit_end = number.size() - i * base_length;
            size_t digit_start;
            if( digit_end <= base_length )
            {
                if( str_len == number.size() )
                {
                    digit_start = 0;
                }
                else
                {
                    digit_start = 1;
                }
            }
            else
            {
                digit_start = digit_end - base_length;
            }
            std::string digit = number.substr( digit_start, digit_end - digit_start );
            digits[i] = atoi( digit.c_str() );
        }
    }

    lint::lint( const lint& rhs )
        : sign( rhs.sign )
        , length( rhs.length )
    {
        digits = new size_t[length];
        memcpy( digits, rhs.digits, length * sizeof( size_t ) );
    }

    lint::~lint()
    {
        delete[] digits;
    }

    void lint::swap( lint& rhs )
    {
        std::swap( digits, rhs.digits );
        std::swap( length, rhs.length );
        std::swap( sign, rhs.sign );
    }

    lint& lint::operator=( lint rhs )
    {
        swap( rhs );
        return *this;
    }


    lint::operator int() const
    {
        int result   = 0;
        int cur_base = 1;

        for( size_t i = 0; i < length; ++i )
        {
            result   += digits[i] * cur_base;
            cur_base *= base;
        }
        return result;
    }

    std::string lint::to_string() const
    {
        std::stringstream stream;
        if( sign == minus )
        {
            stream << "-";
        }

        bool lead_zeroes = true;
        for( std::ptrdiff_t digit = length - 1; digit >= 0; --digit )
        {
            if( lead_zeroes && digits[digit] )
            {
                lead_zeroes = false;
                stream << digits[digit];
                continue;
            }

            if( !lead_zeroes )
            {
                stream << std::setw( base_length ) << std::setfill( '0' ) << digits[digit];
            }
        }

        if( !lead_zeroes )
        {
            return stream.str();
        }
        else
        {
            return "0";
        }
    }

    lint::operator bool() const
    {
        return chk_greaters( 0 );
    }

    lint lint::operator-() const
    {
        lint result( *this );
        result.sign = change_sign();

        return result;
    }

    lint operator+( const lint& number )
    {
        return number;
    }

    Sign lint::change_sign( Sign rhs_sgn ) const
    {
        if( sign != rhs_sgn )
        {
            return minus;
        }
        else
        {
            return plus;
        }
    }

    Sign lint::change_sign() const
    {
        if( sign == plus )
        {
            return minus;
        }
        else
        {
            return plus;
        }
    }


    size_t lint::add_or_sub( const lint& rhs, Sign op )
    {
        /*
         * Common part for operator+= and operator-=
         * They differ only in sign, so it's expressed below in:
         *
         * //1 - addition/substraction for digits
         *
         * //2 - obtaining remainder
         *
        */

        size_t max_length;
        if( length < rhs.length )
        {
            max_length = rhs.length;
            expand_to( max_length );
        }
        else
        {
            max_length = length;
        }

        size_t remainder = 0;
        for( size_t i = 0; i < max_length; ++i )
        {
            wchar_t new_digit;
            if( i < rhs.length )
            {
                new_digit = rhs.digits[i];
            }
            else
            {
                new_digit = 0;
            }


            //1 - addition/substraction for digits
            if( op == plus )
            {
                digits[i] += new_digit + remainder;
            }
            else
            {
                digits[i] -= new_digit + remainder;
            }

            if( digits[i] >= base )
            {
                //2 - obtaining remainder
                if( op == plus )
                {
                    digits[i] -= base;
                }
                else
                {
                    digits[i] += base;
                }

                remainder = 1;
            }
            else
            {
                remainder = 0;
            }
        }
        return remainder;
    }

    lint& lint::operator+=( const lint& rhs )
    {
        if( sign != rhs.sign )
        {
            return *this -= ( -rhs );
        }

        size_t remainder = add_or_sub( rhs, plus );

        if( remainder == 1 )
        {
            expand();
            digits[length - 1] = 1;
        }
        return *this;
    }

    lint& lint::operator-=( const lint& rhs )
    {
        if( sign != rhs.sign )
        {
            return *this += ( -rhs );
        }

        size_t remainder = add_or_sub( rhs, minus );

        if( remainder == 1 )
        {
            for( size_t i = 0; i < length; ++i )
            {
                if( i == 0 )
                {
                    digits[i] = base - digits[i];
                }
                else
                {
                    digits[i] = base - digits[i] - 1;
                }
            }
            sign = change_sign();
        }
        return *this;
    }

    lint& lint::operator*=( const lint& rhs )
    {
        size_t* new_digits = new size_t[length + rhs.length]();
        for( size_t i = 0; i < length; i++ )
        {
            int r = 0;
            for( size_t j = 0; ( j < rhs.length ) || r; j++ )
            {
                if( j < rhs.length )
                {
                    new_digits[i + j] += digits[i] * rhs.digits[j] + r;
                }
                else
                {
                    new_digits[i + j] += r;
                }

                r = new_digits[i + j] / base;
                new_digits[i + j] -= r * base;
            }
        }

        sign = change_sign( rhs.sign );

        delete[] digits;

        digits = new_digits;
        length += rhs.length;

        return *this;
    }

    lint& lint::operator/=( const lint& rhs )
    {
        if( rhs == lint( "0" ) )
        {
            ( void )( lint( 1 ) / 0 );
        }

        lint result;
        lint a = abs( *this );
        lint b = abs( rhs );
        result.expand_to( a.length );

        lint curr_value;
        for( std::ptrdiff_t i = a.length - 1; i >= 0; i-- )
        {
            curr_value.expand_up();
            curr_value.digits[0] = a.digits[i];
            int x = 0;
            int l = 0, r = lint::base;
            while( l <= r )
            {
                int m = ( l + r ) >> 1;
                lint cur = b * m;
                if( cur <= curr_value )
                {
                    x = m;
                    l = m + 1;
                }
                else
                {
                    r = m - 1;
                }
            }
            result.digits[i] = x;
            curr_value -= b * x;
        }

        result.sign = change_sign( rhs.sign );

        *this = lint( result );
        return *this;
    }

    lint& operator++( lint& number )
    {
        number += 1;
        return number;
    }

    lint operator++( lint& number, int )
    {
        lint temp = number;
        number += 1;
        return temp;
    }

    lint& operator--( lint& number )
    {
        number -= 1;
        return number;
    }

    lint operator--( lint& number, int )
    {
        lint temp = number;
        number -= 1;
        return temp;
    }

    size_t lint::num_digits( long number )
    {
        size_t counter = 0;
        do
        {
            number /= 10;
            ++counter;
        }
        while( number );

        return counter;
    }

    bool lint::chk_greaters( size_t start_index ) const
    {
        for( size_t i = start_index; i < length; ++i )
        {
            if( digits[i] )
            {
                return true;
            }
        }

        return false;
    }

    void lint::expand()
    {
        expand_to( length + 1 );
    }

    void lint::expand_to( size_t new_size )
    {
        size_t* new_digits = new size_t[new_size];
        memset( new_digits, 0, new_size * sizeof( size_t ) );
        memcpy( new_digits, digits, length * sizeof( size_t ) );

        length = new_size;
        delete[] digits;
        digits = new_digits;
    }

    void lint::expand_up()
    {
        if( digits[length - 1] )
        {
            expand();
        }

        for( std::ptrdiff_t i = length - 1; i >= 1; i-- )
        {
            digits[i] = digits[i - 1];
        }
    }

    bool operator<( const lint& lhs, const lint& rhs )
    {
        if( lhs.sign != rhs.sign )
        {
            if( lhs.sign == minus && rhs.sign == plus )
            {
                if( !lhs.chk_greaters( 0 ) )
                {
                    return rhs.chk_greaters( 0 );
                }
                return true;
            }
            else
            {
                return false;
            }
        }

        bool less;
        if( lhs.sign == plus )
        {
            less = true;
        }
        else
        {
            less = false;
        }

        size_t minlen;
        if( lhs.length > rhs.length )
        {
            if( lhs.chk_greaters( rhs.length ) )
            {
                return !less;
            }
            minlen = rhs.length;
        }
        else
        {
            if( rhs.chk_greaters( lhs.length ) )
            {
                return less;
            }
            minlen = lhs.length;
        }

        for( std::ptrdiff_t i = minlen - 1; i >= 0; --i )
        {
            if( lhs.digits[i] > rhs.digits[i] )
            {
                return !less;
            }
            else if( lhs.digits[i] < rhs.digits[i] )
            {
                return less;
            }
        }
        return false;
    }

    bool operator==( const lint& lhs, const lint& rhs )
    {
        return !( rhs < lhs ) && !( lhs < rhs );
    }

    bool operator<=( const lint& lhs, const lint& rhs )
    {
        return !( rhs < lhs );
    }

    bool operator>( const lint& lhs, const lint& rhs )
    {
        return !( lhs <= rhs );
    }

    bool operator>=( const lint& lhs, const lint& rhs )
    {
        return !( lhs < rhs );
    }

    bool operator!=( const lint& lhs, const lint& rhs )
    {
        return !( lhs == rhs );
    }

    std::ostream& operator<<( std::ostream& out, const lint& number )
    {
        out << number.to_string();
        return out;
    }

    std::istream& operator>>( std::istream& in, lint& number )
    {
        std::string str_number;
        in >> str_number;
        number = lint( str_number );

        return in;
    }

    lint operator+( const lint& lhs, const lint& rhs )
    {
        lint result( lhs );
        result += rhs;

        return result;
    }

    lint operator-( const lint& lhs, const lint& rhs )
    {
        lint result( lhs );
        result -= rhs;

        return result;
    }

    lint operator*( const lint& lhs, const lint& rhs )
    {
        lint result( lhs );
        result *= rhs;

        return result;
    }

    lint operator/( const lint& lhs, const lint& rhs )
    {
        lint result( lhs );
        result /= rhs;

        return result;
    }

    lint abs( const lint& number )
    {
        if( number < 0 )
        {
            return -number;
        }

        return number;
    }

    lint pow( const lint& number, int power )
    {
        lint res( 1 );
        lint cur( number );

        int n = power;
        while( n )
        {
            if( n & 1 )
            {
                res = res * cur;
            }
            cur = cur * cur;
            n >>= 1;
        }

        return res;
    }
}//namespace apa
