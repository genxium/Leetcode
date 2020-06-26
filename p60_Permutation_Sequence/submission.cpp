vector<int> factorials;

class Solution {
public:
    Solution() {
        int cur = 1;
        factorials.push_back(cur);
        for (int i = 1; i < 10; ++i) {
            cur *= i;
            factorials.push_back(cur);
        }
    }
    
    void genPermutationExtra(string &prefix, set<int> &sortedPostfix, int k, bool &found) {   
        // At each call, we must have "prefix.length() + sortedPostfix.size() == n".
        if (found) {
            return;
        }
        int l = sortedPostfix.size();
        //printf("genPermutationExtra, prefix:%s, k:%d, l:%d\n", prefix.c_str(), k, l);

        if (0 == sortedPostfix.size()) {
            found = true;
            return;
        }
        
        // Find "t in [0, l)" such that "t*(l-1)! <= k < (t+1)*(l-1)!", then "sortedPostfix[t]" should be extracted and pushed back to "prefix".
        // Can the following steps be optimized in terms of time-complexity?
        
        bool foundT = false;
        int targetT = 0;
        
        auto it = sortedPostfix.begin();
        for (int t = 0; t < l; ++t, ++it) {
            //printf("\tchecking t:%d, *it:%d\n", t, *it);
            if (t*factorials[l-1] <= k && k < (t+1)*factorials[l-1]) {
                foundT = true;
                targetT = t;
                //printf("\t\tfound targetT:%d, *it:%d\n", targetT, *it);
                break;
            }
        }
        if (foundT) {
            int toPushBack = (*it);
            sortedPostfix.erase(it);
            prefix.push_back('0' + toPushBack);
            genPermutationExtra(prefix, sortedPostfix, k-targetT*factorials[l-1], found);
            // No backtracking needed for this problem.
        }
    }
    
    string getPermutation(int n, int k) {
        k -= 1;
        string prefix = "";
        set<int> sortedPostfix;
        bool found = false;
        
        for (int i = 0; i < n; ++i) {
            sortedPostfix.insert(i+1);
        }
        
        genPermutationExtra(prefix, sortedPostfix, k, found);
        return prefix;
    }
};
