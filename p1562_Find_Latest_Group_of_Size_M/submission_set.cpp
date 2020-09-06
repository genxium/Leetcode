/*
The solution here is based on https://github.com/genxium/Leetcode/blob/master/p352_Data_Stream_as_Disjoint_Intervals/submission_set.cpp.
*/

#define INVALID -1
#define INTERVAL_T pair<int, int>

class Solution {
private:
    /*
    Be very careful when using "collection.erase(it)", which blocks forever if "it" is "collection.end()"! 
    */
    unordered_map<int, int> sizeCounts;
    set<INTERVAL_T, less<INTERVAL_T>> collection;
    int targetM;

    void addNum(int val, int stepIdx, int &ans) {
        //printf("addNum, val:%d, stepIdx:%d\n", val, stepIdx);
        if (collection.empty()) {
            // The initial case.  
            collection.insert({val, val});
            ++sizeCounts[1];
            return;
        }

        INTERVAL_T newInterval = {val, INT_MAX}; 
        auto it = collection.upper_bound(newInterval);

        if (it != collection.end()) {
            // Now that "it" is "existingRightInterval".
            //printf("\texistingRightInterval [%d, %d]\n", it->first, it->second);

            if (it->first == newInterval.first+1) {
                int oldSize = it->second - it->first + 1;
                //printf("\t\toldSize is %d\n", oldSize);
                newInterval.second = it->second;
                it = collection.erase(it); // moves to next of "existingRightInterval"
                --sizeCounts[oldSize];
                if (oldSize == targetM && 0 == sizeCounts[oldSize]) {
                    ans = stepIdx;
                }
            }
        }
        if (newInterval.second == INT_MAX) {
            newInterval.second = val;
        }

        if (it != collection.begin()) {
            --it;
        }

        if (it != collection.end()) {
            // Now that "it" is "existingLeftInterval".
            //printf("\texistingLeftInterval [%d, %d]\n", it->first, it->second);
            if (it->first <= newInterval.first && newInterval.first <= it->second) {
                // The "newInterval" is already included.
                return;
            }
            if (it->second == newInterval.first-1) {
                int oldSize = it->second - it->first + 1;   
                //printf("\t\toldSize is %d\n", oldSize);
                newInterval.first = it->first;
                collection.erase(it);
                --sizeCounts[oldSize];
                if (oldSize == targetM && 0 == sizeCounts[oldSize]) {
                    ans = stepIdx;
                }
            }
        }

        int newSize = newInterval.second - newInterval.first + 1;
        collection.insert(newInterval);
        ++sizeCounts[newSize];
    }

public:
    int findLatestStep(vector<int>& arr, int m) {
        int n = arr.size();
        targetM = m;
        int ans = INVALID;
        for (int i = 0; i < n; ++i) {
            addNum(arr[i], i, ans);
        }
        if (INVALID == ans && sizeCounts[targetM] > 0) {
            ans = n;
        }
        return ans;
    }
};
