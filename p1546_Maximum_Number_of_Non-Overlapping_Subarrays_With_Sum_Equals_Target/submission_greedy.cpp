class Solution {
public:
    int maxNonOverlapping(vector<int>& nums, int target) {
        int prefixSum = 0, ans = 0;
        /*
        The code is easy, it's more important to prove the correctness of why the greedy approach works :)
        
        Consider that we've found the first pair (i, j) such that "sum(nums[i, ..., j]) == target", then even if the second pair (u, v) sufficed that "sum(nums[u, ..., v]) == target && i < u < j && j < v", such (u, v) wouldn't be accounted for ans.
        
        That's because "1 + count(nums[j+1, ..., n], target)" is always ">= 1 + count(nums[v+1, ..., n])".
        */
        unordered_map<int, int> counter;
        for (int i = 0; i < nums.size(); ++i) {
            if (counter.empty()) {
                counter[0] = 0;
            } 
            prefixSum += nums[i];
            if (counter.find(prefixSum-target) != counter.end()) {
                ++ans;
                prefixSum = 0;
                counter.clear();
            } else {
                counter[prefixSum] = 1;
            }
        }
        return ans;
    }
};
