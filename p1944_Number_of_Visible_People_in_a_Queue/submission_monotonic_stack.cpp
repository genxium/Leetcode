typedef vector<int> VI;
bool debug = false;
class Solution {
public:
    VI canSeePersonsCount(VI& heights) {
        stack<int> monostk;
        int n = heights.size();
        VI ans(n, 0);
        for (int i = n-1; i >= 0; --i) {
            if (debug) printf("i:%d, monostk.size():%lu, monostk.top():%d\n", i, monostk.size(), monostk.empty() ? -1 : monostk.top());
            while (!monostk.empty() && heights[monostk.top()] < heights[i]) {
                // no i' < i can see "monostk.top()" later because "heights[monostk.top()] < heights[i]"
                monostk.pop();
                ++ans[i];
            }
            ans[i] += (false == monostk.empty()); // can see the immediate neighbour to the right (after those in the middle all popped)
            monostk.push(i);
        }

        return ans;
    }
};
