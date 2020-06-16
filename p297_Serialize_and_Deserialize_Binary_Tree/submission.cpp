/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

#define NIL "nil"
#define DELIMITER ','

class Codec {
public:
    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        if (NULL == root) return "";
        vector<string> preToRet;
        
        queue<TreeNode*> q;
        q.push(root);
        while (false == q.empty()) {
            TreeNode* e = q.front();
            q.pop();
            if (NULL == e) {
                preToRet.push_back(NIL);
            }
            if (NULL != e) {
                preToRet.push_back(to_string(e->val));
                q.push(e->left);
                q.push(e->right);
            }
        }
        
        string toRet = "";
        for (int i = 0; i < preToRet.size(); ++i) {
            toRet.append(preToRet[i]);
            if (i < preToRet.size() - 1) {
                toRet.push_back(DELIMITER);
            }
        }
        return toRet;
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
         /*
        * The caller is responsible for deallocating the whole returned tree.
        */
        if (data.empty()) return NULL;
        
        list<string> parsed;
        string tmp = "";
        for (int i = 0; i < data.length(); ++i) {
            if (data[i] == DELIMITER) {
                parsed.push_back(tmp.substr(0, tmp.length()));
                tmp = "";
            } else {
                tmp.push_back(data[i]);
            }
        }
        parsed.push_back(tmp.substr(0, tmp.length()));
        
        int level = 0;
        int completeChildrenCount = (1 << level);
        int nilCountAtThisLevel = 0;
        
        list<TreeNode*> preLevelNodes;
        list<TreeNode*> curLevelNodes;
        
        TreeNode *root;
        while (false == parsed.empty()) {
            if (level > 0) {
                while (false == preLevelNodes.empty()) {
                    TreeNode* parent = preLevelNodes.front(); 
                    preLevelNodes.pop_front();
                    // The serialization method guarantees that for each non-nil parent, there must be 2 children serialized together.
                    string l = parsed.front();
                    parsed.pop_front();
                    if (0 != l.compare(NIL)) {
                        TreeNode* lChild = new TreeNode(atoi(l.c_str()));
                        parent->left = lChild;
                        curLevelNodes.push_back(lChild);
                    }
                    string r = parsed.front();
                    parsed.pop_front();
                    if (0 != r.compare(NIL)) {
                        TreeNode* rChild = new TreeNode(atoi(r.c_str()));
                        parent->right = rChild;
                        curLevelNodes.push_back(rChild);
                    }
                }
            } else {
                string v = parsed.front();
                parsed.pop_front();
                root = new TreeNode(atoi(v.c_str()));
                curLevelNodes.push_back(root);
            }
            // Level traversal ends, flushes "curLevelNodes" into "preLevelNodes".
            while (false == curLevelNodes.empty()) {
                TreeNode *e = curLevelNodes.front();
                curLevelNodes.pop_front();
                preLevelNodes.push_back(e);
            }
            ++level;
        }
        
        return root;
    }
};

// Your Codec object will be instantiated and called as such:
// Codec codec;
// codec.deserialize(codec.serialize(root));
