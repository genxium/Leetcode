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
test case#1
[100,50,150,2,80,null,200,null,null,78,82]
4

test case#2
[50,30,70,null,40,60,80]
50

test case#3
[2,0,33,null,1,25,40,null,null,11,31,34,45,10,18,29,32,null,36,43,46,4,null,12,24,26,30,null,null,35,39,42,44,null,48,3,9,null,14,22,null,null,27,null,null,null,null,38,null,41,null,null,null,47,49,null,null,5,null,13,15,21,23,null,28,37,null,null,null,null,null,null,null,null,8,null,null,null,17,19,null,null,null,null,null,null,null,7,null,16,null,null,20,6]
33
*/
class Solution {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
      if (!root) return NULL;
      TreeNode* toRet = root;
      if (key == root->val) {
        if (!root->left && !root->right) {
          toRet = NULL;
        } else if (!root->left && root->right) {
          toRet = root->right;
        } else if (root->left && !root->right) {
          toRet = root->left;
        } else {
          // root->left && root->right
          auto r = root->right;
          if (r->left) {
            TreeNode *prev = r, *cur = r->left;
            while (cur->left) {
              prev = cur;
              cur = cur->left;
            }
            // now that "NULL != cur && NULL == cur->left"
            prev->left = cur->right;
            toRet = cur;
            toRet->left = root->left;
            toRet->right = root->right;
          } else {
            toRet = r;
            toRet->left = root->left;
            // no need to update "toRet->right"
          }
        }
      } else if (key < root->val) {
        root->left = deleteNode(root->left, key);
      } else {
        // key > root->val
        root->right = deleteNode(root->right, key);
      }
      return toRet;
    }
};
