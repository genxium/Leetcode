class Solution {
public:
    int minMoves(vector<int>& nums) {
        // The trick is to understand that "increment n-1 elements per step" is equivalent to "decrementing 1 largest element per step".
        int minNum = INT_MAX;
        for (auto &num : nums) {
            if (num < minNum) minNum = num;
        }
        int ans = 0;
        for (int i = 0; i < nums.size(); ++i) {
            ans += (nums[i]-minNum);
        }
        return ans;
    }
};
