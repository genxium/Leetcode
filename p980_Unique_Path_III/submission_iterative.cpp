#define MAXN 20

class StepCountTuple {
  public:
    int y, x;    
    int stepCountFromSrc;
    // It's guaranteed in the problem description that "y*xUpper + x <= 20", for a grid with more number of cells than 64, we might use a "string" to store "visitedNodesInThisPath".
    int visitedNodesInThisPath;

    StepCountTuple() {
      this->y = -1;
      this->x = -1;
      this->stepCountFromSrc = 0;
      this->visitedNodesInThisPath = 0;
    }
    StepCountTuple(int aY, int aX) {
      this->y = aY;
      this->x = aX;
      this->stepCountFromSrc = 0;
      this->visitedNodesInThisPath = 0;
    }
};

vector<int> diry{0, 0, -1, +1}; // +1 == down
vector<int> dirx{-1, +1, 0, 0}; // +1 == right

int visitedNodeBit(int &y, int &x, int &xUpper) {
  return (1 << (y*xUpper + x)); 
}

string bitwisePrint(int visitedNodesInThisPath) {
  string toRet = "";

  for (int bitShift = 0; bitShift < 21; ++bitShift) {
    if ((visitedNodesInThisPath & (1 << bitShift)) > 0) {
        toRet.push_back('1');
    } else {
        toRet.push_back('0');
    }      
  }
  reverse(toRet.begin(), toRet.end());
  return toRet;
}

class Solution {
  public:
    int iterativeSearch(vector<vector<int>>& grid, int &nonObstacleCount, int &stY, int &stX, int &edY, int &edX, int &yUpper, int &xUpper) {

      queue<StepCountTuple> openQueue;
      StepCountTuple st(stY, stX); st.visitedNodesInThisPath |= visitedNodeBit(stY, stX, xUpper);
      openQueue.push(st);
      
      for (int nSteps = 0; nSteps < nonObstacleCount; ++nSteps) {
        vector<StepCountTuple> expanderList;
        while (false == openQueue.empty()) {
          StepCountTuple single = openQueue.front(); openQueue.pop();
          expanderList.push_back(single);
        }
        
        //printf("At nSteps == %d, expanderList:\n", nSteps);
        
        // Now that "openQueue" is empty.
        for (int i = 0; i < expanderList.size(); ++i) {
          StepCountTuple single = expanderList[i];
          //printf("\t(y:%d, x:%d, stepCountFromSrc:%d, visitedNodesInThisPath:%s)\n", single.y, single.x, single.stepCountFromSrc, bitwisePrint(single.visitedNodesInThisPath).c_str());

          for (int t = 0; t < 4; ++t) {
            int nbY = single.y + diry[t];
            int nbX = single.x + dirx[t];
            if (nbY < 0 || nbY >= yUpper || nbX < 0 || nbX >= xUpper) {
              //printf("\t\t(y:%d, x:%d) is out of the grid [x]\n", nbY, nbX);
              continue;
            }
            if (grid[nbY][nbX] == -1) {
              //printf("\t\t(y:%d, x:%d) is an obstacle [x]\n", nbY, nbX);
              continue;
            }
            int visitedNodeBitOfNb = visitedNodeBit(nbY, nbX, xUpper);
            if ((single.visitedNodesInThisPath & visitedNodeBitOfNb) > 0) {
              //printf("\t\t(y:%d, x:%d) is already visited in path history [x]\n", nbY, nbX);
              continue; 
            }
            StepCountTuple nbTuple(nbY, nbX); 
            nbTuple.stepCountFromSrc = single.stepCountFromSrc + 1; 
            nbTuple.visitedNodesInThisPath = (single.visitedNodesInThisPath | visitedNodeBitOfNb);
            /*
            * In the solution here, we keep effectively only 1 path count per "StepCountTuple", due to the need of de-duplicating. 
            *
            * Therefore we allow "tuple1" and "tuple2" to coexist simultaneously in "openQueue" where 
	          * - tuple1.y == tuple2.y && tuple1.x == tuple2.x, and
	          * - tuple1.stepCountFromSrc == tuple2.stepCountFromSrc, but
            * - tuple1.visitedNodesInThisPath != tuple2.visitedNodesInThisPath.
            */
            //printf("\t\tadding (y:%d, x:%d, stepCountFromSrc:%d, visitedNodesInThisPath:%s) [v]\n", nbTuple.y, nbTuple.x, nbTuple.stepCountFromSrc, bitwisePrint(nbTuple.visitedNodesInThisPath).c_str());
            openQueue.push(nbTuple); 
          }
        }
      }
      
      //printf("\nResidues in the openQueue:\n");
      int toRet = 0;
      while (false == openQueue.empty()) {
        auto single = openQueue.front(); openQueue.pop();
        //printf("\t(y:%d, x:%d, stepCountFromSrc:%d, visitedNodesInThisPath:%s)\n", single.y, single.x, single.stepCountFromSrc, bitwisePrint(single.visitedNodesInThisPath).c_str());
        if (single.y == edY && single.x == edX) {
          ++toRet; 
        }
      }

      return toRet;
    }

    int uniquePathsIII(vector<vector<int>>& grid) {
      int yUpper = grid.size();
      if (0 == yUpper) return 0;
      int xUpper = grid[0].size();

      int stY, stX, edY, edX;
      int nonObstacleCount = 0; // Including ed but excluding st for convenience.
      for (int y = 0; y < yUpper; ++y) {
        for (int x = 0; x < xUpper; ++x) {
          if (grid[y][x] == -1) {
              continue;
          }
          if (grid[y][x] == 1) {
            stY = y;
            stX = x;
            continue;
          }

          nonObstacleCount += 1;
          if (grid[y][x] == 2) {
            edY = y;
            edX = x;
          }
        }    
      }

      int ans = iterativeSearch(grid, nonObstacleCount, stY, stX, edY, edX, yUpper, xUpper);
      return ans;
    }
};
