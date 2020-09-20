int minL, maxR, minB, maxT;

class Rectangle {
public:
  int l, r, t, b;
  int xIndexL, xIndexR;
  int yIndexB, yIndexT;
  
  Rectangle(int aL, int aR, int aB, int aT) {
    l = aL;
    r = aR;
    b = aB;
    t = aT;
  }
};

class QuadtreeNode {
public:
  bool fullyCovered = false;
  int lXClosed, rXClosed, bYClosed, tYClosed;
  QuadtreeNode *nwChild = NULL, *neChild = NULL, *swChild = NULL, *seChild = NULL; // NorthWest, NorthEast, SouthWest, SouthEast
  
  QuadtreeNode(int newLXClosed, int newRXClosed, int newBYClosed, int newTYClosed) {
    lXClosed = newLXClosed;
    rXClosed = newRXClosed;
    bYClosed = newBYClosed;
    tYClosed = newTYClosed;
  }
  
  void RangeAdd(int newLXClosed, int newRXClosed, int newBYClosed, int newTYClosed, bool &gotOverlay, int level) {
    auto root = this;
    // Reject invalid intervals
    if (newLXClosed >= newRXClosed) return;
    if (newLXClosed > root->rXClosed) return;
    if (newRXClosed < root->lXClosed) return;
    
    if (newBYClosed >= newTYClosed) return;
    if (newBYClosed > root->tYClosed) return;
    if (newTYClosed < root->bYClosed) return;

    // Snap valid intervals.
    newLXClosed = max(newLXClosed, root->lXClosed);
    newRXClosed = min(newRXClosed, root->rXClosed);
    newBYClosed = max(newBYClosed, root->bYClosed);
    newTYClosed = min(newTYClosed, root->tYClosed);

    // int indentSpaceCount = (level << 1);
    if (
      newLXClosed <= root->lXClosed && newRXClosed >= root->rXClosed
       &&
       newBYClosed <= root->bYClosed && newTYClosed >= root->tYClosed
    ) {
      // Proactively stops at "full cover" update.
      if (true == root->fullyCovered) {
        gotOverlay = true;
      } else {
        root->fullyCovered = true;        
      }
      return;
    }
    
    if (root->fullyCovered) {
      // this is a critical check
      gotOverlay = true;
      return;
    }
    
    int midX = ((root->lXClosed + root->rXClosed) >> 1);
    int midY = ((root->bYClosed + root->tYClosed) >> 1);
    
    bool shouldHaveNw = false, hasNw = false, isNwFullyCovered = false;
    if (root->lXClosed < midX && root->tYClosed > midY) {
      shouldHaveNw = true;
    }
    if (shouldHaveNw && newLXClosed < midX && newTYClosed > midY) {
      if (NULL == root->nwChild) {
        root->nwChild = new QuadtreeNode(root->lXClosed, midX, midY, root->tYClosed);
      }
      hasNw = true;
      root->nwChild->RangeAdd(newLXClosed, newRXClosed, newBYClosed, newTYClosed, gotOverlay, level+1);
      isNwFullyCovered = root->nwChild->fullyCovered;
    }
    
    bool shouldHaveNe = false, hasNe = false, isNeFullyCovered = false;
    if (root->rXClosed > midX && root->tYClosed > midY) {
      shouldHaveNe = true;
    }
    if (shouldHaveNe && newRXClosed > midX && newTYClosed > midY) {
      if (NULL == root->neChild) {
        root->neChild = new QuadtreeNode(midX, root->rXClosed, midY, root->tYClosed);
      }
      hasNe = true;
      root->neChild->RangeAdd(newLXClosed, newRXClosed, newBYClosed, newTYClosed, gotOverlay, level+1);
      isNeFullyCovered = root->neChild->fullyCovered;
    }
    
    bool shouldHaveSw = false, hasSw = false, isSwFullyCovered = false;
    if (root->lXClosed < midX && root->bYClosed < midY) {
      shouldHaveSw = true;
    }
    if (shouldHaveSw && newLXClosed < midX && newBYClosed < midY) {
      if (NULL == root->swChild) {
        root->swChild = new QuadtreeNode(root->lXClosed, midX, root->bYClosed, midY);
      }
      hasSw = true;
      root->swChild->RangeAdd(newLXClosed, newRXClosed, newBYClosed, newTYClosed, gotOverlay, level+1);
      isSwFullyCovered = root->swChild->fullyCovered;
    }
    
    bool shouldHaveSe = false, hasSe = false, isSeFullyCovered = false;
    if (root->rXClosed > midX && root->bYClosed < midY) {
      shouldHaveSe = true;
    }
    if (shouldHaveSe && newRXClosed > midX && newBYClosed < midY) {
      if (NULL == root->seChild) {
        root->seChild = new QuadtreeNode(midX, root->rXClosed, root->bYClosed, midY);
      }
      hasSe = true;
      root->seChild->RangeAdd(newLXClosed, newRXClosed, newBYClosed, newTYClosed, gotOverlay, level+1);
      isSeFullyCovered = root->seChild->fullyCovered;
    }
    
    if (
      (!shouldHaveNw || (shouldHaveNw && hasNw && isNwFullyCovered))
      &&
      (!shouldHaveNe || (shouldHaveNe && hasNe && isNeFullyCovered))
      &&
      (!shouldHaveSw || (shouldHaveSw && hasSw && isSwFullyCovered))
      &&
      (!shouldHaveSe || (shouldHaveSe && hasSe && isSeFullyCovered))
    ) {
      root->fullyCovered = true;
    }
  }
};

