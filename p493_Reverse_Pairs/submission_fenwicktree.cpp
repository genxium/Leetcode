typedef int64_t LL;

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
        if (l == 0) return sum(r);
        else return sum(r) - sum(l-1);
    }

    void add(int idx, int delta) {
        for (; idx < n; idx = idx | (idx + 1))
            bit[idx] += delta;
    }
};

/*
test case#1
[2147483647,2147483647,2147483647,2147483647,2147483647,2147483647]
*/
class Solution {
public:
    int reversePairs(vector<int>& nums) {
        int n = nums.size();
        if (0 == n) return 0;
        map<LL, int> numsWithIdx; // value->orderIdx;
        for (int i = 0; i < nums.size(); ++i) numsWithIdx[nums[i]] = 1;
        int orderIdx = 0;
        for (auto &it : numsWithIdx) {
            it.second = orderIdx++;
        }
        int ans = 0;
        int m = numsWithIdx.size();
        FenwickTree ftIns(m);
        for (int j = 0; j < n; ++j) {
            int orderIdx = numsWithIdx[nums[j]];
            LL targetValueLow = (LL)nums[j] + (LL)nums[j]; 

            // printf("@j:%d, nums[j:%d] == %d, orderIdxOfNumsJ:%d, targetValueLow:%d\n", j, j, nums[j], orderIdx, targetValueLow);
            auto targetNumsWithIdxIt = numsWithIdx.upper_bound(targetValueLow);

            if (targetNumsWithIdxIt != numsWithIdx.end()) {
                int thatOrderIdx = targetNumsWithIdxIt->second;
                // printf("\ttargetNumsWithIdxIt: {value:%d, thatOrderIdx:%d}\n", targetNumsWithIdxIt->first, thatOrderIdx);

                int beforeAndGreaterThanCount = ftIns.sum(thatOrderIdx, m-1); // total count of those "> 2*nums[j]" and are already inserted into the FenwickTree, note that "m-1" is just the largest possible "orderIdx"

                // printf("\tbeforeAndGreaterThanCount:%d\n", beforeAndGreaterThanCount);

                ans += beforeAndGreaterThanCount;
            }
            ftIns.add(orderIdx, 1);
        }
        return ans;
    }
};
