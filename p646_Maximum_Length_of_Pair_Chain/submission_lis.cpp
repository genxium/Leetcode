typedef vector<int> VI;

bool cmpfind(VI const& lhs, VI const& rhs) {
    return lhs[1] < rhs[0];
}

bool cmpsort(VI const& lhs, VI const& rhs) {
    if (lhs[0] != rhs[0]) return lhs[0] < rhs[0];
    return lhs[1] < rhs[1];
}

/*
test cases
[[1,2], [2,3], [3,4]]
[[3,4],[2,3],[1,2]]
[[3,10],[3,7],[7,10],[7,9],[-1,7],[-9,5],[2,8]]
*/
class Solution {
public:
    int solveNlogn(vector<VI>& pairs) {
        sort(pairs.begin(), pairs.end(), &cmpsort);
        vector<VI> sortedCache;
        for (auto &pair : pairs) {
            auto lower = lower_bound(sortedCache.begin(), sortedCache.end(), pair, &cmpfind);
            if (lower == sortedCache.end()) {
              sortedCache.push_back(pair);
            } else {
              if ((*lower)[1] > pair[1]) *lower = pair;
            }
        }
        return sortedCache.size();
    }

    int findLongestChain(vector<vector<int>>& pairs) {
        return solveNlogn(pairs);
    }
};
