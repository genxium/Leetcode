typedef pair<int, int> PII;

class Solution {
public:
    vector<int> kthSmallestPrimeFraction(vector<int>& arr, int k) {
        int n = arr.size();
        vector<PII> quotients;
        for (int i = 0; i+1 < n; ++i) {
            for (int j = i+1; j < n; ++j) {
                quotients.push_back({arr[i], arr[j]});
            }
        }
        
        sort(quotients.begin(), quotients.end(), [](PII const& lhs, PII const &rhs) {
            return lhs.first*rhs.second < lhs.second*rhs.first; 
        });
        
        return {quotients[k-1].first, quotients[k-1].second};
    }
};
