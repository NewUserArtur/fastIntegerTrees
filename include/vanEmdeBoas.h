#ifndef VANEMDEBOAS_H
#define VANEMDEBOAS_H

#include <vector>
#include <unordered_map>
#include "my_lib.h"

// general namespace for data structures that support integer keys
namespace fastIntegerTrees
{

// T is an unsigned integer type
template <class T>
// helper class for van Emde Boas trees
class vEB_node
{
public:
// destructor
~vEB_node()
{
    if (summary) delete summary;
    for (auto p: children) delete p.second;
}

// checks if the tree node is empty
// the tree is implemented is such a way that no node apart from the root can be empty outside of a member function call
inline bool empty() const
{
    return sz==0;
}

// returns the minimal element of the tree
// if the tree is empty the result is undefined
inline T get_mn() const
{
    return mn;
}

// returns the maximal element of the tree
// if the tree is empty the result is undefined
inline T get_mx() const
{
    return mx;
}

// returns the size of the tree
inline size_t get_sz() const
{
    return sz;
}

// constructor
// `w` is number of bits the node handles
vEB_node(u32 w)
    : w(w),
    summary(0),
    sz(0),
    mx(0),
    mn(max_value<T>(w))
{}

// returns the predecessor of a value (maximum element less than a given value)
// if no such value exists the return is undefined
T pred(const T &x)
{
    if (mx < x) return mx;
    if (w == 1) return 0;

    T c = x>>(w>>1), i = x&max_value<T>(w>>1);

    if (children.find(c) != children.end() && i > children[c]->get_mn())
        return (c<<(w>>1))^children[c]->pred(i);

    if (c <= summary->get_mn())
        return mn;

    c = summary->pred(c);
    return (c<<(w>>1))^children[c]->get_mx();
}

// returns the successor of a value (minimal element less that a givenvalue)
// if no such value exists the return is undefined
T succ(const T &x)
{
    if (x < mn) return mn;
    if (w == 1) return 1;

    T c = x>>(w>>1), i = x&max_value<T>(w>>1);

    if (children.find(c) != children.end() && i < children[c]->mx)
        return (c<<(w>>1))^children[c]->succ(i);

    if (summary->get_mx() <= c)
        return mx;

    c = summary->succ(c);
    return (c<<(w>>1))^children[c]->get_mn();
}

// inserts x into the tree
bool insert(T x)
{
    if (sz == 0)
    {
        sz = 1;
        mn = mx = x;
        return true;
    }
    if (x == mn || x == mx) return false;
    if (w == 1)
    {
        ++sz;
        mn = 0; mx = 1;
        return true;
    }

    if (x < mn) swap<T>(mn, x);
    else if (mx < x) mx = x;

    T c = x>>(w>>1), i = x&max_value<T>(w>>1);

    if (children.find(c) == children.end())
    {
        children[c] = new vEB_node<T>(w>>1);

        if (!summary)
            summary = new vEB_node<T>(w>>1);
        summary->insert(c);
    }
    bool ins = children[c]->insert(i);
    if (ins) ++sz;
    return ins;
}

// erases one instance of x from the tree if such exists
// returns an indicator whether there was a deletion
bool erase(const T &x)
{
    if (x < mn || mx < x) return false;
    if (sz == 1)
    {
        if (mn != x) return false;
        sz = 0;
        return true;
    }
    if (w == 1)
    {
        mn = mx = x^1;
        --sz;
        return true;
    }

    T c = x>>(w>>1), i = x&max_value<T>(w>>1);

    if (x == mn)
    {
        --sz;
        c = summary->get_mn(), i = children[c]->get_mn();
        mn = (c<<(w>>1))^i;
        children[c]->erase(i);
        if (children[c]->empty())
        {
            delete children[c];
            children.erase(c);
            summary->erase(c);
        }
        return true;
    }

    if (children.find(c) == children.end()) return false;

    if (!children[c]->erase(i)) return false;
    --sz;
    if (sz == 0) return true;
    if (children[c]->empty())
    {
        delete children[c];
        children.erase(c);
        summary->erase(c);
    }
    if (x == mx)
    {
        if (sz == 1) mx = mn;
        else
        {
            c = summary->get_mx(), i = children[c]->get_mx();
            mx = (c<<(w>>1))^i;
        }
    }

    return true;
}

// inserts all values in the tree into the vector
void to_list(std::vector <T> &v, T x) const
{
    v.push_back(x^mn);
    if (sz == 1) return;
    if (w == 1) return void(v.push_back(x^mx));

    for (std::pair <T, vEB_node<T>*> p: children) p.second->to_list(v, x^(p.first<<(w>>1)));
}

// counts the occurences of the value in the tree
// if no such value exists the return is undefined
bool contains(const T &x)
{
    if (x < mn || mx < x) return false;
    if (x == mn || x == mx) return true;
    if (w == 1) return false;

    T c = x>>(w>>1), i = x&max_value<T>(w>>1);

    return children.find(c) == children.end() ? false : children[c]->contains(i);
}

private:
std::unordered_map <T, vEB_node<T>*> children;
vEB_node<T> *summary;
const u32 w;
T mn, mx;
size_t sz;
};


// T is an unsigned integer type
template <class T>
// user-friendly wrapper class for the one above
// note: values are stored with multiple instances (multiset)
class vanEmdeBoas
{
public:
// constructor
vanEmdeBoas()
    :root(0)
{
    //ctor
}

// inserts a value into the tree
void insert(const T &x)
{
    if (!root)
        root = new vEB_node<T>(w);
    root->insert(x);
}

// erases an instance of a value from the tree
void erase(const T &x)
{
    if (!root) return;

    root->erase(x);
    if (root->empty())
    {
        delete root;
        root = 0;
    }
}

// returns the predecessor of a value
// returns maximum value of the type if no lesser value exists
inline T pred(const T &x) const
{
    return root == nullptr || x <= root->get_mn() ? max_value<T>(size_bits<T>()) : root->pred(x);
}


// returns the successor of a value
// return 0 if no greater value exists
inline T succ(const T &x) const
{
    return root == nullptr || root->get_mx() <= x ? 0 : root->succ(x);
}


// returns the size of the tree
inline size_t get_sz() const
{
    return root ? root->get_sz() : 0;
}

// destructor
~vanEmdeBoas()
{
    if (root) delete root;
}

// returns a vector containing the elements of the tree
std::vector<T> to_list() const
{
    std::vector <T> ans;
    if (root)
        root->to_list(ans, 0);
    return ans;
}

// counts occurences of the value in the tree
inline bool contains(const T &x) const
{
    return root ? root->contains(x) : false;
}

private:
const u32 w = size_bits<T>();
vEB_node<T> *root;
};


};

#endif // VANEMDEBOAS_H
