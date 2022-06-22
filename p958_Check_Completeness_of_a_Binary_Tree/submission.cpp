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
[1,2,3,4,5,6]
[1,2,3,4,5,null,7]
[1,2,3,5,null,null,8]
*/
bool debug = false;
class Solution {
public:
    bool isCompleteTree(TreeNode* root) {
        if (!root) return false;
        queue<TreeNode*> q;
        q.push(root);
        int expectedLvSize = 1;
        while (!q.empty()) {
            int sz = q.size();
            if (sz != expectedLvSize) {
                if (debug) printf("#1 sz:%d, expectedLvSize: %d\n", sz, expectedLvSize);
                for (int i = 0; i < sz; ++i) {
                    auto f = q.front(); q.pop();
                    if (f->left || f->right) return false;
                }
            } else {
                if (debug) printf("#2 sz:%d, expectedLvSize: %d\n", sz, expectedLvSize);
                bool prevIsRight = true, shouldEnd = false;
                for (int i = 0; i < sz; ++i) {
                    auto f = q.front(); q.pop();
                    if (shouldEnd && (f->left || f->right)) return false;
                    if (f->left) {
                        if (false == prevIsRight) return false;
                        q.push(f->left);
                        prevIsRight = false;
                    }
                    if (f->right) {
                        if (prevIsRight) return false;
                        q.push(f->right);
                        prevIsRight = true;
                    } else {
                        shouldEnd = true;
                    }
                }                
            }
            expectedLvSize <<= 1;
        }
        return true;
    }
};
