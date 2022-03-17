#ifndef VANEMDEBOAS_H
#define VANEMDEBOAS_H

#include <vector>
#include <unordered_map>
#include "my_lib.h"

namespace fastIntegerTrees
{


template <class T>
class vEB_node
{
public:

~vEB_node()
{
    if (summary) delete summary;
    for (auto p: children) delete p.second;
}

inline bool empty() const
{
    return sz==0;
}

inline T get_mn() const
{
    return mn;
}

inline T get_mx() const
{
    return mx;
}

inline size_t get_sz() const
{
    return sz;
}

vEB_node(u32 w)
    : w(w),
    summary(0),
    sz(0),
    mx(0),
    mn(max_value<T>(w))
{}

T pred(const T &x)
{
    if (w <= 1) return 0;

    T c = x>>(w>>1), i = x&max_value<T>(w>>1);

    if (children.find(c) != children.end() && i > children[c]->get_mn())
        return (c<<(w>>1))^children[c]->pred(i);

    c = summary->pred(c);
    return (c<<(w>>1))^children[c]->get_mx();
}

T succ(const T &x)
{
    if (w <= 1) return 1;

    T c = x>>(w>>1), i = x&max_value<T>(w>>1);

    if (children.find(c) != children.end() && i < children[c]->mx)
        return (c<<(w>>1))^children[c]->succ(i);

    c = summary->succ(c);
    return (c<<(w>>1))^children[c]->get_mn();
}

void insert(const T &x)
{
    ++sz;
    if (w == 0) return;

    T c = x>>(w>>1), i = x&max_value<T>(w>>1);

    if (!summary)
        summary = new vEB_node<T>(w>>1);
    summary->insert(c);

    if (children.find(c) == children.end())
        children[c] = new vEB_node<T>(w>>1);

    children[c]->insert(i);

    if (x < mn) mn = x;
    if (mx < x) mx = x;
}

bool erase(const T &x)
{
    if (w == 0)
    {
        --sz;
        return true;
    }
    if (x < mn || mx < x) return false;

    T c = x>>(w>>1), i = x&max_value<T>(w>>1);

    if (children.find(c) == children.end()) return false;

    bool er = children[c]->erase(i);
    if (!er) return false;
    --sz;
    if (sz == 0) return true;

    summary->erase(c);
    if (children[c]->empty())
    {
        delete children[c];
        children.erase(c);
    }

    c = w == 1 ? children.find(0) == children.end() : summary->get_mn();
    i = children[c]->get_mn();
    mn = (c<<(w>>1))^i;
    c = w == 1 ? children.find(1) != children.end() : summary->get_mx();
    i = children[c]->get_mx();
    mx = (c<<(w>>1))^i;

    return true;
}

void to_list(std::vector <T> &v, T x) const
{
    if (w == 0)
    {
        for (T i = 0; i < sz; ++i) v.push_back(x);
        return;
    }

    for (std::pair <T, vEB_node<T>*> p: children) p.second->to_list(v, x^(p.first<<(w>>1)));
}

size_t count(const T &x)
{
    if (x < mn || mx < x) return 0;
    if (w == 0) return sz;

    T c = x>>(w>>1), i = x&max_value<T>(w>>1);

    return children.find(c) == children.end() ? 0 : children[c]->count(i);
}

size_t count_less(const T &x)
{
    if (x <= mn) return 0;
    if (w == 1) return children[0]->sz;

    T c = x>>(w>>1), i = x&max_value<T>(w>>1);

    return summary->count_less(c) +
            (children.find(c) == children.end() ? 0 : children[c]->count_less(i));
}

size_t count_greater(const T &x)
{
    if (mx <= x) return 0;
    if (w == 1) return children[1]->sz;

    T c = x>>(w>>1), i = x&max_value<T>(w>>1);

    return summary->count_greater(c) +
            (children.find(c) == children.end() ? 0 : children[c]->count_greater(i));
}
        
private:
std::unordered_map <T, vEB_node<T>*> children;
vEB_node<T> *summary;
const u32 w;
T mn, mx;
size_t sz;
};

template <class T>
class vanEmdeBoas
{
public:
vanEmdeBoas()
    :root(0)
{
    //ctor
}

void insert(const T &x)
{
    if (!root)
        root = new vEB_node<T>(w);
    root->insert(x);
}

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

inline T pred(const T &x) const
{
    return root == nullptr || x <= root->get_mn() ? max_value<T>(size_bits<T>()) : root->pred(x);
}

inline T succ(const T &x) const
{
    return root == nullptr || root->get_mx() <= x ? 0 : root->succ(x);
}

inline size_t get_sz() const
{
    return root ? root->get_sz() : 0;
}

~vanEmdeBoas()
{
    if (root) delete root;
}

std::vector<T> to_list() const
{
    std::vector <T> ans;
    if (root)
        root->to_list(ans, 0);
    return ans;
}

inline size_t count(const T &x) const
{
    return root ? root->count(x) : 0;
}

inline size_t count_less(const T &x) const
{
    return root ? root->count_less(x) : 0;
}

inline size_t count_greater(const T &x) const
{
    return root ? root->count_greater(x) : 0;
}

private:
const u32 w = size_bits<T>();
vEB_node<T> *root;
};


};

#endif // VANEMDEBOAS_H
