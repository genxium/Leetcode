bool debug = false;

typedef pair<int, int> COOR;
class Solution {
private:
    bool isValid(COOR& next, int yUpper, int xUpper) {
      if (next.first < 0 || next.second < 0) return false;
      if (next.first >= yUpper || next.second >= xUpper) return false;
      return true;
    }
public:
    vector<int> findDiagonalOrder(vector<vector<int>>& matrix) {
      vector<int> ans;
      int yUpper = matrix.size();
      if (0 >= yUpper) return ans;
      int xUpper = matrix[0].size();
      
      int d = +1, count = 0, limit = yUpper*xUpper;
      COOR cur = {0, 0}; // {y, x} in the original matrix
      while (count < limit) {
        if (debug) printf("coordinate {y:%d, x:%d} is valid, now d == %d\n", cur.first, cur.second, d);
        ans.push_back(matrix[cur.first][cur.second]);
        ++count;
        if (count == limit) break;
        COOR next = {cur.first + (-1)*d, cur.second + (+1)*d};
        if (!isValid(next, yUpper, xUpper)) {
          if (debug) printf("\tcoordinate {y:%d, x:%d} is invalid#1, now d == %d\n", next.first, next.second, d);
          next.second += 1; // move right first
          d = -d; // turn around
          while (!isValid(next, yUpper, xUpper)) {
            if (debug) printf("\tcoordinate {y:%d, x:%d} is invalid#2, now d == %d\n", next.first, next.second, d);
            next.first += (-1)*d;
            next.second += (+1)*d;
          }
        }
        cur.first = next.first;
        cur.second = next.second;
      }
      
      return ans;
    }
};
