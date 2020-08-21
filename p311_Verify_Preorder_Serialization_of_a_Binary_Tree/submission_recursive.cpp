/*
Note that "countNodes(TreeNode* root) = 1 + countNodes(root->left) + countNodes(root-right)", and the terminating condition is "root->val == #" or "root->left->val == # && root->right->val ==#", which means that each "countNodes(...)" must return odd.
*/ 
class Solution {
public:
    void traverse(vector<string> &part, int &travIndex) {
        if (travIndex >= part.size()) return;
        if ("#" == part[travIndex]) return;
        
        traverse(part, ++travIndex); // left subtree
        traverse(part, ++travIndex); // right subtree
    }
    
    bool isValidSerialization(string preorder) {
        if (0 == preorder.length()) {
            return true;
        }
        
        stringstream ss(preorder);
        string single;
        vector<string> part;
        
        while (getline(ss, single, ',')) {
            part.push_back(single);
        }
                
        // just follow the preorder traversal, i.e. reconstructing the tree
        int travIndex = 0;
        traverse(part, travIndex);
        //printf("travIndex == %d, part.size() == %d\n", travIndex, part.size());
        return travIndex == part.size()-1;
    }
};
