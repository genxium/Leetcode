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
    void genPath(TreeNode* root, vector<int> &candidate, int &tmpSum, int &targetSum, vector<vector<int>> &ans) {
        if (NULL == root) {
            return;
        }
        candidate.push_back(root->val);
        tmpSum += root->val;
        if (NULL == root->left && NULL == root->right) {
            if (tmpSum == targetSum) {
                vector<int> clone(candidate);
                ans.push_back(clone);            
            }
        } else {
            if (root->left) {
                genPath(root->left, candidate, tmpSum, targetSum, ans);
            }
            if (root->right) {
                genPath(root->right, candidate, tmpSum, targetSum, ans);
            }
        }
        // backtracking
        candidate.pop_back();
        tmpSum -= root->val;
    }
    
    vector<vector<int>> pathSum(TreeNode* root, int sum) {
        vector<int> candidate;
        int tmpSum = 0;
        vector<vector<int>> ans;
        genPath(root, candidate, tmpSum, sum, ans);
        return ans;
    }
}; 
