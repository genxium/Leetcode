typedef int64_t LL;

double const PI = acos(-1.0);
bool debug = false;

typedef struct DiffedPoint {
  double theta, thetaDegrees;
  
  DiffedPoint(int aDx, int aDy) {
    theta = atan2(aDy, aDx);
    if (theta < 0) theta += 2*PI;
    thetaDegrees = theta*180.0/PI;
  }
} DiffedPoint;

class DiffedPointCompare {
public:
  bool operator() (DiffedPoint const &lhs, DiffedPoint const &rhs) {
    return lhs.thetaDegrees < rhs.thetaDegrees;
  }
  
  bool operator() (double lhs, DiffedPoint const &rhs) {
    return lhs < rhs.thetaDegrees;
  }
};

/*
test cases
[[2,1],[2,2],[3,3]]
90
[1,1]
[[2,1],[2,2],[3,4],[1,1]]
90
[1,1]
[[1,0],[2,1]]
13
[1,1]
[[2,1],[2,2],[3,4],[1,1]]
45
[1,1]
[[2,2],[2,0]]
90
[0,0]
[[2,2],[2,0]]
89
[1,1]
[[1,1],[2,2],[1,2],[2,1]]
0
[1,1]
[[1,1],[2,2],[3,3],[4,4],[1,2],[2,1]]
0
[1,1]
*/
class Solution {
public:
    int visiblePoints(vector<vector<int>>& points, int angle, vector<int>& location) {
      vector<DiffedPoint> diffedPoints; // whose magnitude is larger than 0 
      int ans = 0, atLocationCount = 0;
      int n = points.size();
      diffedPoints.reserve((n << 1));
      for (int i = 0; i < n; ++i) {
        int dx = points[i][0] - location[0];
        int dy = points[i][1] - location[1];
        if (dx == 0 && dy == 0) {
          ++atLocationCount;
          continue;
        }
        DiffedPoint single(dx, dy);
        diffedPoints.push_back(single);
      }
      
      sort(diffedPoints.begin(), diffedPoints.end(), DiffedPointCompare());
      int m = diffedPoints.size();
      if (debug) printf("There're %d diffedPoints in single-cycle\n", m);
      
      for (int i = 0; i < m; ++i) {
        DiffedPoint single = diffedPoints[i];         
        single.theta += 2*PI;
        single.thetaDegrees += 360.0;
        diffedPoints.push_back(single);
      }
      int mostCountInSight = 0;
      
      double prevTheta = 2*PI+1.0;
      auto cmpf = DiffedPointCompare();
      for (int i = 0; i < diffedPoints.size(); ++i) {
        if (diffedPoints[i].theta == prevTheta) {
          continue;
        }
        double maxCounterpart = (double)angle + diffedPoints[i].thetaDegrees;
        auto it = upper_bound(diffedPoints.begin(), diffedPoints.end(), maxCounterpart, cmpf);
        
        if (debug) printf("For the %d-th double-cycle diffedPoint {thetaDegrees: %.3lf}, maxCounterpart is %.3lf, (int)(it-diffedPoints.begin()):%d\n", i, diffedPoints[i].thetaDegrees, maxCounterpart, (int)(it-diffedPoints.begin()));

        int candidateCountInSight = (it-diffedPoints.begin()-i);
        mostCountInSight = mostCountInSight > candidateCountInSight ? mostCountInSight : candidateCountInSight;
        prevTheta = diffedPoints[i].theta;
      }
      
      return mostCountInSight+atLocationCount;
    }
};

