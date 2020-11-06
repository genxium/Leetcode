typedef int64_t LL;

double const PI = acos(-1.0);

class DiffedPoint {
public:
  int dx, dy;
  LL magnitudeSquared;
  double magnitude;
  double sineTheta, cosineTheta;
  double cosineHalfTheta;
  
  double theta, thetaDegrees;
  
  DiffedPoint(int aDx, int aDy) {
    dx = aDx;
    dy = aDy;
    magnitudeSquared = (LL)dx*(LL)dx + (LL)dy*(LL)dy;
    magnitude = sqrt(magnitudeSquared);
    sineTheta = dy/magnitude;
    cosineTheta = dx/magnitude;
    cosineHalfTheta = (sineTheta >= 0 ? +1 : -1)*sqrt((1+cosineTheta)/2);
    
    theta = dy >= 0 ? acos(cosineTheta) : 2*PI-acos(cosineTheta);
    thetaDegrees = theta*180.0/PI;
  }
};

class DiffedPointCompare {
public:
  bool operator() (DiffedPoint const &lhs, DiffedPoint const &rhs) {
    if (lhs.thetaDegrees != rhs.thetaDegrees) {
      return lhs.thetaDegrees < rhs.thetaDegrees;
    }
    return lhs.magnitude < rhs.magnitude;
  }
  
  bool operator() (double lhs, DiffedPoint const &rhs) {
    return lhs < rhs.thetaDegrees;
  }
};

/*
test case#1
[[2,1],[2,2],[3,3]]
90
[1,1]

test case#2
[[2,1],[2,2],[3,4],[1,1]]
90
[1,1]

test case#3
[[1,0],[2,1]]
13
[1,1]

test case#4
[[2,1],[2,2],[3,4],[1,1]]
45
[1,1]

test case#5
[[2,2],[2,0]]
90
[0,0]

test case#6
[[2,2],[2,0]]
89
[1,1]

test case#7
[[1,1],[2,2],[1,2],[2,1]]
0
[1,1]

test case#8
[[1000000000,999999999],[999999999,1000000000]]
90
[0,0]
*/
class Solution {
public:
    int visiblePoints(vector<vector<int>>& points, int angle, vector<int>& location) {
      vector<DiffedPoint> diffedPoints; // whose magnitude is larger than 0 
      int ans = 0, atLocationCount = 0;
      int n = points.size();
      for (int i = 0; i < n; ++i) {
        int dx = points[i][0] - location[0];
        int dy = points[i][1] - location[1];
        if (dx == 0 && dy == 0) {
          ++atLocationCount;
          continue;
        }
        DiffedPoint single(dx, dy);
        diffedPoints.push_back(single);
        //printf("A new single-cycle diffedPoint is {thetaDegrees: %.3lf}\n", single.thetaDegrees);
      }
      
      sort(diffedPoints.begin(), diffedPoints.end(), DiffedPointCompare());
      int m = diffedPoints.size();
      //printf("There're %d diffedPoints in single-cycle\n", m);
      
      for (int i = 0; i < m; ++i) {
        DiffedPoint single(diffedPoints[i].dx, diffedPoints[i].dy);
        single.theta += 2*PI;
        single.thetaDegrees += 360.0;
        diffedPoints.push_back(single);
      }
      int mostCountInSight = 0;
      
      double prevTheta = 2*PI+1.0;
      for (int i = 0; i < diffedPoints.size(); ++i) {
        if (diffedPoints[i].theta == prevTheta) {
          continue;
        }
        double maxCounterpart = (double)angle + diffedPoints[i].thetaDegrees;
        if (maxCounterpart < 0) maxCounterpart += 360.0;

        auto it = upper_bound(diffedPoints.begin(), diffedPoints.end(), maxCounterpart, DiffedPointCompare());
        
        //printf("For the %d-th double-cycle diffedPoint {thetaDegrees: %.3lf}, maxCounterpart is %.3lf, (int)(it-diffedPoints.begin()):%d\n", i, diffedPoints[i].thetaDegrees, maxCounterpart, (int)(it-diffedPoints.begin()));

        int candidateCountInSight = (it-diffedPoints.begin()-i);
        mostCountInSight = max(mostCountInSight, candidateCountInSight);
        prevTheta = diffedPoints[i].theta;
      }
      
      return mostCountInSight+atLocationCount;
    }
};
