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
    void search(TreeNode *root, int &nowK, int targetK, int* &pAns) {
      if (NULL != pAns) {
        return;
      }
    
      // in order traversal
      if (root->left) {
        search(root->left, nowK, targetK, pAns);
      } 
      ++nowK;
      if (nowK == targetK) {
        pAns = &(root->val);
        return;
      }
      if (root->right) {
        search(root->right, nowK, targetK, pAns);
      }
    }
  
    int kthSmallest(TreeNode* root, int k) {
      /*
      [FOR THE FOLLOW UP]
      Maintain a new field "TreeNode.size" dynamically.
      */
      int *pAns = NULL;
      int nowK = 0;
      search(root, nowK, k, pAns);
      return (*pAns);
    }
};
