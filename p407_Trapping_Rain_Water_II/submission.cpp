/*
Although being a higher-dimension-version of https://github.com/genxium/Leetcode/tree/master/p42_Trapping_Rain_Water, the solution here for this problem is significantly different from https://github.com/genxium/Leetcode/blob/master/p42_Trapping_Rain_Water/solution.cpp.

For each (y[chosen], x[chosen]), a "Fence" is a sequence {(y[0], x[0]), ..., (y[i], x[i]), ..., (y[k-1], x[k-1])} where 
-α- each z(y[i], x[i]) >= z(y[chosen], x[chosen]), and
-β- each (y[i], x[i]), (y[(i+1)%k], x[(i+1)%k]) are 8-directional neighbor, and
-γ- the "Fence" can be concave.
*/
typedef vector<int> VI;
VI diry{+1, -1, 0, 0}; // +1 means down
VI dirx{0, 0, +1, -1}; // +1 means right

/*
test cases      
[[12,13,1,12],[13,4,13,12],[13,8,10,12],[12,13,12,12],[13,13,13,13]]
[[12,10,12,12],[12,1,2,12],[12,2,2,12],[12,12,12,12]]
*/
class Solution {
public:
    int trapRainWater(vector<vector<int>>& heightMap) {
      
      priority_queue<VI, vector<VI>, greater<VI>> pq; // a min-heap for z, i.e. the first value of each element
      
      set<VI> alreadyFencePillarCandidate;      
      int yUpper = heightMap.size(), xUpper = heightMap[0].size();
    
      // Init with the outermost "Fence".
      for (int y = 0; y < yUpper; ++y) {
        VI pos1 = {y, 0};
        VI ele1 = {heightMap[y][0], y, 0}; 
        if (!alreadyFencePillarCandidate.count(pos1)) {
          alreadyFencePillarCandidate.insert(pos1);
          pq.push(ele1);  
        }
        
        VI pos2 = {y, xUpper-1};
        VI ele2 = {heightMap[y][xUpper-1], y, xUpper-1}; 
        if (!alreadyFencePillarCandidate.count(pos2)) {
          alreadyFencePillarCandidate.insert(pos2);
          pq.push(ele2);  
        }
      }
      
      for (int x = 0; x < xUpper; ++x) {
        VI pos1 = {0, x};
        VI ele1 = {heightMap[0][x], 0, x};
        if (!alreadyFencePillarCandidate.count(pos1)) {
          alreadyFencePillarCandidate.insert(pos1);
          pq.push(ele1);  
        }
        
        VI pos2 = {yUpper-1, x};
        VI ele2 = {heightMap[yUpper-1][x], yUpper-1, x};
        if (!alreadyFencePillarCandidate.count(pos2)) {
          alreadyFencePillarCandidate.insert(pos2);
          pq.push(ele2);  
        }
      }
      
      int maxCriticalPillarOfFencesByFar = pq.top()[0];
      int waterStored = 0;
      // Loop
      while (false == pq.empty()) {
        VI criticalPillarOfCurrentFence = pq.top(); 
        pq.pop(); // the fence will be "pushed inward" by the following traversal of its neighbours
        
        if (criticalPillarOfCurrentFence[0] > maxCriticalPillarOfFencesByFar) {
          maxCriticalPillarOfFencesByFar = criticalPillarOfCurrentFence[0];
        }
        
        for (int i = 0; i < 4; ++i) {
          int ny = criticalPillarOfCurrentFence[1] + diry[i];
          int nx = criticalPillarOfCurrentFence[2] + dirx[i];
          if (0 > ny || 0 > nx || yUpper <= ny || xUpper <= nx) continue;
          VI currentPos = {ny, nx};
          if (alreadyFencePillarCandidate.count(currentPos)) continue;
          
          int nz = heightMap[ny][nx];
          if (maxCriticalPillarOfFencesByFar > nz) {
            // the next, more "inward" fence has a higher critical pillar, which increases "water storable" for later to-be-traversed pillars, i.e. more "inward" pillars 
            waterStored += (maxCriticalPillarOfFencesByFar - nz);
          }
          
          alreadyFencePillarCandidate.insert(currentPos);
          pq.push({nz, ny, nx});
        }
      }
      
      return waterStored;
    }
};
