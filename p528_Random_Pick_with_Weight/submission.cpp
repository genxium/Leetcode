/*
test cases
["Solution","pickIndex"]
[[[1]],[]]
["Solution","pickIndex","pickIndex","pickIndex","pickIndex","pickIndex"]
[[[1,3]],[],[],[],[],[]]
*/
class Solution {
private:
    vector<int> ps; // prefixSum
public:
    Solution(vector<int>& w) {
        ps.push_back(0);
        for (auto &single : w) {
            ps.push_back(single + ps.back());
        }
    }
    
    int pickIndex() {
        auto it = upper_bound(ps.begin(), ps.end(), rand()%ps.back());
        return it-ps.begin()-1;
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(w);
 * int param_1 = obj->pickIndex();
 */
