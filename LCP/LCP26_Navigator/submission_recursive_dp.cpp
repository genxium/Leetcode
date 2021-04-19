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
[1,2,null,3,4,null,null,5,6,7,8,9,10]
[1,2,3,4,5,6,7,8,9,10]
*/
class Solution {
public:
    int dfs(TreeNode* root, bool hasExtNav, bool putHere, bool hasParent) {
        if (!root) return putHere ? INT_MAX : 0; // impossible to put on nonexistent node
        int u = root->val;
        if (INVALID != memo[u][hasExtNav][putHere]) return memo[u][hasExtNav][putHere];
                
        // Prepare the values to choose from.
        int lNotPutNoExtNav = dfs(root->left, false, false, true), lPutNoExtNav = dfs(root->left, false, true, true);
        int lNotPutWithExtNav = dfs(root->left, true, false, true), lPutWithExtNav = dfs(root->left, true, true, true);
        int rNotPutNoExtNav = dfs(root->right, false, false, true), rPutNoExtNav = dfs(root->right, false, true, true);
        int rNotPutWithExtNav = dfs(root->right, true, false, true), rPutWithExtNav = dfs(root->right, true, true, true);
        
        int cand = INT_MAX;
        if (!root->left && !root->right) {
            // a leaf
            if (!hasExtNav && !putHere) cand = INT_MAX; // impossible
            else cand = putHere;
        } else {
            bool hasExtNavNow = (hasExtNav | putHere);
            
            if (!root->left || !root->right) {
                // only 1 child, "external navigator from sibling" is unnecessary
                int lhs = putHere;
                if (debug) printf("\t[u:%d][hasExtNav:%d][putHere:%d], lhs is %d\n", u, hasExtNav, putHere, lhs);
                
                int rhs = INT_MAX;
                if (root->left) {
                    if (hasExtNavNow) {
                        rhs = min(lNotPutWithExtNav, lPutWithExtNav);
                        // if (debug) printf("\t[u:%d][hasExtNav:%d][putHere:%d], rhs is %d #1\n", u, hasExtNav, putHere, rhs);
                    } else {
                        rhs = min(lNotPutNoExtNav, lPutNoExtNav);;
                        // if (debug) printf("\t[u:%d][hasExtNav:%d][putHere:%d], rhs is %d #2\n", u, hasExtNav, putHere, rhs);
                    }
                } else {
                    if (hasExtNavNow) {
                        rhs = min(rNotPutWithExtNav, rPutWithExtNav);
                        // if (debug) printf("\t[u:%d][hasExtNav:%d][putHere:%d], rhs is %d #3\n", u, hasExtNav, putHere, rhs);
                    } else {
                        rhs = min(rNotPutNoExtNav, rPutNoExtNav);
                        // if (debug) printf("\t[u:%d][hasExtNav:%d][putHere:%d], rhs is %d #4\n", u, hasExtNav, putHere, rhs);
                    }
                }
                cand = lhs+rhs;
            } else {
                // both "root->left" and "root->right" are non-null
                bool needNavForAbove = false;
                if (hasParent) {
                    needNavForAbove = (false == (hasExtNav | putHere));
                }
                
                // traverse all possible uses of "MUTUAL external navigator from sibling"    
                int tmp = putHere + lPutWithExtNav + rPutWithExtNav; // Base case, both using "put" can be regarded as "MUTUAL external navigator for each other"
                cand = tmp;
                                
                if ((lNotPutWithExtNav > 0 && INT_MAX != lNotPutWithExtNav)) {
                    tmp = putHere + lNotPutWithExtNav +rPutWithExtNav;
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

                // can try to inherit the "hasExtNavNow" for each subtree
                if (false == needNavForAbove) {
                    // can use single-sided "external navigator from sibling"    
                    if (false == hasExtNavNow) {
                        tmp = putHere + lPutNoExtNav + min(rNotPutWithExtNav, rPutWithExtNav);
                        cand = min(cand, tmp);
                        if ((lNotPutNoExtNav > 0 && INT_MAX != lNotPutNoExtNav)) {
                            tmp = putHere + lNotPutNoExtNav + min(rNotPutWithExtNav, rPutWithExtNav);
                            cand = min(cand, tmp);
                            if (debug) printf("\t[u:%d][hasExtNav:%d][putHere:%d], lNotPutNoExtNav is %d, rNotPutWithExtNav is %d, rPutWithExtNav is %d #5\n", u, hasExtNav, putHere, lNotPutNoExtNav, rNotPutWithExtNav, rPutWithExtNav);
                        }

                        tmp = putHere + rPutNoExtNav + min(lNotPutWithExtNav, lPutWithExtNav);
                        cand = min(cand, tmp);
                        if ((rNotPutNoExtNav > 0 && INT_MAX != rNotPutNoExtNav)) {
                            tmp = putHere + rNotPutNoExtNav + min(lNotPutWithExtNav, lPutWithExtNav);
                            cand = min(cand, tmp);
                        }
                    } else {
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
        }
        
        if (debug) printf("memo[u:%d][hasExtNav:%d][putHere:%d] is %d\n", u, hasExtNav, putHere, cand);
        return memo[u][hasExtNav][putHere] = cand;
    }
    
    int navigation(TreeNode* root) {
        int ans = 1;
        memset(memo, INVALID, sizeof memo);
        int cand1 = dfs(root, false, false, false);
        if (debug) printf("\n");
        int cand2 = dfs(root, false, true, false);
        return min(cand1, cand2);
    }
};


