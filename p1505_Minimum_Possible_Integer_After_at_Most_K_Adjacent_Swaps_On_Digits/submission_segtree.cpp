#define min(x,y) (x <= y ? x : y) 
#define max(x,y) (x >= y ? x : y) 
#define INVALID -1
#define MAXL 30000

typedef struct SegTreeNode {
  int unifiedValue; 
  int infinityCount;
  SegTreeNode* minLeaf;
  int leftIndexClosed; // To be initialized upon construction.
  int rightIndexOpen; // To be initialized upon construction.
  struct SegTreeNode* lChild; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
  struct SegTreeNode* rChild; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
} SegTreeNode;

SegTreeNode* createBlankSegmentTree(int newSegLeftIndexClosed, int newSegRightIndexOpen) {
  if (newSegLeftIndexClosed >= newSegRightIndexOpen) {
    return NULL;
  } 

  //printf("createBlankSegmentTree, newSegLeftIndexClosed = %d, newSegRightIndexOpen = %d.\n", newSegLeftIndexClosed, newSegRightIndexOpen);

  SegTreeNode* root = (SegTreeNode*)malloc(sizeof(SegTreeNode)); 
  root->leftIndexClosed = newSegLeftIndexClosed;
  root->rightIndexOpen = newSegRightIndexOpen;
  root->unifiedValue = INVALID;
  root->infinityCount = 0;
  root->minLeaf = NULL;

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

void RangeWrite(SegTreeNode* currentRoot, int newSegLeftIndexClosed, int newSegRightIndexOpen, int unifiedValue, bool allowResiduePropagation, int level) {
  // Reject invalid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s. 
  if (newSegLeftIndexClosed >= newSegRightIndexOpen) return;
  if (newSegLeftIndexClosed >= currentRoot->rightIndexOpen) return;
  if (newSegRightIndexOpen <= currentRoot->leftIndexClosed) return;

  // Snap valid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s.
  newSegLeftIndexClosed = max(newSegLeftIndexClosed, currentRoot->leftIndexClosed);
  newSegRightIndexOpen = min(newSegRightIndexOpen, currentRoot->rightIndexOpen);

  int indentSpaceCount = (level << 1);
  //printf("%*sRangeWrite, assigning [%d, %d) <- %d into [%d, %d):%d, allowResiduePropagation = %d.\n", indentSpaceCount, "", newSegLeftIndexClosed, newSegRightIndexOpen, unifiedValue, currentRoot->leftIndexClosed, currentRoot->rightIndexOpen, currentRoot->unifiedValue, allowResiduePropagation);

  if (newSegLeftIndexClosed <= currentRoot->leftIndexClosed && newSegRightIndexOpen >= currentRoot->rightIndexOpen) {
    // Proactively stops at "full cover" update.
    currentRoot->unifiedValue = unifiedValue;
    currentRoot->minLeaf = currentRoot; // In this problem, the "minNode" is always a "minLeaf".
    if (unifiedValue == INT_MAX) {
      currentRoot->infinityCount = 1;
    }
    //printf("%*sRangeWrite, [%d, %d) updated to (unifiedValue:%d, infinityCount:%d).\n", indentSpaceCount, "", currentRoot->leftIndexClosed, currentRoot->rightIndexOpen, currentRoot->unifiedValue, currentRoot->infinityCount);
    return;
  }

  if (unifiedValue == currentRoot->unifiedValue) {
    // Prohibits a larger value to be written in.
    return;
  }

  int residueUnifiedValue = currentRoot->unifiedValue;
  currentRoot->unifiedValue = INVALID; // By now it's certain that "currentRoot->unifiedValue" is no longer unified.

  if (true == allowResiduePropagation && INVALID != residueUnifiedValue) {
    /*
     * Having "INVALID != residueUnifiedValue" implies that it's "more recent" than "any residueUnifiedValue from the subtree".
     */
    int leftResidueLeftClosed = currentRoot->leftIndexClosed;
    int leftResidueRightOpen = newSegLeftIndexClosed;

    int rightResidueLeftClosed = newSegRightIndexOpen;
    int rightResidueRightOpen = currentRoot->rightIndexOpen;
    if (NULL != currentRoot->lChild) {
      RangeWrite(currentRoot->lChild, leftResidueLeftClosed, leftResidueRightOpen, residueUnifiedValue, false, level+1);
      RangeWrite(currentRoot->lChild, rightResidueLeftClosed, rightResidueRightOpen, residueUnifiedValue, false, level+1);
    } 

    if (NULL != currentRoot->rChild) {
      RangeWrite(currentRoot->rChild, leftResidueLeftClosed, leftResidueRightOpen, residueUnifiedValue, false, level+1);
      RangeWrite(currentRoot->rChild, rightResidueLeftClosed, rightResidueRightOpen, residueUnifiedValue, false, level+1);
    }
  }

  currentRoot->infinityCount = 0;
  if (NULL != currentRoot->lChild) {
    RangeWrite(currentRoot->lChild, newSegLeftIndexClosed, newSegRightIndexOpen, unifiedValue, allowResiduePropagation, level+1);
    //printf("%*sRangeWrite, [%d, %d) returned from lChild.\n", indentSpaceCount, "", currentRoot->leftIndexClosed, currentRoot->rightIndexOpen);
    currentRoot->infinityCount += currentRoot->lChild->infinityCount;
  }
  if (NULL != currentRoot->rChild) {
    RangeWrite(currentRoot->rChild, newSegLeftIndexClosed, newSegRightIndexOpen, unifiedValue, allowResiduePropagation, level+1);
    //printf("%*sRangeWrite, [%d, %d) returned from rChild.\n", indentSpaceCount, "", currentRoot->leftIndexClosed, currentRoot->rightIndexOpen);
    currentRoot->infinityCount += currentRoot->rChild->infinityCount;
  }

  // It's important that we have a bias for the "lChild", such that in case of a tie value we pick the earlier position for "RangeMin".
  SegTreeNode* minLeafOfLeft = currentRoot->lChild->minLeaf;
  SegTreeNode* minLeafOfRight = currentRoot->rChild->minLeaf;

  if (NULL == minLeafOfLeft || (NULL != minLeafOfRight && minLeafOfRight->unifiedValue < minLeafOfLeft->unifiedValue)) {
    currentRoot->minLeaf = minLeafOfRight;
  } else {
    currentRoot->minLeaf = minLeafOfLeft;
  }
  
  //printf("%*sRangeWrite, [%d, %d) updated to (unifiedValue:%d, infinityCount:%d).\n", indentSpaceCount, "", currentRoot->leftIndexClosed, currentRoot->rightIndexOpen, currentRoot->unifiedValue, currentRoot->infinityCount);

  /*
     if (NULL != currentRoot->minLeaf) {
     printf("%*sRangeWrite, [%d, %d), minLeaf updated to (leftIndexClosed:%d, unifiedValue:%d).\n", indentSpaceCount, "", currentRoot->leftIndexClosed, currentRoot->rightIndexOpen, currentRoot->minLeaf->leftIndexClosed, currentRoot->minLeaf->unifiedValue);
     }
   */
}

void update(SegTreeNode* obj, int i, int val) {
  RangeWrite(obj, i, i+1, val, true, 0); 
}

void RangeInfinityCount(SegTreeNode* root, int targetLeftIndexClosed, int targetRightIndexOpen, int* pResult) {
  // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
  if (targetLeftIndexClosed >= targetRightIndexOpen) return;
  if (targetLeftIndexClosed >= root->rightIndexOpen) return;
  if (targetRightIndexOpen <= root->leftIndexClosed) return;

  // Snap valid "[targetLeftIndexClosed, targetRightIndexOpen)"s.
  targetLeftIndexClosed = max(targetLeftIndexClosed, root->leftIndexClosed);
  targetRightIndexOpen = min(targetRightIndexOpen, root->rightIndexOpen);

  if (targetLeftIndexClosed <= root->leftIndexClosed && targetRightIndexOpen >= root->rightIndexOpen) {
    // Only at "full cover" shall we respect the "minLeaf".
    *pResult = root->infinityCount; 
    return;
  }

  int candidate = 0;
  if (NULL != root->lChild) {
    // It's important that we visit the "lChild" first, such that in case of a tie value we pick the earlier position for "RangeMin".
    int lCandidate = 0;
    RangeInfinityCount(root->lChild, targetLeftIndexClosed, targetRightIndexOpen, &lCandidate);
    candidate += lCandidate;
  }
  if (NULL != root->rChild) {
    int rCandidate = 0;
    RangeInfinityCount(root->rChild, targetLeftIndexClosed, targetRightIndexOpen, &rCandidate);
    candidate += rCandidate;
  }
  *pResult = candidate;
}

void RangeMin(SegTreeNode* root, int targetLeftIndexClosed, int targetRightIndexOpen, SegTreeNode** pResult) {
  // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
  if (targetLeftIndexClosed >= targetRightIndexOpen) return;
  if (targetLeftIndexClosed >= root->rightIndexOpen) return;
  if (targetRightIndexOpen <= root->leftIndexClosed) return;

  // Snap valid "[targetLeftIndexClosed, targetRightIndexOpen)"s.
  targetLeftIndexClosed = max(targetLeftIndexClosed, root->leftIndexClosed);
  targetRightIndexOpen = min(targetRightIndexOpen, root->rightIndexOpen);

  if (targetLeftIndexClosed <= root->leftIndexClosed && targetRightIndexOpen >= root->rightIndexOpen) {
    // Only at "full cover" shall we respect the "minLeaf".
    *pResult = root->minLeaf; 
    return;
  }

  int candidateVal = INT_MAX;
  if (NULL != root->lChild) {
    // It's important that we visit the "lChild" first, such that in case of a tie value we pick the earlier position for "RangeMin".
    SegTreeNode* lResult = NULL;
    RangeMin(root->lChild, targetLeftIndexClosed, targetRightIndexOpen, &lResult);
    if (NULL != lResult && lResult->unifiedValue < candidateVal) {
      candidateVal = lResult->unifiedValue;
      *pResult = lResult; 
    }
  }
  if (NULL != root->rChild) {
    SegTreeNode* rResult = NULL;
    RangeMin(root->rChild, targetLeftIndexClosed, targetRightIndexOpen, &rResult);
    if (NULL != rResult && rResult->unifiedValue < candidateVal) {
      candidateVal = rResult->unifiedValue;
      *pResult = rResult; 
    }
  }
}

bool markedInfinity[MAXL];
class Solution {
  public:
    void findMin(string &str, int k, int stepsUsed, int offset, string &ans) {
      int n = str.length();
      memset(markedInfinity, false, sizeof(markedInfinity));
      
      SegTreeNode* root = createBlankSegmentTree(0, n);
      for (int i = 0; i < n; ++i) {
        int val = (str[i] - '0');
        update(root, i, val);
      }

      while (stepsUsed < k && offset < n) {
        int origUpper = (k-stepsUsed)+1;
        if (origUpper > n) origUpper = n;
        
        int upper = origUpper, infinityCountInRange = 0, newInfinityCountInRange = 0;
        RangeInfinityCount(root, 0, upper, &newInfinityCountInRange);
        while (newInfinityCountInRange > infinityCountInRange) {
          infinityCountInRange = newInfinityCountInRange;
          upper = origUpper + infinityCountInRange;
          if (upper > n) upper = n;
          RangeInfinityCount(root, 0, upper, &newInfinityCountInRange);
        }
        infinityCountInRange = newInfinityCountInRange;
        
        //printf("For ans:%s, k:%d, stepsUsed:%d, offset:%d, found infinityCountInRange:%d, thus, upper:%d for origUpper:%d\n", ans.c_str(), k, stepsUsed, offset, infinityCountInRange, upper, origUpper);

        SegTreeNode* minLeaf = NULL;
        RangeMin(root, 0, upper, &minLeaf);

        //printf("\tfound minLeaf of [0, upper:%d) to be (minLeaf->leftIndexClosed:%d, minLeaf->unifiedValue:%d) \n", upper, minLeaf->leftIndexClosed, minLeaf->unifiedValue);
        
        char minm = '0' + minLeaf->unifiedValue;
        int minmIdx = minLeaf->leftIndexClosed;

        ans.push_back(minm);
        update(root, minmIdx, INT_MAX); // To disable that node from being searched
        markedInfinity[minmIdx] = true;

        // no backtracking needed, go greedy
        int infinityCountInRangeToWaive = 0;
        RangeInfinityCount(root, 0, minmIdx, &infinityCountInRangeToWaive);
        //printf("\tfound infinityCountInRangeToWaive of [0, minmIdx:%d) to be %d \n", minmIdx, infinityCountInRangeToWaive);

        int additionalUsedSteps = (minmIdx - infinityCountInRangeToWaive);
        stepsUsed += additionalUsedSteps;
        offset += 1;
      }
       
      for (int i = 0; i < n; ++i) {
        if (markedInfinity[i]) continue;
        ans.push_back(str[i]);
      }
    }
  
    string minInteger(string num, int k) {
      /*
      test case #1
      "9000900"
      3
      
      test case #2
      "9438957234785635408"
      1000000000
      
      test case #3
      "294984148179"
      11
      */
      string ans = "";
      findMin(num, k, 0, 0, ans);
      return ans;
    }
};

