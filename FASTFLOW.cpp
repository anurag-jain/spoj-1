#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <climits>
#include <numeric>
#include <memory.h>
#include <ctime>
using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<ll> vl;
typedef vector<vl> vvl;
typedef vector<bool> vb;
typedef vector<vb> vvb;
typedef vector<double> vd;
typedef vector<vd> vvd;
typedef vector<string> vs;
typedef pair<int, int> pii;
typedef vector<pii> vpii;
typedef pair<double, double> pdd;
typedef vector<pdd> vpdd;

#define fr(e,a,b) for(int e(a),_b(b);e<=_b;++e)
#define frd(e,a,b) for(int e(a),_b(b);e>=_b;--e)
#define rep(e,n) for(int e(0),_n(n);e<_n;++e)
#define reps(e,a) fr(e,0,sz(a)-1)
#define all(a) a.begin(),a.end()
#define pb push_back
#define mp make_pair
#define clr(u,a) memset(u,a,sizeof(u))
#define findx(a,u) (find(all(a),u)-a.begin())
#define two(X) (1LL<<(X))
#define contain(S,X) (((S)&two(X))!=0)

template<class T> void print(T const &x) {ostringstream os; os<<x; cout<<os.str()<<endl;}
template<class T> int sz(const T&c){return (int)c.size();}
template<class T> void srt(T&c){std::sort(c.begin(),c.end());}
template<class T> void checkmin(T &a,T b){if(b<a) a=b;}
template<class T> void checkmax(T &a,T b){if(b>a) a=b;}
template<class T> T sqr(T x){return x*x;}
template<class T, class U> T cast (U x) { ostringstream os; os<<x; T res; istringstream is(os.str()); is>>res; return res; }
template<class T> vector<T> split(string s, string x=" ") {vector<T> res; for(int i=0;i<s.size();i++){string a; while(i<(int)s.size()&&x.find(s[i])==string::npos)a+=s[i++]; if(!a.empty())res.push_back(cast<T>(a));} return res;}
template<class T> T inside(T r,T c,T R, T C){return r>=0 && r<R && c>=0 && c<C;}

char buf[1<<20];
int pos;

int nextInt() {
	while (buf[pos] <= ' ') ++pos;
	int res = 0;
	while (buf[pos] > ' ') 
		res = res * 10 + (buf[pos++] & 15);
	return res;
}

const int maxnodes = 5000;

int nodes = maxnodes, src, dest;
short dist[maxnodes], q[maxnodes], work[maxnodes];

struct Edge {
	short to,rev;
	int f, cap;
};

vector<Edge> g[maxnodes];

void addEdge(int s, int t, int cap){
	Edge a = {t, g[t].size(), 0, cap};
	Edge b = {s, g[s].size(), 0, cap};
	g[s].push_back(a);
	g[t].push_back(b);
}

bool dinic_bfs() {
	fill(dist, dist + nodes, -1);
	dist[src] = 0;
	int qt = 0;
	q[qt++] = src;
	for (int qh = 0; qh < qt; qh++) {
		int u = q[qh];
		for (int j = 0; j < (int) g[u].size(); j++) {
			Edge &e = g[u][j];
			int v = e.to;
			if (dist[v] < 0 && e.f < e.cap) {
				dist[v] = dist[u] + 1;
				q[qt++] = v;
			}
		}
	}
	return dist[dest] >= 0;
}

int dinic_dfs(int u, int f) {
	if (u == dest)
		return f;
	for (short &i = work[u]; i < (int) g[u].size(); i++) {
		Edge &e = g[u][i];
		if (e.cap <= e.f) continue;
		int v = e.to;
		if (dist[v] == dist[u] + 1) {
			int df = dinic_dfs(v, min(f, e.cap - e.f));
			if (df > 0) {
				e.f += df;
				g[v][e.rev].f -= df;
				return df;
			}
		}
	}
	return 0;
}

ll maxFlow(int _src, int _dest) {
	src = _src;
	dest = _dest;
	ll result = 0;
	while (dinic_bfs()) {
		fill(work, work + nodes, 0);
		while (int delta = dinic_dfs(src, INT_MAX))
			result += delta;
	}
	return result;
}

int main( int argc, char* argv[] ) {
	#ifndef ONLINE_JUDGE
	freopen("input.txt","r",stdin);
	#endif	

	fread(buf, 1, sizeof(buf), stdin);
	
	int n = nextInt();
	int m = nextInt();

	nodes = n;

	rep(i,m){
		int a = nextInt();
		int b = nextInt();
		int c = nextInt();
		addEdge(a - 1, b - 1, c);
	}

	clock_t start=clock();
	ll res = maxFlow(0, n-1);
	fprintf(stderr,"time=%.3lfsec\n",0.001*(clock()-start));

	printf("%lld\n", res);

	return 0;
}
