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
class BSTIterator {
private:
    /*
    test case #1
    ["BSTIterator","next","next","hasNext","next","hasNext","next","hasNext","next","hasNext"]
[[[7,3,15,null,null,9,20]],[null],[null],[null],[null],[null],[null],[null],[null],[null]]
    
    test case #2
    ["BSTIterator","next","next","hasNext","next","hasNext","next","hasNext","next","hasNext"]
[[[7,3,15,null,5,9,20]],[null],[null],[null],[null],[null],[null],[null],[null],[null]]
    */
    TreeNode *root, *current;
    TreeNode* upperBound(TreeNode *root, int target) { 
        if (NULL == root) return NULL;
        if (root->val <= target) {
            return upperBound(root->right, target);
        } else {
            // root->val > target
            /* 
             * Consider the sample tree "[7,3,15,null,null,9,20]", if we seek upperBound(root, 4), 
             * then NONE of the left-subtree is a correct upper-bound.
             */
            TreeNode* temp = upperBound(root->left, target);
            if (NULL == temp || temp->val < target) return root;
            return temp;
        }
    }
    
public:
    BSTIterator(TreeNode* root) {
        this->root = root;
        this->current = NULL;
    }
    
    /** @return the next smallest number */
    int next() {
        int curVal = (NULL == current ? INT_MIN : current->val);
        TreeNode* ub = upperBound(root, curVal);
        if (NULL == ub) {
            return INT_MAX;
        } else {
            current = ub;
            return ub->val;
        }
    }
    
    /** @return whether we have a next smallest number */
    bool hasNext() {
        int curVal = (NULL == current ? INT_MIN : current->val);
        TreeNode* ub = upperBound(root, curVal);
        if (NULL == ub) {
            return false;
        } else {
            return true;
        }
    }
};

/**
 * Your BSTIterator object will be instantiated and called as such:
 * BSTIterator* obj = new BSTIterator(root);
 * int param_1 = obj->next();
 * bool param_2 = obj->hasNext();
 */
