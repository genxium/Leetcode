typedef pair<int, int> PII;
typedef vector<int> VI;
bool const debug = false;

class Cmp {
public:
    bool operator() (VI const &lhs, VI const &rhs) const {
        return lhs[0] > rhs[0];
    }
};

/*
test cases
[1,3]
[2,1]
[1,2]
[3,1]
[2,4,3]
[1,6,7]
[9,8,3,8]
[10,6,9,5]
*/
class Solution {
public:
    int stoneGameVI(vector<int>& aliceValues, vector<int>& bobValues) {
        /*
        Assume that 
        
        "x = sum(aliceValues[i], i chosen by Alice) - sum(bobValues[j], j chosen by Bob)"
        
        , it's the same value as 
        
        "y = sum(aliceValues[i]+bobValues[i], i chosen by Alice) - sum(bobValues[j], j chosen by Bob) - sum(bobValues[i], i chosen by Alice) = sum(aliceValues[i]+bobValues[i], i chosen by Alice) - sum(bobValues[k], all k)".
        
        Therefore if Alice wants to win, she should maximize "x == y", yet "sum(bobValues[k], all k)" is a constant value, the only variable she should go after greedily is each "aliceValues[i]+bobValues[i]" -- same for Bob.
        */
        int n = aliceValues.size();
        vector<VI> items;
        for (int i = 0; i < n; ++i) {
            items.push_back({aliceValues[i]+bobValues[i], aliceValues[i], bobValues[i]});
        }
        sort(items.begin(), items.end(), Cmp());
        
        bool aliceTurn = true;
        int aliceScore = 0, bobScore = 0;
        for (int i = 0; i < n; ++i) {
            auto &t = items[i];
            if (aliceTurn) {
                aliceScore += t[1];
            } else {
                bobScore += t[2];
            }
            aliceTurn = !aliceTurn;
        }
        if (debug) printf("aliceScore: %d, bobScore: %d\n", aliceScore, bobScore);
        if (aliceScore == bobScore) return 0;
        if (aliceScore > bobScore) return 1;
        return -1;
    }
};
