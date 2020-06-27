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
    /*
    * Assume that the caller is responsible for deallocation.
    */
    void generateTreesExtra(vector<int> &sortedCandidates, int candidateRangeLClosed, int candidateRangeROpen, vector<TreeNode*> &out) {        
        // It's intended that "[candidateRangeLClosed, candidateRangeROpen)" is used instead of a deep copy of the collection of candidates.
        
        for (int i = candidateRangeLClosed; i < candidateRangeROpen; ++i) {
            int rootVal = sortedCandidates[i];
            
            vector<TreeNode*> leftSubtrees;
            generateTreesExtra(sortedCandidates, candidateRangeLClosed, i, leftSubtrees);
            
            vector<TreeNode*> rightSubtrees;
            generateTreesExtra(sortedCandidates, i+1, candidateRangeROpen, rightSubtrees);

            if (true == leftSubtrees.empty() && true == rightSubtrees.empty()) {
                TreeNode* root = new TreeNode(rootVal);
                out.push_back(root);
            } else if (true == leftSubtrees.empty()) {
                for (auto rightSubtreeRoot : rightSubtrees) {
                    TreeNode* root = new TreeNode(rootVal);
                    root->right = rightSubtreeRoot;
                    out.push_back(root);
                }
            } else if (true == rightSubtrees.empty()) {
                for (auto leftSubtreeRoot : leftSubtrees) {
                    TreeNode* root = new TreeNode(rootVal);
                    root->left = leftSubtreeRoot;
                    out.push_back(root);
                }
            } else {
                for (auto leftSubtreeRoot : leftSubtrees) {
                    for (auto rightSubtreeRoot : rightSubtrees) {
                        TreeNode* root = new TreeNode(rootVal);
                        root->left = leftSubtreeRoot;
                        root->right = rightSubtreeRoot;
                        out.push_back(root);
                    }
                }
            }
        }
    }
    
    vector<TreeNode*> generateTrees(int n) {
        vector<TreeNode*> ans;
        if (0 == n) {
            return ans;
        }
        if (1 == n) {
            TreeNode* root = new TreeNode(1);
            ans.push_back(root);
            return ans;
        }
        vector<int> sortedCandidates;
        for (int i = 0; i < n; ++i) {
            sortedCandidates.push_back(i+1);
        }
        generateTreesExtra(sortedCandidates, 0, n, ans);
        return ans;
    }
};
