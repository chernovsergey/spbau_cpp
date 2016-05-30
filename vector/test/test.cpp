#include <iostream>
#include <cassert>
#include <exception>
#include "vector.h"
#include <memory>

#define TEST_PART1
#define TEST_PART2
#define TEST_PART3

using namespace my;

struct test_exception: std::exception
{};

struct test_struct
{
    test_struct(size_t data = 0)
        : data_(data)
        , should_throw_(false)
    {}

    test_struct(double a, short c)
        : test_struct(static_cast<size_t>(a * c))
    {}

    test_struct(const test_struct &src)
        : data_(src.data_)
        , should_throw_(src.should_throw_)
    {
        if (should_throw_)
            throw test_exception();
        ++copies_cnt_;
    }

    test_struct(test_struct &&src)
        : data_(src.data_)
        , should_throw_(src.should_throw_)
    {
        if (should_throw_)
            throw test_exception();
        ++moves_cnt_;
    }

    explicit operator size_t() const
    {
        return data_;
    }

    test_struct& operator=(size_t value)
    {
        data_ = value;
        return *this;
    }

    bool operator==(size_t value) const
    {
        return data_ == value;
    }

    bool operator==(const test_struct &other) const
    {
        return (data_ == other.data_)
            && (should_throw_ == other.should_throw_);
    }

    size_t data_;
    bool should_throw_;
    static size_t moves_cnt_;
    static size_t copies_cnt_;
};
size_t test_struct::moves_cnt_ = 0;
size_t test_struct::copies_cnt_ = 0;

typedef vector<test_struct> test_vec_t;
typedef std::unique_ptr<test_vec_t> test_vec_ptr_t;
// return unique ptr to avoid copying.
// copying is not available in TEST_PART1
test_vec_ptr_t make_vec_n(const size_t n)
{
    test_vec_t *vec = new test_vec_t(n);
    for (size_t i = 0; i < n; ++i)
        (*vec)[i] = i;
    return test_vec_ptr_t(vec);
}

#ifdef TEST_PART1
void test_simple_constructors()
{
    test_vec_t vec;
    assert(vec.size() == 0);
    assert(vec.capacity() == 1);

    test_vec_t vec2(10);
    assert(vec2.size() == 10);
    assert(vec2.capacity() == 10);
}

void test_vec_push_back()
{
    test_vec_t vec;
    test_struct ts1(100);
    test_struct::copies_cnt_ = 0;
    test_struct::moves_cnt_ = 0;

    // capacity = 1, size = 1, no resize
    vec.push_back(12);
    assert(test_struct::copies_cnt_ == 0);
    assert(test_struct::moves_cnt_ > 0);
    test_struct::copies_cnt_ = 0;
    test_struct::moves_cnt_ = 0;

    // capacity = 1, size = 2, resize
    vec.push_back(123);
    assert(test_struct::copies_cnt_ >= 0);
    assert(test_struct::moves_cnt_ > 0);
    test_struct::copies_cnt_ = 0;
    test_struct::moves_cnt_ = 0;

    // capacity = 2, size = 3, resize
    vec.push_back(ts1);
    assert(test_struct::copies_cnt_ >= 0);
    assert(test_struct::moves_cnt_ >= 0);
    test_struct::copies_cnt_ = 0;
    test_struct::moves_cnt_ = 0;

    // capacity = 4, size = 4, no resize
    vec.push_back(std::move(ts1));
    assert(test_struct::copies_cnt_ == 0);
    assert(test_struct::moves_cnt_ > 0);

    assert(vec[0] == 12);
    assert(vec[1] == 123);
    assert(vec[2] == 100);
    assert(vec[3] == 100);
}

void test_vec_operator_square_brackets()
{
    test_vec_ptr_t vec = make_vec_n(10);
    const test_vec_t &cvec =
        const_cast<const test_vec_t&>(*vec);

    test_struct &ref1 = (*vec)[0];
    const test_struct &cref1 = cvec[0];

    assert(ref1 == 0);
    assert(cref1 == 0);
}

void test_vec_capacity_size()
{
    test_vec_t vec;
    assert(vec.size() == 0);
    assert(vec.empty());
    assert(vec.capacity() == 1);

    for(size_t i = 0; i < 100; ++i)
    {
        vec.push_back(0);
        assert(vec.size() == (i + 1));
    }
    assert(vec.capacity() == 128);
}

void test_vec_100_items()
{
    vector<test_struct> v;
    assert(v.empty());

    const size_t n = 100;
    for (size_t i = 0; i < n; ++i)
        v.push_back(i);

    assert(v.size() == static_cast<size_t>(n));
    assert(v.capacity() == 128);

    for (size_t i = 0; i < n; ++i)
        assert(v[i] == i);
}

void test_vec_item_constr_throw()
{
    auto vec = make_vec_n(128);
    test_struct ts(777);
    ts.should_throw_ = true;
    try
    {
        vec->push_back(ts);
        assert(false);
    }
    catch(test_exception &ex)
    {}
    catch(...)
    {
        assert(false);
    }
}

