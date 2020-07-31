#define INVALID -1
#define INVALID_CH '\0'

#define NODE_TYPE_ROOT 0
#define NODE_TYPE_INTERNAL 1
#define NODE_TYPE_LEAF 2

#define TERMINATOR_CH '#'
#define CONCAT_CH '_'

class SuffixTreeNode {
public:
  int weight; // special field for this problem
  
  int nodeType;   
  int start; // Used by both "NODE_TYPE_INTERNAL" and "NODE_TYPE_LEAF"
  int end; // Used by "NODE_TYPE_INTERNAL" only 
  int* sharedLeafEnd; // Used by "NODE_TYPE_LEAF" only

  SuffixTreeNode *parent;

  unordered_map<char, SuffixTreeNode*> children;   
  
  SuffixTreeNode *suffixLink;

  SuffixTreeNode() {
    nodeType = NODE_TYPE_ROOT;
    start = INVALID;
    end = INVALID; 
    parent = NULL;
    sharedLeafEnd = NULL;
    suffixLink = NULL;
  }

  SuffixTreeNode(int aStart, int aEnd) : SuffixTreeNode() {
    nodeType = NODE_TYPE_INTERNAL;
    start = aStart;
    end = aEnd; 
  }

  SuffixTreeNode(int aStart, int* aSharedLeafEnd) : SuffixTreeNode(aStart, INVALID) {
    nodeType = NODE_TYPE_LEAF;
    sharedLeafEnd = aSharedLeafEnd;
  }

  int length() {
    switch (nodeType) {
      case NODE_TYPE_ROOT:
        return 0;
      case NODE_TYPE_INTERNAL:
        return end-start;
      case NODE_TYPE_LEAF:
        return (*sharedLeafEnd) - start;
    }
    return 0;
  }
  
  char charAt(int offset, string& article) {
    return article[start + offset];
  }
  
  void print(string& article, int level) {
    int indentSpaceCount = (level << 1);
    int innerIndentSpaceCount = ((level+1) << 1);
    if (INVALID != start) {
      char fromCh = article[start];
      printf("%*s%c: %s, w == %d\n", innerIndentSpaceCount, "", fromCh, article.substr(start, this->length()).c_str(), weight);
    } else {
      printf("%*s/\n", innerIndentSpaceCount, "");
    }
    for (auto &it : this->children) {
      it.second->print(article, level+1);
    } 
  }
};

class ActivePoint {
public:
  SuffixTreeNode* head = NULL; // Can only be "NODE_TYPE_ROOT" or "NODE_TYPE_INTERNAL"
  int downChPosInArticle = INVALID; // Deliberately typed "int" instead of "char" to contain position information of the "downCh" 
  int offset = INVALID; // Deliberately using "offset" instead of "length" here

  ActivePoint() {
  }

  ActivePoint(SuffixTreeNode* aHead):ActivePoint() {
    head = aHead;
  }

  SuffixTreeNode* tail(string &article) {
    if (INVALID == downChPosInArticle) {
      return NULL; 
    }
    char downCh = article[downChPosInArticle]; 
    auto it = head->children.find(downCh);
    if (it == head->children.end()) {
      return NULL;
    }
    return it->second;
  }

  void print(string& article, int level) {
    int indentSpaceCount = (level << 1);
    int innerIndentSpaceCount = ((level+1) << 1);
    printf("%*s{\n", indentSpaceCount, "");
    if (INVALID == head->start) {
      printf("%*shead at root\n", innerIndentSpaceCount, "");
    } else {
      printf("%*shead:{start: %d, length:%d} == \"%s\"\n", innerIndentSpaceCount, "", head->start, head->length(), article.substr(head->start, head->length()).c_str());
    }
    printf("%*sdownChPosInArticle:%d == \'%c\'\n", innerIndentSpaceCount, "", downChPosInArticle, article[downChPosInArticle]);
    printf("%*soffset: %d\n", innerIndentSpaceCount, "", offset);
    printf("%*s}\n", indentSpaceCount, "");
  }
};

