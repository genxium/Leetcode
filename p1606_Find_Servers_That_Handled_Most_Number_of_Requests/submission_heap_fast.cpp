bool const debug = false;

typedef vector<int> VI;
typedef vector<VI> VVI;

struct PqCmp {
    bool operator() (VI const& lhs, VI const& rhs) const {
        // each is {endTIme, serverIdx}
        if (lhs[0] != rhs[0]) return lhs[0] > rhs[0]; // for a min-heap
        return lhs[1] > rhs[1];
    }
};

/*
test cases
3
[1,2,3,4,5]
[5,2,3,3,3]
3
[1,2,3,4]
[1,2,1,2]
3
[1,2,3]
[10,12,11]
4
[1,3,4,5,10,12]
[11,9,3,1,9,12]
*/
class Solution {
public:
    vector<int> busiestServers(int k, vector<int>& arrival, vector<int>& load) {
        // The fact that "arrival" is guaranteed in ascending order makes this problem easier than the more general "find first element in x[] smaller than given u" problem, which is solved in the segtree solution https://github.com/genxium/Leetcode/blob/master/p1606_Find_Servers_That_Handled_Most_Number_of_Requests/submission_segtree.cpp.

        unordered_map<int, int> counter;
        set<int> feasibleIdxes; 
        priority_queue<VI, VVI, PqCmp> pq;
        for (int i = 0; i < k; ++i) feasibleIdxes.insert(i);
        for (int i = 0; i < arrival.size(); ++i) {
            while (!pq.empty()) {
               VI const& tuple = pq.top(); // with minimum "tuple[0] a.k.a. endtime" in heap
               bool qualified = (tuple[0] <= arrival[i]); 
               if (qualified) {
                   feasibleIdxes.insert(tuple[1]);
                   pq.pop();
               } else break; // no chance for the next top to be qualified 
            }
            if (feasibleIdxes.empty()) continue;
            int st = i%k;
            auto it = feasibleIdxes.lower_bound(st);
            if (it == feasibleIdxes.end()) it = feasibleIdxes.begin();
            int j = *it;
            ++counter[j];
            pq.push({arrival[i]+load[i], j});
            feasibleIdxes.erase(it);
        }

        int mx = INT_MIN;
        for (auto &[i,cnt] : counter) {
            if (cnt > mx) mx = cnt;
        }
        vector<int> ans;
        for (auto &[i,cnt] : counter) {
            if (cnt == mx) ans.push_back(i); 
        }

        return ans;
    }
};
