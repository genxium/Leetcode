bool const debug = false;

struct FenwickTree {
    vector<int> bit;  // binary indexed tree
    int n;

    FenwickTree(int n) {
        this->n = n;
        bit.assign(n, 0);
    }

    FenwickTree(vector<int> &a) : FenwickTree(a.size()) {
        for (size_t i = 0; i < a.size(); i++)
            add(i, a[i]);
    }

    int sum(int r) {
        int ret = 0;
        for (; r >= 0; r = (r & (r + 1)) - 1)
            ret += bit[r];
        return ret;
    }

    int sum(int l, int r) {
        return sum(r) - sum(l - 1);
    }

    void add(int idx, int delta) {
        for (; idx < n; idx = idx | (idx + 1))
            bit[idx] += delta;
    }
};

/*
test cases
[0]
[0,1]
[1,0,2]
[1,2,0]
*/
class Solution {
public:
    bool isIdealPermutation(vector<int>& A) {
        int n = A.size();
        int globalInversion = 0, localInversion = 0;
        FenwickTree ftIns(n);
        for (int i = 0; i < A.size(); ++i) {
            int num = A[i];
            if (i > 0) {
                localInversion += (num < A[i-1]);
            }
            int beforeAndGreaterThanCount = ftIns.sum(num+1, n-1);
            globalInversion += beforeAndGreaterThanCount;
            ftIns.add(num, 1);
        }
        return globalInversion == localInversion;  
    }
};
