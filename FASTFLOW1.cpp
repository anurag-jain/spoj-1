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

const int BUF_SIZE = 65536;
char input[BUF_SIZE];

struct Scanner {
	char* curPos;

	Scanner() {
		fread(input, 1, sizeof(input), stdin);
		curPos = input;
	}

	void ensureCapacity(){
		int size = input + BUF_SIZE - curPos;
		if (size < 100) {
			memcpy(input, curPos, size);
			fread(input + size, 1, BUF_SIZE - size, stdin);
			curPos = input;
		}
	}

	int nextInt() {
		ensureCapacity();
		while (*curPos <= ' ') ++curPos;
		int res = 0;
		while (*curPos > ' ') 
			res = res * 10 + (*(curPos++) & 15);
		return res;
	}
};

const int maxnodes = 10000;
const int maxedges = 600000;

int src, dest, edges, nodes;
int head[maxedges], dist[maxnodes], Q[maxnodes];
int last[maxnodes], prev[maxedges], work[maxnodes];
int flow[maxedges], cap[maxedges];

void graphClear(int _nodes) {
	fill(last, last + _nodes, -1);
	edges = 0;
	nodes = _nodes;
}

void addEdge(int u, int v, int cap1, int cap2) {
	head[edges] = v;
	cap[edges] = cap1;
	flow[edges] = 0;
	::prev[edges] = last[u];
	last[u] = edges++;
	head[edges] = u;
	cap[edges] = cap2;
	flow[edges] = 0;
	::prev[edges] = last[v];
	last[v] = edges++;
}

bool dinic_bfs() {
	fill(dist, dist + nodes, -1);
	dist[src] = 0;
	int sizeQ = 0;
	Q[sizeQ++] = src;
	for (int i = 0; i < sizeQ; i++) {
		int u = Q[i];
		for (int e = last[u]; e >= 0; e = ::prev[e]) {
			int v = head[e];
			if (dist[v] < 0 && flow[e] < cap[e]) {
				dist[v] = dist[u] + 1;
				Q[sizeQ++] = v;
			}
		}
	}
	return dist[dest] >= 0;
}

int dinic_dfs(int u, int f) {
	if (u == dest)
		return f;
	for (int &e = work[u]; e >= 0; e = ::prev[e]) {
		int v = head[e];
		if (dist[v] == dist[u] + 1 && flow[e] < cap[e]) {
			int df = dinic_dfs(v, min(f, cap[e] - flow[e]));
			if (df > 0) {
				flow[e] += df;
				flow[e ^ 1] -= df;
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
		copy(last, last + nodes, work);
		while (int delta = dinic_dfs(src, INT_MAX))
			result += delta;
	}
	return result;
}

int main( int argc, char* argv[] ) {
	/*
	{
		freopen("input.txt","w",stdout);
		int n=10000;
		int m=100000;
		cout<<n<<" "<<m<<endl;
		vvb used(n, vb(n));
		rep(i,m){
			int a,b;
			while(1){
				a=rand()%n;
				b=rand()%n;
				if(a>b)swap(a,b);
				if(a!=b && !used[a][b])break;
			}
			used[a][b]=true;
			int c=rand()*rand()+1;
			cout<<a+1<<" "<<b+1<<" "<<c<<endl;
		}
		exit(0);
	}
	*/

	#ifndef ONLINE_JUDGE
	freopen("input.txt","r",stdin);
	#endif	

	Scanner sc;
	int n = sc.nextInt();
	int m = sc.nextInt();

	graphClear(n);

	rep(i,m){
		int a = sc.nextInt();
		int b = sc.nextInt();
		int c = sc.nextInt();
		addEdge(a-1, b-1, c, c);
	}

	clock_t start=clock();
	ll res = maxFlow(0, n-1);
	fprintf(stderr,"time=%.3lfsec\n",0.001*(clock()-start));

	printf("%lld\n", res);

	return 0;
}
