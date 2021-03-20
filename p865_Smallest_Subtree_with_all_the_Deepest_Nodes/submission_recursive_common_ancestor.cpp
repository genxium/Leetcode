int const MAXN = 500;
int const INVALID = -1;
int h[MAXN];

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
[3,5,1,6,2,0,8,null,null,7,4]
[1]
[0,1,3,null,2]
*/
class Solution {
public:
    int countDeepestChildren(TreeNode* root, int deepestH, TreeNode* &ans, int &ansCount) {
        if (!root) return 0;
        int tmp = 0;
        if (h[root->val] == deepestH) {
            tmp = 1;
        } else {
            tmp = countDeepestChildren(root->left, deepestH, ans, ansCount) + countDeepestChildren(root->right, deepestH, ans, ansCount);    
        }
        if (tmp > ansCount) {
            ansCount = tmp;
            ans = root;
        }
        return tmp;
    }
    
    void assignH(TreeNode* root, int lv, int &deepestH) {
        if (!root) return;
        deepestH = max(deepestH, lv);
        h[root->val] = lv;
        assignH(root->left, lv+1, deepestH);
        assignH(root->right, lv+1, deepestH);
    }
    
    TreeNode* subtreeWithAllDeepest(TreeNode* root) {
        memset(h, INVALID, sizeof h);
        int deepestH = INVALID;
        assignH(root, 0, deepestH);
        
        TreeNode* ans = NULL;
        int ansCount = 0;
        
        countDeepestChildren(root, deepestH, ans, ansCount);
        
        return ans;
    }
};
