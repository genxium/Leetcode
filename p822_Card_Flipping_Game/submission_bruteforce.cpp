class Solution {
private:
    bool isGood(int i, vector<int>& ours, vector<int>& theirs) {
        int val = ours[i];
        for (int j = 0; j < theirs.size(); ++j) {
            if (theirs[j] != val) continue;
            if (ours[j] == val) {
                // Couldn't flip cards[j] to make "val" non-existence in "theirs"
                return false;
            }
        }
        return true;
    }
    
public:
    int flipgame(vector<int>& fronts, vector<int>& backs) {
        int ans = INT_MAX, n = fronts.size();
        
        for (int i = 0; i < n; ++i) {
            if (isGood(i, backs, fronts)) {
                ans = min(ans, backs[i]);
            }
        }
    
        for (int i = 0; i < n; ++i) {
            if (isGood(i, fronts, backs)) {
                ans = min(ans, fronts[i]);
            }
        }
        
        return INT_MAX == ans ? 0 : ans;
    }
};
