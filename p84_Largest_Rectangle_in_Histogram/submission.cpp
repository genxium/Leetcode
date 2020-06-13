class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        int n = heights.size();
        if (0 == n) {
            return 0;
        }
        vector<int> l(n, n); // l[i] is the leftmost index such that heights[l[i]] >= heights[l[i]+1] >= ... >= heights[i]
        vector<int> r(n, n); // r[i] is the rightmost index such that heights[i] <= ... <= heights[r[i]-1] <= heights[r[i]]
            
        /*
        * The following case
        * ```
        * pos:     0  1   2  3
        * heights: 8  13  7  9
        * i      :           ^
        * ```
        * is worth noticing, here "l[i-1] == l[2] == 0" yet "l[i] == l[3] == 3".
        */
        
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
};
