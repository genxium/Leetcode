class Solution {
public:
    vector<int> solve(vector<int> &nums) {
      int n = nums.size();
      vector<int> productOfLeft(n, 1); // productOfLeft[i] = multiply(nums[j] | 0 <= j < i)
      for (int i = 1; i < n; ++i) {
        productOfLeft[i] = nums[i-1]*productOfLeft[i-1];
      }
      
      vector<int> productOfRight(n, 1); // productOfRight[i] = multiply(nums[j] | i < j < n)
      for (int i = n-2; i >= 0; --i) {
        productOfRight[i] = productOfRight[i+1]*nums[i+1];
      }
      
      vector<int> toRet(n, 0);
      for (int i = 0; i < n; ++i) {
        toRet[i] = productOfLeft[i]*productOfRight[i];
      }
      return toRet;
    }
  
    vector<int> solveConstSpace(vector<int> &nums) {
      int n = nums.size();
      vector<int> toRet(n, 1); // toRet[i] == productOfLeft[i] = multiply(nums[j] | 0 <= j < i)
      for (int i = 1; i < n; ++i) {
        toRet[i] = nums[i-1]*toRet[i-1];
      }
      
      int productOfRight = 1;
      for (int i = n-1; i >= 0; --i) {
        toRet[i] *= productOfRight;
        productOfRight *= nums[i];
      }
      
      return toRet;
    }

    vector<int> productExceptSelf(vector<int>& nums) {
      vector<int> ans = solveConstSpace(nums);
      return ans;
    }
};
