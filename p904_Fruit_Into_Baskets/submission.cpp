/*
test cases
[1,2,1]
[0,1,2,2]
[1,2,3,2,2]
[3,3,3,1,2,1,1,2,3,3,4]
[0]
*/
class Solution {
public:
    int totalFruit(vector<int>& tree) {
        /*
        The problem is asking to find the "longest subarray with EXACTLY 2 different types of fruits".
        */
        int n = tree.size();
        unordered_map<int, int> counter;
        
        int l = 0, r = 0, ans = 0;
        while (r < n) {
            ++counter[tree[r]];
            while (l < r && counter.size() > 2) {
                --counter[tree[l]];
                if (0 == counter[tree[l]]) counter.erase(tree[l]);
                ++l;
            }
            
            ans = max(ans, r-l+1);
            ++r;
        }
        
        return ans;
    }
};
