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

/*
dp[root][ROBBED] = dp[root->left][NOT_ROBBED] + dp[root->right][NOT_ROBBED] + root->val 

dp[root][NOT_ROBBED] = max{dp[root->left][NOT_ROBBED], dp[root->left][ROBBED]} + max{dp[root->right][NOT_ROBBED], dp[root->right][ROBBED]}
*/

class Solution {
public:
  void search(TreeNode* root, int &robbedProfit, int &notRobbedProfit) {
    if (NULL == root) return;
    
    int lRobbedProfit = 0, lNotRobbedProfit = 0;
    search(root->left, lRobbedProfit, lNotRobbedProfit);
    
    int rRobbedProfit = 0, rNotRobbedProfit = 0;
    search(root->right, rRobbedProfit, rNotRobbedProfit);
    
    robbedProfit = root->val;
    robbedProfit += lNotRobbedProfit; 
    robbedProfit += rNotRobbedProfit;
    
    notRobbedProfit += max(lRobbedProfit, lNotRobbedProfit);
    notRobbedProfit += max(rRobbedProfit, rNotRobbedProfit);
  }
  
  int rob(TreeNode* root) {
    int robbedProfit = 0, notRobbedProfit = 0;
    search(root, robbedProfit, notRobbedProfit);
    int ans = max(robbedProfit, notRobbedProfit);
    return ans;
  }
};
