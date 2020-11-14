int const MAXN = 100000;
typedef int64_t LL;
typedef pair<int, int> PII; // interval 

LL prefixSum[MAXN];

class Compare {
public:
  bool operator() (PII const &lhs, PII const &rhs) {
    if (lhs.first != rhs.first) {
      return lhs.first < rhs.first;
    } else {
      // Under the same "start", the one with smaller "end" has less probability of getting overlapped.
      return lhs.second < rhs.second;
    }
  }
};

class Solution {
public:
    int maxNonOverlapping(vector<int>& nums, int target) {  
        unordered_map<LL, int> lastPosOfPrefixSum;
        
        memset(prefixSum, 0, sizeof(prefixSum));
        int n = nums.size();
        vector<PII> intervals;
        prefixSum[0] = nums[0];
        lastPosOfPrefixSum[prefixSum[0]] = 0;
        lastPosOfPrefixSum[0] = -1; // magic
      
        if (nums[0] == target) {
          intervals.push_back({0, 0});
        }
        
        for (int i = 1; i < n; ++i) {
          if (nums[i] == target) {
            intervals.push_back({i, i});
          }
          prefixSum[i] = prefixSum[i-1] + nums[i];
          int residue = prefixSum[i] - target;
          // printf("prefixSum[i:%d] == %d, residue == %d\n", i, prefixSum[i], residue);
          if (lastPosOfPrefixSum.find(residue) != lastPosOfPrefixSum.end()) {
              int st = lastPosOfPrefixSum[residue]+1;
              int ed = i;
              intervals.push_back({st, ed});
              // printf("\tgenerated interval {%d, %d}\n", st, ed);
          }
          
          lastPosOfPrefixSum[prefixSum[i]] = i; // for a tie we want the intervals to be as narrow as possible
        }
        
        list<PII> nonoverlappingResult;
      
        sort(intervals.begin(), intervals.end(), Compare());

        for (auto &candidate : intervals) {
          // printf("Evaluating candidate interval {%d, %d}\n", candidate.first, candidate.second);

          if (nonoverlappingResult.empty()) {
            nonoverlappingResult.push_back(candidate);
            continue;
          }
          auto &last = nonoverlappingResult.back();
          // Note that "last.first <= candidate.first" by the predefined "Compare()".
          if (candidate.first > last.second) {
            nonoverlappingResult.push_back(candidate);
          } else {
            // candidate.first <= last.second
            if (last.second > candidate.second) {
              nonoverlappingResult.pop_back();
              nonoverlappingResult.push_back(candidate);
            }
          }
        }

        return nonoverlappingResult.size();
    }
};
