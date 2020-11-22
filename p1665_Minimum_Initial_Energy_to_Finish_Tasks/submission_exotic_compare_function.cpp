typedef vector<int> VI;

class TaskCompare {
public: 
  bool operator() (VI const& lhs, VI const &rhs) {
    /*
    See "sample#2", we might have to proceed with a task with smaller task[1].
    
    The thought here is that if "x-lhs[0] >= rhs[1] but x-rhs[0] < lhs[1]", then we should do "lhs" first.
    */
    return (-lhs[0]-rhs[1]) > (-rhs[0]-lhs[1]);
  }
};

class Solution {
public:
    int minimumEffort(vector<vector<int>>& tasks) {
      int l = 0, r = 0; 
      sort(tasks.begin(), tasks.end(), TaskCompare());
      
      for (auto &task : tasks) {
        r += task[1];
      }

      int ans = 0, remained = 0;
      for (auto task : tasks) {
        if (remained < task[1]) {
          ans += (task[1] - remained);
          remained = task[1];
        }
        remained -= task[0];
      }
      return ans;
    }
};
