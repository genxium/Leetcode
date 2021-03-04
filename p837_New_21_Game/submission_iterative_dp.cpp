int const MAXV = 20000;
double prob[MAXV]; // prob[v] is the probablity to reach "v" points, IF we stop drawing immediately after getting "v" or more points

/*
test cases
10
1
10
6
1
10
21
17
10
1
0
1
*/
class Solution {
public:
    double new21Game(int N, int K, int W) {
        /*
        prob[v] = SUM{prob[u]*(1/W) | max(0, v-W) <= u <= min(K-1, v-1)}, and "0 <= v <= N"
        */
        if (0 == K) return 1.0;
        
        // Init for step 0
        memset(prob, 0, sizeof prob);
        prob[0] = 1.0;
        double ratio = 1.0/W;
        double previousProbSum = 1.0; // the SUM{prob[u]} for at most "W" items 
        
        for (int v = 1; v <= N; ++v) {
            prob[v] = ratio*previousProbSum;
            if (v-W >= 0) {
                // The first one being subtracted will be "prob[0]".
                previousProbSum -= prob[v-W];                
            }
            if (v < K) {
                previousProbSum += prob[v];                
            }
        }
        
        double ans = 0.0;
        for (int v = K; v <= N; ++v) {
            ans += prob[v];
        }
        
        return ans;
    }
};
