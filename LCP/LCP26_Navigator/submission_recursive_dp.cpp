/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

bool debug = false;
int const MAXN = 50000, INVALID = -1;
int memo[MAXN+1][2][2]; // memo[u][hasExternalNavigator][putOneNavigatorAtU] = min #navigators needed for the "subtree u"

/*
test cases
[1,2,null,3,4]
[1,2,3,4]
[1,2,3,4,5]
[1,2,null,3,4,null,null,5,6,7,8,9,10]
[1,2,3,4,5,6,7,8,9,10]
*/
class Solution {
public:
    int dfs(TreeNode* root, bool hasExtNav, bool putHere) {
        if (!root) return putHere ? INT_MAX : 0; // impossible to put on nonexistent node
        int u = root->val;
        if (INVALID != memo[u][hasExtNav][putHere]) return memo[u][hasExtNav][putHere];
                
        // Prepare the values to choose from.
        int lNotPutNoExtNav = dfs(root->left, false, false), lPutNoExtNav = dfs(root->left, false, true);
        int lNotPutWithExtNav = dfs(root->left, true, false), lPutWithExtNav = dfs(root->left, true, true);
        int rNotPutNoExtNav = dfs(root->right, false, false), rPutNoExtNav = dfs(root->right, false, true);
        int rNotPutWithExtNav = dfs(root->right, true, false), rPutWithExtNav = dfs(root->right, true, true);
        
        int cand = INT_MAX;
        if (!root->left && !root->right) {
            // a leaf
            if (!hasExtNav && !putHere) cand = INT_MAX; // impossible
            else cand = putHere;
        } else {
            bool hasExtNavNow = (hasExtNav | putHere);
            
            if (!root->left || !root->right) {
                // only 1 child, "external navigator from sibling" is unnecessary
                int foo = putHere;
                if (debug) printf("\t[u:%d][hasExtNav:%d][putHere:%d], foo is %d\n", u, hasExtNav, putHere, foo);
                
                int bar = hasExtNavNow 
                          ?
                          (root->left ? min(lNotPutWithExtNav, lPutWithExtNav) : min(rNotPutWithExtNav, rPutWithExtNav))
                          :
                          (root->left ? min(lNotPutNoExtNav, lPutNoExtNav) : min(rNotPutNoExtNav, rPutNoExtNav))
                          ;
                cand = foo+bar;
            } else {
                // both "root->left" and "root->right" are non-null
                
                // [TYPE-A combination] traverse all possible uses of "MUTUAL external navigator from sibling"    
                int tmp = putHere + lPutWithExtNav + rPutWithExtNav; // Base case
                cand = tmp;
                                
                if ((lNotPutWithExtNav > 0 && INT_MAX != lNotPutWithExtNav)) {
                    // [WARNING] It's important to check "lNotPutWithExtNav > 0" here, otherwise we're NOT providing the "MUTUAL external navigator" for the "right subtree", i.e. shouldn't use "rPutWithExtNav".
                    tmp = putHere + lNotPutWithExtNav + rPutWithExtNav;
                    cand = min(cand, tmp);
                }

                if ((rNotPutWithExtNav > 0 && INT_MAX != rNotPutWithExtNav)) {
                    tmp = putHere + lPutWithExtNav + rNotPutWithExtNav;
                    cand = min(cand, tmp);
                }

                if ((lNotPutWithExtNav > 0 && INT_MAX != lNotPutWithExtNav) && (rNotPutWithExtNav > 0 && INT_MAX != rNotPutWithExtNav)) {
                    tmp = putHere + lNotPutWithExtNav + rNotPutWithExtNav;
                    cand = min(cand, tmp);
                }

                // [TYPE-B combination] not necessarily having "MUTUAL external navigator from sibling"
                if (true == hasExtNavNow) {
                    tmp = putHere + lPutWithExtNav + min(rNotPutWithExtNav, rPutWithExtNav);
                    cand = min(cand, tmp);
                    if ((lNotPutWithExtNav > 0 && INT_MAX != lNotPutWithExtNav)) {
                        tmp = putHere + lNotPutWithExtNav + min(rNotPutWithExtNav, rPutWithExtNav);
                        cand = min(cand, tmp);
                        if (debug) printf("\t[u:%d][hasExtNav:%d][putHere:%d], lNotPutWithExtNav is %d, rNotPutWithExtNav is %d, rPutWithExtNav is %d #7\n", u, hasExtNav, putHere, lNotPutWithExtNav, rNotPutWithExtNav, rPutWithExtNav);
                    }

                    tmp = putHere + rPutWithExtNav + min(lNotPutWithExtNav, lPutWithExtNav);
                    cand = min(cand, tmp);
                    if ((rNotPutWithExtNav > 0 && INT_MAX != rNotPutWithExtNav)) {
                        tmp = putHere + rNotPutWithExtNav + min(lNotPutWithExtNav, lPutWithExtNav);
                        cand = min(cand, tmp);
                    }
                }
            }
        }
        
        if (debug) printf("memo[u:%d][hasExtNav:%d][putHere:%d] is %d\n", u, hasExtNav, putHere, cand);
        return memo[u][hasExtNav][putHere] = cand;
    }
    
    int navigation(TreeNode* root) {
        memset(memo, INVALID, sizeof memo);
        int ans = dfs(root, false, false);
        ans = min(ans, dfs(root, false, true));

        if (root->left && root->right) {
            // [TYPE-ROOT combination] the root doesn't necessarily need "MUTUAL external navigator from sibling" for its left and right child, the "putHere" for this case is always "false"

            // Prepare the values to choose from.
            int lNotPutNoExtNav = dfs(root->left, false, false), lPutNoExtNav = dfs(root->left, false, true);
            int lNotPutWithExtNav = dfs(root->left, true, false), lPutWithExtNav = dfs(root->left, true, true);
            int rNotPutNoExtNav = dfs(root->right, false, false), rPutNoExtNav = dfs(root->right, false, true);
            int rNotPutWithExtNav = dfs(root->right, true, false), rPutWithExtNav = dfs(root->right, true, true);

            int tmp = lPutNoExtNav + min(rNotPutWithExtNav, rPutWithExtNav);
            ans = min(ans, tmp);
            if ((lNotPutNoExtNav > 0 && INT_MAX != lNotPutNoExtNav)) {
                tmp = lNotPutNoExtNav + min(rNotPutWithExtNav, rPutWithExtNav);
                ans = min(ans, tmp);
            }

            tmp = rPutNoExtNav + min(lNotPutWithExtNav, lPutWithExtNav);
            ans = min(ans, tmp);
            if ((rNotPutNoExtNav > 0 && INT_MAX != rNotPutNoExtNav)) {
                tmp = rNotPutNoExtNav + min(lNotPutWithExtNav, lPutWithExtNav);
                ans = min(ans, tmp);
            }
        }

        return ans;
    }
};
