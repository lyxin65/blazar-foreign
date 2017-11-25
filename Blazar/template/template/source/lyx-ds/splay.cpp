struct Node {
    int fa, c[2];
    int val, sum, sz;
    int rev, tag, inv;
    int lmx, lmn, rmx, rmn;
} T[N];
int root;

#define fa(o) T[o].fa
#define lc(o) T[o].c[0]
#define rc(o) T[o].c[1]
#define sz(o) T[o].sz
#define val(o) T[o].val
#define sum(o) T[o].sum
#define rev(o) T[o].rev
#define tag(o) T[o].tag
#define inv(o) T[o].inv
#define lmx(o) T[o].lmx
#define rmx(o) T[o].rmx
#define lmn(o) T[o].lmn
#define rmn(o) T[o].rmn
#define ctype(x) (x == rc(fa(x)))

inline void setc(int x, int y, const bool d) { if(x) fa(x) = y; if(y) T[y].c[d] = x; }
void set_tag(int o, int v) {
    if(o == 0) return;
    rev(o) = inv(o) = 0;
    val(o) = tag(o) = v;
    sum(o) = v * sz(o);
    lmx(o) = lmn(o) = rmx(o) = rmn(o) = 0;
    relax(lmn(o), sum(o)); tense(lmx(o), sum(o));
    relax(rmn(o), sum(o)); tense(rmx(o), sum(o));
}
void set_inv(int o) {
    if(o == 0) return;
    inv(o) ^= 1; sum(o) *= -1; val(o) *= -1;
    swap(lmn(o), lmx(o)); lmn(o) *= -1; lmx(o) *= -1;
    swap(rmn(o), rmx(o)); rmn(o) *= -1; rmx(o) *= -1;
}
void set_rev(int o) {
    if(o == 0) return;
    rev(o) ^= 1;
    swap(lc(o), rc(o));
    swap(lmx(o), rmx(o));
    swap(lmn(o), rmn(o));
}
inline void pushdown(int o) {
    if(tag(o)) set_tag(lc(o), tag(o)), set_tag(rc(o), tag(o)), tag(o) = 0;
    if(rev(o)) set_rev(lc(o)), set_rev(rc(o)), rev(o) = 0;
    if(inv(o)) set_inv(lc(o)), set_inv(rc(o)), inv(o) = 0;
}
inline void update(int o) {
    if(o == 0) return;
    sz(o) = sz(lc(o)) + sz(rc(o)) + 1;
    sum(o) = sum(lc(o)) + sum(rc(o)) + val(o);
    lmx(o) = lmn(o) = rmx(o) = rmn(o) = 0;
    relax(lmn(o), lmn(lc(o))); relax(lmn(o), sum(lc(o)) + val(o) + lmn(rc(o)));
    tense(lmx(o), lmx(lc(o))); tense(lmx(o), sum(lc(o)) + val(o) + lmx(rc(o)));
    relax(rmn(o), rmn(rc(o))); relax(rmn(o), sum(rc(o)) + val(o) + rmn(lc(o)));
    tense(rmx(o), rmx(rc(o))); tense(rmx(o), sum(rc(o)) + val(o) + rmx(lc(o)));
}
inline void rotate(int o) {
    int p = fa(o), mark = ctype(o);
    if(fa(p)) setc(o, fa(p), ctype(p));
    else fa(o) = 0, root = o;
    setc(T[o].c[mark ^ 1], p, mark);
    setc(p, o, mark ^ 1);
    update(p); update(o);
}
void splay(int x, int rt = 0) {
    static int q[N], top;
    q[top = 1] = x;
    for(int i = x; i != rt; i = fa(i))
        q[++top] = fa(i);
    while(top) pushdown(q[top--]);
    while(fa(x) != rt) {
        if(fa(fa(x)) != rt) {
            if(ctype(x) == ctype(fa(x)))
                rotate(fa(x));
            else rotate(x);
        }
        rotate(x);
    }
}
int find(int rank) {
    int x = root;
    while(true) {
        pushdown(x);
        int s = sz(lc(x)) + 1;
        if(rank < s) x = lc(x);
        else if(rank == s) return x;
        else rank -= s, x = rc(x);
    }
}
void init() {
    for(int i = 1; i <= n; ++i) {
        sz(i) = 1;
        if(src[i] == '(') {
            val(i) = sum(i) = 1;
            lmx(i) = rmx(i) = 1;
            lmn(i) = rmn(i) = 0;
        } else {
            val(i) = sum(i) = -1;
            lmx(i) = rmx(i) = 0;
            lmn(i) = rmn(i) = -1;
        }
    }
    for(int i = 1; i <= n; ++i) {
        setc(i, i + 1, 0);
        update(i + 1);
    }
    root = n + 1;
}
inline int interval(int l, int r) {
    int x, y;
    splay(x = find(r + 1));
    if(l - 1) {
        splay(y = find(l - 1), x);
        return rc(y);
    } else {
		return lc(x);
	}
}
