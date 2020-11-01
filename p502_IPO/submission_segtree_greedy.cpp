int const INIT_INDENT = 2;
int const INVALID = -1;
typedef pair<int, int> PII;

class Ele {
public:
  int profit;
  int cost;
  int costOrder;
};

class EleCompare {
public:
  bool operator() (Ele const& lhs, Ele const &rhs) {
    if (lhs.cost != rhs.cost) return lhs.cost < rhs.cost;
    return (lhs.profit-lhs.cost) < (rhs.profit-rhs.cost);  
  }

  bool operator() (int lhs, Ele const& rhs) {
    return lhs < rhs.cost;
  }
};

class SegmentTreeNode {
public:
  int localMax = INT_MIN; 
  int localMaxIndex = INVALID; 
  
  int leftIndexClosed;
  int rightIndexOpen; 
  
  SegmentTreeNode* lChild = NULL; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
  SegmentTreeNode* rChild = NULL; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
  
  SegmentTreeNode(int newSegLeftIndexClosed, int newSegRightIndexOpen) {
    this->leftIndexClosed = newSegLeftIndexClosed;
    this->rightIndexOpen = newSegRightIndexOpen;
  }
  
  void RangeAdd(int newSegLeftIndexClosed, int newSegRightIndexOpen, int val, int level) {
    auto root = this;
    // Reject invalid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s. 
    if (newSegLeftIndexClosed >= newSegRightIndexOpen) return;
    if (newSegLeftIndexClosed >= root->rightIndexOpen) return;
    if (newSegRightIndexOpen <= root->leftIndexClosed) return;

    // Snap valid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s.
    newSegLeftIndexClosed = max(newSegLeftIndexClosed, root->leftIndexClosed);
    newSegRightIndexOpen = min(newSegRightIndexOpen, root->rightIndexOpen);

    int indentSpaceCount = (level << 1);
    // printf("%*sRangeAdd, [newSegLeftIndexClosed:%d, newSegRightIndexOpen:%d), (val: %d)\n", indentSpaceCount, "", newSegLeftIndexClosed, newSegRightIndexOpen, val);

    if (newSegLeftIndexClosed <= root->leftIndexClosed && newSegRightIndexOpen >= root->rightIndexOpen) {
      // [WARNING] In this problem we ALWAYS invoke "RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen == newSegLeftIndexClosed+1)"!
      // Proactively stops at "full cover" update.
      if (INT_MIN == localMax) {
        localMax = val;
        localMaxIndex = root->leftIndexClosed;
      } else {
        localMax += val;
      }
      // printf("%*sRangeAdd-END#1, updated {localMax:%d, localMaxIndex:%d}\n", indentSpaceCount, "", localMax, localMaxIndex);
      return;
    }
    
    int mid = ((root->leftIndexClosed + root->rightIndexOpen) >> 1);
    if (newSegLeftIndexClosed < mid) {
      if (NULL == root->lChild) {
        root->lChild = new SegmentTreeNode(root->leftIndexClosed, mid);
      }
      root->lChild->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, val, level+1);
    }
    
