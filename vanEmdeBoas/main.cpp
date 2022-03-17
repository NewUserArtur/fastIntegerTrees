#include <bits/stdc++.h>
#include <vanEmdeBoas.cpp>
#include <my_lib.h>

typedef uint64_t u64;

using namespace std;

void interactive_test()
{
    cout << "maximum value: " << max_value<u64>(64) << "\n\n";
    cout << "commands:\n";
    cout << "+ x: add a number\n";
    cout << "- x: delete a number\n";
    cout << "> x: get successor of  a number\n";
    cout << "< x: get predecessor of  a number\n";
    cout << "p: print the whole set (random order)\n";
    cout << "c x: count number of occurences of a number\n";
    cout << "l x: count numbers less than a given number\n";
    cout << "g x: count numbers greater than a given number\n";
    cout << "\n";

    using fastIntegerTrees::vanEmdeBoas;

    vanEmdeBoas<u64> t;

    while (true)
    {
        char c;
        cin >> c;
        if (c == 'q') break;

        switch (c)
        {
            case '+':
            {
                int x;
                cin >> x;
                t.insert(x);

                break;
            }
            case '-':
            {
                int x;
                cin >> x;
                t.erase(x);

                break;
            }
            case '<':
            {
                int x;
                cin >> x;
                cout << t.pred(x) << "\n";

                break;
            }
            case '>':
            {
                int x;
                cin >> x;
                cout << t.succ(x) << "\n";

                break;
            }
            case 'p':
            {
                vector <u64> v = t.to_list();
                cout << v.size() << "\n";
                for (u64 x: v) cout << x << " ";
                cout << "\n";

                break;
            }
            case 'c':
            {
                int x;
                cin >> x;
                cout << t.count(x) << "\n";

                break;
            }
            case 'l':
            {
                int x;
                cin >> x;
                cout << t.count_less(x) << "\n";

                break;
            }
            case 'g':
            {
                int x;
                cin >> x;
                cout << t.count_greater(x) << "\n";

                break;
            }
            default:
            {
                cout << "invalid operation\n";

                break;
            }
        }
    }
}

void automatic_test(int k, bool debug = false)
{
    typedef u16 T;

    using fastIntegerTrees::vanEmdeBoas;

    mt19937_64 gen(1337);
    vanEmdeBoas <T> t1;
    multiset <T> t2;

    for (int i = 0; i < k; ++i)
    {
        cout << "step " << i << "\n";
        //if (t1.root) t1.root->check();
        if (false)
        {
            int x = 1;
            vector <T> v = t1.to_list();
            sort(v.begin(), v.end());
            for (T x: v) cout << (int)x << " ";
            cout << "\n";
        }

        switch (gen()%8)
        {
            case 0:
            {
                T x = gen();
                if (debug) cout << "+ " << (u64)x << "\n";
                t1.insert(x);
                t2.insert(x);

                break;
            }
            case 1:
            {
                T x = gen();
                if (debug) cout << "- " << (u64)x << "\n";
                t1.erase(x);
                auto it = t2.find(x);
                if (it != t2.end()) t2.erase(it);

                break;
            }
            case 2:
            {
                T x = gen();
                if (debug) cout << "< " << x << "\n";
                T y = t1.pred(x);
                auto it = lower_bound(t2.begin(), t2.end(), x);
                T z = it == t2.begin() ? max_value<T>(size_bits<T>()) : *prev(it);

                if (y == z);// cout << "OK\n";
                else
                {
                    cout << "ERROR\n";
                    cout << y << " " << z << "\n";
                }

                break;
            }
            case 3:
            {
                T x = gen();
                if (debug) cout << "> " << x << "\n";
                T y = t1.succ(x);
                auto it = upper_bound(t2.begin(), t2.end(), x);
                T z = it == t2.end() ? 0 : *it;

                if (y == z);// cout << "OK\n";
                else
                {
                    cout << "ERROR\n";
                    cout << y << " " << z << "\n";

                    return;
                }

                break;
            }
            case 4:
            {
                if (debug) cout << "p\n";

                vector <T> v1 = t1.to_list();
                sort(v1.begin(), v1.end());
                vector <T> v2;
                for (T x: t2) v2.push_back(x);

                bool ok = v1.size() == v2.size();
                if (!ok)
                {
                    for (int i = 0; i < v1.size(); ++i)
                    {
                        if (v1[i] != v2[i])
                        {
                            ok = false;
                            break;
                        }
                    }
                }

                if (ok);// cout << "OK\n";
                else
                {
                    cout << "ERROR\n";
                    cout << v1.size() << "\n";
                    for (int x: v1) cout << x << " ";
                    cout << "\n";
                    cout << v2.size() << "\n";
                    for (int x: v2) cout << x << " ";
                    cout << "\n";

                    return;
                }

                break;
            }
            case 5:
            {
                T x = gen();
                if (debug) cout << "c " << x << "\n";

                T y = t1.count(x);
                T z = t2.count(x);

                if (y == z);// cout << "OK\n";
                else
                {
                    cout << "ERROR\n";
                    cout << y << " " << z << "\n";

                    return;
                }

                break;
            }
            case 6:
            {
                T x = gen();
                if (debug) cout << "l " << x << "\n";

                T y = t1.count_less(x);
                T z = distance(t2.begin(), lower_bound(t2.begin(), t2.end(), x));

                if (y == z);// cout << "OK\n";
                else
                {
                    cout << "ERROR\n";
                    cout << y << " " << z << "\n";

                    return;
                }

                break;
            }
            case 7:
            {
                T x = gen();
                if (debug) cout << "g " << x << "\n";

                T y = t1.count_greater(x);
                T z = distance(upper_bound(t2.begin(), t2.end(), x), t2.end());

                if (y == z);//cout << "OK\n";
                else
                {
                    cout << "ERROR\n";
                    cout << y << " " << z << "\n";

                    return;
                }

                break;
            }
        }
    }

    cout << "\nALLZ GOOD!\n";
}

