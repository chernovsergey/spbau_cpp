#ifndef AUXILARYTYPES
#define AUXILARYTYPES

#include <vector>
#include <map>

typedef std::vector<bool> code;

//typedef std::map<char, code> codemap;
typedef std::vector<code> v_codemap;

class node
{
public:
    const int f;

    virtual ~node() {}

protected:
    node(int f) : f(f) {}
};

class tree_node : public node
{
public:
    node* left;
    node* right;

    tree_node(node* lchild, node* rchild)
        : node(lchild->f + rchild->f),
          left(lchild),
          right(rchild)
          {}

    tree_node()
        :node(-1),
         left(nullptr),
         right(nullptr)
         {}

    ~tree_node()
    {
        delete left;
        delete right;
    }
};

class leaf : public node
{
public:
    const char c;
    leaf(int f, char c) : node(f), c(c) {}
};

struct comparator
{
    bool operator()(const node* a, const node* b) const
    {
        return a->f > b->f;
    }
};


#endif // AUXILARYTYPES

