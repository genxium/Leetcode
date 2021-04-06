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
    TreeNode* dfs(vector<int>& pre, int preL, int preR, vector<int>& post, int postL, int postR) {
        if (preL > preR) return NULL;
        TreeNode* ret = NULL;
        int rootVal = pre[preL]; 
        if (rootVal != post[postR]) return NULL; // must equal "post[postR]"
        int preLeftBm = 0, postLeftBm = 0; // both bitmasks
        int lChildSizeUpper = preR-preL; // [preL+1, preR]
        for (int lChildSize = 0; lChildSize <= lChildSizeUpper; ++lChildSize) {
            int rChildSize = lChildSizeUpper-lChildSize;
            if (lChildSize > 0) {
                preLeftBm |= (1 << pre[preL+lChildSize]);
                postLeftBm |= (1 << post[postL-1+lChildSize]);    
            }
            if (preLeftBm != postLeftBm) continue;
            // In this case we can have a try to split here
            
            TreeNode* lChild = dfs(pre, preL+1, preL+lChildSize, post, postL, postL-1+lChildSize);
            if (lChildSize > 0 && NULL == lChild) continue;
            TreeNode* rChild = dfs(pre, preL+lChildSize+1, preR, post, postL+lChildSize, postR-1);
            if (rChildSize > 0 && NULL == rChild) continue;
            
            ret = new TreeNode(rootVal);
            ret->left = lChild;
            ret->right = rChild;
            break;
        }
        
        return ret;
    }
    
    TreeNode* constructFromPrePost(vector<int>& pre, vector<int>& post) {
        return dfs(pre, 0, pre.size()-1, post, 0, post.size()-1);
    }
};
