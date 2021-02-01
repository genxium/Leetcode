bool const debug = false;
int const BRANCHES = 27;
int const END_BRANCH = 26;

class TrieNode {
public:
    TrieNode* parent = NULL;
    vector<TrieNode*> branches;
    
    TrieNode() {
        for (int i = 0; i < BRANCHES; ++i) {
            branches.push_back(NULL);
        }
    }
    
    TrieNode* getBranch(char ch) {
        int idx = ch-'a';
        if (NULL == branches[idx]) {
            branches[idx] = new TrieNode();
            branches[idx]->parent = this;
        }
        return branches[idx];
    }
    
    void insert(string &word) {
        if (debug) printf("insert word:%s\n", word.c_str());
        TrieNode* trav = this;
        // move down
        for (auto ch : word) {
            if (debug) printf("\tmoving to ch:%c at trav:%p\n", ch, trav);
            trav = trav->getBranch(ch);
        }
        
        // end
        trav->getBranch('a'+END_BRANCH);
        if (debug) printf("\tappended word end to trav:%p\n", trav);
    }
    
    bool query(int i, string &w, int quotaToSkip) {
        if (debug) printf("\ti:%d, whilst quotaToSkip == %d, branches.size():%d\n", i, quotaToSkip, branches.size());
        if (i == w.length()) {
            if (0 < quotaToSkip) return false; // must use EXACTLY 1 "quotaToSkip"
            bool tmp = (NULL != branches[END_BRANCH]);  
            if (debug) printf("\treturning tmp:%d because %p has no word end\n", tmp, this);
            return tmp;
        }
        char ch = w[i];
        int idx = ch-'a';
        if (!branches[idx]) {
            if (debug) printf("\t\tno branch to move on\n");
            if (quotaToSkip > 0) {
                --quotaToSkip;
                bool res = false;
                for (int j = 0; j < BRANCHES-1; ++j) {
                    auto b = branches[j];
                    if (NULL == b) continue;
                    if (debug) printf("\t\tchosen branch j:%d\n", j);
                    res |= b->query(i+1, w, 0);
                    if (res) return true;
                }
                return res;
            } else {
                return false;
            } 
        } else {
            if (debug) printf("\t\thas branch to move on\n");
            bool res = false;
            res |= branches[idx]->query(i+1, w, quotaToSkip);
            if (res) return true;
            if (quotaToSkip > 0) {
                // we can also change branch here 
                --quotaToSkip;
                bool res = false;
                for (int j = 0; j < BRANCHES-1; ++j) {
                    if (j == idx) continue;
                    auto b = branches[j];
                    if (NULL == b) continue;
                    res |= b->query(i+1, w, 0);
                    if (res) return true;
                }
            }
            return res;
        }
    }
};

/*
test cases
["MagicDictionary", "buildDict", "search", "search", "search", "search"]
[[], [["hello","leetcode"]], ["hello"], ["hhllo"], ["hell"], ["leetcoded"]]
["MagicDictionary", "buildDict", "search", "search", "search", "search"]
[[], [["hello","hallo","leetcode"]], ["hello"], ["hhllo"], ["hell"], ["leetcoded"]]
*/
class MagicDictionary {
private:
    TrieNode* root = NULL;

public:
    /** Initialize your data structure here. */
    MagicDictionary() {
        root = new TrieNode();        
    }
    
    void buildDict(vector<string> dictionary) {
        for (auto &word: dictionary) {
            root->insert(word);
        }
    }
    
    bool search(string searchWord) {
        if (debug) printf("seaching for %s\n", searchWord.c_str());
        return root->query(0, searchWord, 1);
    }
};

/**
 * Your MagicDictionary object will be instantiated and called as such:
 * MagicDictionary* obj = new MagicDictionary();
 * obj->buildDict(dictionary);
 * bool param_2 = obj->search(searchWord);
 */
