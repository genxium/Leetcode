typedef vector<int> VI;
int const MAXN = 3000;
int topick[MAXN]; // "topick[i] == x" means that we still need choose "x" numbers from "intervals[i]".

class ItvlCmp {
public: 
    bool operator () (VI const& lhs, VI const& rhs) {
        if (lhs[1] != rhs[1]) return lhs[1] < rhs[1]; // Earlier end comes first
        
        /*
        [WARNING]
        
        The order of "starting" being reversed w.r.t. the order of "ending" is critical, see test case "[[1,3],[3,7],[5,7],[7,8]]" for an example.
        */
        return lhs[0] > rhs[0];
    }
};

/*
test cases
[[1,3],[1,4],[2,5],[3,5]]
[[1,2],[2,3],[2,4],[4,5]]
[[2,10],[3,7],[3,15],[4,11],[6,12],[6,16],[7,8],[7,11],[7,15],[11,12]]
[[1,3],[3,7],[5,7],[7,8]]
*/
class Solution {
private:
    bool withinInterval(int point, VI &itvl) {
        return itvl[0] <= point && point <= itvl[1];
    }
    
public:
    int intersectionSizeTwo(vector<vector<int>>& intervals) {
        int n = intervals.size();
        sort(intervals.begin(), intervals.end(), ItvlCmp());
        // init
        int ans = 0;
        for (int i = 0; i < n; ++i) topick[i] = 2;
    
        // loop
        for (int i = 0; i < n; ++i) {
            if (topick[i] <= 0) continue;
            int edi = intervals[i][1];
            int borderi = edi - topick[i];
            for (int j = i+1; j < n; ++j) {
                /*
                if (edj == edi) then "stj < sti", this is to avoid the error for the following edge case if "st" were also sorted ascendingly
                ```
                i-iteration#1
                -------------
                i-> [st1        ed1], topick: 2
                j->            [st2            ed2], topick: was 2 and now reduced to 1 by value "ed1 == st2"
                                       [st3    ed3], topick: 2
               -------------
                
                i-iteration#2   
                -------------
                    [st1        ed1], topick: 2
                i->            [st2            ed2], topick: 1
                j->                    [st3    ed3], topick: was 2 and now reduced to 1 by value "ed2 == ed3", note that "ed2" is also used to reduce further "topick[j]"s in this "i-iteration".
                -------------
                                       
                i-iteration#3  
                -------------
                    [st1        ed1], topick: 2
                               [st2            ed2], topick: 1
                i->                    [st3    ed3], topick: 1, the value "ed3 == ed2" will be reused to reduce further some "topick[j]", which is WRONG!
                -------------
                ```
                */
                for (int p = edi; p > borderi; --p) {
                    if (false == withinInterval(p, intervals[j])) continue;
                    --topick[j];
                }
            }
            ans += topick[i];
        }
        
        return ans;
    }
};
