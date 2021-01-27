bool const debug = false;
int const MAXN = 100;
int const MAXCOLOR = 27;
int const NOCOLOR = 0;
int const INVALID = -1;

int colors[MAXN];
int nextSameColorPos[MAXN];

int cache[MAXCOLOR][MAXN][MAXN]; 

int calcColor(char ch) {
  return ch-'a'+1;
}

int dfs(int prefixColor, int i, int j, int level) {  
    int indent = 2*level;
    if (INVALID != cache[prefixColor][i][j]) {
        return cache[prefixColor][i][j]; 
    }

    if (i == j) {
        return cache[prefixColor][i][j] = (prefixColor == colors[i] ? 0 : 1);
    }

    int cand = INT_MAX;
    for (int k = i; k < j; k = nextSameColorPos[k]) {
        int lhs = dfs(prefixColor, i, k, level+1); 
        if (INT_MAX != lhs) {
            // use "colors[k]" as "prefixColor"
            int rhs = dfs(colors[k], k+1, j, level+1);
            if (INT_MAX != rhs && lhs+rhs < cand) {
                cand = lhs+rhs;
            }
            if (NOCOLOR == prefixColor) continue;
            // use existing "prefixColor", which might have reached here from a far early recurssion...
            rhs = dfs(prefixColor, k+1, j, level+1);
            if (INT_MAX != rhs && lhs+rhs < cand) {
                cand = lhs+rhs;
            }
        }
    }

    return cache[prefixColor][i][j] = cand;
}

class Solution {    
public:
    int strangePrinter(string s) {
        // remove consecutively repeating characters
        int n = 0;
        for (char ch : s) {
            int color = calcColor(ch);
            if (n == 0) colors[n++] = color;
            else if (color != colors[n-1]) colors[n++] = color;
        }
        
        if (0 == n) return 0;
        if (1 == n) return 1;
        
        // [WARNING] It's very important to make use of "nextSameColorPos[...]" to quickly identify the useful partitions! Only such partitions make full use of each printing step. 
        for (int i = 0; i < n; ++i) {
            int j = i+1;
            for (; j < n; ++j) {
                if (colors[j] == colors[i]) break;
            }
            nextSameColorPos[i] = j;
        }
        
        int inPrefixColor = NOCOLOR; 
        memset(cache, INVALID, sizeof cache);
        int outPrefixColor = colors[n-1]; // it's safe to assume that the final outPrefixColor is the last color
        int ans = dfs(inPrefixColor, 0, n-1, 0); 
        return ans;        
    }
};
