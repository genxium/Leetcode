class Solution {
public:
     int largestRectangleArea(vector<int>& heights) {
        int n = heights.size();
        if (0 == n) {
            return 0;
        }
        vector<int> l(n, n); // l[i] is the leftmost index such that anyOf(heights[l[i]], heights[l[i]+1], ..., heights[i-1]) >= heights[i]
        vector<int> r(n, n); // r[i] is the rightmost index such that heights[i] <= anyOf(heights[i+1], ..., heights[r[i]-1], heights[r[i]])
        
        // l[] init
        l[0] = 0;
            
        // l[] loop
        for (int i = 1; i < n; ++i) {
            if (heights[i-1] < heights[i]) {
                l[i] = i;
            } else {
                // heights[i-1] >= heights[i]
                int temp = l[i-1];
                while(temp-1 >= 0 && heights[temp-1] >= heights[i]) {
                    temp = l[temp-1];
                }
                l[i] = temp;
            }
        }
            
        // r[] init
        r[n-1] = n-1;
        
        // r[] loop
        for (int i = n-2; i >= 0; --i) {
            if (heights[i+1] < heights[i]) {
                r[i] = i;
            } else {
                // heights[1+1] >= heights[i]
                int temp = r[i+1];
                while(temp+1 < n && heights[temp+1] >= heights[i]) {
                    temp = r[temp+1];
                }
                r[i] = temp;
            }
        }
        
        int ans = 0;
        for (int i = 0; i < n; ++i) {
            int area = (r[i] - l[i] + 1)*heights[i];
            if (area > ans) ans = area;
        }
        return ans;
    }
    
    int maximalRectangle(vector<vector<char>>& matrix) {
        int yUpper = matrix.size();
        if (0 == yUpper) return 0;
        int xUpper = matrix[0].size();
        int ans = 0;
        vector<int> heights(xUpper, 0); // refreshed for every row(a.k.a. y value) 
        for (int y = 0; y < yUpper; ++y) {
            int yPrevRow = y-1;
            
            // Obtain "vector<int> heights" for the current y.
            for (int x = 0; x < xUpper; ++x) {
                if (matrix[y][x] == '0') {
                    heights[x] = 0;
                } else {
                    if (yPrevRow >= 0 && matrix[yPrevRow][x] == '1') {
                        heights[x] = heights[x] + 1;
                    } else {
                        heights[x] = 1;
                    }
                }
            }
            int candidate = largestRectangleArea(heights);
            if (candidate > ans) {
                ans = candidate;
            }
        }
        return ans;
    }
};
