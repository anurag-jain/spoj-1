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
//#include <ctime>
//clock_t start=clock();
//fprintf(stderr,"time=%.3lfsec\n",0.001*(clock()-start));
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
typedef vector<vpii> Graph;

#define fr(i,a,b) for(int i(a),_b(b);i<=_b;++i)
#define frd(i,a,b) for(int i(a),_b(b);i>=_b;--i)
#define rep(i,n) for(int i(0),_n(n);i<_n;++i)
#define reps(i,a) fr(i,0,sz(a)-1)
#define all(a) a.begin(),a.end()
#define pb push_back
#define mp make_pair
#define clr(x,a) memset(x,a,sizeof(x))
#define findx(a,x) (find(all(a),x)-a.begin())
#define two(X) (1LL<<(X))
#define contain(S,X) (((S)&two(X))!=0)

const int dr[]={0,-1,0,1,-1,1, 1,-1};
const int dc[]={1,0,-1,0, 1,1,-1,-1};
const double eps=1e-9;

template<class T> void print(vector<T> const &v) {ostringstream os; for(int i=0; i<(int)v.size(); ++i){if(i)os<<' ';os<<v[i];} cout<<os.str()<<endl;}
template<class T> int sz(const T&c){return (int)c.size();}
template<class T> void srt(T&c){std::sort(c.begin(),c.end());}
template<class T> void checkmin(T &a,T b){if(b<a) a=b;}
template<class T> void checkmax(T &a,T b){if(b>a) a=b;}
template<class T> T sqr(T x){return x*x;}
template<class T, class U> T cast (U x) { ostringstream os; os<<x; T res; istringstream is(os.str()); is>>res; return res; }
template<class T> vector<T> split(string s, string x=" ") {vector<T> res; for(int i=0;i<s.size();i++){string a; while(i<(int)s.size()&&x.find(s[i])==string::npos)a+=s[i++]; if(!a.empty())res.push_back(cast<T>(a));} return res;}
template<class T> bool inside(T r,T c,T R, T C){return r>=0 && r<R && c>=0 && c<C;}

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
		bool sign = false;
		if(*curPos == '-') {
			sign = true;
			++curPos;
		}
		int res = 0;
		while (*curPos > ' ') 
			res = res * 10 + (*(curPos++) & 15);
		return sign ? -res : res;
	}
};

int solveSlow(int cnt[], int a[], int haveLeft, int haveRight, int needLeft, int needRight) {
	int res = 0;
	for (int i = haveRight + 1; i <= needRight; ++i) {
		res += cnt[a[i]] == 0 ? 1 : 0;
		++cnt[a[i]];
	}
	for (int i = haveLeft; i < needLeft; ++i) {
		res += cnt[a[i]] == 1 ? -1 : 0;
		--cnt[a[i]];
	}
	for (int i = haveRight + 1; i <= needRight; ++i) {
		--cnt[a[i]];
	}
	for (int i = haveLeft; i < needLeft; ++i) {
		++cnt[a[i]];
	}
	return res;
}


int head[256][256];
int Left[256000];
int Right[256000];
int Next[256000];
int res[256000];
int cnt[65536];
int a[65536];

int main( int argc, char* argv[] ) {
	#ifndef ONLINE_JUDGE
	freopen("input.txt","r",stdin);
	#endif	
	
	Scanner sc;
	int n = sc.nextInt();
	rep(i,n)a[i]=sc.nextInt();
	map<int,int> m;
	int id = 0;
	for (int i = 0; i < n; i++) {
		if (!m.count(a[i]))
			m[a[i]] = id++;
		a[i] = m[a[i]];
	}
	int blockSize = (int) sqrt((double)n);
	int blocks = (n - 1) / blockSize + 1;
	for (int i = 0; i < blocks; i++)
		for (int j = 0; j < blocks; j++) head[i][j]=-1;
	int t = sc.nextInt();
	for (int i = 0; i < t; i++) {
		Left[i] = sc.nextInt() - 1;
		Right[i] = sc.nextInt() - 1;
		int lb = Left[i] / blockSize;
		int rb = Right[i] / blockSize;
		Next[i] = head[lb][rb];
		head[lb][rb] = i;
	}
	for (int leftBlock = 0; leftBlock < blocks; leftBlock++) {
		rep(i,id)cnt[i]=0;
		int curAnswer = 0;
		for (int rightBlock = leftBlock; rightBlock < blocks; rightBlock++) {
			for (int i = head[leftBlock][rightBlock]; i >= 0; i = Next[i]) {
				res[i] = curAnswer
					+ solveSlow(cnt, a, leftBlock * blockSize, rightBlock * blockSize - 1, Left[i], Right[i]);
			}
			if (rightBlock + 1 < blocks) {
				for (int i = 0; i < blockSize; i++) {
					int cur = a[rightBlock * blockSize + i];
					curAnswer += cnt[cur] == 0 ? 1 : 0;
					++cnt[cur];
				}
			}
		}
	}

	rep(i,t)printf("%d\n", res[i]);
	return 0;
}
