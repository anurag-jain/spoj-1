/** ` Micro Mezzo Macro Flation -- Overheated Economy ., **/
 
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <vector>
 
using namespace std;
 
#define REP(i, n) for (int i=0;i<int(n);++i)
#define FOR(i, a, b) for (int i=int(a);i<int(b);++i)
#define REP_1(i, n) for (int i=1;i<=int(n);++i)
#define FOR_C(i, a, b) for (int b____=int(b),i=a;i<b____;++i)
#define DO(n) while(n--)
#define SZ(A) int(A.size())
#define PB push_back
#define MP(A, B) make_pair(A, B)
#define Rush int T____; RD(T____); DO(T____)
#pragma comment(linker, "/STACK:36777216")
#pragma GCC optimize ("O2")
 
typedef vector<int> VI;
template<class T> inline void RD(T &);
template<class T> inline void OT(const T &);
template<class T> inline T& _RD(T &x){ RD(x); return x;}
inline void RS(char *s){scanf("%s", s);}
template<class T0, class T1> inline void RD(T0 &x0, T1 &x1){RD(x0), RD(x1);}
template<class T0, class T1, class T2> inline void RD(T0 &x0, T1 &x1, T2 &x2){RD(x0), RD(x1), RD(x2);}
template<class T> inline void RST(T &A){memset(A, 0, sizeof(A));}
template<class T> inline void CLR(T &A){A.clear();}
template<class T> inline void checkMax(T &a,const T b){if (b>a) a=b;}
inline int _1(int i){return 1<<i;}
template<class T> inline void RD(T &x){char c; for (c = getchar(); c < '0'; c = getchar()); x = c - '0'; for (c = getchar(); c >= '0'; c = getchar()) x = x * 10 + c - '0';}
template<class T> inline void OT(const T &x){printf("%d\n", x);}
 
/* .................................................................................................................................. */
 
const int N = 10001, L = 15;
 
struct Vertex;
struct Edge;
struct Path;
 
#define null &V[0]
struct Vertex{
 
    vector<Edge*> adj; Edge *host;
    int depth, size, euler_index;
 
    int id();
} V[N];
 
struct Edge{
 
    Vertex *a, *b; Path *host;
    int w;
 
    inline bool isLight(){return host == NULL;}
    int id();
} E[N-1];
 
struct Path{
 
    VI leaf; int *key; Vertex *head;
    int size, res, a, b;
 
private:
 
#define root 1, 0, size
#define lx (x << 1)
#define rx (lx | 1)
#define m ((l + r) >> 1)
#define lc lx, l, m
#define rc rx, m+1, r
 
#define x_ (x ^ 1)
#define y (x >> 1)
 
#define Update key[x] = max(key[lx], key[rx])
 
    inline void _B(int x, int l, int r){
 
        if (l == r){
            key[x] = leaf[l];
            leaf[l] = x;
        }
        else {
            _B(lc), _B(rc), Update;
        }
    }
 
    inline void _Q(int x, int l, int r){
 
        if (a <= l && r <= b){
            checkMax(res, key[x]);
        }
        else {
            if (a <= m) _Q(lc);
            if (m < b) _Q(rc);
        }
    }
 
    inline void _M(int x){
        if (b < key[x]){
 
            key[x] = b;
 
            while (y){
                if (key[y] == key[x_]) return;
                key[y] = max(key[x], key[x_]), x >>= 1;
            }
 
        }
        else if (b > key[x]){
 
            key[x] = b;
 
            while (y){
                if (key[x] <= key[x_]) return;
                key[y] = key[x], x >>= 1;
            }
 
        }
    }
 
public:
 
    inline void Build(){
        size = SZ(leaf), key = new int [4 * size];
        --size, _B(root);
    }
 
    inline int Query(Vertex *_a, Vertex *_b){
        res = 0, a = _a->depth - head->depth, b = _b->depth - head->depth - 1, _Q(root);
        return res;
    }
 
