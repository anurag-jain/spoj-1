#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <memory.h>

using namespace std;

#define FOR(i,a,b) for (int i = (a); i < (b); ++i)
#define FIR(n) FOR(i,0,n)
#define FI FIR(n)
#define FJR(n) FOR(j,0,n)
#define FJ FJR(n)
#define GI(n) scanf("%d", &n);

typedef pair<int, int> PI;
typedef vector<PI> VPI;
typedef vector<int> VI;

#define MIN_VAL 0

int par[10010];
int parc[10010];

struct Path {
	int top;
	VI seq;
	VI tree;
	int lim;

	void init(int top) {this->top = top;}

	int go_setup(int p) {
		if (p >= lim) return tree[p];
		return tree[p] = max(go_setup(2*p), go_setup(2*p+1));
	}

	void setup() {
		lim = 1;
		while(lim < seq.size()) lim <<= 1;
		tree.resize(lim + lim);
		FIR(seq.size()) tree[lim+i] = parc[seq[i]];
		FOR(i, lim+seq.size(),lim+lim) tree[i]=MIN_VAL;
		go_setup(1);
	}

	void update(int seq_pos, int v) {
	  seq_pos += lim;
		tree[seq_pos] = v;
		while(seq_pos >>= 1) {
			tree[seq_pos] = max(tree[seq_pos<<1], tree[(seq_pos<<1)+1]);
		}
	}

	int go_q(int p, int l, int r, int a, int b) const {
		if (l == a && r == b) return tree[p];
		int m = (l+r) >> 1;

		int res = MIN_VAL;
		p*=2;
		if (a < m) res = go_q(p, l, m, a, min(m, b));
		if (b > m) res = max(res, go_q(p+1, m, r, max(a, m), b));
		return res;
	}

	// [a, b)
	int query(int a, int b) const {
		return go_q(1, 0, lim, a, b);
	}


	int size() {
		return 1 + seq.size();
	}

	void clear() {
		seq.clear();
		tree.clear();
	}
};

int n;
VI g[10010];
VI gc[10010];
Path paths[10010];
int path_cnt;
int edge_a[10010], edge_b[10010];

void input() {
	GI(n);
	int a,b,c;
	FOR(i, 1, n) {
		scanf("%d %d %d", &a, &b, &c);
		--a;--b;
		edge_a[i] =a; edge_b[i] = b;
		g[a].push_back(b); gc[a].push_back(c);
		g[b].push_back(a); gc[b].push_back(c);
	}
}

void cleanup() {
	FI g[i].clear(), gc[i].clear();
	FIR(path_cnt) paths[i].clear();
}

int inpos[10010], outpos[10010];
int subtree_cnt[10010];
int tpos;
int path_num[10010], path_pos[10010];

int go(int v, int parent) {
	inpos[v] = tpos++;
	par[v] = parent;

	subtree_cnt[v] = 1;
	FIR(g[v].size()) if (g[v][i] != parent) subtree_cnt[v] += go(g[v][i], v); else parc[v] = gc[v][i];

	outpos[v] = tpos++;
	return subtree_cnt[v];
}

void build_paths(int v, int par, int on_path) {
	FIR(g[v].size()) {
		int c = g[v][i];
		if (par == c) continue;

		if (subtree_cnt[v] <= 2*subtree_cnt[c]) {
			if (on_path < 0) {
			  on_path = path_cnt++;
				paths[on_path].init(v);
				//path_num[v] = on_path;
				//path_pos[v] = 0;
			}

			path_num[c] = on_path;
			path_pos[c] = paths[on_path].size();
			paths[on_path].seq.push_back(c);
			build_paths(c, v, on_path);
		} else
			build_paths(c, v, -1);
	}
}

void preprocess() {
	tpos = 0;
	go(0, -1);

	memset(path_num, -1, n* sizeof(path_num[0]));
	path_cnt = 0;
	build_paths(0, -1, -1);
	FIR(path_cnt) paths[i].setup();
}

bool is_parent_of(int a, int b) {
	return inpos[a] < inpos[b] && outpos[a] > outpos[b];
}

int direct_q(int anc, int v) {
	bool on_path = path_num[anc] >= 0;
	int res = MIN_VAL;
	while (v != anc) {
		if (path_num[v] < 0) res = max(res, parc[v]), v=par[v];
		else if (!on_path || path_num[v] != path_num[anc]) {
			const Path& p = paths[path_num[v]];
			res = max(res, p.query(0, path_pos[v]));
			v = p.top;
		} else {
			const Path& p = paths[path_num[v]];
			res = max(res, p.query(path_pos[anc], path_pos[v]));
			break;
		}
	}

	return res;
}

int do_query(int v1, int v2) {
	if (v1 == v2) return 0;
	if (is_parent_of(v2, v1)) return do_query(v2, v1);

	int lca = v1;
	while (!is_parent_of(lca, v2))  {
		if (path_num[lca] < 0)
			lca = par[lca];
		else {
			const Path& p = paths[ path_num[lca] ];
			int second_in_path = p.seq[0];
			if (!is_parent_of(second_in_path, v2)) {
			   lca = p.top;
			} else {
				int l = 0, r = p.seq.size() -1;
				while (l < r) {
					int m = (l+r+1) >> 1;
					if (is_parent_of(p.seq[m], v2)) l = m; else r = m-1;
				}
				lca = p.seq[l];
			}
		}
	}

	int res = direct_q(lca, v2);
	if (v1 != lca) res = max(res, direct_q(lca, v1));
	return res;
}

void update(int v1, int v2, int val) {
	int v = inpos[v1] < inpos[v2] ? v2 :v1;
	parc[v] = val;
	if (path_num[v] >= 0) {
		paths[path_num[v]].update(path_pos[v]-1, val);
	}
}

int main() {
#ifndef ONLINE_JUDGE
freopen("input.txt", "rt", stdin);
#endif

	int tests;
	GI(tests);
	while (tests--) {
		input();
		preprocess();

		char buf[20];
		int a, b;

		while (true) {
			scanf("%s", buf);
			if (buf[0] == 'D') break;
			scanf("%d %d", &a, &b);

			if (buf[0] == 'Q') {
				printf("%d\n", do_query(a-1, b-1));
			}

			if (buf[0] == 'C') {
				update(edge_a[a], edge_b[a], b);
			}
		}


		cleanup();
	}
}