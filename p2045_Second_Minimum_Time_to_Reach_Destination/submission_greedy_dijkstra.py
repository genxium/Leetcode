debug = False
INVALID = -1
MAXN = 10000

def buildAdj(edges):
    adj = defaultdict(set)
    for e in edges:
        adj[e[0]-1].add(e[1]-1)
        adj[e[1]-1].add(e[0]-1)
    return adj

def dijkstra(adj, n, time, change):
    g = defaultdict(lambda: INVALID) # g[u] is the shortest path len from st to u, g[u+MAXN] is the 2nd shortest path len
    g[0] = 0
    q = [] # priority queue of (g[u], u) and (g[u+MAXN], u+MAXN)
    heapq.heappush(q, (g[0], 0)) # init
    closed = set()
    while q:
        f = heapq.heappop(q)
        exg, exu = f[0], f[1] # expander g, expander u
        if exu in closed: continue
        closed.add(exu)
        if exu == n-1+MAXN:
            break
        underlyingU = ((exu - MAXN) if exu >= MAXN else exu)
        timeslot = int(exg/change)
        timeWaitForGreen = 0
        if ((timeslot & 1) > 0):
            timeWaitForGreen = (change - exg%change)
        if debug: print(f'exg={exg}, exu={exu}, underlyingU={underlyingU}, timeWaitForGreen={timeWaitForGreen}')
        for nb in adj[underlyingU]:
            cand = exg+timeWaitForGreen+time
            if nb not in closed:
                # g[nb] is still open for relaxation
                if INVALID == g[nb] or cand < g[nb]:
                    g[nb+MAXN] = g[nb]
                    g[nb] = cand
                    heapq.heappush(q, (g[nb], nb))
                    if debug: print(f'\t#1 relaxed g[nb:{nb}]={g[nb]}, g[nb+MAXN:{nb+MAXN}]={g[nb+MAXN]}')
            if (INVALID != g[nb] and g[nb] < cand) and (nb+MAXN not in closed):      
                # [WARNING] g[nb] doesn't need be closed by now!      
                # g[nb+MAXN] is still open for relaxation
                if INVALID == g[nb+MAXN] or cand < g[nb+MAXN]:
                    g[nb+MAXN] = cand
                    heapq.heappush(q, (g[nb+MAXN], nb+MAXN))
                    if debug: print(f'\t#2 relaxed g[nb+MAXN:{nb+MAXN}]={g[nb+MAXN]}')
    return g

class Solution:
    def secondMinimum(self, n: int, edges: List[List[int]], time: int, change: int) -> int:
        adj = buildAdj(edges)
        if debug: print(f'adj:\n{adj}')
        g = dijkstra(adj, n, time, change)
        if debug: print(f'g:\n{g}')
        return g[n-1+MAXN]
