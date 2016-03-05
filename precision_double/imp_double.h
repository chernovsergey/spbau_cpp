#ifndef IMP_DOUBLE_H
#define IMP_DOUBLE_H

#include <algorithm>
#include <fstream>
#include <cmath>
#include <string>

namespace math
{
    class imp_double
    {
            double value;
            double precision;
            const double eps = 1e-15;

        public:
            imp_double();
            imp_double( double value );
            imp_double( double value, double delta );
            imp_double( const imp_double& rhs );

            imp_double& operator +=( const imp_double& rhs );
            imp_double& operator -=( const imp_double& rhs );
            imp_double& operator *=( const imp_double& rhs );
            imp_double& operator /=( const imp_double& rhs );

            imp_double& operator =( const imp_double& rhs );

            imp_double operator-() const;

            friend std::ostream& operator <<( std::ostream& os, imp_double& rhs );
            friend std::istream& operator >>( std::istream& is, imp_double& rhs );

            double get_value() const ;
            double get_delta() const;
    };

    bool operator  <( const imp_double& lhs,  const imp_double& rhs );
    bool operator  >( const imp_double& lhs,  const imp_double& rhs );
    bool operator <=( const imp_double& lhs,  const imp_double& rhs );
    bool operator >=( const imp_double& lhs,  const imp_double& rhs );
    bool operator ==( const imp_double& lhs,  const imp_double& rhs );
    bool operator !=( const imp_double& lhs,  const imp_double& rhs );


    imp_double operator +( const imp_double& lhs, const imp_double& rhs );
    imp_double operator -( const imp_double& lhs, const imp_double& rhs );
    imp_double operator *( const imp_double& lhs, const imp_double& rhs );
    imp_double operator /( const imp_double& lhs, const imp_double& rhs );


}

#endif // IMP_DOUBLE_H
