#define ACTIVE 0
#define HALF_TERMINATED 1
#define TERMINATED 2

class Solution {
public:
    bool isValidSerialization(string preorder) {
        stringstream ss(preorder);
        string single;
        vector<string> part;
        while(getline(ss, single, ',')) {
            part.push_back(single);
        }
        
        if ("#" == part[0]) {
            if (1 == part.size()) {
                return true;                
            } else {
                return false;
            }
        }
        stack<int> stk;
        stk.push(ACTIVE); // for `"#" != part[0]` 
        
        for (int i = 1; i < part.size(); ++i) {
            string single = part[i];
            if ("#" == single) {
                if (stk.empty()) return false;
                stk.top() += 1; // "ACTIVE -> HALF_TERMINATED", or "HALF_TERMINATED -> TERMINATED"
            } else {
                stk.push(ACTIVE);
            }
            //printf("single == %s, stk.top() == %d\n", single.c_str(), stk.top());
            while ( TERMINATED == stk.top() ) {
                // Think of this process as "collapsing the tree from leaf and going up"
                stk.pop();
                if (stk.empty()) {
                    if (i == part.size()-1) return true;
                    else return false;
                }
                else stk.top() += 1;
            }
        }
        //printf("stk.size() == %d\n", stk.size());
        return stk.empty();
    }
};
