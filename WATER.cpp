#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <queue>

#include <memory.h>

using namespace std;

typedef vector<int> VI;
typedef pair<int, int> PI;
typedef vector<PI> VPI;

#define FOR(i,a,n) for (int i = (a); i < (n); ++i)
#define FORE(i,a,n) for (int i = (a); i <= (n); ++i)
#define FORD(i,a,b) for (int i = (a); i >= (b); --i)
#define REP(i,n) FOR(i,0,n)
#define REPE(i,n) FORE(i,0,n)
#define LL long long
#define FIR(n) REP(i,n)
#define FJR(n) REP(j,n)
#define ALL(v) v.begin(), v.end()

#define FI FIR(n)
#define FJ FJR(n)
#define FR(i,a) FOR(i,a,n)
#define REPN(i) REP(i,n)

int h[102][102];
int f[102][102];
int vis[102][102];

int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

int n, m;

PI do_pair(int r, int c) {
	return make_pair(h[r][c], (r << 8 ) | c);
}

int main() {
#ifndef ONLINE_JUDGE
	freopen("input.txt", "rt", stdin);
#endif

	int cases; scanf("%d", &cases);
	while (cases--) {
		scanf("%d %d", &n, &m);

		memset(h, 0, sizeof h);
		memset(vis, 0, sizeof vis);

		FORE(i, 1, n) FORE(j, 1, m) 
			scanf("%d", h[i] + j);

		if (n == 1 || m == 1) {
			printf("0\n");
			continue;
		}
		
		priority_queue<PI, VPI, greater<PI> > q;

		FORE(i, 0, n+1) vis[i][0] = vis[i][m+1] = true;
		FORE(j, 0, m+1) vis[0][j] = vis[n+1][j] = true;
		FORE(i, 1, n) {
			vis[i][1] = true;
			f[i][1] = 0;
			q.push(do_pair(i, 1));

			vis[i][m] = true;
			f[i][m] = 0;
			q.push(do_pair(i, m));
		}

		FOR(i, 2, m) {
			vis[1][i] = true;
			f[1][i] = 0;
			q.push(do_pair(1, i));

			vis[n][i] = true;
			f[n][i] = 0;
			q.push(do_pair(n, i));
		}

		while (!q.empty()) {
			PI p = q.top(); q.pop();
			int r = p.second >> 8, c = p.second & 255;
			int _h = f[r][c];
			
			FIR(4) {
				int rr = r + dx[i], cc = c + dy[i];
				if (!vis[rr][cc]) {
					vis[rr][cc] = true;
					f[rr][cc] = h[r][c] <= h[rr][cc] ? _h : max(_h, h[r][c]);
					q.push(do_pair(rr, cc));
				}
			}
		}

		int res = 0;
		FORE(i, 1, n) FORE(j, 1, m) if (f[i][j]>h[i][j]) 
			res += f[i][j] - h[i][j];
		printf("%d\n", res);
	}
}
