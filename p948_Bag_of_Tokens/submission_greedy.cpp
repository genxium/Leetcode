bool const debug = false;
/*
test cases
[100]
50
[100,200]
150
[100,200,300,400]
200
[99,14,40,68,88,50,92,32,50,50]
67
*/
class Solution {
public:
    int bagOfTokensScore(vector<int>& tokens, int power) {
        sort(tokens.begin(), tokens.end());
        deque<int> dq(tokens.begin(), tokens.end());
        int ans = 0;
        while (!dq.empty()) {
            auto d = dq.front();
            auto inc = dq.back();
            if (power >= d) {
                ++ans;
                power -= d;
                dq.pop_front();
                if (debug) printf("Consumed %d power, ans = %d\n", d, ans);
            } else if (ans > 0 && inc > d) {
                power += inc;
                --ans;
                dq.pop_back();
                if (debug) printf("Gained %d power, ans = %d\n", inc, ans);
            } else {
                break;
            }
        }
        return ans;
    }
};