class Solution {
public:
    bool isRectangleCover(vector<vector<int>>& rectangles) {
      minL = INT_MAX, maxR = INT_MIN, minB = INT_MAX, maxT = INT_MIN;
      vector<Rectangle> recs;
      for (auto &rectangle : rectangles) {
        Rectangle rec(rectangle[0], rectangle[2], rectangle[1], rectangle[3]);
        recs.push_back(rec);
        minL = min(minL, rec.l);
        maxR = max(maxR, rec.r);
        minB = min(minB, rec.b);
        maxT = max(maxT, rec.t);
      }
      int hullArea = (maxR-minL)*(maxT-minB);
      
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
      
      for (auto &rec : recs) {
        auto itL = lower_bound(sortedDedupedXList.begin(), sortedDedupedXList.end(), rec.l);
        rec.xIndexL = (int)(itL - sortedDedupedXList.begin());
                
        auto itR = lower_bound(sortedDedupedXList.begin(), sortedDedupedXList.end(), rec.r);
        rec.xIndexR = (int)(itR - sortedDedupedXList.begin());
      }
      
      // Discretize and index each of "rec.b & rec.t".
      set<int> sortedDedupedYSet;
      for (auto &rec : recs) {
        sortedDedupedYSet.insert(rec.b);
        sortedDedupedYSet.insert(rec.t);
      }
      vector<int> sortedDedupedYList;
      for (auto it = sortedDedupedYSet.begin(); it != sortedDedupedYSet.end(); ++it) {
        sortedDedupedYList.push_back(*it);
      }
      
      for (auto &rec : recs) {
        auto itB = lower_bound(sortedDedupedYList.begin(), sortedDedupedYList.end(), rec.b);
        rec.yIndexB = (int)(itB - sortedDedupedYList.begin());
                
        auto itT = lower_bound(sortedDedupedYList.begin(), sortedDedupedYList.end(), rec.t);
        rec.yIndexT = (int)(itT - sortedDedupedYList.begin());
      }
      
      // Construct the Quadtree with discretized x and y
      // Rephrase "minL, maxR, minB, maxT" to use discretized coordinates
      minL = 0;
      maxR = sortedDedupedXList.size()-1;
      minB = 0;
      maxT = sortedDedupedYList.size()-1;
      
      //printf("After discretized, minL:%d, maxR:%d, minB:%d, maxT:%d\n", minL, maxR, minB, maxT);
      
      int mergedArea = 0;
      bool gotOverlay = false;
      QuadtreeNode* root = new QuadtreeNode(minL, maxR, minB, maxT);
      for (auto &rec : recs) {
        // printf("Adding [l:%d, r:%d]x[b:%d, t:%d]\n", rec.l, rec.r, rec.b, rec.t);
        root->RangeAdd(rec.xIndexL, rec.xIndexR, rec.yIndexB, rec.yIndexT, gotOverlay, 0);
        if (gotOverlay) {
          // printf("Got overlay when adding [l:%d, r:%d, b:%d, t:%d]\n", rec.l, rec.r, rec.b, rec.t);
          return false;
        }
        mergedArea += (rec.r-rec.l)*(rec.t-rec.b);
      }
      return (mergedArea == hullArea);
    }
};
