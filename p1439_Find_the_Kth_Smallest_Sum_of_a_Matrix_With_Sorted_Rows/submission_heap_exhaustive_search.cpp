typedef vector<int> VI;
bool const debug = false;

class HeapNode {
public:
    vector<int> colIdList;
    int sum = 0;
    
    void print() {
        printf("sum: %d, cols: [", sum);
        for (int i = 0; i < colIdList.size(); ++i) {
            int colId = colIdList[i];
            if (i == colIdList.size()-1) printf("%d]\n", colId);
            else printf("%d, ", colId);
        }
    }
};

class HeapCmp { 
public: 
    int operator() (HeapNode* const p1, HeapNode* const p2) { 
        return p1->sum > p2->sum; // smaller first 
    }
}; 

struct VectorHash {
    size_t operator()(const std::vector<int>& v) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
};

/*
test cases
[[1,3,11],[2,4,6]]
5
[[1,3,11],[2,4,6]]
9
[[1,10,10],[1,4,5],[2,3,6]]
7
[[1,1,10],[2,2,9]]
7
*/
class Solution {
public:
    int solve(vector<vector<int>>& lists, int k) {
        unordered_set<vector<int>, VectorHash> openSet;
        unordered_set<vector<int>, VectorHash> closedSet;
        
        int const m = lists.size();
        int const n = lists[0].size();
        
        priority_queue<HeapNode*, vector<HeapNode*>, HeapCmp> pq; // use a min-heap w.r.t "-value+nextValue"

        HeapNode* root = new HeapNode();
        root->colIdList.assign(m, 0);
        root->sum = 0;
        for (int rowId = 0; rowId < m; ++rowId) {
            int value = lists[rowId][0];
            root->sum += value;
        }
        pq.push(root);
        
        int toRet = root->sum;
        
        int count = 0;
        while (0 < pq.size() && count < k) {
            HeapNode* minPair = pq.top(); pq.pop();
            if (debug) minPair->print();

            closedSet.insert(minPair->colIdList);
            openSet.insert(minPair->colIdList);
            ++count;
            if (count == k) {
                toRet = minPair->sum;
                break;
            }
            for (int rowId = 0; rowId < m; ++rowId) {
                vector<int> colIdList(minPair->colIdList); // a clone
                if (colIdList[rowId] + 1 >= n) continue;
                ++colIdList[rowId];
                if (openSet.find(colIdList) != openSet.end() || closedSet.find(colIdList) != closedSet.end()) continue;
                HeapNode* candidate = new HeapNode();
                candidate->colIdList = colIdList;
                candidate->sum = minPair->sum - lists[rowId][colIdList[rowId]-1] + lists[rowId][colIdList[rowId]];
                openSet.insert(colIdList);
                pq.push(candidate);
            }
            
            delete minPair;
        }
        
        return toRet;
    }
  
    int kthSmallest(vector<vector<int>>& mat, int k) {
        int ans = solve(mat, k);
        return ans;
    }
};
