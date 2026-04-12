#pragma once
#include <bits/stdc++.h>
using namespace std;

class Debug {
public:
#ifndef ONLINE_JUDGE
    template<typename... Args>
    static void log(const string& names, Args... args) {
        cerr << "[" << names << "] = [";
        print(args...);
        cerr << "]\n";
    }
#else
    template<typename... Args>
    static void log(const string&, Args...) {}
#endif

private:
#ifndef ONLINE_JUDGE
    static void print() {}

    template<typename T, typename... Args>
    static void print(T a, Args... args) {
        cerr << a;
        if (sizeof...(args)) {
            cerr << ", ";
            print(args...);
        }
    }
#endif
};

#ifndef ONLINE_JUDGE
#define debug(...) Debug::log(#__VA_ARGS__, __VA_ARGS__)
#else
#define debug(...)
#endif

/*   /\_/\
*   (= ._.)
*   / >  \>
*/
