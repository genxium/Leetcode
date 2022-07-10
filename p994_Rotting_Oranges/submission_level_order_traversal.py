debug = False
dy = [-1, +1, 0, 0]
dx = [0, 0, -1, +1]

class Solution:
    def orangesRotting(self, grid: List[List[int]]) -> int:
        allcnt = 0
        rotcnt = 0
        q = deque()
        seen = set()
        yupper, xupper = len(grid), len(grid[0]) 
        for y in range(yupper):
            for x in range(xupper):
                e = grid[y][x]
                allcnt += (1 if e > 0 else 0)
                if e == 2:
                    q.append((y,x))
                    seen.add((y,x))
            
        mins = 0
        while q:
            sz = len(q)
            if debug: print(f'after {mins} mins')
            for i in range(sz):
                f = q.popleft()
                rotcnt += 1
                y,x = f[0],f[1]
                if debug: print(f'\t(y:{y}, x:{x}) rotted')
                for j in range(4):
                    ny,nx = y+dy[j],x+dx[j] 
                    if ny < 0 or ny >= yupper or nx < 0 or nx >= xupper: 
                        continue
                    if grid[ny][nx] != 1: continue
                    nc = (ny,nx) # next coordinate
                    if nc in seen:
                        continue
                    q.append(nc)
                    seen.add(nc)
                
            mins += 1
            
        if debug: print(f'rotcnt = {rotcnt}, allcnt = {allcnt}')
        if rotcnt < allcnt:
            return -1
        
        return max(0, mins-1)
