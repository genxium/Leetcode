bool debug = false;
typedef pair<int, int> PII;

struct PqCmp {
    bool operator() (PII const& lhs, PII const& rhs) {
        /*
        It's comparing 
        ```
        (numerator+1)/(denominator+1) - (numerator)/(denominator)
        ```
        and popping the one with the largest diff if both numerator and denominator are incremented by 1. 
        */
        double lVal = ((double)lhs.first)/(lhs.second);
        double rVal = ((double)rhs.first)/(rhs.second);
        double ldiff = ((double)lhs.first+1)/(lhs.second+1)-lVal;
        double rdiff = ((double)rhs.first+1)/(rhs.second+1)-rVal;
        if (ldiff != rdiff) return ldiff < rdiff;
        return ((double)lhs.first)/(lhs.second) < ((double)rhs.first)/(rhs.second); // if diffs are the same, pop the larger value one
    }
};

class Solution {
public:
    double maxAverageRatio(vector<vector<int>>& classes, int extraStudents) {
        int n = classes.size();
        priority_queue<PII, vector<PII>, PqCmp> pq; // a max-heap on "classes[i][0]/classes[i][1]"
        for (int i = 0; i < n; ++i) {
            pq.push({classes[i][0], classes[i][1]});
        }

        double totRate = 0.0;
        while (!pq.empty() && extraStudents > 0) {
            auto t = pq.top(); pq.pop();
            if (debug) {
                printf("About to add extra student to {pass: %d, total: %d}\n", t.first, t.second);
            }
            if (t.first == t.second) {
                totRate += 1.0;            
            } else {
                t.first += 1;
                t.second += 1;
                if (debug) printf("\tAdding back {pass: %d, total: %d}\n", t.first, t.second);
                extraStudents -= 1;
                pq.push(t);
            }
        }

        while (!pq.empty()) {
            auto &t = pq.top();
            totRate += (double)t.first/t.second;
            pq.pop();
        }

        return totRate/n;
    }
};
