int const BRANCHING_FACTOR = 2;
class TrieNode {
public:
    TrieNode* children[BRANCHING_FACTOR];

    TrieNode() {
        for (int i = 0; i < BRANCHING_FACTOR; ++i) {
            children[i] = NULL;
        }
    }
    
    void insert(int num, int singleSetbitMask) {
        if (!singleSetbitMask) return;
        int branch = (num & singleSetbitMask) > 0 ? 1 : 0;
        if (!children[branch]) {
            children[branch] = new TrieNode();                
        }
        children[branch]->insert(num, (singleSetbitMask >> 1));
    }
    
    int getMax(int xi, int mi, int singleSetbitMask, int accNum) {
        if (accNum > mi) return INT_MIN;            
        if (0 == singleSetbitMask) return 0;
        if (!children[0]) {
            // we've no choice but to go "children[1]"
            int incrementHere = ((xi & singleSetbitMask) ? 0 : singleSetbitMask); // Because the "num" chose '1', if ((xi & singleSetbitMask) > 0) then the bit at "xi" here is '1', resulting in XOR result 0. 
            return incrementHere + children[1]->getMax(xi, mi, (singleSetbitMask >> 1), accNum + singleSetbitMask);
        }
        if (!children[1]) {
            // we've no choice but to go "children[0]"
            int incrementHere = ((xi & singleSetbitMask) ? singleSetbitMask : 0); // Because the "num" chose '1', if ((xi & singleSetbitMask) > 0) then the bit at "xi" here is '1', resulting in XOR result 0. 
            return incrementHere + children[0]->getMax(xi, mi, (singleSetbitMask >> 1), accNum);
        }
        int preferredBranch = ((xi & singleSetbitMask) > 0) ? 0 : 1; // if ((xi & singleSetbitMask) > 0) then the bit at "xi" here is '1', we thus prefer to try the "num" with '0' first.
        int candidate = singleSetbitMask + children[preferredBranch]->getMax(xi, mi, (singleSetbitMask >> 1), accNum + preferredBranch*singleSetbitMask);
        if (candidate < 0) {
            int theOtherBranch = !preferredBranch;
            return children[theOtherBranch]->getMax(xi, mi, (singleSetbitMask >> 1), accNum + theOtherBranch*singleSetbitMask); 
        } else {
            return candidate;
        }
    }
};

/*
test cases
[0,1,2,3,4]
[[3,1],[1,3],[5,6]]
[5,2,4,6,6,3]
[[12,4],[8,1],[6,3]]
*/
class Solution {
public:
    vector<int> maximizeXor(vector<int>& nums, vector<vector<int>>& queries) {
        int maxMi = 0;
        for (auto &q : queries) {
            int xi = q[0], mi = q[1];
            maxMi = max({maxMi, xi, mi});
        }

        int highestSingleSetbitMask = 1 << (int)log2(maxMi);
        TrieNode* root = new TrieNode();
        for (auto num : nums) {
            root->insert(num, highestSingleSetbitMask);
        }

        vector<int> ans;
        for (auto &q : queries) {
            int xi = q[0], mi = q[1];
            auto singleAns = root->getMax(xi, mi, highestSingleSetbitMask, 0);
            ans.push_back(singleAns < 0 ? -1 : singleAns);
        }
        
        // TODO: destruct the Trie
        return ans;
    }
};
