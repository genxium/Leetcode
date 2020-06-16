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
    TreeNode* buildTreeWithAnchors(vector<int>& preorder, int preorderL, int preorderR, vector<int>& inorder, int inorderL, int inorderR) {
        int rootVal = preorder[preorderL];
        int rootValPosAtInorderSequence;
        for (rootValPosAtInorderSequence = inorderL; rootValPosAtInorderSequence < inorderR; ++rootValPosAtInorderSequence) {
            if (rootVal == inorder[rootValPosAtInorderSequence]) {
                break;
            }
        }
        
        TreeNode* root = new TreeNode(rootVal);
        
        if (rootValPosAtInorderSequence != inorderL) {
            int leftSubtreeSize = (rootValPosAtInorderSequence - inorderL);
            TreeNode* leftRoot = buildTreeWithAnchors(preorder, preorderL+1, preorderL+1+leftSubtreeSize, inorder, inorderL, inorderL+leftSubtreeSize);
            root->left = leftRoot;
        } else {
            // There is no left subtree in this case.
        }
        
        if (rootValPosAtInorderSequence != inorderR-1) {
            int rightSubtreeSize = (inorderR-(rootValPosAtInorderSequence+1));
            TreeNode* rightRoot = buildTreeWithAnchors(preorder, preorderR-rightSubtreeSize, preorderR, inorder, rootValPosAtInorderSequence+1, inorderR);
            root->right = rightRoot;
        } else {
            // There is no right subtree in this case.
        }
        return root;
    }
    
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        if (0 == preorder.size()) return NULL;
        int preorderL = 0, preorderR = preorder.size();
        int inorderL = 0, inorderR = inorder.size();
        TreeNode *root = buildTreeWithAnchors(preorder, preorderL, preorderR, inorder, inorderL, inorderR);
        return root;
    }
};
