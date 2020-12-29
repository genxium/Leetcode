bool const debug = false;
int const MAXN = 50000;

bool openSet[MAXN+5], closedSet[MAXN+5];
/*
test cases
[100,-23,-23,404,100,23,23,23,3,404]
[7]
[7,6,9,6,9,6,9,7]
[6,1,9]
[11,22,7,7,7,7,7,7,7,22,13]
*/
class Solution {
public:
    int minJumps(vector<int>& arr) {
        int n = arr.size();
        if (1 >= n) return 0;
        
        unordered_map<int, unordered_set<int>> counter;
        for (int i = 0; i < n; ++i) {
            counter[arr[i]].insert(i);
        }
        
        // init
        queue<int> q, rq;
        
        int lv = 0, rlv = 0;
        memset(openSet, false, sizeof openSet);
        memset(closedSet, false, sizeof closedSet);
        
        q.push(0);
        openSet[0] = true;
        
        // loop
        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {
                int t = q.front(); q.pop();
                if (debug) printf("lv: %d, t: %d\n", lv, t);
                if (t == n-1) return lv;
                openSet[t] = false;
                closedSet[t] = true;
                
                if (0 <= t-1 && !openSet[t-1] && !closedSet[t-1]) {
                    q.push(t-1);
                    openSet[t-1] = true;
                }
                if (t+1 < n  && !openSet[t+1] && !closedSet[t+1]) {
                    q.push(t+1);
                    openSet[t+1] = true;
                }
                
                auto &theSet = counter[arr[t]];  
                while (!theSet.empty()) {
                    int c = *theSet.begin();
                    theSet.erase(c);
                    if (openSet[c] || closedSet[c]) continue;
                    q.push(c);
                    openSet[c] = true;
                }
            }
            
            ++lv;
        }
        
        return lv;
    }
};
