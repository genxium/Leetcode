class Solution {
public:
    int diff(string &t1, string &t2) {
        int h1 = stoi(t1.substr(0, 2));
        int m1 = stoi(t1.substr(3, 2));
        int h2 = stoi(t2.substr(0, 2));
        int m2 = stoi(t2.substr(3, 2));
        return abs((h2 - h1)*60 + (m2 - m1));
    }
    
    int findMinDifference(vector<string>& timePoints) {
        int n = timePoints.size();
        sort(timePoints.begin(), timePoints.end());
        int ans = INT_MAX;
        for (int i = 0; i < n; ++i) {
            int localDiff = diff(timePoints[(i+1)%n], timePoints[i]);
            localDiff = min(localDiff, 1440-localDiff); // there're 1440 mins in an hour
            ans = min(localDiff, ans);
        }
        return ans;
    }
};
