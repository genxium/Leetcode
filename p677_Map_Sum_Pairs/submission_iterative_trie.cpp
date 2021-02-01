bool const debug = false;
int const BRANCHES = 27;
int const END_BRANCH = 26;

class TrieNode {
public:
    TrieNode* parent = NULL;
    vector<TrieNode*> branches;
    int sum = 0;
    
    TrieNode() {
        for (int i = 0; i < BRANCHES; ++i) branches.push_back(NULL);
    }
    
    TrieNode* getBranch(char ch) {
        int idx = ch-'a';
        if (NULL == branches[idx]) {
            branches[idx] = new TrieNode();
            branches[idx]->parent = this;
        }
        return branches[idx];
    }
    
    void insert(string &key, int val) {
        if (debug) printf("insert key:%s, val:%d\n", key.c_str(), val);
        TrieNode* trav = this;
        // move down
        for (auto ch : key) {
            if (debug) printf("\tmoving to ch:%c at trav:%p\n", ch, trav);
            trav = trav->getBranch(ch);
        }
        
        // update
        int diff = val-(trav->getBranch('a'+END_BRANCH)->sum);
        trav->getBranch('a'+END_BRANCH)->sum = val;
        
        // move up with diff
        while (trav) {
            trav->sum += diff;
            trav = trav->parent;
        }
    }
    
    int querySum(string &prefix) {
        if (debug) printf("querySum prefix:%s\n", prefix.c_str());
        TrieNode* trav = this;
        for (auto ch : prefix) {
            if (debug) printf("\tmoving to ch:%c\n", ch);
            int idx = ch-'a';
            if (!trav->branches[idx]) return 0;
            trav = trav->branches[idx];
        }
        
        return trav->sum;
    }
};

/*
test cases
["MapSum", "insert", "sum", "insert", "sum"]
[[], ["apple",3], ["ap"], ["app",2], ["ap"]]
["MapSum", "insert", "sum", "insert", "sum"]
[[], ["apple",3], ["apple"], ["app",2], ["ap"]]
["MapSum", "insert", "sum", "insert", "insert", "sum"]
[[], ["apple",3], ["ap"], ["app",2], ["apple", 2], ["ap"]]
*/
class MapSum {
private:
    TrieNode* root = NULL;
public:
    /** Initialize your data structure here. */
    MapSum() {
        root = new TrieNode();
    }
    
    void insert(string key, int val) {
        root->insert(key, val);
    }
    
    int sum(string prefix) {
        return root->querySum(prefix);
    }
};

/**
 * Your MapSum object will be instantiated and called as such:
 * MapSum* obj = new MapSum();
 * obj->insert(key,val);
 * int param_2 = obj->sum(prefix);
 */
