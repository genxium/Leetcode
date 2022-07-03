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
test cases
[3,0,0]
[0,3,0]
[0,0,null,0,0,4,null,null,0,3]
[5,0,0,null,null,0,0,3,null,null,0,null,null,null,0]
*/
bool debug = false;

class Solution {
public:
    int dfs(TreeNode* root, int& ans) {
        // returns "sum(#coins)-sum(#nodes)" at and under root
        if (!root) return 0;
        int lres = dfs(root->left, ans), rres = dfs(root->right, ans);
        // by now we "bubbled up" the lack or excess of coins in the whole left subtree to "root->left", and the same argument is applicable to the right subtree as well
        ans += abs(lres) + abs(rres);
        int lackOrExcess = (root->val-1) + lres + rres; 
        // by now we "bubbled up" the lack or excess of coins in the whole root tree to "root"
        return lackOrExcess; 
    }

    int distributeCoins(TreeNode* root) {
        int ans = 0;
        int res = dfs(root, ans);
        return ans;
    }
};
