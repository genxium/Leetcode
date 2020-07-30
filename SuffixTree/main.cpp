#include <stdio.h>
#include <unordered_map>
#include <string>
#include <vector>
#define INVALID -1
#define INVALID_CH '\0'

#define NODE_TYPE_ROOT 0
#define NODE_TYPE_INTERNAL 1
#define NODE_TYPE_LEAF 2

#define TERMINATOR_CH '#'
using namespace std;

class SuffixTreeNode {
public:
  /*
  [start, end) interval specifies the edge, by which the 
   node is connected to its parent node. Each edge will 
   connect two nodes,  one parent and one child, and 
   [start, end] interval of a given edge  will be stored 
   in the child node. 
    
   For example A-[5,8)->B has this indices [5, 8) stored in node B. 

  Similar argument is true for [start, *sharedLeafEnd).
  */
  int nodeType;   
  int start; // Used by both "NODE_TYPE_INTERNAL" and "NODE_TYPE_LEAF"
  int end; // Used by "NODE_TYPE_INTERNAL" only 
  int* sharedLeafEnd; // Used by "NODE_TYPE_LEAF" only

  SuffixTreeNode *parent;

  unordered_map<char, SuffixTreeNode*> children;   
  
  /*
     - The "RootNode" has a null "SuffixLink".
     - Any "LeafNode" has a null "SuffixLink"
     - Only an "InternalNode" has a non-null "SuffixLink"

     When an existing "InternalNode(say `P1`)" or "LeafNode(say `P2`)" is being splited, i.e. for creating a new "InternalNode(say `Q`)" between [start, end) or [start, *sharedLeafEnd), we 
     - keep "P1->end (or P2->end)" and "P1->suffixLink (or P2->suffixLink)" unchanged, then 
     - set "Q->start = P1->start (or P2->start)".
  */
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
  }
  
  char charAt(int offset, string& article) {
    return article[start + offset];
  }
  
  void print(string& article, int level) {
    int indentSpaceCount = (level << 1);
    if (INVALID != start) {
      char fromCh = article[start];
      printf("%*s%c: %s\n", indentSpaceCount, "", fromCh, article.substr(start, this->length()).c_str());
    } else {
      printf("%*s/\n", indentSpaceCount, "");
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
    printf("%*shead:{start: %d, length:%d}, downChPosInArticle:%d, offset: %d\n", indentSpaceCount, "", head->start, head->length(), downChPosInArticle, offset);
  }
};

