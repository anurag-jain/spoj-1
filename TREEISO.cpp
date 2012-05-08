#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <vector>
#include <set>
#include <cstdio>
#include <iostream>
#include <memory.h>

using namespace std;

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

	int nextChar() {
		ensureCapacity();
		while (*curPos <= ' ') ++curPos;
		return *(curPos++);
	}
};


typedef vector<int> vi;
typedef vector<vi> vvi;

vvi children, subtreeLabels, tree, L;
vi pred, map;
int n;

bool compare(int a, int b) {
	return subtreeLabels[a] < subtreeLabels[b];
}

bool equals(int a, int b) {
	return subtreeLabels[a] == subtreeLabels[b];
}

void generateMapping(int r1, int r2) {
	map.resize(n);
	map[r1] = r2 - n;
	sort(children[r1].begin(), children[r1].end(), compare);
	sort(children[r2].begin(), children[r2].end(), compare);
	for (int i = 0; i < (int) children[r1].size(); i++) {
		int u = children[r1][i];
		int v = children[r2][i];
		generateMapping(u, v);
	}
}

vi findCenter(int offset = 0) {
	int cnt = n;
	vi a;
	vi deg(n);
	for (int i = 0; i < n; i++) {
		deg[i] = tree[i + offset].size();
		if (deg[i] <= 1) {
			a.push_back(i + offset);
			--cnt;
		}
	}
	while (cnt > 0) {
		vi na;
		for (int i = 0; i < (int) a.size(); i++) {
			int u = a[i];
			for (int j = 0; j < (int) tree[u].size(); j++) {
				int v = tree[u][j];
				if (--deg[v - offset] == 1) {
					na.push_back(v);
					--cnt;
				}
			}
		}
		a = na;
	}
	return a;
}

int dfs(int u, int p = -1, int depth = 0) {
	L[depth].push_back(u);
	int h = 0;
	for (int i = 0; i < (int) tree[u].size(); i++) {
		int v = tree[u][i];
		if (v == p)
			continue;
		pred[v] = u;
		children[u].push_back(v);
		h = max(h, dfs(v, u, depth + 1));
	}
	return h + 1;
}

bool rootedTreeIsomorphism(int r1, int r2) {
	L.assign(n, vi());
	pred.assign(2 * n, -1);
	children.assign(2 * n, vi());

	int h1 = dfs(r1);
	int h2 = dfs(r2);
	if (h1 != h2)
		return false;

	int h = h1 - 1;
	vi label(2 * n);
	subtreeLabels.assign(2 * n, vi());

	for (int i = h - 1; i >= 0; i--) {
		for (int j = 0; j < (int) L[i + 1].size(); j++) {
			int v = L[i + 1][j];
			subtreeLabels[pred[v]].push_back(label[v]);
		}
		/*for (int j = 0; j < (int) L[i].size(); j++) {
			int v = L[i][j];
			sort(subtreeLabels[v].begin(), subtreeLabels[v].end());
		}*/

		sort(L[i].begin(), L[i].end(), compare);

		for (int j = 0, cnt = 0; j < (int) L[i].size(); j++) {
			if (j && !equals(L[i][j], L[i][j - 1]))
				++cnt;
			label[L[i][j]] = cnt;
		}
	}

	if (!equals(r1, r2))
		return false;

	//generateMapping(r1, r2);
	return true;
}

bool treeIsomorphism() {
	vi c1 = findCenter();
	vi c2 = findCenter(n);
	if (c1.size() == c2.size()) {
		if (rootedTreeIsomorphism(c1[0], c2[0]))
			return true;
		else if (c1.size() > 1)
			return rootedTreeIsomorphism(c1[1], c2[0]);
	}
	return false;
}

int main() {
	#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	#endif

	//Scanner sc;

	//int cases = sc.nextInt();;
	int cases;
	cin >> cases;

	while (cases--) {

		//n = sc.nextInt();
		cin >> n;

		//set<pair<int,int> > e1, e2;

		vvi t1(n);
		for (int i = 0; i < n - 1; i++) {
			//int a=sc.nextInt();
			//int b=sc.nextInt();
			int a, b;
			scanf("%d%d", &a, &b);
			--a, --b;
			t1[a].push_back(b);
			t1[b].push_back(a);
			//e1.insert(make_pair(a, b));
		}

		vvi t2(n);
		for (int i = 0; i < n - 1; i++) {
			//int a=sc.nextInt();
			//int b=sc.nextInt();
			int a, b;
			scanf("%d%d", &a, &b);
			--a, --b;
			t2[a].push_back(b);
			t2[b].push_back(a);
			//e2.insert(make_pair(a, b));
		}

		tree.assign(2 * n, vi());
		for (int u = 0; u < n; u++) {
			for (int i = 0; i < t1[u].size(); i++) {
				int v = t1[u][i];
				tree[u].push_back(v);
			}
			for (int i = 0; i < t2[u].size(); i++) {
				int v = t2[u][i];
				tree[u + n].push_back(v + n);
			}
		}

		bool res = treeIsomorphism();
		cout << (res ? "YES" : "NO") << endl;

		/*
		if (res) {
			bool ok=true;
			for(set<pair<int,int> >::iterator it = e1.begin(); it!=e1.end(); it++){
				int a=map[it->first];
				int b=map[it->second];
				if(!e2.count(make_pair(a,b)) && !e2.count(make_pair(b,a))){
					ok=false;
					break;
				}				
			}
			if(!ok)
				cout<<"################### ERROR"<<endl;
		}
		*/
	}
}
