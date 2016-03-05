#include "imp_double.h"

namespace math
{

    imp_double::imp_double()
        : value( 0.0 )
        , precision( eps )
    {}


    imp_double::imp_double( double value )
        : value( value )
        , precision( eps )
    {}


    imp_double::imp_double( double value, double delta )
        : value( value )
        , precision( eps )
    {
        precision = std::max( delta, eps * abs( value ) );
    }


    imp_double::imp_double( const imp_double& rhs )
        : value( rhs.value )
        , precision( rhs.precision )
    {}


    std::ostream& operator << ( std::ostream& os, imp_double& rhs )
    {
        os << rhs.value << "#" << rhs.precision;
        return os;
    }


    std::istream& operator >> ( std::istream& is, imp_double& rhs )
    {
        std::string sep = "#";
        double val = 0.;
        double prec = 0.;
        is >> val >> sep >> prec;
        rhs = imp_double( val, prec );
        return is;
    }


    double imp_double::get_value() const
    {
        return value;
    }

    double imp_double::get_delta() const
    {
        return precision;
    }


    bool operator < ( const imp_double& lhs, const imp_double& rhs )
    {
        if( lhs.get_value() + lhs.get_delta() < rhs.get_value() - rhs.get_delta() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }


    bool operator > ( const imp_double& lhs, const imp_double& rhs )
    {
        return rhs < lhs;
    }


    bool operator <=( const imp_double& lhs, const imp_double& rhs )
    {
        return !( lhs > rhs );
    }


    bool operator >=( const imp_double& lhs, const imp_double& rhs )
    {
        return !( lhs < rhs );
    }


    bool operator == ( const imp_double& lhs, const imp_double& rhs )
    {
        return !( ( lhs < rhs ) || ( lhs > rhs ) );
    }


    bool operator != ( const imp_double& lhs, const imp_double& rhs )
    {
        return !( lhs == rhs );
    }

    imp_double imp_double::operator - () const
    {
        return imp_double( -value, precision );
    }


    imp_double& imp_double::operator += ( const imp_double& rhs )
    {
        value += rhs.value;
        precision += rhs.precision;

        return *this;
    }


    imp_double& imp_double::operator -= ( const imp_double& rhs )
    {
        value -= rhs.value;
        precision += rhs.precision;

        return *this;
    }


    imp_double& imp_double::operator *= ( const imp_double& rhs )
    {
        value *= rhs.value;
        precision = precision * abs( rhs.value ) + rhs.precision * value;

        return *this;
    }


    imp_double& imp_double::operator /= ( const imp_double& rhs )
    {
        value /= rhs.value;
        precision = ( precision / rhs.value ) + ( abs( value ) * rhs.precision / ( rhs.value * rhs.value ) );

        return *this;
    }


    imp_double operator + ( const imp_double& lhs, const imp_double& rhs )
    {
        imp_double tmp(lhs);
        tmp += rhs;
        return tmp;
    }


    imp_double operator - (  const imp_double& lhs, const imp_double& rhs )
    {
        imp_double tmp(lhs);
        tmp -= rhs;
        return tmp;
    }


    imp_double operator * (  const imp_double& lhs, const imp_double& rhs )
    {
        imp_double tmp(lhs);
        tmp *= rhs;
        return tmp;
    }


    imp_double operator / (  const imp_double& lhs, const imp_double& rhs )
    {
        imp_double tmp(lhs);
        tmp /= rhs;
        return tmp;
    }


    imp_double& imp_double::operator =( const imp_double& rhs )
    {
        value = rhs.value;
        precision = rhs.precision;
        return *this;
    }
}
