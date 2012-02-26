/** ` Micro Mezzo Macro Flation -- Overheated Economy ., **/
#include <iostream>
#include <cstdio>
using namespace std;
#define REP_1_C(i, n) for (int n____=int(n),i=1;i<=n____;++i)
#define DO(n) while(n--)
#define DO_C(n) int n____ = n; while(n____--)
#define Rush int T____; RD(T____); DO(T____)
template<class T> inline void RD(T &);
template<class T> inline void OT(const T &);
template<class T> inline T& _RD(T &x){ RD(x); return x;}
inline int RD(){ int x; RD(x); return x;}
template<class T0, class T1, class T2, class T3> inline void RD(T0 &x0, T1 &x1, T2 &x2, T3 &x3){RD(x0), RD(x1), RD(x2), RD(x3);}
template<class T> inline T max(T a, T b, T c){return max(max(a, b), c);}
template<class T> inline void RD(T &x){scanf("%d", &x);}
template<class T> inline void OT(const T &x){printf("%d\n", x);}
 
/* .................................................................................................................................. */
 
const int N = 10001;
 
struct Seg{
    int ss, ls, rs, ms;
} T[4*N];
int A[N], n, a, b;
 
#define root 1, 1, n
#define lx (x << 1)
#define rx (lx + 1)
#define lc lx, l, m
#define rc rx, m+1, r
 
void Update(Seg &x, Seg l, Seg r){
    x.ss = l.ss + r.ss;
    x.ls = max(l.ls, l.ss + r.ls);
    x.rs = max(r.rs, r.ss + l.rs);
    x.ms = max(l.ms, r.ms, l.rs + r.ls);
}
 
void Build(int x, int l, int r){
 
    if (l == r){
        T[x].ss = T[x].ls = T[x].rs = T[x].ms = A[l];
    }
    else{
        int m = (l + r) >> 1;
        Build(lc), Build(rc);
        Update(T[x], T[lx], T[rx]);
    }
}
 
Seg _Query(int x, int l, int r){
    if (a <= l && r <= b)
        return T[x];
    else {
        int m = (l + r) >> 1;
        if (b <= m) return _Query(lc); if (m < a) return _Query(rc);
        Seg res; Update(res, _Query(lc), _Query(rc)); return res;
    }
}
 
Seg Query(int _a, int _b){
    if (_a > _b) return T[0];
    a = _a, b = _b;
    return _Query(root);
}
 
int main(){
 
    //freopen("in.txt", "r", stdin);
    //freopen("out.txt", "w", stdout);
    //ios::sync_with_stdio(false);
 
    Rush{
        REP_1_C(i, _RD(n)) RD(A[i]);
        Build(root);
 
        int x1, y1, x2, y2; DO_C(RD()){
            RD(x1, y1, x2, y2);
            if (y1 < x2) OT( Query(x1, y1).rs + Query(y1 + 1, x2 - 1).ss + Query(x2, y2).ls );
            else OT( max( Query(x2, y1).ms, Query(x1, x2-1).rs + Query(x2, y2).ls, Query(x1, y1).rs + Query(y1+1, y2).ls) );
        }
    }
 
}