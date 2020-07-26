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
    vector<pair<TreeNode*, int>> findLeafs(TreeNode* root, int &distance, int height, int &ans) {
      vector<pair<TreeNode*, int>> toRet;
      if (!root) {
        return toRet;
      }

      if (!root->left && !root->right) {
        toRet.push_back({root, height}); // only for leafs
        return toRet;
      }

      vector<pair<TreeNode*, int>> leftLeafs = findLeafs(root->left, distance, height+1, ans);
      vector<pair<TreeNode*, int>> rightLeafs = findLeafs(root->right, distance, height+1, ans);

      for (auto &p : leftLeafs) {
        for (auto &q : rightLeafs) {
          int pathLength = (p.second - height) + (q.second - height);
          if (pathLength <= distance) {
            ++ans;
          }
        }
      }
      
      for (auto &p : leftLeafs) {
        toRet.push_back(p);
      }
      for (auto &q : rightLeafs) {
        toRet.push_back(q);
      }
      return toRet;
    }

    int countPairs(TreeNode* root, int distance) {
      int ans = 0;
      findLeafs(root, distance, 0, ans);

      return ans;
    }
};

