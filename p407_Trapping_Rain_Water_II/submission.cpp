/*
Although being a higher-dimension-version of https://github.com/genxium/Leetcode/tree/master/p42_Trapping_Rain_Water, the solution here for this problem is significantly different from https://github.com/genxium/Leetcode/blob/master/p42_Trapping_Rain_Water/solution.cpp.

For each (y[chosen], x[chosen]), a "Fence" is a sequence {(y[0], x[0]), ..., (y[u], x[u]), ..., (y[k-1], x[k-1])} where 
-α- each z(y[i], x[i]) >= z(y[chosen], x[chosen]), and
-β- each (y[i], x[i]), (y[(i+1)%k], x[(i+1)%k]) are 8-directional neighbor, and
-γ- the "Fence" can be concave.

Define
```
CriticalPillar(Fence) = min{ z(y[u], x[u]) | 0 <= u < k }
```
, it's obvious that a valid "Fence" for "(y[chosen1], x[chosen1])" might be invalid for "(y[chosen2], x[chosen2])" if "CriticalPillar(Fence) < z(y[chosen2], x[chosen2])".

On the other hand, there's a non-trivial but useful fact, that if a (y[chosen], x[chosen]) has many valid "Fence"es, then 
```
ContributionToWaterStorage(y[chosen], x[chosen]) = max{ CriticalPillar(Fence[t]) | Fence[t] contains (y[chosen], x[chosen]) }
```
, moreover as long as "Fence[i]" and "Fence[j]" both contain (y[chosen], x[chosen]), the equation holds true even if "Fence[i]" intersected with "Fence[j]".
*/
class HeightWithPosition {
public:
  int z;
  int y,x;
};

class HeightWithPositionCompareForHeap {
public:
  bool operator() (HeightWithPosition const &lhs, HeightWithPosition const &rhs) const {
    return lhs.z > rhs.z; // I want a "MinHeap" and the default is a "MaxHeap"
  }
};

vector<int> diry{+1, -1, 0, 0}; // +1 means down
vector<int> dirx{0, 0, +1, -1}; // +1 means right

class Solution {
public:
    int trapRainWater(vector<vector<int>>& heightMap) {
      /*
      test case #1
      [[12,13,1,12],[13,4,13,12],[13,8,10,12],[12,13,12,12],[13,13,13,13]]
      
      test case #2
      [[12,10,12,12],[12,1,2,12],[12,2,2,12],[12,12,12,12]]
      */
      priority_queue<HeightWithPosition, vector<HeightWithPosition>, HeightWithPositionCompareForHeap> pq;
      
      unordered_set<int> alreadyFencePillarCandidate;      
      int yUpper = heightMap.size(), xUpper = heightMap[0].size();
    
      // Init with the outermost "Fence".
      for (int y = 0; y < yUpper; ++y) {
        HeightWithPosition l; 
        l.y = y; l.x = 0;
        l.z = heightMap[l.y][l.x];
        if (alreadyFencePillarCandidate.find(l.y*xUpper + l.x) == alreadyFencePillarCandidate.end()) {
          alreadyFencePillarCandidate.insert(l.y*xUpper + l.x);
          pq.push(l);  
        }
        
        HeightWithPosition r; 
        r.y = y; r.x = xUpper-1;
        r.z = heightMap[r.y][r.x]; 
        if (alreadyFencePillarCandidate.find(r.y*xUpper + r.x) == alreadyFencePillarCandidate.end()) {
          alreadyFencePillarCandidate.insert(r.y*xUpper + r.x);
          pq.push(r);  
        }
      }
      
      for (int x = 0; x < xUpper; ++x) {
        HeightWithPosition t; 
        t.y = 0; t.x = x;
        t.z = heightMap[t.y][t.x]; 
        if (alreadyFencePillarCandidate.find(t.y*xUpper + t.x) == alreadyFencePillarCandidate.end()) {
          alreadyFencePillarCandidate.insert(t.y*xUpper + t.x);
          pq.push(t);  
        }
        
        HeightWithPosition b; 
        b.y = yUpper-1; b.x = x;
        b.z = heightMap[b.y][b.x]; 
        if (alreadyFencePillarCandidate.find(b.y*xUpper + b.x) == alreadyFencePillarCandidate.end()) {
          alreadyFencePillarCandidate.insert(b.y*xUpper + b.x);
          pq.push(b);  
        }
      }
      
      int maxCriticalPillarOfFencesByFar = pq.top().z;
      int waterStored = 0;
      // Loop
      while (false == pq.empty()) {
        // By "SomeFence", I mean seriously I don't know the whole sequence of it, but it doesn't matter :)
        HeightWithPosition criticalPillarOfSomeFence = pq.top(); pq.pop();
        
        //printf("criticalPillarOfSomeFence (z:%d, (y:%d, x:%d))\n", criticalPillarOfSomeFence.z, criticalPillarOfSomeFence.y, criticalPillarOfSomeFence.x);

        if (criticalPillarOfSomeFence.z > maxCriticalPillarOfFencesByFar) {
          maxCriticalPillarOfFencesByFar = criticalPillarOfSomeFence.z;
          //printf("\tupdated maxCriticalPillarOfFencesByFar to %d\n", maxCriticalPillarOfFencesByFar);
        }
        
        for (int i = 0; i < 4; ++i) {
          int ny = criticalPillarOfSomeFence.y + diry[i];
          int nx = criticalPillarOfSomeFence.x + dirx[i];
          if (0 > ny || 0 > nx || yUpper <= ny || xUpper <= nx) continue;
          int nPosIndex = ny*xUpper + nx;
          if (alreadyFencePillarCandidate.find(nPosIndex) != alreadyFencePillarCandidate.end()) continue;
          
          int nz = heightMap[ny][nx];
          if (maxCriticalPillarOfFencesByFar > nz) {
            // suffices condition (α)
            //printf("\t@maxCriticalPillarOfFencesByFar:%d, added contribution to waterStored at (nz:%d, (ny:%d, nx:%d)), \n", maxCriticalPillarOfFencesByFar, nz, ny, nx);

            waterStored += (maxCriticalPillarOfFencesByFar - nz);
          }
          
          // The new "Fence" formed here by "candidateCriticalPillarOfCandidateFence" automatically suffices condition (β).
          /*
          It's important that we take (ny, nx) whose "maxCriticalPillarOfFencesByFar > nz" as a "candidateCriticalPillarOfCandidateFence", consider "test case #2".
          ```
            x 0   1   2   3
          y ---------------- 
          0 | 12, 10, 12, 12
          1 | 12, 1,  2,  12
          2 | 12, 2,  2,  12
          3 | 12, 12, 12, 12
          ```
          where we immediately meet "(nz:1, (ny:1, nx:1))" by "(criticalPillarOfSomeFence.z:10, (criticalPillarOfSomeFence.y:0, criticalPillarOfSomeFence.x:1))". If we DIDN'T take this "(nz:1, (ny:1, nx:1))" as a candidate, then we WOULD HAVE GOT "maxCriticalPillarOfFencesByFar == 12" by reaching "(nz':2, (ny':1, nx':2))".
          */
          HeightWithPosition candidateCriticalPillarOfCandidateFence;
          candidateCriticalPillarOfCandidateFence.z = nz;
          candidateCriticalPillarOfCandidateFence.y = ny;
          candidateCriticalPillarOfCandidateFence.x = nx;
          alreadyFencePillarCandidate.insert(nPosIndex);
          
          pq.push(candidateCriticalPillarOfCandidateFence);
        }
      }
      
      return waterStored;
    }
};
