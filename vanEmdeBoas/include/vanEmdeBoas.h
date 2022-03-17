#ifndef VANEMDEBOAS_H
#define VANEMDEBOAS_H

#include <vector>
#include <unordered_map>
#include <my_lib.h>

namespace fastIntegerTrees
{


template <class T>
class vEB_node
{
    public:
        vEB_node(u32 w);
        void insert(const T &x);
        bool erase(const T &x);
        T pred(const T &x);
        T succ(const T &x);
        inline bool empty() const;
        inline T get_mn() const;
        inline T get_mx() const;
        inline size_t get_sz() const;
        void to_list(std::vector <T> &v, T x) const;
        size_t count(const T &x);
        size_t count_less(const T &x);
        size_t count_greater(const T &x);
        ~vEB_node();

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
        vanEmdeBoas();
        void insert(const T &x);
        void erase(const T &x);
        inline T pred(const T &x) const;
        inline T succ(const T &x) const;
        inline size_t get_sz() const;
        inline size_t count(const T &x) const;
        inline size_t count_less(const T &x) const;
        inline size_t count_greater(const T &x) const;
        std::vector <T> to_list() const;
        ~vanEmdeBoas();

    private:
        const u32 w = size_bits<T>();
        vEB_node<T> *root;
};


};

#endif // VANEMDEBOAS_H
