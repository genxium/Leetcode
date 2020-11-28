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
    void count(TreeNode* root, unordered_map<int, int> &counter, int &localSum, int &highestFreq) {
      if (!root) return;
      localSum = root->val;
      if (root->left) {
        int leftSum = 0;
        count(root->left, counter, leftSum, highestFreq);
        localSum += leftSum;
      }
      if (root->right) {
        int rightSum = 0;
        count(root->right, counter, rightSum, highestFreq);
        localSum += rightSum;
      }
      
      ++counter[localSum];
      if (counter[localSum] <= highestFreq) return;
      highestFreq = counter[localSum];
    }
  
    vector<int> findFrequentTreeSum(TreeNode* root) {
      unordered_map<int, int> counter;
      int rtSum = 0, highestFreq = 0;
      count(root, counter, rtSum, highestFreq);
      vector<int> ans;
      for (auto &it : counter) {
        if (it.second != highestFreq) continue;
        ans.push_back(it.first);
      }
      return ans;
    }
};
