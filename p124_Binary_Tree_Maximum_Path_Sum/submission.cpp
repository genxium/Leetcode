/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    int maxDownSinglePathSum(TreeNode* includingRoot, int &outAns) {
        int rootVal = includingRoot->val;
        int maxLeftDownSinglePathSum = 0;
        if (includingRoot->left) {
            int tmp = maxDownSinglePathSum(includingRoot->left, outAns);
            if (tmp > maxLeftDownSinglePathSum) maxLeftDownSinglePathSum = tmp;
        }
        
        int maxRightDownSinglePathSum = 0;
        if (includingRoot->right) {
            int tmp = maxDownSinglePathSum(includingRoot->right, outAns);
            if (tmp > maxRightDownSinglePathSum) maxRightDownSinglePathSum = tmp;
        }

        int switchRootCandidate = (rootVal + maxLeftDownSinglePathSum + maxRightDownSinglePathSum);
        if (outAns < switchRootCandidate) {
            outAns = switchRootCandidate;
        }

        int candidate1 = rootVal + maxLeftDownSinglePathSum;
        int candidate2 = rootVal + maxRightDownSinglePathSum;
        int toRet = (candidate1 > candidate2 ? candidate1 : candidate2);
        return toRet;
    }

    int maxPathSum(TreeNode* root) {
        // [1,2,3]
        // [-10,9,-20,null,null,15,7]
        // [-10,9,20,null,null,15,7]
        // [-10,9,-20,null,null,-15,-7]

        // An edge case to test
        // [-10, 9,-20, -11, -50, -15, -7, null, 4, 2, null]
        if (NULL == root) return 0;
        int outAns = root->val;
        int maxDownSinglePathSumOfGrandRoot = maxDownSinglePathSum(root, outAns);
        return outAns;
    }
};
