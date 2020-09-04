#define PI pair<int, int>
#define PSET unordered_set<PI, PairHash>
#define MAXZ 1000000
bool validZ[MAXZ+1];

class PairHash {
public:
    inline std::size_t operator()(PI const& v) const {
        return v.first*31+v.second;
    }
};

class Solution {
public:
    bool canMeasureWater(int x, int y, int z) {
        if (x < y) swap(x, y);
        
        PSET pairCache;
        memset(validZ, false, sizeof(validZ));
        
        queue<PI> q;
        
        // init
        q.push({0, 0});
        pairCache.insert({0, 0});
        validZ[0] = true;
        
        // loop
        while (!q.empty()) {
            int prevSize = q.size();
            while (prevSize--) {
                PI expander = q.front(); q.pop();
                
                int containedInX = expander.first;
                int containedInY = expander.second;

                // printf("containedInX: %d, containedInY: %d\n", containedInX, containedInY);
                int sum = containedInX + containedInY;
                if (sum <= MAXZ) {
                    validZ[sum] = true;
                }
                validZ[containedInX] = true;
                validZ[containedInY] = true;

                vector<PI> candidates = {
                    {0, containedInY}, // empty x
                    {containedInX, 0}, // empty y
                    {x, containedInY}, // fill x
                    {containedInX, y}, // fill y
                    {sum > x ? x : sum, sum > x ? sum-x : 0}, // try to pour containedInY into x
                    {sum > y ? sum-y : 0, sum > y ? y : sum}, // // try to pour containedInX into y
                };
                
                for (auto &candidate : candidates) {
                    if (pairCache.find(candidate) == pairCache.end()) {
                        q.push(candidate);
                        pairCache.insert(candidate);
                    }
                } 
            }
        }
        
        return validZ[z];
    }
};
