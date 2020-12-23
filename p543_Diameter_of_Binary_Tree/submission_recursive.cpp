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
    void dfs(TreeNode* root, int &lPathLen, int &rPathLen, int &ans) {
        if (!root) return;
        if (root->left) {
            int llPathLen = 0, lrPathLen = 0;
            dfs(root->left, llPathLen, lrPathLen, ans);
            lPathLen = max(1+llPathLen, 1+lrPathLen);
        }
        if (root->right) {
            int rlPathLen = 0, rrPathLen = 0;
            dfs(root->right, rlPathLen, rrPathLen, ans);
            rPathLen = max(1+rlPathLen, 1+rrPathLen);
        }
        ans = max(ans, lPathLen+rPathLen);
    }
    
    int diameterOfBinaryTree(TreeNode* root) {
        int ans = 0;
        int lPathLen = 0, rPathLen = 0;
        dfs(root, lPathLen, rPathLen, ans);
        return ans;
    }
};
