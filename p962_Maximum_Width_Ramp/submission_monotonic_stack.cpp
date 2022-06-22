/*
test cases
[6,0,8,2,1,5]
[9,8,1,0,1,9,4,0,4,1]
*/
class Solution {
public:
    int maxWidthRamp(vector<int>& nums) {
        stack<int> monostk; // top is kept smallest
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            if (monostk.empty() || nums[monostk.top()] >= nums[i]) {
                monostk.push(i);                
            }
        }
        int ans = INT_MIN;
        for (int i = n-1; i >= 0; --i) {
            while (!monostk.empty() && nums[monostk.top()] <= nums[i]) {
                ans = max(ans, i-monostk.top());
                monostk.pop(); // nums[monostk.top()] is no longer needed, because we'd be checking for a smaller i' < i in the next iteration which wouldn't make a larger "ans" for the same "monostk.top()"
            }
        }
        return ans;
    }
};
