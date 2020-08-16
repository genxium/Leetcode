#define MAXN 1000001

class Seg {
public:
    int l, r;
    Seg *prev = NULL, *next = NULL;
    Seg(int aL, int aR) {
        l = aL;
        r = aR;
    }
    
    int length() {
        return (r-l);
    }
};

class Solution {
private:
    void printList(Seg* grandHead) {
        Seg* trav = grandHead->next;
        while (NULL != trav) {
            printf("[%d, %d] ", trav->l, trav->r);
            trav = trav->next;
        }
        printf("\n");
    }
    
    void cover(Seg* s) {
        if (s->prev) {
            s->prev->r = s->r;
            s->prev->next = s->next;
        }
        if (s->next) {
            s->next->prev = s->prev;
        }
    }
    
    void uncover(Seg* s) {
        if (s->prev) {
            s->prev->next = s;
            s->prev->r = s->l;
        }    
        if (s->next) {
            s->next->prev = s;
        }
    }
    
public:
    void dfs(Seg* grandHead, int acc, int &ans) {
        if (acc >= ans) return;
        
        Seg *trav = grandHead->next;
        if (NULL == trav || NULL == trav->next) {
            ans = acc;
            return;
        }
        
        while (NULL != trav && NULL != trav->next) {
            int inc = trav->length() + trav->next->length(); 
            Seg* cachedNext = trav->next;
            cover(cachedNext);
            dfs(grandHead, acc+inc, ans);
            uncover(cachedNext);
            trav = trav->next;
        }
    }
    
    int minCost(int n, vector<int>& cuts) {
        /*
        - The "state" after "cuts[]" is irrelevant to the order of cutting.
        - The min cost by cutting from "state1(fewer segs)" to "state2(more segs)" is identical to the min cost by merging from "state2" to "state1", assuming that "cost of merging is sum of both lengths".
        */
        
        /*
        test case #1
        36
        [5,6,8,11,13,15]
        
        test case #2
        36
        [13,17,15,18,3,22,27,6,35,7,11,28,26,20,4,5,21,10,8]
        */
        sort(cuts.begin(), cuts.end());
        
        Seg *grandHead = new Seg(0, n);
        int cntCuts = cuts.size();
        Seg *curr = new Seg(0, n);
        grandHead->next = curr;
        curr->prev = grandHead;
        for (int i = 0; i < cntCuts; ++i) {
            curr->r = cuts[i];
            Seg *newCurr = new Seg(cuts[i], n);
            newCurr->prev = curr;
            curr->next = newCurr;
            curr = newCurr;
        }
        
        //printList(grandHead);
        
        int ans = MAXN;
        dfs(grandHead, 0, ans);
        return ans;
    }
};
