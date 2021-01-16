typedef pair<int, int> PII;
typedef pair<vector<int>::iterator, int> PTI;

/*
This problem is quite like the "Merge K Sorted List" one, but with a few extra "two ptrs" operations to implement. 
*/
class HeapCmp {
public:
    bool operator() (PTI const& lhs, PTI const& rhs) const {
        return *(lhs.first) > *(rhs.first); // a min-heap
    }
};

/*
test cases
[[4,10,15,24,26],[0,9,12,20],[5,18,22,30]]
[[1,2,3],[1,2,3],[1,2,3]]
[[10,10],[11,11]]
[[10],[11]]
[[1],[2],[3],[4],[5],[6],[7]]
*/
class Solution {
public:
    vector<int> smallestRange(vector<vector<int>>& nums) {
        int n = nums.size();
        list<PII> cache;
        unordered_map<int, int> counter; // "counter[i] == x" means that there're "x" elements of the i-th group in "cache"
    
        priority_queue<PTI, vector<PTI>, HeapCmp> pq; // a min-heap
        for (int i = 0; i < n; ++i) {
            vector<int> &singleList = nums[i];
            pq.push({singleList.begin(), i});
        }
        
        vector<int> ans = {-1, -1};        
        while (!pq.empty()) {
            while (!pq.empty() && counter.size() < n) {
                PTI t = pq.top(); pq.pop();      
                int val = *(t.first);
                int toAddI = t.second;
                cache.push_back({val, t.second});
                ++counter[t.second];
                ++t.first;
                if (t.first == nums[t.second].end()) continue;
                pq.push({t.first, t.second});
            } 

            while (cache.size() > 0 && counter.size() == n) {
                int cand = (cache.rbegin())->first-(cache.begin())->first;
                if (ans[0] == -1 || ans[1]-ans[0] > cand) {
                    ans[0] = (cache.begin())->first;
                    ans[1] = (cache.rbegin())->first;
                }
                int toRemoveI = cache.begin()->second;
                --counter[toRemoveI];
                cache.pop_front();
                if (0 == counter[toRemoveI]) counter.erase(toRemoveI);
            }
        }
        
        return ans;
    }
};
