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
    void dfs(TreeNode* root, int& oneCnt) {
        if (!root) return;
        if (root->val == 1) oneCnt += 1;
        
        int lCnt = 0, rCnt = 0;
        dfs(root->left, lCnt);
        oneCnt += lCnt;
        if (0 == lCnt) root->left = NULL; // TODO: delete root->left; ? 
            
        dfs(root->right, rCnt);
        oneCnt += rCnt;
        if (0 == rCnt) root->right = NULL; // TODO: delete root->right; ? 
    }
    
    TreeNode* pruneTree(TreeNode* root) {
        int oneCnt = 0;
        dfs(root, oneCnt);
        if (0 == oneCnt) return NULL;
        return root;
    }
};
