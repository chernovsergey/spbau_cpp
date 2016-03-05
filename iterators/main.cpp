#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <list>
#include <assert.h>
#include <unordered_set>

using namespace std;

template<class fwd_it>
void print( fwd_it begin, fwd_it end )
{
    for( auto it = begin; it != end; ++it )
    {
        cout << *it << " ";
    }

    cout << endl;
}


template<class fwd_it, class comparator = std::less<typename fwd_it::value_type>>
void tree_sort( fwd_it begin, fwd_it end )
{
    using value_type = typename fwd_it::value_type;

    multiset<value_type, comparator> my_set;
    for( auto it = begin; it != end; ++it )
    {
        my_set.insert( *it );
    }

    for( auto it = begin, it_set = my_set.begin(); it != end; ++it, ++it_set )
    {
        *it = *it_set;
    }
}


template<class T, class Pred = std::equal_to<T>, class Hash = std::hash<T>>
void remove_duplicates( list<T>& lst )
{
    unordered_set<T, Hash, Pred> my_set;

    for( auto it = lst.begin(); it != lst.end(); )
    {
        bool hasInserted = my_set.insert( *it ).second;
        if( hasInserted == false )
        {
            it = lst.erase( it );
        }
        else
        {
            ++it;
        }
    }
}


template<class cbd_it, class pred = std::equal_to<typename cbd_it::value_type>>
bool is_palindrome( cbd_it begin, cbd_it end )
{
    pred equal;
    auto rbegin = reverse_iterator<cbd_it>( end );
    for( auto it = begin, rev_it = rbegin; it != end; ++it, ++rev_it )
    {
        if( !equal( *it, *rev_it ) )
        {
            return false;
        }
    }

    return true;
}


template<class fwd_it>
struct alpha_iterator
{
    alpha_iterator(fwd_it begin, fwd_it end)
        :iter(begin)
        ,iter_end(end)
    {
        if(!isalpha(*iter))
        {
            move_to_the_next_alpha();
        }
    }

    alpha_iterator operator++()
    {
        move_to_the_next_alpha();
        return iter;
    }

    alpha_iterator operator++(int)
    {
        auto tmp = iter;
        move_to_the_next_alpha();
        return tmp;
    }

    bool operator==(alpha_iterator rhs)
    {
        return iter == rhs.iter;
    }

    bool operator!=(alpha_iterator rhs)
    {
        return iter != rhs.iter;
    }

    using wtf_type = std::iterator<forward_iterator_tag, class iterator_traits<fwd_it>::value_type>;

    typename wtf_type::reference operator*()
    {
        return *iter;
    }

    typename wtf_type::pointer operator->()
    {
        return &(*iter);
    }

private:
    void move_to_the_next_alpha()
    {
        while(iter != iter_end)
        {
            iter++;
            if(isalpha(*iter))
            {
                break;
            }
        }
    }

    fwd_it iter;
    fwd_it iter_end;
};

template<class fwd_it>
alpha_iterator<fwd_it> make_alpha_iterator(fwd_it begin, fwd_it end)
{
    return alpha_iterator<fwd_it>(begin, end);
}

int main()
{
    // #1
    vector<int> vec = {10, -1, 6, 5, 8, 9, 1, 4};
    print( vec.begin(), vec.end() );


    // #2
    tree_sort( vec.begin(), vec.end() );
    print( vec.begin(), vec.end() );

    tree_sort<vector<int>::iterator, std::greater<int>>( vec.begin(), vec.end() );
    print( vec.begin(), vec.end() );


    // #3
    list<int> lst = { 1, 1, 2, 3, 4, 5, 6, 5, 7, 5, 8, 9 };
    remove_duplicates( lst );
    print( lst.begin(), lst.end() );


    // #4
    std::string polystr = "asdfghjhgfdsa";
    std::string nonpolystr = "asdfghj1hgfdsa";
    assert( is_palindrome( polystr.cbegin(), polystr.cend() ) );
    assert( !is_palindrome( nonpolystr.cbegin(), nonpolystr.cend() ) );



    // #5
    std::string characters = "asd\1\1asd\2\2\2asd\3\3\3\3asd\4\4\4\4\4"
                             "asd\5\5\5\5\5\5asd\6\6\6\6\6\6\6\6";

    auto letters_it = make_alpha_iterator(characters.begin(), characters.end());

    return 0;
}

