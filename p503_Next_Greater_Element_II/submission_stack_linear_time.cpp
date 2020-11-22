typedef pair<int, int> PII;

class Solution {
public:
    vector<int> nextGreaterElements(vector<int>& nums) {
      stack<PII> stk;
      int n = nums.size();
      vector<int> ans(n, -1);

      for (int i = 0; i < (n+n); ++i) {
        int effectiveI = (i%n);
        while (!stk.empty() && stk.top().first < nums[effectiveI]) {
          ans[stk.top().second] = nums[effectiveI]; 
          stk.pop();
        }
        
        // now that "stk.empty() || stk.top().first >= nums[effectiveI]"
        if (i < n) {
          stk.push({nums[effectiveI], i});  
        }
      }
      
      return ans;
    }
};
