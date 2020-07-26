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
    void lowestCommonAncestorExtra(TreeNode* root, TreeNode* p, TreeNode* q, unordered_map<TreeNode*, TreeNode*> &parents, TreeNode* &ans) {
      parents[root] = NULL;
      
      queue<TreeNode*> searchSpace;
      searchSpace.push(root);
      
      while (!searchSpace.empty() 
             && 
             (parents.find(p) == parents.end() || parents.find(q) == parents.end())
            ) {
        vector<TreeNode*> expanders;
        while (!searchSpace.empty()) {
          expanders.push_back(searchSpace.front()); searchSpace.pop();
        }
        for (auto &expander : expanders) {
          if (expander->left) {
            parents[expander->left] = expander;
            searchSpace.push(expander->left);
          }
          if (expander->right) {
            parents[expander->right] = expander;
            searchSpace.push(expander->right);
          }
        }
      }
      
      // now that we've got both "parents[p]" and "parents[q]"
      unordered_set<TreeNode*> pAncestors;
      TreeNode *cur = p;
      while (cur != NULL) {
        pAncestors.insert(cur);
        cur = parents[cur];
      }
      
      cur = q;
      while (cur != NULL) {
        auto it = pAncestors.find(cur);
        if (it != pAncestors.end()) {
          ans = (*it);
          return;
        }
        cur = parents[cur];
      }
    }
    
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
      if (NULL == root || NULL == p || NULL == q) return NULL;
      TreeNode* ans = NULL;
      unordered_map<TreeNode*, TreeNode*> parents;
      lowestCommonAncestorExtra(root, p, q, parents, ans);
      return ans;
    }
};