    if (newSegRightIndexOpen > mid) {
      if (NULL == root->rChild) {
        root->rChild = new SegmentTreeNode(mid, root->rightIndexOpen);
      }
      root->rChild->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, val, level+1);
    }
    
    int localMaxOfLeftAndRight = INT_MIN;
    int localMaxOfLeftAndRightIndex = INVALID;
    if (root->lChild && root->lChild->localMax > localMaxOfLeftAndRight) {
      localMaxOfLeftAndRight = root->lChild->localMax;
      localMaxOfLeftAndRightIndex = root->lChild->localMaxIndex;
    }
    
    if (root->rChild && root->rChild->localMax > localMaxOfLeftAndRight) {
      localMaxOfLeftAndRight = root->rChild->localMax;
      localMaxOfLeftAndRightIndex = root->rChild->localMaxIndex;
    }
    
    if (INT_MIN != localMaxOfLeftAndRight) {
      localMax = localMaxOfLeftAndRight;
      localMaxIndex = localMaxOfLeftAndRightIndex;
      // printf("%*sRangeAdd-END#2, updated {localMax:%d, localMaxIndex:%d}\n", indentSpaceCount, "", localMax, localMaxIndex);
    }
  }

  void RangeMax(int targetLeftIndexClosed, int targetRightIndexOpen, PII& pResult) {
    auto root = this;
    // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
    if (targetLeftIndexClosed >= targetRightIndexOpen) return;
    if (targetLeftIndexClosed >= root->rightIndexOpen) return;
    if (targetRightIndexOpen <= root->leftIndexClosed) return;

    // Snap valid "[targetLeftIndexClosed, targetRightIndexOpen)"s.
    targetLeftIndexClosed = max(targetLeftIndexClosed, root->leftIndexClosed);
    targetRightIndexOpen = min(targetRightIndexOpen, root->rightIndexOpen);

    if (targetLeftIndexClosed <= root->leftIndexClosed && targetRightIndexOpen >= root->rightIndexOpen) {
      if (pResult.first < root->localMax) {
        pResult.first = root->localMax;
        pResult.second = root->localMaxIndex;
      }
      return;
    }

    if (NULL != root->lChild) {
      root->lChild->RangeMax(targetLeftIndexClosed, targetRightIndexOpen, pResult);
    }
    if (NULL != root->rChild) {
      root->rChild->RangeMax(targetLeftIndexClosed, targetRightIndexOpen, pResult);
    }
  }
};

/*
test case#1
2
0
[1,2,3]
[0,1,1]

test case#2
1
0
[1,2,3]
[0,1,1]

test case#3
10
1000
[1]
[2]

test case#4
10
1000
[1, 100]
[2, 20]
*/
class Solution {
public:
  int findMaximizedCapital(int k, int W, vector<int>& Profits, vector<int>& Capital) {
    int n = Profits.size();
    vector<Ele> eles;
    for (int i = 0; i < n; ++i) {
      Ele single; single.profit = Profits[i]; single.cost = Capital[i];
      eles.push_back(single);
    }

    sort(eles.begin(), eles.end(), EleCompare());
    
    // printf("Sorted eles:\n");
    SegmentTreeNode* root = new SegmentTreeNode(0, n);
    for (int i = 0; i < n; ++i) {
      Ele &single = eles[i];
      single.costOrder = i;
      // printf("eles[i:%d] {profit: %d, cost: %d, costOrder: %d}\n", i, single.profit, single.cost, i);
      root->RangeAdd(i, i+1, single.profit, INIT_INDENT);
    }
    
    int remainingProjectCount = n;
    while (k > 0 && remainingProjectCount > 0) {
      auto upperIt = upper_bound(eles.begin(), eles.end(), W, EleCompare());
      int costOrderUpperOffsetOpen = (upperIt - eles.begin());
      if (costOrderUpperOffsetOpen < 1) break; // no project can be chosen within cost "W"

      PII candidate{INT_MIN, INVALID};
      root->RangeMax(0, costOrderUpperOffsetOpen, candidate);
      if (INT_MIN == candidate.first) break; // no project can be chosen within cost "W"

      // printf("With W:%d & costOrderUpperOffsetOpen:%d, we chose candidate project {profit:%d, costOrder: %d}\n", W, costOrderUpperOffsetOpen, candidate.first,candidate.second);
      
      W += candidate.first;

      int idx = candidate.second;
      
      // invalidate this used project
      root->RangeAdd(idx, idx+1, -candidate.first, INIT_INDENT); // [WARNING] This is a point where "FenwickTree" COULDN'T be applied, because a FenwickTree supporting "getMax(int endClosed)" operation DOESN'T ALLOW "update(index, val)" to decrement the underlying array, i.e. if we invoke "update(sameIndex, val1)", "update(sameIndex, val2)" and "update(sameIndex, val3)" in order we must guarantee that "val1 <= val2 <= val3", because "max(...)" is an irreversible operator. 
      --k;
      --remainingProjectCount;
    }

    return W;  
  }
};

