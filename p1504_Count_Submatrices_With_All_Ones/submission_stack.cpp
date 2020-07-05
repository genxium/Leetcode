#define MAXN 150
int p_arr[MAXN][MAXN];

class Solution {
public:
    void findPrefixCount(int p_arr[][MAXN], vector<vector<int>> arr) { 
        int yUpper = arr.size();
        int xUpper = arr[0].size();
        for (int i = 0; i < yUpper; i++) { 
            for (int j = xUpper - 1; j >= 0; j--) {  
                if (0 == arr[i][j]) 
                    continue; 
  
                if (j != xUpper - 1) 
                    p_arr[i][j] += p_arr[i][j + 1]; 
  
                p_arr[i][j] += (int)arr[i][j]; 
            } 
        } 
    } 

    int numSubmat(vector<vector<int>>& arr) {
        memset(p_arr, 0, sizeof(p_arr));  
        findPrefixCount(p_arr, arr); 
  
        int ans = 0; 
        int yUpper = arr.size();
        int xUpper = arr[0].size();
        
        for (int j = 0; j < xUpper; j++) { 
            int i = yUpper - 1; 
            stack<pair<int, int>> q; 
            int to_sum = 0; 
  
            while (i >= 0) { 
                int c = 0; 
                while (q.size() != 0 and q.top().first > p_arr[i][j]) { 
                    to_sum -= (q.top().second + 1) * (q.top().first - p_arr[i][j]); 
                    c += q.top().second + 1; 
                    q.pop(); 
                }
                to_sum += p_arr[i][j]; 
                ans += to_sum; 
                q.push({ p_arr[i][j], c }); 
                i--; 
            } 
        }
        return ans; 
    }
};
