vector<int> sqNums;

class Solution {
public:
  void search(int n, int count, int &ans) {
    if (count > ans) {
      return;
    }
    if (0 == n) {
      if (count < ans) {
        ans = count;
      }
      return;
    }
    auto upper = upper_bound(sqNums.begin(), sqNums.end(), n);
    //printf("(int)(upper-sqNums.begin()) for n:%d is %d\n", n, (int)(upper-sqNums.begin()));
    for (auto it = make_reverse_iterator(upper); it != sqNums.rend(); ++it) {
      int toDeduce = (*it);
      search(n-toDeduce, count+1, ans);
    }
  }
  
  int numSquares(int n) {
    int sqrtUpper = floor(sqrt(n));
    for (int s = 1; s <= sqrtUpper; ++s) {
      sqNums.push_back(s*s);
    }
    /*
    for (auto &sqNum : sqNums) {
      printf("%d ", sqNum);
    }
    printf("\n");
    */
    int ans = INT_MAX;
    search(n, 0, ans);
    return ans;
  }
};
