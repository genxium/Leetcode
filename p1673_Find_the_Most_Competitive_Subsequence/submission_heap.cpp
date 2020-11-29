typedef pair<int, int> PII;

class Solution {
public:
    vector<int> mostCompetitive(vector<int>& nums, int k) {
      vector<int> ans;
      int n = nums.size(), remainedK = k;
      int smallest = INT_MAX, smallestPos = -1;
      priority_queue<PII, vector<PII>, greater<PII>> pq;
      
      int iUpper0 = n-(remainedK-1)-1;
      for (int i = 0; i < iUpper0; ++i) {
        pq.push({nums[i], i});
      }
      
      while (ans.size() < k) {
        int i = smallestPos+1;
        int iUpper = n-(remainedK-1);
        pq.push({nums[iUpper-1], iUpper-1});
        while (!pq.empty() && pq.top().second < i) {
          pq.pop();
        }
        smallest = pq.top().first;
        smallestPos = pq.top().second;
        pq.pop();
        ans.push_back(smallest);
        --remainedK;
      }
      return ans;
    }
};
