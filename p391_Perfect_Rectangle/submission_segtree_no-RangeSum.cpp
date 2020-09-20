#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,tune=native")

bool TOP = false, BOTTOM = true; // Made sure that "TOP" precedes "BOTTOM" in "sortedYEdgeList" for same "y-value", such that we can detect an overlay in "RangeAdd". 

class Rectangle {
public:
  int l, r, t, b;
  int xIndexL, xIndexR;
  
  Rectangle(int aL, int aR, int aB, int aT) {
    l = aL;
    r = aR;
    b = aB;
    t = aT;
  }
};

class SegmentTreeNode {
public:
  int idx = 0;
  int fullCoverAccDiff = 0; // Accumulated diff of this node by "full cover" updates.  
  int leftIndexClosed; // To be initialized upon construction.
  int rightIndexOpen; // To be initialized upon construction.
  int leftXClosed;
  int rightXClosed; // It's BY DESIGN that "rightXClosed = discretizedXDict[rightIndexOpen]" in this problem/solution.
};

#define MAX_DISCRETIZED_X 5501
SegmentTreeNode theTree[((1+MAX_DISCRETIZED_X) << 1)+(1+MAX_DISCRETIZED_X)]; // to avoid the lag caused by heap allocation
int discretizedXDict[MAX_DISCRETIZED_X]; // sortedDedupedIndex -> value
int discretizedXDictSize = 0;

void RangeAdd(SegmentTreeNode* currentRoot, int newSegLeftIndexClosed, int newSegRightIndexOpen, pair<int, bool> unifiedYEdge, bool &gotOverlay, int level) {
  // Reject invalid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s. 
  if (newSegLeftIndexClosed >= newSegRightIndexOpen) return;
  if (newSegLeftIndexClosed >= currentRoot->rightIndexOpen) return;
  if (newSegRightIndexOpen <= currentRoot->leftIndexClosed) return;

  // Snap valid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s.
  newSegLeftIndexClosed = (newSegLeftIndexClosed > currentRoot->leftIndexClosed ? newSegLeftIndexClosed : currentRoot->leftIndexClosed);
  newSegRightIndexOpen = (newSegRightIndexOpen < currentRoot->rightIndexOpen ? newSegRightIndexOpen : currentRoot->rightIndexOpen);
  
  int indentSpaceCount = (level << 1);

  // printf("%*sRangeAdd, currentRoot:[%d, %d), [newSegLeftIndexClosed:%d, newSegRightIndexOpen:%d), (y: %d, flag:%s)\n", indentSpaceCount, "", currentRoot->leftIndexClosed, currentRoot->rightIndexOpen, newSegLeftIndexClosed, newSegRightIndexOpen, unifiedYEdge.first, unifiedYEdge.second == TOP ? "TOP" : "BOTTOM");

  if (0 < currentRoot->fullCoverAccDiff && unifiedYEdge.second == BOTTOM) {
    gotOverlay = true;
    return;
  }

  if (newSegLeftIndexClosed <= currentRoot->leftIndexClosed && newSegRightIndexOpen >= currentRoot->rightIndexOpen) {
    // printf("%*sRangeAdd(pre), currentRoot:[%d, %d).fullCoverAccDiff == %d\n", indentSpaceCount, "", currentRoot->leftIndexClosed, currentRoot->rightIndexOpen, currentRoot->fullCoverAccDiff);
    // Proactively stops at "full cover" update.
    if (unifiedYEdge.second == TOP) {
      currentRoot->fullCoverAccDiff += -1;
    } else {
      currentRoot->fullCoverAccDiff += +1;
    }
    // printf("%*sRangeAdd(post), currentRoot:[%d, %d).fullCoverAccDiff == %d\n", indentSpaceCount, "", currentRoot->leftIndexClosed, currentRoot->rightIndexOpen, currentRoot->fullCoverAccDiff);
    return;
  }
  int mid = ((currentRoot->leftIndexClosed + currentRoot->rightIndexOpen) >> 1);
  int lChildIdx = (currentRoot->idx << 1);
  if (newSegLeftIndexClosed < mid) {
    if (0 == theTree[lChildIdx].idx) {
      theTree[lChildIdx].idx = lChildIdx;
      theTree[lChildIdx].leftIndexClosed = currentRoot->leftIndexClosed;
      theTree[lChildIdx].rightIndexOpen = mid;
      // auto &lChild = theTree[lChildIdx];
      // printf("%*sinitializing lChild with idx:%d, leftIndexClosed:%d, rightIndexOpen:%d\n", indentSpaceCount, "", lChildIdx, lChild.leftIndexClosed, lChild.rightIndexOpen);

      theTree[lChildIdx].leftXClosed = discretizedXDict[theTree[lChildIdx].leftIndexClosed];
      theTree[lChildIdx].rightXClosed = (
                            theTree[lChildIdx].rightIndexOpen >= discretizedXDictSize 
                            ?
                            INT_MAX 
                            : 
                            discretizedXDict[theTree[lChildIdx].rightIndexOpen]
                            );
    }
    RangeAdd(&(theTree[lChildIdx]), newSegLeftIndexClosed, newSegRightIndexOpen, unifiedYEdge, gotOverlay, level+1);
  }

  int rChildIdx = (currentRoot->idx << 1)+1;
  if (newSegRightIndexOpen > mid) {
    if (0 == theTree[rChildIdx].idx) {
      theTree[rChildIdx].idx = rChildIdx;
      theTree[rChildIdx].leftIndexClosed = mid;
      theTree[rChildIdx].rightIndexOpen = currentRoot->rightIndexOpen;
      // printf("%*sinitializing rChild with idx:%d, leftIndexClosed:%d, rightIndexOpen:%d\n", indentSpaceCount, "", rChildIdx, rChild.leftIndexClosed, rChild.rightIndexOpen);
      theTree[rChildIdx].leftXClosed = discretizedXDict[theTree[rChildIdx].leftIndexClosed];
      theTree[rChildIdx].rightXClosed = (
                            theTree[rChildIdx].rightIndexOpen >= discretizedXDictSize 
                            ?
                            INT_MAX 
                            : 
                            discretizedXDict[theTree[rChildIdx].rightIndexOpen]
                            );
    }
    RangeAdd(&(theTree[rChildIdx]), newSegLeftIndexClosed, newSegRightIndexOpen, unifiedYEdge, gotOverlay, level+1);
  }
  
  currentRoot->fullCoverAccDiff = min(theTree[lChildIdx].fullCoverAccDiff, theTree[rChildIdx].fullCoverAccDiff);
}

