debug = False
def findRoot(u, pars):
    if u not in pars:
        pars[u] = u
    
    if pars[u] != u:
        pars[u] = findRoot(pars[u], pars)
    
    return pars[u]
    
def unite(u, v, pars):
    pu, pv = findRoot(u, pars), findRoot(v, pars)
    pars[pu] = pars[pv] = min(pu, pv)
    if debug: print(f'uniting {u}->{pu} and {v}->{pv} into {pars[pu]}')
    return pars[pu]

"""
test cases
["a==b","b!=a"]
["b==a","a==b"]
["b!=f","f==f","d==f","b==f","a==f"]
["a==b","e==c","b==c","a!=e"]
"""
class Solution:
    def equationsPossible(self, equations: List[str]) -> bool:
        pars = dict()
        for s in equations:
            if "==" in s:
                unite(ord(s[0]), ord(s[3]), pars)
        
        for s in equations:
            if "!=" in s:
                pu, pv = findRoot(ord(s[0]), pars), findRoot(ord(s[3]), pars)
                if debug:
                    print(f'checking inequalities for {ord(s[0])}->{pu}, {ord(s[3])}->{pv}')
                if pu == pv:
                    return False
                
        return True
        
        
        
