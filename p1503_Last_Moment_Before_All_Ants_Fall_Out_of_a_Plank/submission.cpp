class Solution {
public:
    /*
    Consider now only 2 ants, one at p[1] moving towards right, the other at p[2] moving towards left, p[1] < p[2].
    When they collide, the time each of them falls(from "now", not from "collision") only swaps with that of the other.
    */
    int getLastMoment(int n, vector<int>& left, vector<int>& right) {
        int ans = 0;
        for (auto &val : left) {
            if (val > ans)
                ans = val;
        }
        for (auto &val : right) {
            if (n-val > ans)
                ans = n-val;
        }
        return ans;
    }
};
