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


class Solution {
public:
    void solve(TreeNode* root, int targetSum, unordered_map<int, int> &suffixSumCounter, int &ans) {
      if (!root) return;
      unordered_map<int, int> newSuffixSumCounter;
      newSuffixSumCounter[root->val] += 1;
      for (auto &it : suffixSumCounter) {
        int newSum = it.first + root->val;
        newSuffixSumCounter[newSum] += it.second;
      }
      
      ans += newSuffixSumCounter[targetSum];
      
      solve(root->left, targetSum, newSuffixSumCounter, ans);
      solve(root->right, targetSum, newSuffixSumCounter, ans);
    }
  
    int pathSum(TreeNode* root, int sum) {
      unordered_map<int, int> suffixSumCounter;
      int ans = 0;
      solve(root, sum, suffixSumCounter, ans);
      return ans;
    }
};