template <class T>
pair <double, double> speed_test_run(int K, mt19937_64 &gen, bool debug = false)
{
    if (debug) cout << "generating " << K << " tests...\n";
    vector <pair <int, T> > p(K);
    for (int i = 0; i < K; ++i) p[i] = {(int)(gen()%5), (T)gen()};
    vector <u64> ans1(K), ans2(K);

    if (debug) cout << "measuring multiset performance...\n";
    auto start = chrono::steady_clock::now();
    multiset <T> t1;
    for (int i = 0; i < K; ++i)
    {
        switch (p[i].first)
        {
            case 0:
            {
                t1.insert(p[i].second);
                break;
            }
            case 1:
            {
                auto it = t1.find(p[i].second);
                if (it != t1.end()) t1.erase(it);
                break;
            }
            case 2:
            {
                auto it = lower_bound(t1.begin(), t1.end(), p[i].second);
                ans1[i] = it == t1.begin() ? max_value<T>(size_bits<T>()) : *prev(it);
                break;
            }
            case 3:
            {
                auto it = upper_bound(t1.begin(), t1.end(), p[i].second);
                ans1[i] = it == t1.end() ? 0 : *it;
                break;
            }
            case 4:
            {
                ans1[i] = t1.count(p[i].second);
                break;
            }
            case 5:
            {
                ans1[i] = distance(t1.begin(), lower_bound(t1.begin(), t1.end(), p[i].second));
                break;
            }
            case 6:
            {
                ans1[i] = distance(upper_bound(t1.begin(), t1.end(), p[i].second), t1.end());
                break;
            }
        }
    }
    auto finish = chrono::steady_clock::now();
    double tim1 = chrono::duration_cast<chrono::microseconds>(finish-start).count()/1e6;
    if (debug) cout << "it took " << tim1 << " seconds\n";

    t1.clear();

    if (debug) cout << "measuring van Emde Boas performance...\n";
    start = chrono::steady_clock::now();
    fastIntegerTrees::vanEmdeBoas<T> t2;
    for (int i = 0; i < K; ++i)
    {
        switch (p[i].first)
        {
            case 0:
            {
                t2.insert(p[i].second);
                break;
            }
            case 1:
            {
                t2.erase(p[i].second);
                break;
            }
            case 2:
            {
                ans2[i] = t2.pred(p[i].second);
                break;
            }
            case 3:
            {
                ans2[i] = t2.succ(p[i].second);
                break;
            }
            case 4:
            {
                ans2[i] = t2.count(p[i].second);
                break;
            }
            case 5:
            {
                ans2[i] = t2.count_less(p[i].second);
                break;
            }
            case 6:
            {
                ans2[i] = t2.count_greater(p[i].second);
                break;
            }
        }
    }
    finish = chrono::steady_clock::now();
    double tim2 = chrono::duration_cast<chrono::microseconds>(finish-start).count()/1e6;
    if (debug) cout << "it took " << tim2 << " seconds\n";

    bool ok = true;
    for (int i = 0; i < K; ++i)
    {
        if (ans1[i] != ans2[i])
        {
            //cout << "error on " << i << "\n";
            //cout << p[i].first << " " << (int)p[i].second << " " << ans1[i] << " " << ans2[i] << "\n";
            ok = false;
            break;
        }
    }

    if (!ok) cout << "ERROR!\n";

    return {tim1, ok ? tim2 : 1e9};
}

template <class T>
void speed_test(int n, int k)
{
    mt19937_64 gen(1337);

    cout << "comparing performance on " << n << " runs with " << k << " tests each\n";
    double t1 = 0, t2 = 0;
    for (int i = 0; i < n; ++i)
    {
        cout << "run " << i << "\n";
        pair <double, double> rt = speed_test_run<T>(k, gen);
        printf("%.6lf : %.6lf (%.3lf)\n", rt.first, rt.second, rt.first/rt.second);
        t1 += rt.first;
        t2 += rt.second;
    }

    cout << "multiset: " << t1 << "\n";
    cout << "vanEmdeBoas: " << t2 << "\n";
    cout << t1/t2 << "x increase in performace\n";
}

void speed_test_all_types(int n, int k)
{
    cout << "testing on " << size_bits<u8>() << " bit type\n";
    speed_test<u8>(n, k);
    cout << "\n";

    cout << "testing on " << size_bits<u16>() << " bit type\n";
    speed_test<u16>(n, k);
    cout << "\n";

    cout << "testing on " << size_bits<u32>() << " bit type\n";
    speed_test<u32>(n, k);
    cout << "\n";

    cout << "testing on " << size_bits<u64>() << " bit type\n";
    speed_test<u64>(n, k);
    cout << "\n";
}

int main()
{
    //interactive_test();
    //automatic_test(1e7, false);
    speed_test_all_types(100, 3e4);

    return 0;
}