void DoPhase(SuffixTreeNode* root, int i, string &article, int* sharedLeafEnd, vector<SuffixTreeNode*>& leafs, SuffixTreeNode **pLastNewInternalNode, ActivePoint* pAp) {
  /*
  This method is inevitably called "article.length()" times, which means that the overall time-complexity of "BuildSuffixTree" can only be “O(article.length())" if the time-complexity of "DoPhase(...)" is "O(1)" or "amortized O(1)", but how?

  In this method we're investigating article [0, ..., i], and will add ALL OF
  - article[0, ..., i],
  ...
  - article[j, ..., i],
  ...
  - article[i, ..., i]
  to the current "root".
  
  The time-complexity of such process is normally "sum(i-j+1 for each i) == O((article.length())^2)" and couldn't be reduced to "O(1)" without some cached information from the previous phase "i-1".

  \\\TRICK STARTS/// 
  Consider adding each "article[j, ..., i]" to “root”, the cached information we might need is “each `SuffixTreeNode* leafs[j]` that corresponds to article[j, ..., i-1]", where the whole "leafs[]" increases monotonically. 
    
  Moreover, assume that "k = `leafs.size() just after phase (i-1)`", i.e. there're "k" leafs after finishing "phase (i-1)", we can assert that ALL OF  
  - article[0, ..., i],
  ...
  - article[k-1, ..., i],
  can be added to "root" within just "O(1)" time-complexity, by making all "*(leafs[0 <= j < k]->sharedLeafEnd)" equal "i".  

  Is it possible that there exists a "u" such that "0 <= u < k" and "article[u, ..., i]" DOESN'T end at a leaf, yet another "v" such that "v >= k" and "article[v, ..., i]" ends at a leaf instead? Well no, but the proof is neglected here. 
  ///TRICK ENDS\\\

  Now that we're to handle the (i+1-k) remaining suffixes in the current phase "i"
  - article[k, ..., i],
  ...
  - article[i, ..., i],
  and still have to add all of them to "root" within "O(1)".

  Consider just article[k, ..., i]. We DON'T have a corresponding "leafs[k]" here, but DO have "leafs[k-1]" where 
  - either "leafs[k-1]->parent->nodeType == NODE_TYPE_INTERNAL" && "leafs[k-1]->parent->suffixLink" exists, or    
  - "leafs[k-1]->parent->nodeType == NODE_TYPE_ROOT"
  , because if "leafs[k-1]->parent->nodeType == NODE_TYPE_INTERNAL" then in phase "i-1" the "leafs[k-1]->parent->suffixLink" was set after adding "article[k, ..., i-1]".  

  \\\TRICK STARTS/// 
  It's actually tricky that "leafs[j]" in phase "i" must represent a substring article[j, ..., i], if "leafs[j]" exists, the proof is neglected here.
  ///TRICK ENDS\\\

  \\\TRICK STARTS/// 
  Consider the fact that "height(leafs[j]) <= 1+article.length()" is true for all "0 <= j < article.length()", which of course covers "0 <= j <= i".

  - Denote "cur = leafs[k-1] & H = height(cur)".
  - The value of "H" will change as "cur" moves from "leafs[k-1]".
  - Moving along "cur->parent" certainly decreases "H" by 1.   
  - Moving along "cur->suffixLink (if exists)" has uncertain effect on "H", i.e. you might increase "H" by such move, but if it decreases "H" that'd be at most by 1. See [memo#1] for a proof for the latter argument.    
  - Moving along "cur->children[...]" certainly increases "H" by 1.

  Let's say that adding 
  - article[k, ..., i],
  ...
  - article[k'-1, ..., i],
  to "root" will create new leafs in phase "i", namely "leafs[k, ..., k'-1]" -- while from "article[k', ..., i]" on there comes no new leaf creation in phase "i".

  -------------------------------------------------------------
  | change  |move| ->parent | ->children[...] | ->suffixLink   |
  |       o -----|          |                 |                |
  | ------ f     |          |                 |                |
  | starts|   H  |          |                 |                |
  --------------------------------------------------------------
  | leafs[k-1]   |  -1      |    +(>=0)       |  +(>=0), or -1 |
  | leafs[k]     |  -1      |    +(>=0)       |  +(>=0), or -1 |
  | leafs[k+1]   |  -1      |    +(>=0)       |  +(>=0), or -1 |
  | ...          |  -1      |    +(>=0)       |  +(>=0), or -1 |
  | leafs[k'-1]  |  -1      |    +(>=0)       |  +(>=0), or -1 |
  --------------------------------------------------------------

  In phase "i", we can move at most "2*(k'-k+1)" up-steps, assuming all "->suffixLink" steps contribute to "-1(up)", therefore the sum of all down-steps together couldn't exceed "2*(k'-k+1)+article.length() <= 3*article.length()", assuming "leafs[k-1] == root".  
  ///TRICK ENDS\\\

  \\\TRICK STARTS/// 
  Moreover, in the next phase "i+1" we'll add leafs[k', ..., k''-1], and then on in future phases till the final leaf is added, therefore over all phases we created leafs[0, ..., <=article.length()), and thus move at most "2*article.length()" up-steps plus "3*article.length()" down-steps.   
  ///TRICK ENDS\\\

  \\\TRICK STARTS/// 
  In fact for each "j" in phase "i", to make full use of "->suffixLink" we COULDN'T start going up from "leafs[j-1]" that represents "article[j-1, ..., i]"(after incremented "sharedLeafEnd"), but instead from an "ActivePoint" that exactly represents the end of "article[j, ..., i-1]" in the "SuffixTree". 
  ///TRICK ENDS\\\
  */

  (*sharedLeafEnd) = (i+1);
  if (article[i] == TERMINATOR_CH) {
    /*
    There could be multiple "TERMINATOR_CH"s in "article". 
    */
    return;
  }

  int k = leafs.size();
  printf("\nDoPhase, i:%d, *sharedLeafEnd:%d, k:%d\n", i, *sharedLeafEnd, k);
  int level = 0;
  int indentSpaceCount = (1 << level);

  for (int j = k; j <= i; ++j) { 
    printf("%*s@j:%d, activePoint is\n", indentSpaceCount, "", j);
    pAp->print(article, level+1);

    SuffixTreeNode* currentTail = pAp->tail(article);
    /*
    For each "j", if "NULL != currentTail" exists then 
    - it's guaranteed that "article[currentTail->start + pAp->offset] == article[i-1]", but 
    - it's NOT GUARANTEED that "currentTail->start + pAp->offset == i-1", i.e. "currentTail" is NOT NECESSARILY of NODE_TYPE_LEAF. 
    */
    if (NULL == currentTail) {
      printf("%*sNULL == currentTail\n", indentSpaceCount, "");
      char newDownCh = article[pAp->downChPosInArticle];
      SuffixTreeNode *newLeaf = new SuffixTreeNode(pAp->downChPosInArticle, sharedLeafEnd);
      pAp->head->children[newDownCh] = newLeaf;  
      newLeaf->parent = pAp->head;
      leafs.push_back(newLeaf);

      printf("%*screated newLeaf only \n", indentSpaceCount, "");
      newLeaf->print(article, level+1);

      ++pAp->downChPosInArticle;
      if (0 <= pAp->offset) {
        --pAp->offset;
      }
    } else {
      printf("%*scurrentTail is \n", indentSpaceCount, "");
      currentTail->print(article, level+1);
      int toMatchLength = i-(pAp->downChPosInArticle)+1;
      while (NULL != currentTail && toMatchLength > currentTail->length()) {
        printf("%*swalking down...\n", indentSpaceCount, "");
        // walk the "ActivePoint" down
        pAp->head = currentTail;
        pAp->downChPosInArticle += currentTail->length();
        pAp->offset -= currentTail->length();

        toMatchLength = i-(pAp->downChPosInArticle)+1;
        currentTail = pAp->tail(article);
      }
      // now that either "NULL == currentTail" or "toMatchLength <= currentTail->length()" 
      if (NULL == currentTail) {
        printf("%*scurrentTail is NULL after the walk-down\n", indentSpaceCount, "");
        char newDownCh = article[pAp->downChPosInArticle];
        SuffixTreeNode *newLeaf = new SuffixTreeNode(pAp->downChPosInArticle, sharedLeafEnd);
        pAp->head->children[newDownCh] = newLeaf;  
        newLeaf->parent = pAp->head;
        leafs.push_back(newLeaf);

        printf("%*screated newLeaf only \n", indentSpaceCount, "");
        newLeaf->print(article, level+1);

        ++pAp->downChPosInArticle;
        if (0 <= pAp->offset) {
          --pAp->offset;
        }
      } else {
        printf("%*sactivePoint is updated by the walk-down to \n", indentSpaceCount, "");
        pAp->print(article, level+1);
        printf("%*scurrentTail is updated by the walk-down to\n", indentSpaceCount, "");
        currentTail->print(article, level+1);
        // now that "toMatchLength <= currentTail->length()" 
        if (article[i] != article[currentTail->start+pAp->offset+1]) {
          /*
          Consider "article: xabxac#", we'll go into this case at 
          ```
          xabxac#
             ^ ^
             j i
          ```
          and "j == 3, i == 5", where 
          - "pAp->head" is root
          - "pAp->downChPosInArticle == 3" 
          - "currentTail->start == 0"
          - toMatchLength == (i-(pAp->downChPosInArticle)+1):3 <= (currentTail->length()):6 
          - "pAp->offset == 1" 

          therefore 
          ```
          article[i:5]:'c' != article[(currentTail->start+pAp->offset+1):2]:'b'
          ```
          */
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

          printf("%*screated newLeaf \n", indentSpaceCount, "");
          newLeaf->print(article, level+1);

          printf("%*screated newInternalNode \n", indentSpaceCount, "");
          newInternalNode->print(article, level+1); 

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
          /*
          Consider "article: xabxac#", we'll go into this case at 
          ```
          xabxac#
             ^
          both i,j
          ```
          and "j == i == 3", where 
          - "pAp->head" is root
          - "pAp->downChPosInArticle == 3" after just created 3 leafs
          - "currentTail->start == 0"
          - toMatchLength == (i-(pAp->downChPosInArticle)+1):1 <= (currentTail->length()):4 
          - "pAp->offset == INVALID == -1" 

          therefore 
          ```
          article[i:3] == article[(currentTail->start+pAp->offset+1):0] == 'x'
          ```
          */

          ++pAp->offset; // the initial value of "offset" is "INVALID == -1", hence no need to check validity and set it to 0 in an extra block
          /*
          We're going immediately to the next phase "i+1".
          */
          break;

          /*
          Then in the next phase "i+1", still "k == 3".
          ```
          xabxac#
             ^^
             ji
          ```
          and "j == 3, i == 4", where 
          - "pAp->head" is root
          - "pAp->downChPosInArticle == 3"
          - "currentTail->start == 0"
          - toMatchLength == (i-(pAp->downChPosInArticle)+1):2 <= (currentTail->length()):5 
          - "pAp->offset == 0" thus the whole "pAp" represents "article[j, ..., i-1]: x"

          therefore 
          ```
          article[i:4] == article[(currentTail->start+pAp->offset+1):1] == 'a'
          ```
          */
        }
      }
    } 
  }
}