void test_part1()
{
    test_simple_constructors();
    test_vec_push_back();
    test_vec_operator_square_brackets();
    test_vec_capacity_size();
    test_vec_100_items();
    test_vec_item_constr_throw();
}
#else
void test_part1() {}
#endif //TEST_PART1

#ifdef TEST_PART2
void test_vec_copying()
{
    const size_t n = 1000;
    test_vec_ptr_t v = make_vec_n(n);
    test_vec_t v_copy(*v);

    for (size_t i = 0; i < n; ++i)
    {
        assert(v_copy[i] == i);
        assert((*v)[i] == v_copy[i]);
    }
    assert(v->size() == v_copy.size());
    assert(v_copy.size() == n);
}

void test_vec_move()
{
    const size_t n = 1000;
    test_vec_ptr_t v_moved = make_vec_n(n);
    test_vec_t v(std::move(*v_moved));

    for (size_t i = 0; i < n; ++i)
        assert(v[i] == i);
    assert(v.size() == n);
}

void test_vec_swap()
{
    const size_t n1 = 10;
    const size_t n2 = 20;
    auto v1 = make_vec_n(n1);
    auto v2 = make_vec_n(n2);
    swap(*v1, *v2);

    for(size_t i = 0; i < n1; ++i)
        assert((*v2)[i] == i);
    for(size_t i = 0; i < n2; ++i)
        assert((*v1)[i] == i);

    assert(v1->size() == n2);
    assert(v2->size() == n1);
}

void test_vec_at()
{
    test_vec_ptr_t vec = make_vec_n(10);
    const test_vec_t &cvec =
        const_cast<const test_vec_t&>(*vec);

    test_struct &ref2 = vec->at(0);
    const test_struct &cref2 = cvec.at(0);
    assert(ref2 == 0);
    assert(cref2 == 0);

    try
    {
        vec->at(110);
        assert(false);
    }
    catch(const std::out_of_range &ex)
    {}
    catch(...)
    {
        assert(false);
    }

    try
    {
        cvec.at(110);
        assert(false);
    }
    catch(const std::out_of_range &ex)
    {}
    catch(...)
    {
        assert(false);
    }
}


void test_part2()
{
    test_vec_copying();
    test_vec_move();
    test_vec_swap();
    test_vec_at();
}
#else
void test_part2() {}
#endif // TEST_PART2

#ifdef TEST_PART3
void test_vec_assignment()
{
    const size_t n1 = 10;
    const size_t n2 = 100;
    auto v1 = make_vec_n(n1);
    auto v2 = make_vec_n(n2);
    *v2 = *v1;

    for(size_t i = 0; i < n1; ++i)
        assert((*v1)[i] == (*v2)[i]);
    assert(v2->size() == v1->size());
    assert(v2->size() == n1);

    test_vec_ptr_t v3 = make_vec_n(n2);
    *v1 = std::move(*v3);
    for(size_t i = 0; i < n2; ++i)
        assert((*v1)[i] == i);
    assert(v1->size() == n2);
}

void test_vec_emplace_back()
{
    vector<test_struct> vec;
    size_t prev_moves_cnt = 0;

    vec.emplace_back(3.0, 4);
    vec.emplace_back(123);

    vec.reserve(4);
    test_struct::copies_cnt_ = 0;
    test_struct::moves_cnt_ = 0;
    test_struct ts1(100);
    vec.emplace_back(ts1);
    assert(test_struct::copies_cnt_ == 1);
    prev_moves_cnt = test_struct::moves_cnt_;
    vec.emplace_back(std::move(ts1));
    assert(test_struct::copies_cnt_ == 1);
    assert(test_struct::moves_cnt_ > prev_moves_cnt);

    vec.reserve(6);
    test_struct::copies_cnt_ = 0;
    test_struct::moves_cnt_ = 0;
    vec.emplace_back(1000);
    // perfect forwarding: should call
    // test_struct(size_t) and can call
    // any number of move constructors
    assert(test_struct::copies_cnt_ == 0);
    assert(test_struct::moves_cnt_ >= 0);
    prev_moves_cnt = test_struct::moves_cnt_;
    vec.emplace_back(1001);
    assert(test_struct::copies_cnt_ == 0);
    assert(test_struct::moves_cnt_ >= prev_moves_cnt);

    assert(vec[0] == 12);
    assert(vec[1] == 123);
    assert(vec[2] == 100);
    assert(vec[3] == 100);
    assert(vec[4] == 1000);
    assert(vec[5] == 1001);
}

void test_vec_reserve()
{
    test_vec_ptr_t vec2 = make_vec_n(1);

    assert(vec2->capacity() == 1);
    vec2->reserve(12345);
    assert(vec2->capacity() == 12345);

    vec2->reserve(100);
    assert(vec2->capacity() == 12345);

    vec2->reserve(12346);
    assert(vec2->capacity() == 12346);
}

void test_part3()
{
    test_vec_assignment();
    test_vec_emplace_back();
    test_vec_reserve();
}
#else
void test_part3() {}
#endif // TEST_PART3

int main()
{
    test_part1();
    test_part2();
    test_part3();
    std::cout << "Ok!" << std::endl;
    return 0;
}
