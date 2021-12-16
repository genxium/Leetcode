typedef int64_t LL;
typedef vector<int> VI;
bool const debug = false;

struct FenwickTree {
    vector<int> bit;  // binary indexed tree
    int n;

    FenwickTree(int n) {
        this->n = n;
        bit.assign(n, 0);
    }

    FenwickTree(vector<int> &a) : FenwickTree(a.size()) {
        for (size_t i = 0; i < a.size(); i++)
            add(i, a[i]);
    }

    int sum(int r) {
        int ret = 0;
        for (; r >= 0; r = (r & (r + 1)) - 1) {
            ret += bit[r];
        }
        return ret;
    }

    int sum(int l, int r) {
        if (l == 0) return sum(r);
        else return sum(r) - sum(l-1);
    }

    void add(int idx, int delta) {
        for (; idx < n; idx = idx | (idx + 1))
            bit[idx] += delta;
    }
};

/*
test cases
[[2,8],[6,3],[8,6]]
5
4
[[0,9],[4,1],[5,7],[6,2],[7,4],[10,9]]
5
4
[[0,3],[6,4],[8,5]]
3
2
[[0,10000]]
200000
200000
*/
class Solution {
public:
    int maxTotalFruits(vector<vector<int>>& fruits, int startPos, int k) {
        // It's obvious that we need AT MOST 1 U-TURN to maximize the result. Therefore besides making a straight move of k steps in each direction from startPos, we can also traverse the "U-TURN gain" at each fruit location.
        int maxPos = startPos;
        for (VI &fruit : fruits) {
            if (maxPos < fruit[0]) maxPos = fruit[0];
        }

        FenwickTree ftIns(maxPos+1);
        if (debug) printf("maxPos is %d\n", maxPos);

        int n = fruits.size();
        int amtAtStart = 0;
        for (VI &fruit : fruits) {
            ftIns.add(fruit[0], fruit[1]);
            if (fruit[0] == startPos) amtAtStart = fruit[1];
        }

        if (debug) printf("ftIns initialized, amtAtStart is %d\n", amtAtStart);

        int ans = 0, rightReachable = min(startPos+k, maxPos),leftReachable = max(startPos-k, 0);
        if (debug) printf("startPos is %d, rightReachable is %d, leftReachable is %d\n", startPos, rightReachable, leftReachable);
        int cand1 = ftIns.sum(startPos, rightReachable);
        if (debug) printf("cand1 is %d\n", cand1);

        int cand2 = ftIns.sum(leftReachable, startPos);
        if (debug) printf("cand2 is %d\n", cand2);

        ans = max(cand1, cand2);

        for (VI &fruit : fruits) {
            int usedSteps = abs(fruit[0]-startPos);
            if (usedSteps >= k) continue;
            int remainingSteps = (k-usedSteps);
            if (fruit[0] >= startPos) {
                int leftReachable = max(fruit[0]-remainingSteps, 0);
                if (leftReachable < startPos) {
                    int cand = ftIns.sum(startPos, fruit[0]) 
                            + ftIns.sum(leftReachable, startPos) - amtAtStart;
                    ans = max(ans, cand);
                }
            } else {
                int rightReachable = min(fruit[0]+remainingSteps, maxPos);
                if (rightReachable > startPos) {
                    int cand = ftIns.sum(fruit[0], startPos) 
                            + ftIns.sum(startPos, rightReachable) - amtAtStart;
                    ans = max(ans, cand);
                }
            }
        }

        return ans;
    }
};