SuffixTreeNode* BuildSuffixTree(string & article) {
  /*
  To be deallocated by the caller.
  */
  SuffixTreeNode* root = new SuffixTreeNode(); 
  int theSharedLeafEnd = 0;
  vector<SuffixTreeNode*> leafs;
  SuffixTreeNode* lastNewInternalNode = NULL;

  ActivePoint ap(root);
  ap.downChPosInArticle = 0; // initially points to article[downChPosInArticle:0], but no valid offset  

  for (int i = 0; i < article.length(); ++i) { 
    DoPhase(root, i, article, &theSharedLeafEnd, leafs, &lastNewInternalNode, &ap);
  }

  return root;
}

// driver program to test above functions 
int main(int argc, char *argv[]) 
{ 
  /*
  An easy case.
  */
  // string article = "xabxac"; article.push_back(TERMINATOR_CH);

  /*
  A moderate case, we'll have the first "activePoint walk-down" for phase "9", @j:6.
  ```
  0123456789
  xabxacxaba
        ^  ^
        k  i
        j
  ```
  */
  //string article = "xabxacxaba"; article.push_back(TERMINATOR_CH); 

  string article = "xabxacxabac"; article.push_back(TERMINATOR_CH); // still outputs wrong answer
  SuffixTreeNode* root = BuildSuffixTree(article);
  printf("\nThe SuffixTree for \"%s\" is\n", article.c_str());
  root->print(article, 1);
  return 0; 
} 

/*
[memo#1] 
For a completely built SuffixTree, if there're an "InternalNode `v`" whose "height(v) > 1" and another "InternalNode `u`" whose "height(u) > 1", where "u" is an ancestor of "v", then between "root" and "v->suffixLink" there must exist a "u->suffixLink", EXCEPT for the case when "[root, u]" represents only the 1st letter. Hence "height(v->suffixLink) >= height(v)-1".  
*/

