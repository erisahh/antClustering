#include <bits/stdc++.h>
#define int long long
#define endl '\n'

// ≽(•⩊ •マ≼
using namespace std;

template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v) { // opcional
    os << "{";
    int n = (int)v.size();
    for (int i = 0; i < n; i++) {
        os << v[i];
        if (i < n - 1) os << ", ";
    }
    os << "}";
    return os;
}

void _print() { }
template <typename T, typename... U>
void _print(T a, U... b) {
    if (sizeof...(b)) {
        cerr << a << ", ";
        _print(b...);
    } else cerr << a;
}

#ifndef ONLINE_JUDGE
#define debug(x...) cerr << "[" << #x << "] = [", _print(x), cerr << "]" << endl
#else
#define debug(...)
#endif

void solve() {

}

signed main(){
	cin.tie(0)->sync_with_stdio(0);
    solve();	
}
/*   /\_/\
*   (= ._.)
*   / >  \>
*/

