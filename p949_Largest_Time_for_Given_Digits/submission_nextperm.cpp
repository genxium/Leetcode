class Solution {
public:
    string toTime(vector<int>& arr) {
        int hh = arr[0]*10 + arr[1];
        int mm = arr[2]*10 + arr[3];
        if (0 > hh || 24 <= hh || 0 > mm || 60 <= mm) return "";
        char buff[100];
        memset(buff, '\0', sizeof buff);
        sprintf(buff, "%02d:%02d", hh, mm);
        return string(buff);
    }
    
    string largestTimeFromDigits(vector<int>& arr) {
        sort(arr.begin(), arr.end());
        string ans = "";
        do {
            string cand = toTime(arr);
            if ("" == cand) continue;
            if ("" == ans) ans = cand;
            else if (cand > ans) ans = cand;
        } while(next_permutation(arr.begin(), arr.end()));
        return ans;
    }
};
