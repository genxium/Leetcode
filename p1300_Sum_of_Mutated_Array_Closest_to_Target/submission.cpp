class Solution {
public:
    int findBestValue(vector<int>& arr, int target) {
        int n = arr.size();
        map<int, int> counterDict;
        for (int i = 0; i < n; ++i) {
            if (counterDict.end() == counterDict.find(arr[i])) {
                counterDict[arr[i]] = 1;                
            } else {
                counterDict[arr[i]] += 1;
            }
        }
        vector<int> psv(n, 0); // partial sum of value
        vector<int> psc(n, 0); // partial sum of count
        int idx = 0, m = 0;
        for (auto it = counterDict.begin(); it != counterDict.end(); ++it) {
            // The traversal is in order due to that "map" is implemented red-black.
            if (0 == idx) {
                psc[idx] = (it->second);
                psv[idx] = (it->first*it->second);
            } else {
                psc[idx] = psc[idx-1] + (it->second);
                psv[idx] = psv[idx-1] + (it->first*it->second);
            }
            ++idx;
            ++m;
        }
        int minDifference = INT_MAX;
        int ans = INT_MAX;
        idx = 0;
        int preValueAsLowerBound = INT_MIN;
        for (auto it = counterDict.begin(); it != counterDict.end(); ++it) {
            // At each "it", evaluate the "candidateX" to replace [it, ..., counterDict.end()).
            
            int remainingCount = psc[m-1] - (idx > 0 ? psc[idx-1] : 0);
            int remainingValue = target - (idx > 0 ? psv[idx-1] : 0);
            
            //printf("At idx == %d, remainingCount == psc[n-1] - psc[idx-1] == %d - %d == %d\n", idx, psc[n-1], (idx > 0 ? psc[idx-1] : 0), remainingCount);
            //printf("At idx == %d, remainingValue == target - psv[idx-1] == %d - %d == %d\n", idx, target, (idx > 0 ? psv[idx-1] : 0), remainingValue);

            
            int newValueLowerBound = preValueAsLowerBound;
            int newValueUpperBound = (it->first);
            
            double residueValue = (double)remainingValue/remainingCount;
            int candidate1 = (int)floor(residueValue);
            int candidate2 = (int)ceil(residueValue);
            
            // We must include (*it) to be replaced, thus capping "candidateX" by "newValueUpperBound".
            candidate1 = min(candidate1, newValueUpperBound);
            candidate2 = min(candidate2, newValueUpperBound);
            
            //printf("Evaluating newValueLowerBound == %d, newValueUpperBound == %d, remainingCount == %d, remainingValue == %d\n", newValueLowerBound, newValueUpperBound, remainingCount, remainingValue);

            if (newValueLowerBound <= candidate1) {
                int s = candidate1*remainingCount;
                int difference = abs(s - remainingValue);
                //printf("\tcandidate1 == %d, candidate1*remainingCount == %d, difference = %d\n", candidate1, s, difference);

                if (difference < minDifference) {
                    minDifference = difference;
                    ans = candidate1;
                }
            }
            if (newValueLowerBound <= candidate2) {
                int s = candidate2*remainingCount;
                int difference = abs(s - remainingValue);
                //printf("\tcandidate2 == %d, candidate2*remainingCount == %d, difference = %d\n", candidate2, s, difference);
                if (difference < minDifference) {
                    minDifference = difference;
                    ans = candidate2;
                }
            }
            ++idx;
            preValueAsLowerBound = (it->first);
        }
        return ans;
    }
};
