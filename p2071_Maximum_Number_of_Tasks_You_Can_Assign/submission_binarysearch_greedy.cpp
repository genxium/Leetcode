bool const debug = false;

class Solution {
public:
    bool isValid(int k, vector<int>& tasks, vector<int>& workers, int pills, int strength) {
        // Greedily checks whether the "strongest k workers" can take the "easiest k tasks".
        int n = tasks.size(), m = workers.size();
        multiset<int> candWorkers = multiset<int>(workers.begin()+(m-k), workers.end()); 

        // [WARNING] MUST traverse from the hardest to easiest, only in this order can we assert a failure when there's "no available worker with pill" for each "current task".
        for (int j = k-1; j >= 0; --j) {
            auto cand1 = candWorkers.lower_bound(tasks[j]);
            if (cand1 != candWorkers.end()) {
                candWorkers.erase(cand1);
                continue; 
            }

            if (0 >= pills) return false;

            /* 
            Whenever it's possible to take a pill to complete "tasks[j]", we choose the weakiest possible worker, because for all remaining "i < j" we know that "tasks[i] < tasks[j]", no need to waste a stronger worker who might be able to later take "tasks[i]" without pill.
            */
            auto cand2 = candWorkers.lower_bound(tasks[j]-strength);
            if (cand2 != candWorkers.end()) {
                --pills;
                candWorkers.erase(cand2);
                continue;
            }

            // By now "cand2 == candWorkers.end()", the current "tasks[j]" couldn't be completed even by the strongest worker
            if (debug) printf("False for k:%d @j:%d\n", k, j);
            return false;
        }

        return true;
    }

    int maxTaskAssign(vector<int>& tasks, vector<int>& workers, int pills, int strength) {
        int n = tasks.size(), m = workers.size();
        sort(tasks.begin(), tasks.end());
        sort(workers.begin(), workers.end());
        int kL = 0, kR = min(n, m);
        int ans = 0;
        while (kL <= kR) {
            int k = ((kL + kR) >> 1);
            if (isValid(k, tasks, workers, pills, strength)) {
                ans = max(ans, k);
                kL = k+1;
            } else {
                kR = k-1;
            }
        }

        return ans;
    }
};