void DoPhase(SuffixTreeNode* root, int i, string &article, int* sharedLeafEnd, vector<SuffixTreeNode*>& leafs, SuffixTreeNode **pLastNewInternalNode, ActivePoint* pAp, int posAfterLastTerminatorCh, int w) {
  (*sharedLeafEnd) = (i+1);
  if (article[i] == TERMINATOR_CH) {
    /*
    There could be multiple "TERMINATOR_CH"s in "article". 
    */
    return;
  }

  root->weight = w; // for this problem only
  
  int k = leafs.size();
  //printf("\nDoPhase, i:%d, *sharedLeafEnd:%d, k:%d\n", i, *sharedLeafEnd, k);
  int level = 0;
  int indentSpaceCount = (level << 1);

  for (int j = k+posAfterLastTerminatorCh; j <= i; ++j) { 
    /*
    printf("%*s@j:%d, activePoint is\n", indentSpaceCount, "", j);
    pAp->print(article, level+1);
    */
    SuffixTreeNode* currentTail = pAp->tail(article);
    /*
    For each "j", if "NULL != currentTail" exists then 
    - it's guaranteed that "article[currentTail->start + pAp->offset] == article[i-1]", but 
    - it's NOT GUARANTEED that "currentTail->start + pAp->offset == i-1", i.e. "currentTail" is NOT NECESSARILY of NODE_TYPE_LEAF. 
    */
    /*
    if (NULL != currentTail) {
      printf("%*scurrentTail is \n", indentSpaceCount, "");
      currentTail->print(article, level+1);
    }
    */
    int toMatchLength = i-(pAp->downChPosInArticle)+1;
    while (NULL != currentTail && toMatchLength > currentTail->length()) {
      //printf("%*swalking down...\n", indentSpaceCount, "");
      
      pAp->head->weight = w; // for this problem only
      //currentTail->weight = w; // for this problem only

      // walk the "ActivePoint" down
      pAp->head = currentTail;
      pAp->downChPosInArticle += currentTail->length();
      pAp->offset -= currentTail->length();

      toMatchLength = i-(pAp->downChPosInArticle)+1;
      currentTail = pAp->tail(article);
    }
    // now that either "NULL == currentTail" or "toMatchLength <= currentTail->length()" 
    if (NULL == currentTail) {
      //printf("%*scurrentTail is NULL after the walk-down\n", indentSpaceCount, "");
      char newDownCh = article[pAp->downChPosInArticle];
      SuffixTreeNode *newLeaf = new SuffixTreeNode(pAp->downChPosInArticle, sharedLeafEnd);
      pAp->head->children[newDownCh] = newLeaf;  
      newLeaf->parent = pAp->head;
      leafs.push_back(newLeaf);
      /*
      printf("%*screated newLeaf only \n", indentSpaceCount, "");
      newLeaf->print(article, level+1);
      */
      newLeaf->weight = w; // for this problem only

      ++pAp->downChPosInArticle;
      if (0 <= pAp->offset) {
        --pAp->offset;
      }
    } else {
      /*
      printf("%*sactivePoint is updated by the walk-down to \n", indentSpaceCount, "");
      pAp->print(article, level+1);
      printf("%*scurrentTail is updated by the walk-down to\n", indentSpaceCount, "");
      currentTail->print(article, level+1);
      */
      // now that "toMatchLength <= currentTail->length()" 
      if (article[i] != article[currentTail->start+pAp->offset+1]) {
        // spawn a new "InternalNode"
        int newInternalNodeEnd = currentTail->start+pAp->offset+1; 
        SuffixTreeNode *newInternalNode = new SuffixTreeNode(currentTail->start, newInternalNodeEnd);

        // connect "newInternalNode" with "pAp->head" 
        pAp->head->children[article[currentTail->start]] = newInternalNode;
        newInternalNode->parent = pAp->head;

        // update "currentTail->start" and reconnect "newInternalNode" with "currentTail"
        currentTail->start = newInternalNodeEnd;
        newInternalNode->children[article[newInternalNodeEnd]] = currentTail;
        currentTail->parent = newInternalNode;

        // builds "lastNewInternalNode->suffixLink" if applicable
        if (NULL != *pLastNewInternalNode && NULL == (*pLastNewInternalNode)->suffixLink) {
          (*pLastNewInternalNode)->suffixLink = newInternalNode;
        }
        (*pLastNewInternalNode) = newInternalNode;
        if (1 == newInternalNode->length() && newInternalNode->parent == root) {
          // A special case for "->suffixLink" assignment.
          newInternalNode->suffixLink = root;
          (*pLastNewInternalNode) = NULL;
        }

        // create and connect "newLeaf"
        SuffixTreeNode *newLeaf = new SuffixTreeNode(i, sharedLeafEnd); // note that "newLeaf->start" should be "i" instead of "newInternalNodeEnd"
        newInternalNode->children[article[i]] = newLeaf;  
        newLeaf->parent = newInternalNode;
        leafs.push_back(newLeaf);

        /*
        printf("%*screated newLeaf \n", indentSpaceCount, "");
        newLeaf->print(article, level+1);
        */
        newLeaf->weight = w; // for this problem only

        /*
        printf("%*screated newInternalNode \n", indentSpaceCount, "");
        newInternalNode->print(article, level+1); 
        */
        newInternalNode->weight = w; // for this problem only

        if (NODE_TYPE_ROOT != pAp->head && NULL != pAp->head->suffixLink) {
          pAp->head = pAp->head->suffixLink;
          /* 
          Don't change either 
          - "pAp->downChPosInArticle", or 
          - "pAp->offset" (because the equivalent is done at the head) 
          when "pAp->head" follows "pAp->head->suffixLink"!
          */
        } else {
          ++pAp->downChPosInArticle;
          if (0 <= pAp->offset) {
            --pAp->offset;
          }
        }
      } else {
        ++pAp->offset; // the initial value of "offset" is "INVALID == -1", hence no need to check validity and set it to 0 in an extra block
        /*
        We're going immediately to the next phase "i+1".
        */
        break;
      }
    }
  } 
}

