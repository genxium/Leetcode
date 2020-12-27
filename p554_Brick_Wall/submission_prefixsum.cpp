typedef vector<int> VI;
bool const debug = false;

class Solution {
public:
    int leastBricks(vector<vector<int>>& wall) {
        unordered_map<int, int> prefixWidthCounter;
        
        for (int i = 0; i < wall.size(); ++i) {
            int width = 0;
            // DON'T count last brick of each row!
            for (int j = 0; j < wall[i].size()-1; ++j) { 
                width += wall[i][j];
                ++prefixWidthCounter[width];
            }
        }
        
        int ans = wall.size(); // The answer will be no more than count of rows.
        
        for (auto &it : prefixWidthCounter) {
            if (wall.size() - it.second < ans) {
                ans = wall.size()-it.second;                
            }
        }
        
        return ans;
    }
};
