#define PI pair<int, int>
#define PSET unordered_set<PI, PairHash>
class PairHash {
public:
    inline std::size_t operator()(PI const& v) const {
        return v.first*31+v.second;
    }
};

class Solution {
private:
    void dfs(int containedInX, int containedInY, int x, int y, PSET &pairCache, unordered_set<int> &sumCache) {
        /*
        Exhausting all possible actions.
        */
        if (pairCache.find({containedInX, containedInY}) != pairCache.end()) return;
        
        // printf("containedInX: %d, containedInY: %d\n", containedInX, containedInY);
        pairCache.insert({containedInX, containedInY});
        sumCache.insert(containedInX + containedInY);
        sumCache.insert(containedInX);
        sumCache.insert(containedInY);
        
        // empty x
        dfs(0, containedInY, x, y, pairCache, sumCache);
        
        // empty y
        dfs(containedInX, 0, x, y, pairCache, sumCache);

        // fill x
        dfs(x, containedInY, x, y, pairCache, sumCache);
        
        // fill y
        dfs(containedInX, y, x, y, pairCache, sumCache);
        
        int sum = containedInX + containedInY;
        
        // try to pour containedInY into x
        dfs(sum > x ? x : sum, sum > x ? sum-x : 0, x, y, pairCache, sumCache);
        
        // try to pour containedInX into y
        dfs(sum > y ? sum-y : 0, sum > y ? y : sum, x, y, pairCache, sumCache);
    }
public:
    bool canMeasureWater(int x, int y, int z) {
        if (x < y) swap(x, y);
        
        PSET pairCache;
        unordered_set<int> sumCache;
        
        dfs(0, 0, x, y, pairCache, sumCache);
        
        // for (auto &sum : sumCache) {
        //     printf("sum: %d\n", sum);
        // }
        // printf("\n");
        
        if (sumCache.find(z) == sumCache.end()) return false;
        return true;
    }
};
