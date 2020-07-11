struct FenwickTree {
    vector<int> bit;  // binary indexed tree
    int n;

    FenwickTree(int n) {
        this->n = n;
        bit.assign(n, 0);
    }

    FenwickTree(vector<int> a) : FenwickTree(a.size()) {
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

class Solution {
public:
    vector<int> countSmaller(vector<int>& nums) {
        int n = nums.size();
        if (0 == n) return {};
        map<int, int> numsWithIdx; // value->orderIdx;
        for (int i = 0; i < nums.size(); ++i) numsWithIdx[nums[i]] = 1;
        int orderIdx = 0;
        for (auto &it : numsWithIdx) {
          it.second = orderIdx++;
        }
        vector<int> ans;
        FenwickTree ftIns(numsWithIdx.size());
        for (auto rit = nums.rbegin(); rit != nums.rend(); ++rit) {
          int orderIdx = numsWithIdx[*rit];
          int beforeAndLessThanCount = ftIns.sum(orderIdx-1);
          ans.push_back(beforeAndLessThanCount);
          ftIns.add(orderIdx, 1);
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
