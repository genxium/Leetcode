bool const debug = false;
typedef uint64_t ULL;
int const MAXVAL = 100000;

struct FenwickTree {
    vector<ULL> bit;  // binary indexed tree
    int n;

    FenwickTree(int n) {
        this->n = n;
        bit.assign(n, 0);
    }

    FenwickTree(vector<int> &a) : FenwickTree(a.size()) {
        for (size_t i = 0; i < a.size(); i++)
            add(i, a[i]);
    }

    ULL sum(int r) {
        ULL ret = 0;
        for (; r >= 0; r = (r & (r + 1)) - 1)
            ret += bit[r];
        return ret;
    }

    ULL sum(int l, int r) {
        return sum(r) - sum(l - 1);
    }

    void add(int idx, int delta) {
        for (; idx < n; idx = idx | (idx + 1))
            bit[idx] += (ULL)delta;
    }
};

/*
test cases
["MKAverage","addElement","addElement","calculateMKAverage","addElement","calculateMKAverage","addElement","addElement","addElement","calculateMKAverage"]
[[3,1],[3],[1],[],[10],[],[5],[5],[5],[]]
["MKAverage","addElement","addElement","addElement","addElement","addElement","addElement","calculateMKAverage"]
[[6,1],[3],[1],[12],[5],[3],[4],[]]
*/
class MKAverage {
private:
    int m, k;
    FenwickTree *ftCnt = NULL, *ftSum = NULL;
    list<int> effEles; // a.k.a. the last m elements
    
    ULL sumFirstTSmallest(ULL t) {
        // Find the proper v such that "prefixCnt[v-1] < t &&　prefixCnt[v] >= t", i.e. the lower_bound for "t".
        if (debug) printf("sumFirstTSmallest, t:%d\n", t);
        int l = 0, r = MAXVAL, v = 0;
        while (l < r) {
            int v = ((l+r) >> 1);
            ULL prefixCnt = ftCnt->sum(v);
            if (prefixCnt < t) {
                l = v+1;
            } else {
                // prefixCnt >= t
                r = v;
            }
        }
        
        v = ((l+r) >> 1);
        if (debug) printf("\ttarget value is %d\n", v);
        
        ULL prefixCnt = ftCnt->sum(v);
        ULL d = prefixCnt-t;
        
        ULL ret = ftSum->sum(v);
        return ret-d*v;
    }
    
public:
    MKAverage(int m, int k) {
        this->m = m;
        this->k = k;
        ftCnt = new FenwickTree(MAXVAL+1); // the count of values of the last m elements
        ftSum = new FenwickTree(MAXVAL+1); // the sum of "count*value" of the last m elements
    }
    
    void addElement(int num) {
        int toEvict = 0;
        if (effEles.size() == m) {
            toEvict = effEles.front();
            effEles.pop_front();
        }
        effEles.push_back(num);
        
        if (toEvict > 0) {
            ftCnt->add(toEvict, -1);
            ftSum->add(toEvict, -toEvict);
        }
        
        ftCnt->add(num, 1);
        ftSum->add(num, num);
    }
    
    int calculateMKAverage() {
        if (effEles.size() < m) return -1;
        ULL s1 = sumFirstTSmallest(k);
        ULL s2 = sumFirstTSmallest(m-k);
        
        int denominator = m-2*k;
        return (s2 - s1)/denominator;
    }
    
    ~MKAverage() {
        delete ftCnt;
        delete ftSum;
    }
};

/**
 * Your MKAverage object will be instantiated and called as such:
 * MKAverage* obj = new MKAverage(m, k);
 * obj->addElement(num);
 * int param_2 = obj->calculateMKAverage();
 */
