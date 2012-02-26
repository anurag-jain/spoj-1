/*
 * $File: dynalca.cpp
 * $Date: Sat Jun 04 16:20:08 2011 +0800
 * $Author: Zhou Xinyu <zxytim@gmail.com>
 */

#include <cstdio>
#define NDEBUG
#include <cassert>

const int N = 100005;
int n, m;
struct Node
{
	Node *ch[2], *par, *realpar;
} node[N], *pnode[N];
Node *NIL = node;
void init(Node &n)
{
	n.ch[0] = n.ch[1] = n.par = n.realpar = NIL;
}

void init()
{
	for (int i = 0; i <= n; i  ++)
		init(node[i]), pnode[i] = node + i;
}

void rotate(Node *cur, bool dir)
{
	Node *son = cur->ch[!dir];
	assert(son != NIL);
	assert(son->par == cur);
	(cur->ch[!dir] = son->ch[dir])->par = cur;
	son->par = cur->par;
	(son->ch[dir] = cur)->par = son;
	if (son->par != NIL)
		son->par->ch[cur == son->par->ch[1]] = son;
}

void splay(Node *cur, Node *par0 = NIL)
{
	Node *par, *gpar;
	while ((par = cur->par) != par0)
	{
		if ((gpar = par->par) == par0)
			rotate(par, cur == par->ch[0]);
		else
		{
			bool dcur = (cur == par->ch[1]),
				 dpar = (par == gpar->ch[1]);
			if (dcur != dpar)
				rotate(par, !dcur), rotate(gpar, !dpar);
			else rotate(gpar, !dpar), rotate(par, !dcur);
		}
	}
}

Node *find_head(Node *n)
{
	splay(n);
	while (n->ch[0] != NIL)
		n = n->ch[0];
	splay(n);
	return n;
}

void split(Node *cur)
{
	splay(cur);
	if (cur->ch[1] != NIL)
	{
		cur->ch[1]->par = NIL;
		cur->ch[1] = NIL;
	}
}

int expose(int x)
{
	Node *cur = node + x;
	int ret = x;
	split(cur);
	for (; ;)
	{
		cur = find_head(cur);
		if (cur->realpar == NIL)
			break;
		Node *par = cur->realpar;
		ret = par - node;
		split(par);
		assert(par->ch[1] == NIL);
		assert(cur->ch[0] == NIL);
		(par->ch[1] = cur)->par = par;
	}
	return ret;
}

void link(int a, int b) // a is the root
{
	assert(node[a].realpar == NIL);
	node[a].realpar = node + b;

//	expose(b);
	split(node + b);
	assert(node[b].ch[1] == NIL);

	splay(node + a);
	assert(node[a].ch[0] == NIL);
	(node[b].ch[1] = node + a)->par = node + b;
	node[a].realpar = node + b;

//	expose(a);
}

void cut(int x)
{
	splay(node + x);
	if (node[x].ch[0])
	{
		node[x].ch[0]->par = NIL;
		node[x].ch[0] = NIL;
	}
	node[x].realpar = NIL;

	/*
	expose(x);
	splay(node[x].par);
	assert(node + x == node[x].par->ch[1]);
	node[x].realpar = NIL;
	Node *par = node[x].par;
	node[x].par = par->ch[1] = NIL;
	*/
}

int lca(int a, int b)
{
	expose(a);
	return expose(b);
}

void do_print(int x)
{
	if (node[x].ch[0] != NIL)
	{
		assert(node[x].ch[0]->par == node + x);
		fprintf(stderr, "%d lch: %d\n", x, node[x].ch[0] - node);
		do_print(node[x].ch[0] - node);
	}
	if (node[x].ch[1] != NIL)
	{
		assert(node[x].ch[1]->par == node + x);
		fprintf(stderr, "%d rch: %d\n", x, node[x].ch[1] - node);
		do_print(node[x].ch[1] - node);
	}
}

void print(int x)
{
	do_print(x);
	fprintf(stderr, "\n");
}

char ans[N * 10], *ptr = ans;
void out_ans(int x)
{
	static char buf[10];
	int cnt = 0;
	while (x)
		buf[cnt ++] = x % 10 + '0', x /= 10;
	while (cnt --)
		*(ptr ++) = buf[cnt];
	*(ptr ++) = '\n';
}

void do_out_ans()
{
	if (ptr != ans)
	{
		*(ptr - 1) = 0;
		puts(ans);
	}
}

int main()
{
	const int BUFFER_LEN = N * 10;
	static char buffer[BUFFER_LEN];
	char *ptr = buffer, *buf_end = ptr + 1;
#define PTR_NEXT() \
	{ \
		ptr ++; \
		if (ptr == buf_end) \
		{ \
			ptr = buffer; \
			buf_end = buffer + fread(buffer, 1, BUFFER_LEN, stdin); \
		} \
	}
#define READ_INT(_x_) \
	{ \
		while ((*ptr < '0' || *ptr > '9') && *ptr != '-') \
			PTR_NEXT(); \
		bool _nega_ = false; \
		if (*ptr == '-') \
		{ \
			_nega_ = true; \
			PTR_NEXT(); \
		} \
		__typeof(_x_) register _n_ = 0; \
		while (*ptr >= '0' && *ptr <= '9') \
		{ \
			_n_ = _n_ * 10 + *ptr - '0'; \
			PTR_NEXT(); \
		} \
		if (_nega_) \
			_n_ = - _n_; \
		(_x_) = (_n_); \
	}
#define READ_STR(_s_) \
	{ \
		while (*ptr == ' ' || *ptr == '\n' || *ptr == '\r' || *ptr == '\t') \
			PTR_NEXT(); \
		char *_p_ = (_s_); \
		while (!(*ptr == ' ' || *ptr == '\n' || *ptr == '\r' || *ptr == '\t')) \
		{ \
			*(_p_ ++) = *ptr; \
			PTR_NEXT(); \
		} \
		*_p_ = 0; \
	}
#define READ_DOUBLE(_d_) \
	{ \
		while ((*ptr < '0' || *ptr > '9') && *ptr != '.') \
			PTR_NEXT(); \
		double register _r_ = 0, _c_ = 1; \
		bool _f_ = 0; \
		while ((*ptr >= '0' && *ptr <= '9') || *ptr == '.') \
		{ \
			if (*ptr == '.') \
				_f_ = 1; \
			else \
			{ \
				if (_f_) \
				{ \
					_c_ *= 0.1; \
					_r_ += (*ptr - '0') * _c_; \
				} \
				else _r_ = _r_ * 10 + *ptr - '0'; \
			} \
			PTR_NEXT(); \
		} \
		(_d_) = _r_; \
	}
	PTR_NEXT();

	//scanf("%d%d", &n, &m);
	READ_INT(n); READ_INT(m);
	init();
	char cmd[5];
	int a, b;
	while (m --)
	{
		READ_STR(cmd); READ_INT(a);
		//scanf("%s%d", cmd, &a);
		if (cmd[0] == 'l')
		{
			READ_INT(b);
//			scanf("%d", &b);
			if (cmd[1] == 'i')
				link(a, b);
			else
			{
				out_ans(lca(a, b));
//				printf("%d\n", lca(a, b));
			}
		}
		else if (cmd[0] == 'c') 
			cut(a);
	}
	do_out_ans();
	return 0;
}

