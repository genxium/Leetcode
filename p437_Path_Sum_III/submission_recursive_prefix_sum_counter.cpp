/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

unordered_map<int, int> prefixSumCounter;

class Solution {
public:
    void solve(TreeNode* root, int prefixSumByFar, int targetSum, int &ans) {
      if (!root) return;
      int newSum = prefixSumByFar + root->val;
      int prefixSumTargetVal = newSum - targetSum;
      ans += prefixSumCounter[prefixSumTargetVal];

      prefixSumCounter[newSum] += 1;
      
      solve(root->left, newSum, targetSum, ans);
      solve(root->right, newSum, targetSum, ans);
      
      prefixSumCounter[newSum] -= 1;
    }
  
    int pathSum(TreeNode* root, int sum) {
      int ans = 0, prefixSumByFar = 0;
      prefixSumCounter[prefixSumByFar] = 1;
      solve(root, prefixSumByFar, sum, ans);
      return ans;
    }
};