class WordFilter {
private:
    SuffixTreeNode* root = NULL;
    string article = "";
    int theSharedLeafEnd = 0;
public:
    WordFilter(vector<string>& words) {
      /*
      To be deallocated by the caller.
      */
      root = new SuffixTreeNode(); 
      
      vector<SuffixTreeNode*> leafs;
      SuffixTreeNode* lastNewInternalNode = NULL;

      ActivePoint ap(root);
      ap.downChPosInArticle = 0; // initially points to article[downChPosInArticle:0], but no valid offset      
      for (int w = 0; w < words.size(); ++w) {
        string word = words[w];
        article.append(word + CONCAT_CH + word + TERMINATOR_CH);
      }
      
      int w = 0;
      int posAfterLastTerminatorCh = 0;
      for (int i = 0; i < article.length(); ++i) {
        DoPhase(root, i, article, &theSharedLeafEnd, leafs, &lastNewInternalNode, &ap, posAfterLastTerminatorCh, w);
        if (article[i] == TERMINATOR_CH) {
          ++w;
          leafs.clear();
          posAfterLastTerminatorCh = (i+1);
          ap.head = root;
          ap.downChPosInArticle = posAfterLastTerminatorCh;
          ap.offset = INVALID;
        }
      }
      
      printf("\nThe SuffixTree for \"%s\" is\n", article.c_str());
      root->print(article, 1);
    }
    
    int f(string prefix, string suffix) {
      string toMatchPattern = suffix + CONCAT_CH + prefix;
      //printf("\nThe toMatchPattern is \"%s\"\n", toMatchPattern.c_str());

      int idxInPattern = 0;
      char downCh = toMatchPattern[idxInPattern];
      
      SuffixTreeNode *cur = root;
      auto it = cur->children.find(downCh);
      while (it != cur->children.end() && idxInPattern < toMatchPattern.length()) {
        SuffixTreeNode *currentTail = it->second;
        int currentTailLength = currentTail->length();

        int offsetInTail = 0;
        while (idxInPattern < toMatchPattern.length() && offsetInTail < currentTailLength && toMatchPattern[idxInPattern] == article[currentTail->start+offsetInTail]) {
          ++offsetInTail;
          ++idxInPattern;
        }
        if (idxInPattern == toMatchPattern.length()) {
          return currentTail->weight;
        }

        cur = currentTail;
        downCh = toMatchPattern[idxInPattern];
        it = cur->children.find(downCh);
      }
      return -1;
    }
};

/**
 * Your WordFilter object will be instantiated and called as such:
 * WordFilter* obj = new WordFilter(words);
 * int param_1 = obj->f(prefix,suffix);
 */
