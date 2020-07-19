class SegmentTreeNode {
public:
  int andResult = INT_MAX;  
  int leftIndexClosed; // To be initialized upon construction.
  int rightIndexOpen; // To be initialized upon construction.
  
  SegmentTreeNode* lChild = NULL; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
  SegmentTreeNode* rChild = NULL; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
};

void RangeAnd(SegmentTreeNode* currentRoot, int newSegLeftIndexClosed, int newSegRightIndexOpen, int unifiedVal, int level) {
  // Reject invalid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s. 
  if (newSegLeftIndexClosed >= newSegRightIndexOpen) return;
  if (newSegLeftIndexClosed >= currentRoot->rightIndexOpen) return;
  if (newSegRightIndexOpen <= currentRoot->leftIndexClosed) return;
  
  // Snap valid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s.
  newSegLeftIndexClosed = max(newSegLeftIndexClosed, currentRoot->leftIndexClosed);
  newSegRightIndexOpen = min(newSegRightIndexOpen, currentRoot->rightIndexOpen);
  
  int indentSpaceCount = (level << 1);
  //printf("%*sRangeAdd, [newSegLeftIndexClosed:%d, newSegRightIndexOpen:%d), (y: %d, flag:%s)\n", indentSpaceCount, "", newSegLeftIndexClosed, newSegRightIndexOpen, unifiedYEdge.first, unifiedYEdge.second == TOP ? "TOP" : "BOTTOM");
  
  if (newSegLeftIndexClosed <= currentRoot->leftIndexClosed && newSegRightIndexOpen >=  currentRoot->rightIndexOpen) {
    // Proactively stops at "full cover" update.
    currentRoot->andResult &= unifiedVal;
    return;
  }
  if (NULL != currentRoot->lChild) {
    RangeAnd(currentRoot->lChild, newSegLeftIndexClosed, newSegRightIndexOpen, unifiedVal, level+1);
    currentRoot->andResult &= currentRoot->lChild->andResult;
  }
  if (NULL != currentRoot->rChild) {
    RangeAnd(currentRoot->rChild, newSegLeftIndexClosed, newSegRightIndexOpen, unifiedVal, level+1);
    currentRoot->andResult &= currentRoot->rChild->andResult;
  }
}

void RangeSum(SegmentTreeNode* root, int targetLeftIndexClosed, int targetRightIndexOpen, int* pResult) {
  // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
  if (targetLeftIndexClosed >= targetRightIndexOpen) return;
  if (targetLeftIndexClosed >= root->rightIndexOpen) return;
  if (targetRightIndexOpen <= root->leftIndexClosed) return;
  
  // Snap valid "[targetLeftIndexClosed, targetRightIndexOpen)"s.
  targetLeftIndexClosed = max(targetLeftIndexClosed, root->leftIndexClosed);
  targetRightIndexOpen = min(targetRightIndexOpen, root->rightIndexOpen);
  
  if (targetLeftIndexClosed <= root->leftIndexClosed && targetRightIndexOpen >= root->rightIndexOpen) {
    // Proactively stops at "full cover" update.
    (*pResult) = (*pResult) & (root->andResult);
    return;
  }

  if (NULL != root->lChild) {
    RangeSum(root->lChild, targetLeftIndexClosed, targetRightIndexOpen, pResult);
  }
  if (NULL != root->rChild) {
    RangeSum(root->rChild, targetLeftIndexClosed, targetRightIndexOpen, pResult);
  }
}

SegmentTreeNode* createBlankSegmentTree(int newSegLeftIndexClosed, int newSegRightIndexOpen) {
  if (newSegLeftIndexClosed >= newSegRightIndexOpen) {
    return NULL;
  } 

  SegmentTreeNode* root = new SegmentTreeNode(); 
  root->leftIndexClosed = newSegLeftIndexClosed;
  root->rightIndexOpen = newSegRightIndexOpen;
  
  //printf("createBlankSegmentTree, newSegLeftIndexClosed:%d, newSegRightIndexOpen:%d, root->leftXClosed:%d, root->rightXClosed:%d.\n", newSegLeftIndexClosed, newSegRightIndexOpen, root->leftXClosed, root->rightXClosed);

  int mid = ((newSegLeftIndexClosed + newSegRightIndexOpen) >> 1);
  if (mid > newSegLeftIndexClosed) {
    root->lChild = createBlankSegmentTree(newSegLeftIndexClosed, mid);
    if (mid < newSegRightIndexOpen) {
      root->rChild = createBlankSegmentTree(mid, newSegRightIndexOpen);
    } else {
      root->rChild = NULL;
    }
  } else {
    root->lChild = NULL;
    root->rChild = NULL;
  }
  return root;
}

class Solution {
public:
    int closestToTarget(vector<int>& arr, int target) {
      /*
      test case #1
      [5,89,79,44,45,79]
      965
      
      */
      int n = arr.size();
      SegmentTreeNode* root = createBlankSegmentTree(0, n);
      for (int i = 0; i < n; ++i) {
        RangeAnd(root, i, i+1, arr[i], 0);
      }
      
      int l = 0, inclusiveR = 0;
      int ans = INT_MAX;
      while (l <= inclusiveR && inclusiveR < n) {
        int tmp = INT_MAX;
        RangeSum(root, l, inclusiveR+1, &tmp);
        //printf("sum[l:%d, inclusiveR:%d] == %d\n", l, inclusiveR, tmp);
        int candidate = abs(tmp - target);
        if (candidate < ans) {
          ans = candidate;
        }
        if (tmp == target) {
          return 0;
        } else if (tmp > target) {
          ++inclusiveR;
        } else {
          ++l; // It's somewhat a hassle that "bitwise-and" has no reverse-operation, after we evict arr[l], imposing an inconvenience to use FenwickTree.
          if (inclusiveR < l) ++inclusiveR;
        }
      }
      return ans;
    }
};
