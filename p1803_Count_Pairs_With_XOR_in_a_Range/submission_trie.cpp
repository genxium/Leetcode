int const BRANCHING_FACTOR = 2;
int const END_BRANCH = BRANCHING_FACTOR;
class TrieNode {
public:
    TrieNode* children[BRANCHING_FACTOR+1];
    int cnt = 0; // How many existing nums[i] passes this branch

    TrieNode() {
        for (int i = 0; i <= BRANCHING_FACTOR; ++i) {
            children[i] = NULL;
        }
    }
    
    void insert(int num, int singleSetbitMask) {
        if (!singleSetbitMask) {
            if (NULL == children[BRANCHING_FACTOR]) {
                children[BRANCHING_FACTOR] = new TrieNode();
            }
            ++children[BRANCHING_FACTOR]->cnt;
            return;
        }
        int branch = (num & singleSetbitMask) > 0 ? 1 : 0;
        if (!children[branch]) {
            children[branch] = new TrieNode();                
        }
        ++children[branch]->cnt;
        children[branch]->insert(num, (singleSetbitMask >> 1));
    }

    int count(int num, int singleSetbitMask, int xorResultByFar, int low, int high, int lowByFar, int highByFar) {
        // [WARNING] This is a TLE approach!
        if (!singleSetbitMask) {
            // We use the same "highestSingleSetbitMask" for every nums[i], thus every nums[i] ends at the same height.
            if (lowByFar <= xorResultByFar && xorResultByFar <= highByFar) {
                return children[END_BRANCH]->cnt;
            } else {
                return 0;
            }
        }

        int ret = 0;
        int bitVal = (num & singleSetbitMask) > 0 ? 1 : 0;
        int lowBitVal = (low & singleSetbitMask) > 0 ? 1 : 0;
        int newLowByFar = (lowByFar << 1) | lowBitVal;

        int highBitVal = (high & singleSetbitMask) > 0 ? 1 : 0;
        int newHighByFar = (highByFar << 1) | highBitVal;

        for (int i = 0; i < BRANCHING_FACTOR; ++i) {
            if (NULL == children[i]) continue;
            int newXorResultByFar = ((xorResultByFar << 1) | (bitVal ^ i));
            if (newLowByFar > newXorResultByFar) continue;
            if (newHighByFar < newXorResultByFar) continue;
            ret += children[i]->count(num, (singleSetbitMask >> 1), newXorResultByFar, low, high, newLowByFar, newHighByFar);
        }

        return ret;  
    }

    int countLessThan(int num, int singleSetbitMask, int upper) {
        if (!singleSetbitMask) {
            // We use the same "highestSingleSetbitMask" for every nums[i], thus every nums[i] ends at the same height.
            return 0;
        }
        int ret = 0;
        int bitVal = (num & singleSetbitMask) > 0 ? 1 : 0;
        int upperBitVal = (upper & singleSetbitMask) > 0 ? 1 : 0;
        
        if (0 == upperBitVal) {
            // That only "branchToGo^bitVal == 0" should be counted            
            int branchToGo = (0^bitVal);
            if (children[branchToGo]) {
                ret += children[branchToGo]->countLessThan(num, (singleSetbitMask >> 1), upper);
            }
        } else {
            // 1 == upperBitVal
            // That "(branchAllCount^bitVal) == 0" definitely works 
            int branchAllCount = (0^bitVal);
            if (children[branchAllCount]) {
                ret += children[branchAllCount]->cnt;
            }

            int anotherBranch = 1-branchAllCount; 
            if (children[anotherBranch]) {
                ret += children[anotherBranch]->countLessThan(num, (singleSetbitMask >> 1), upper);
            }
        }
        return ret;
    }
};

class Solution {
public:
    int countPairs(vector<int>& nums, int low, int high) {
        int maxNum = 0;
        for (auto num : nums) maxNum = max(maxNum, num);
        int highestSingleSetbitMask = (1 << (int)log2(maxNum));
        int ans = 0;
        TrieNode* root = new TrieNode();
        for (auto num : nums) {
            // ans += root->count(num, highestSingleSetbitMask, 0, low, high, 0, 0);
            ans += (root->countLessThan(num, highestSingleSetbitMask, high+1) - root->countLessThan(num, highestSingleSetbitMask, low));
            root->insert(num, highestSingleSetbitMask);
        }

        return ans;
    }
};
