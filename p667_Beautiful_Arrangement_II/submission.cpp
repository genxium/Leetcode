/*
test cases
3
2
3
1
100
99
100
91
1000
57
*/
class Solution {
public:
    vector<int> constructArray(int n, int k) {
        /*
        Draw the "table of abs-diff for n:7" as shown below.

           1 2 3 4 5 6 7
        1    1 2 3 4 5 6
        2  1   1 2 3 4 5
        3  2 1   1 2 3 4 
        4  3 2 1   1 2 3
        5  4 3 2 1   1 2
        6  5 4 3 2 1   1
        7  6 5 4 3 2 1 

        It can be shown that if n is odd, we can always suffice "k == n-1" by flipping intermittently.
        ({1,7}, d:6) -> ({7,2}, d:5) -> ({2,6}, d:4) -> ({6,3}, d:3) -> ({3,5}, d:2) -> ({5,4}, d:1)
        
        Therefore any "k < n" can be sufficed and we can just stop flipping by having all "d:1" for the rest of the sequence.
        */

        vector<int> ans = {1};
        int d = n-1;
        int op = 1;
        while (k-- > 1) {
            int next = ans.back() + op*d;
            ans.push_back(next);
            op *= -1;
            d--;
        }

        // now that k == 1
        while (ans.size() < n) {
            int next = ans.back() + op*1;
            ans.push_back(next);
        }

        return ans;
    }
};
