bool debug = false;

/*
test cases
[5,4,3,2,1]
[2,1,3,4,4]
[4,2,2,1,1]
[1,1,0,0,1]
[0,0,1,1,1]
[0,3,0,3,2]
[8,8,6,1,0,8,0,6,9,9]
*/
class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int n = arr.size();
        vector<int> minInSuffix(n, INT_MAX);
        minInSuffix[n-1] = arr[n-1];
        for (int i = n-2; i >= 0; --i) {
            minInSuffix[i] = min(minInSuffix[i+1], arr[i]);
        }

        int ans = 1, maxInPrefix = INT_MIN;
        for (int i = 0; i < n; ++i) {
            maxInPrefix = max(maxInPrefix, arr[i]);
            if (i+1 < n && maxInPrefix <= minInSuffix[i+1]) {
                // Make a cut between "arr[i]" and "arr[i+1]".
                ++ans;
            } 
        }

        return ans;
    }
};
