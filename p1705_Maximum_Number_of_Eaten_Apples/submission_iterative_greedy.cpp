int const MAXDAYS = 40000;
int const MAXN = 20000;
bool const debug = false;
    
int in[MAXDAYS], out[MAXDAYS];

/*
test cases
[1,2,3,5,2]
[3,2,1,4,2]
[3,0,0,0,2]
[3,0,0,0,2]
[1]
[2]
[3,1,1,0,0,2]
[3,1,1,0,0,2]
*/
class Solution {
public:
    int eatenApples(vector<int>& apples, vector<int>& days) {
        memset(in, 0, sizeof in);
        memset(out, 0, sizeof out);
        int n = apples.size();
        
        int lastDay = 0;
        for (int i = 0; i < n; ++i) {
            lastDay = max(lastDay, i+days[i]);
            in[i] = apples[i];
            out[i+days[i]] += apples[i]; // possibly multiple entries
        }
        
        int having = 0, eaten = 0, eatenAccountedForOut = 0;
        for (int d = 0; d < lastDay; ++d) {
            having += in[d];
            if (out[d] > 0) {
                int eatenNotYetAccountedForOut = (eaten-eatenAccountedForOut); 
                int rotten = (out[d] - eatenNotYetAccountedForOut);
                having -= rotten; 
                if (having < 0) having = 0;
                eatenAccountedForOut += eatenNotYetAccountedForOut;
            }
            if (debug) printf("At beginning of day:%d, having:%d, in[%d]:%d, out[%d]:%d, eaten:%d, eatenAccountedForOut:%d\n", d, having, d, in[d], d, out[d], eaten, eatenAccountedForOut);
            
            if (having > 0) {
                --having;
                ++eaten;
                if (debug) printf("Ate %d apples at day:%d\n", eaten, d);
            }
        }
        
        return eaten;
    }
};