class Solution {
public:
    bool isRectangleCover(vector<vector<int>>& rectangles) {
      if (1 == rectangles.size()) {
        // the trivial case
        return true;
      }
      
      int rawAreaSum = 0u;
      vector<Rectangle> recs;
      int minL = INT_MAX, maxR = INT_MIN, minB = INT_MAX, maxT = INT_MIN;
      for (auto &rectangle : rectangles) {
        Rectangle rec(rectangle[0], rectangle[2], rectangle[1], rectangle[3]);
        recs.push_back(rec);
        rawAreaSum += abs(rec.r - rec.l)*abs(rec.t - rec.b);
        minL = min(minL, rec.l);
        maxR = max(maxR, rec.r);
        minB = min(minB, rec.b);
        maxT = max(maxT, rec.t);
      }
      
      // Discretize and index each of "rec.l & rec.r".
      set<int> sortedDedupedXSet;
      for (auto &rec : recs) {
        sortedDedupedXSet.insert(rec.l);
        sortedDedupedXSet.insert(rec.r);
      }
      vector<int> sortedDedupedXList;
      for (auto it = sortedDedupedXSet.begin(); it != sortedDedupedXSet.end(); ++it) {
        sortedDedupedXList.push_back(*it);
      }
      
      // printf("After the traversal, sortedDedupedXList.size() == %lu.\n", sortedDedupedXList.size());

      // reset "discretizedXDict" && "theTree"
      memset(discretizedXDict, 0, sizeof(discretizedXDictSize));
      discretizedXDictSize = sortedDedupedXList.size(); 
      int theTreeIdxUpper = 3*(sortedDedupedXList.size()+1);
      for (int i = 0; i < theTreeIdxUpper; ++i) {
        theTree[i].idx = 0;
        theTree[i].fullCoverAccDiff = 0;
      }
      
      for (Rectangle &rec : recs) {
        auto itL = lower_bound(sortedDedupedXList.begin(), sortedDedupedXList.end(), rec.l);
        rec.xIndexL = (int)(itL - sortedDedupedXList.begin());
        discretizedXDict[rec.xIndexL] = rec.l;
                
        auto itR = lower_bound(sortedDedupedXList.begin(), sortedDedupedXList.end(), rec.r);
        rec.xIndexR = (int)(itR - sortedDedupedXList.begin());
        discretizedXDict[rec.xIndexR] = rec.r;
      }
      
      int rootIdx = 1;
      SegmentTreeNode &root = theTree[rootIdx];
      root.idx = rootIdx;
      root.leftIndexClosed = 0;
      root.rightIndexOpen = sortedDedupedXList.size()-1;
      root.leftXClosed = discretizedXDict[root.leftIndexClosed];
      root.rightXClosed = discretizedXDict[root.rightIndexOpen];
      
      vector<vector<int>> sortedYEdgeList;
      for (Rectangle &rec : recs) {
        sortedYEdgeList.push_back({rec.b, BOTTOM, rec.xIndexL, rec.xIndexR});
        sortedYEdgeList.push_back({rec.t, TOP, rec.xIndexL, rec.xIndexR});
      }
      sort(sortedYEdgeList.begin(), sortedYEdgeList.end(), less<vector<int>>());
      
      // printf("sortedDedupedXList.size() == %d, sortedYEdgeList.size() == %d\n", sortedDedupedXList.size(), sortedYEdgeList.size());
      
      int lastBenchY = INT_MAX;
      bool gotOverlay = false;
      int mergedArea = 0;
      for (auto &yEdge : sortedYEdgeList) {
        int currentY = yEdge[0];
        if (INT_MAX != lastBenchY && currentY != lastBenchY) {
          if (0 == root.fullCoverAccDiff) {
            // [TRICK] only check this condition when "lastBenchY" is updated 
            // printf("Not fully covered, <currentY:%d, lastBenchY: %d>, returning false\n", currentY, lastBenchY);
            return false;
          }
          mergedArea += (currentY-lastBenchY)*(maxR-minL);
        }
        RangeAdd(&root, yEdge[2], yEdge[3], {yEdge[0], yEdge[1]}, gotOverlay, 0);
        if (gotOverlay) {
          // printf("Got overlay, returning false");
          return false;
        }
        lastBenchY = currentY;
      }
      int hullArea = abs(maxT - minB)*abs(maxR - minL);
      
      // printf("mergedArea: %d, hullArea: %d, rawAreaSum: %d\n", mergedArea, hullArea, rawAreaSum);
      if (mergedArea != rawAreaSum) return false;
      if (mergedArea != hullArea) return false;
      return true;
    }
};

static const auto magic = []() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  return nullptr;
}();
