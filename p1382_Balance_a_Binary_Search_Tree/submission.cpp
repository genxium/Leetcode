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
    void computeHeightAndBalance(TreeNode** pRoot, int &hOut, int &hLeftOut, int &hRightOut) {
      if (NULL == pRoot) {
        return;
      }
      /*
      [WARNING]
      
      The variable "locallyBalanced" is used here to help trigger multiple-local-rotations when necessary, because the input BST was not kept balanced along with CRUD-operations of each node. Consider "test case #3".
      */
      bool locallyBalanced = false;
      int hL = 0, hR = 0;
      int hLL = 0, hLR = 0, hRL = 0, hRR = 0;
      
      while (false == locallyBalanced) {
        TreeNode *root = *pRoot;
        if (NULL != root->left) {
          computeHeightAndBalance(&(root->left), hL, hLL, hLR);        
        }
        if (NULL != root->right) {
          computeHeightAndBalance(&(root->right), hR, hRL, hRR);        
        }
        //printf("For root->val:%d, before balancing attemp, computed hL:%d, hLL:%d, hLR:%d, hR:%d, hRL:%d, hRR:%d\n", root->val, hL, hLL, hLR, hR, hRL, hRR);
      
        if (hR >= 2+hL) {
          if (hRR >= 1+hL) {
            //printf("\tFor root->val:%d, triggering type \"RR\" rotation\n", root->val);

            TreeNode* origRL = root->right->left;

            // type "RR" rotation
            TreeNode* newRight = root->right->right;
            TreeNode* newRoot = root->right;
            TreeNode* newLeft = root; 
          
            // make necessary reconnections
            newLeft->right = origRL; // origRL > newLeft, complying with BST definition
            newRoot->left = newLeft;
            *pRoot = newRoot;
          } else {
            // hRL >= 1+hL
            //printf("\tFor root->val:%d, triggering type \"RL\" rotation\n", root->val);
          
            TreeNode* origRLL = root->right->left->left;
            TreeNode* origRLR = root->right->left->right;
          
            // type "RL" rotation, in fact I should convert it to type "RR" first, but anyway let me cheat...
            TreeNode* newRight = root->right;
            TreeNode* newRoot = root->right->left;
            TreeNode* newLeft = root; 
          
            // make necessary reconnections
            newLeft->right = origRLL; // origRLL > newLeft, complying with BST definition
            newRight->left = origRLR;
            newRoot->left = newLeft;
            newRoot->right = newRight;
            *pRoot = newRoot;
          }
        } else if (hL >= 2+hR) {
          if (hLL >= 1+hR) {
            //printf("\tFor root->val:%d, triggering type \"LL\" rotation\n", root->val);
            TreeNode* origLR = root->left->right;

            // type "LL" rotation
            TreeNode* newLeft = root->left->left;
            TreeNode* newRoot = root->left;
            TreeNode* newRight = root; 
          
            // make necessary reconnections
            newRight->left = origLR; // origLR < newRight, complying with BST definition
            newRoot->right = newRight;
            *pRoot = newRoot;
          } else {
            // hLR >= 1+hR
            //printf("\tFor root->val:%d, triggering type \"LR\" rotation\n", root->val);
          
            TreeNode* origLRR = root->left->right->right;
            TreeNode* origLRL = root->left->right->left;
          
            // type "LR" rotation, in fact I should convert it to type "LL" first, but anyway let me cheat...
            TreeNode* newLeft = root->left;
            TreeNode* newRoot = root->left->right;
            TreeNode* newRight = root; 
          
            // make necessary reconnections
            newRight->left = origLRR; // origLRR < newRight, complying with BST definition
            newLeft->right = origLRL;
            newRoot->right = newRight;
            newRoot->left = newLeft;
            *pRoot = newRoot;
          }
        } else { 
          locallyBalanced = true;
        }
      }
      
      hLeftOut = hL;
      hRightOut = hR;
      hOut = 1+max(hL, hR);
    }
  
    TreeNode* balanceBST(TreeNode* root) {
      /*
      test case #1
      [1,null,2,null,3,null,4,null,null]
      
      test case #2
      [19,10,null,4,17,null,5]
      
      test case #3
      [1,null,15,14,17,7,null,null,null,2,12,null,3,9,null,null,null,null,11]
      */
      if (NULL == root) return NULL;
      TreeNode** pRoot = &root;
      int h = 0, hL = 0, hR = 0;
      computeHeightAndBalance(pRoot, h, hL, hR);
      return *pRoot;
    }
};