    inline void Modify(Edge *e, int v){
        b = v, _M(leaf[e->a->depth - head->depth]);
    }
 
    int id();
} P[N / 2];
 
#undef y
 
inline bool elder(Vertex *a, Vertex *b){return a->depth < b->depth;}
int Vertex::id(){return this - V;}
int Edge::id(){return this - E;}
int Path::id(){return this - P;}
 
Vertex *ST[L][2*N];
int n, l, cnt, res;
 
#define arc u->adj[i]
 
inline void dfs(Vertex *u = &V[1], Vertex *p = null){
 
    u->size = 1;
    u->depth = p->depth + 1;
 
    ST[0][u->euler_index = ++cnt] = u;
 
    REP(i, SZ(u->adj)){
 
        Vertex *v = arc->a == u ? arc->b : arc->a;
 
        if (v != p){
            dfs(v, u), v->host = arc, ST[0][++cnt] = u;
            u->size += v->size;
        }
    }
}
 
// Heavy_Light_Decomposition
 
inline void hld(Vertex *u = &V[1], Path *t = NULL){
 
    REP(i, SZ(u->adj)){
 
        Vertex *v = arc->b;
 
        if (v == u) continue;
 
        if (v->size * 2 >= u->size){
            if (t == NULL) t = &P[l++], t->head = u;
            arc->host = t, t->leaf.PB(arc->w), hld(v, t);
        }
        else {
            hld(v, NULL);
        }
    }
}
 
inline Vertex *lca(Vertex *a, Vertex *b){
 
    int l = a->euler_index, r = b->euler_index;
 
    if (l > r) swap(l, r); ++r; int lv = log2(r - l);
 
    return min(ST[lv][l], ST[lv][r -_1(lv)], elder);
 
}
 
#undef arc
#define arc x->host
 
inline void Query(Vertex *y, Vertex *x){
 
    if (x == y) return;
 
    if (arc->isLight()){
        checkMax(res, arc->w), Query(y, arc->a);
    }
    else {
 
        Path *path = arc->host;
 
        if (path->head->depth <= y->depth){
            checkMax(res, path->Query(y, x));
        }
        else {
            checkMax(res, path->Query(path->head, x));
            Query(y, path->head);
        }
    }
}
 
int a, b; char cmd[5];
Vertex *x, *y, *z;
 
int main(){
 
    //freopen("in.txt", "r", stdin);
    //freopen("out.txt", "w", stdout);
    //ios::sync_with_stdio(false);
 
    Rush{
 
        // Initializing Phase ...
 
        FOR_C(i, 1, _RD(n)){
            RD(a, b, E[i].w), E[i].a = &V[a], E[i].b = &V[b];
            V[a].adj.PB(&E[i]), V[b].adj.PB(&E[i]);
        }
 
        cnt = 0, dfs();
 
        FOR(i, 1, n) if (elder(E[i].b, E[i].a)){
            swap(E[i].a, E[i].b);
        }
 
        l = 0, hld(); REP(i, l) P[i].Build();
 
        for ( int lv = 1; _1(lv) <= cnt ; ++ lv ){
            for ( int i = 1; i + _1(lv) <= cnt + 1; ++ i )
                ST[lv][i] = min(ST[lv - 1][i], ST[lv - 1][i + _1(lv - 1)], elder);
        }
 
        // Interaction Phase ...
 
        while (true){
 
            RS(cmd);
 
            if (cmd[0] == 'C'){
 
                RD(a, b); if (E[a].isLight()) E[a].w = b;
                else E[a].host->Modify(&E[a], b);
            }
            else if (cmd[0] == 'Q'){
                RD(a, b), x = &V[a], y = &V[b], z = lca(x, y);
                res = 0, Query(z, x), Query(z, y), OT(res);
            }
            else
                break;
        }
 
        // Rececling ....
 
        REP_1(i, n) CLR(V[i].adj), E[i].host = 0;
 
        REP(i, l){
            CLR(P[i].leaf);
            delete [] P[i].key;
        }
    }
}