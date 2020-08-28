#define MAXPOS 1000000007

class Solution {
public:
    int maxDistance(vector<int>& position, int m) {
        int n = position.size();
        sort(position.begin(), position.end());
        
        int l = 0, r = MAXPOS, ans = 0;
        while (l < r) {
            int mid = ((l + r) >> 1);
            if (isAvailable(position, m, mid)) {
                ans = mid;
                l = mid+1;
            } else {
                r = mid;
            }
        }
        
        return ans;
    }
    
    bool isAvailable(vector<int>& position, int m, int candidateMinBetweenEachPair) {
        int lastPlacedPos = position[0];
        --m;
        for (int i = 1; i < position.size(); ++i) {
            if (position[i] - lastPlacedPos < candidateMinBetweenEachPair) {
                // too close
                continue;
            } 
            lastPlacedPos = position[i];
            --m;
            if (0 == m) return true;
        }
        
        return false;
    }
};
