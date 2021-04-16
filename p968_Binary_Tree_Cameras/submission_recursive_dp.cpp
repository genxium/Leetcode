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
[0,0,null,0,0]
[0,0,null,0,null,0,null,null,0] 
[0,0,0,0,null,null,0]
[0,0,null,0,0,0,null,null,0]
[0,0,null,0,0,0,0,null,null] // that using adjacent cameras is best
*/
bool debug = false;
int const MAXN = 1000, INVALID = -1;
int memo[MAXN+1][2][2]; // "memo[dfsIndex][coveredByParent][putAtThisNode]" is the min #cams to put for the tree rooted at "dfsIndex", given that "dfsIndex" is (coveredByParent: true | false, putAtThisNode: true | false)

class Solution {
public:
    void dfs(int &dfsIndex, TreeNode* root) {
        if (!root) return;
        root->val = ++dfsIndex;
        dfs(dfsIndex, root->left);
        dfs(dfsIndex, root->right);
    }
    
    int f(TreeNode* root, bool coveredByParent, bool putAtThisNode) {
        if (!root) return putAtThisNode ? INT_MAX : 0;
        
        int dfsIndex = root->val;
        if (INVALID != memo[dfsIndex][coveredByParent][putAtThisNode]) return memo[dfsIndex][coveredByParent][putAtThisNode];
        
        int ret = 0;
        int lNotPut = f(root->left, putAtThisNode, false);
        int lPut = f(root->left, putAtThisNode, true);
        
        int rNotPut = f(root->right, putAtThisNode, false);
        int rPut = f(root->right, putAtThisNode, true);
        
        int lMin = min(lPut, lNotPut), rMin = min(rPut, rNotPut);
            
        if (coveredByParent) {
            // The lChild and rChild can enjoy any choice
            ret = (putAtThisNode + lMin + rMin);
        } else {
            // Must be covered by one of [self, lChild, rChild], in this case the default returning value should be INT_MAX, indicating impossible.
            ret = INT_MAX;
            
            if (putAtThisNode) {
                // The lChild and rChild can enjoy any choice too 
                ret = (putAtThisNode + lMin + rMin);
            } else {
                // fasle == coveredByParent && false == putAtThisNode, must be covered by one of lChild or rChild
                int cand1 = (0 == lPut || INT_MAX == lPut) ? 
                                INT_MAX 
                                : 
                                (INT_MAX == rMin) ? INT_MAX : lPut + rMin; // ensure covering self by lChild
                int cand2 = (0 == rPut || INT_MAX == rPut) ? 
                                INT_MAX 
                                : 
                                (INT_MAX == lMin) ? INT_MAX : rPut + lMin; // ensure covering self by rChild
                ret = min(cand1, cand2);
            }
        }
        
        if (debug) printf("dfsIndex:%d, coveredByParent:%d, putAtThisNode:%d, ret = %d\n", dfsIndex, coveredByParent, putAtThisNode, ret);
        return  memo[dfsIndex][coveredByParent][putAtThisNode] = ret;
    }

    int minCameraCover(TreeNode* root) {
        memset(memo, INVALID, sizeof memo);
        
        // init "dfsIndex" for each node
        int dfsIndex = 0;
        dfs(dfsIndex, root);
        
        int cand1 = f(root, false, false);
        int cand2 = f(root, false, true);
        
        return min(cand1, cand2);
    }
};
