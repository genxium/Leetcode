bool debug = false;
int const MAXQUERIES = 1001;
int const MAXPOS = 101000001; // 101000001 == 100,000,000 + 1,000,000

//int constexpr MAXLEVELS = (int)ceil(log2((double)MAXPOS));
int const MAXLEVELS = 27;
int const MAXNODES = 4*MAXLEVELS*MAXQUERIES;
int const INVALID = -1;
int const LAZY_TYPE_NONE = -1;
int const LAZY_TYPE_SET = 0;
int const LAZY_TYPE_ADD = +1;

int tree[MAXNODES];     // "tree[v]" = max value of this node's range
int lazyVal[MAXNODES];  // "lazyVal[v]" holds the unified value for "lazyType[v]"
int lazyType[MAXNODES];
int lcIdx[MAXNODES], rcIdx[MAXNODES];
int dfsIdx = 0;

int getLcIdx(int v) {
    if (INVALID == lcIdx[v]) {
        lcIdx[v] = (++dfsIdx);
    }

    return lcIdx[v];
}

int getRcIdx(int v) {
    if (INVALID == rcIdx[v]) {
        rcIdx[v] = (++dfsIdx);
    }

    return rcIdx[v];
}

void apply(int v, const int l, const int r, const int newLazyVal, const int newLazyType) {
    if (r <= l) return;
    if (LAZY_TYPE_SET == newLazyType) {
        if (debug) {
            printf("\tapplied with v=%d, l=%d, r=%d, newLazyVal=%d, newLazyType=%d\n", v, l, r, newLazyVal, newLazyType);
        }
        // lazy set overrides any previous update
        tree[v] = newLazyVal;
        lazyVal[v] = newLazyVal;
        lazyType[v] = newLazyType;
    }
}

void push_down(int v, int l, int r) {
    if (r <= l) return;
    if (LAZY_TYPE_NONE == lazyType[v]) return;
    /*
    if (debug) {
        printf("push_down with v=%d, l=%d, r=%d\n", v, l, r);
    }
    */
    int m = ((l + r) >> 1);
    if (l < m) {
        apply(getLcIdx(v), l, m, lazyVal[v], lazyType[v]);
    }
    if (m < r) {
        apply(getRcIdx(v), m, r, lazyVal[v], lazyType[v]);
    }
    lazyVal[v] = INVALID;
    lazyType[v] = LAZY_TYPE_NONE;
}

void range_update(int v, int l, int r, int ql, int qr, const int newLazyVal, const int newLazyType) {
    if (r <= l) return;
    if (qr <= l || ql >= r) { return; }
    /*
    if (debug) {
        printf("range_update with v=%d, l=%d, r=%d, ql=%d, qr=%d, newLazyVal=%d, newLazyType=%d\n", v, l, r, ql, qr, newLazyVal, newLazyType);
    }
    */
    if (ql <= l && r <= qr) {
        apply(v, l, r, newLazyVal, newLazyType);
    } else {
        push_down(v, l, r);
        tree[v] = INVALID;
        int m = ((l + r) >> 1);
        if (l < m) {
            int lcIdxVal = getLcIdx(v);
            range_update(lcIdxVal, l, m, ql, qr, newLazyVal, newLazyType);
            tree[v] = max(tree[lcIdxVal], tree[v]);
        }

        if (m < r) {
            int rcIdxVal = getRcIdx(v); 
            range_update(rcIdxVal, m, r, ql, qr, newLazyVal, newLazyType);
            tree[v] = max(tree[rcIdxVal], tree[v]);
        }
    }
}

int range_max(int v, int l, int r, int ql, int qr) {
    if (r <= l) return INVALID;
    if (qr <= l || ql >= r) return INVALID;
    if (ql <= l && r <= qr) { 
        // full cover
        int ret = tree[v];
        if (debug) {
            printf("range_max#1 with v=%d, l=%d, r=%d, ql=%d, qr=%d: ret=%d\n", v, l, r, ql, qr, ret);
        }
        
        return ret; 
    }
    int ret = INVALID;
    push_down(v, l, r); // [REMINDER] Might create "lcIdx[v]" and "rcIdx[v]" if applicable.
    int m = ((l + r) >> 1);
    if (INVALID != lcIdx[v]) {
        // [REMINDER] Don't create "lcIdx[v]" here if not already created.
        ret = max(range_max(lcIdx[v], l, m, ql, qr), ret);
    }

    if (INVALID != rcIdx[v]) {
        // [REMINDER] Don't create "rcIdx[v]" here if not already created.
        ret = max(range_max(rcIdx[v], m, r, ql, qr), ret);
    }
    
    if (debug) {
        printf("range_max#2 with v=%d, l=%d, r=%d, ql=%d, qr=%d: ret=%d\n", v, l, r, ql, qr, ret);
    }

    return ret;
}

class Solution {
public:
    vector<int> fallingSquares(vector<vector<int>>& positions) {
        dfsIdx = 0;
        memset(tree, INVALID, MAXNODES*sizeof(int));
        memset(lazyVal, INVALID, MAXNODES*sizeof(int));
        memset(lazyType, INVALID, MAXNODES*sizeof(int));
        memset(lcIdx, INVALID, MAXNODES*sizeof(int));
        memset(rcIdx, INVALID, MAXNODES*sizeof(int));
        if (debug) {
            printf("Initialized all arrays\n");
        }
        vector<int> ans;
        int rootV = dfsIdx, rootL = 0, rootR = MAXPOS;
        for (auto &p : positions) {
            int ql = p[0], qr = ql+p[1];
            int lastHInRange = range_max(rootV, rootL, rootR, ql, qr);
            if (INVALID == lastHInRange) {
                lastHInRange = 0;
            }
            if (debug) {
                printf("lastHInRange=%d for [ql=%d, qr=%d)\n", lastHInRange, ql, qr);
            }
            int newH = lastHInRange + p[1];
            range_update(rootV, rootL, rootR, ql, qr, newH, LAZY_TYPE_SET);
            int maxOverallH = range_max(rootV, rootL, rootR, rootL, rootR);
            if (INVALID == maxOverallH) {
                maxOverallH = 0;
            }
            if (debug) {
                printf("maxOverallH=%d for [ql=%d, qr=%d)\n", maxOverallH, ql, qr);
            }
            ans.push_back(maxOverallH);
        }

        return ans;
    }
};
