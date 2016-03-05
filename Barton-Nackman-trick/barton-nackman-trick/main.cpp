#include <iostream>
#include <assert.h>

using namespace std;


template<class T>
class comparable
{
public:
    friend bool operator==(const T& a, const T& b)
    {
        return !(b < a) && !(a < b);
    }

    friend bool operator!=(const T& a, const T& b)
    {
        return !(a == b);
    }

    friend bool operator>(const T& a, const T& b)
    {
        return (b < a);
    }

    friend bool operator>=(const T& a, const T& b)
    {
        return !(a < b);
    }

    friend bool operator<=(const T& a, const T& b)
    {
        return !(b < a);
    }
};


class Foo : private comparable<Foo>
{
    size_t useless;
public:

    Foo()
        :useless(0)
    {}

    Foo(size_t some_value)
        :useless(some_value)
    {}

    friend bool operator<(const Foo& a, const Foo& b)
    {
        return a.useless < b.useless;
    }
};

int main()
{
    Foo foo1;
    Foo foo2(2);
    Foo foo3(5);

    assert(foo1 == Foo());
    assert(foo1 != foo2);

    assert(foo3 > foo2);
    assert(foo3 >= foo2);

    assert(foo2 < foo3);
    assert(foo2 <= foo3);

    return 0;
}

