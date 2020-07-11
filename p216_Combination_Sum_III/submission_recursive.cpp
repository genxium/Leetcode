bool visited[10];

class Solution {
public:
    void search(int offset, int sum, int count, int &k, int& target, vector<vector<int>> &ans) {
      if (count == k) {
        if (sum == target) {
          vector<int> single;
          for (int i = 1; i < 10; ++i) {
            if (!visited[i]) continue;
            single.push_back(i);
          }
          ans.push_back(single);
        } 
        return;
      }
      
      for (int i = offset+1; i < 10; ++i) {
        if (visited[i]) continue;
        visited[i] = true;
        search(i, sum+i, count+1, k, target, ans);
        visited[i] = false; // backtracking
      }
    }
  
    vector<vector<int>> combinationSum3(int k, int n) {
      memset(visited, false, sizeof(visited));
      vector<vector<int>> ans;
      search(0, 0, 0, k, n, ans);
      return ans;
    }
};
