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
    void reverseInorderTrav(TreeNode* root, int &prefixSum) {
        if (!root) return;
        if (root->right) {
            reverseInorderTrav(root->right, prefixSum);
        }
        int val = root->val;
        root->val += prefixSum;
        prefixSum += val;
        if (root->left) {
            reverseInorderTrav(root->left, prefixSum);
        }
    }
    
    TreeNode* convertBST(TreeNode* root) {
        int prefixSum = 0;
        reverseInorderTrav(root, prefixSum);
        return root;
    }
};
