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
private:
    TreeNode *inorderTraversalPrevNode, *firstOutOfOrderNode, *secondOutOfOrderNode;
public:
    Solution() {
        this->inorderTraversalPrevNode = NULL;
        this->firstOutOfOrderNode = NULL;
        this->secondOutOfOrderNode = NULL;
    }
    
    void swapInt(int* pLeft, int* pRight) {
        int tmp = *pLeft;
        *pLeft = *pRight;
        *pRight = tmp;
    }
    
    void inorderTraverse(TreeNode* root) {
        if (NULL == root) return;
        if (root->left) {
            inorderTraverse(root->left);
        }
        if (NULL != this->inorderTraversalPrevNode && this->inorderTraversalPrevNode->val > root->val) {
            if (NULL == this->firstOutOfOrderNode) {
                //printf("#1 Got this->inorderTraversalPrevNode->val:%d > root->val:%d\n", this->inorderTraversalPrevNode->val, root->val);
                // Always start with the assumption that there's only 1 kink.
                this->firstOutOfOrderNode = inorderTraversalPrevNode;
                this->secondOutOfOrderNode = root;
            } else {
                //printf("#2 Got this->inorderTraversalPrevNode->val:%d > root->val:%d\n", this->inorderTraversalPrevNode->val, root->val);
                // If there's a second kink.
                this->secondOutOfOrderNode = root;
            }
        }
        this->inorderTraversalPrevNode = root;
        if (root->right) {
            inorderTraverse(root->right);
        }
    } 
    
    void recoverTree(TreeNode* root) {
        inorderTraverse(root);
        swapInt(&(this->firstOutOfOrderNode->val), &(this->secondOutOfOrderNode->val));
    }
};
