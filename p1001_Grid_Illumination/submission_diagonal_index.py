dx = [0, 0, 0, +1, -1, +1, -1, +1, -1]
dy = [0, +1, -1, 0, 0, +1, -1, -1, +1]

class Solution:
    def gridIllumination(self, n: int, lamps: List[List[int]], queries: List[List[int]]) -> List[int]:
        lightedx, lightedy, lighteddiag, lightedrdiag = defaultdict(int), defaultdict(int), defaultdict(int), defaultdict(int)
        haslamp = defaultdict(lambda: defaultdict(int))
        for lamp in lamps:
            y, x = lamp[0], lamp[1]
            if haslamp[y][x]: continue
            haslamp[y][x] = True
            lightedy[y] += 1
            lightedx[x] += 1
            diag = y + x
            rdiag = n-1-y + x
            lighteddiag[diag] += 1
            lightedrdiag[rdiag] += 1

        ans = []
        for q in queries:
            qy, qx = q[0], q[1]
            qd, qrd = qy+qx, n-1-qy+qx
            if lightedx[qx] > 0 or lightedy[qy] > 0 or lighteddiag[qd] > 0 or lightedrdiag[qrd] > 0:
                ans.append(1)
            else:
                ans.append(0)
            
            for i in range(9):
                ny, nx = qy+dy[i], qx+dx[i]
                if ny < 0 or ny >= n or nx < 0 or nx >= n: continue 
                if not haslamp[ny][nx]: continue
                haslamp[ny][nx] = False
                nd, nrd = ny+nx, n-1-ny+nx
                lightedx[nx] -= 1
                lightedy[ny] -= 1
                lighteddiag[nd] -= 1
                lightedrdiag[nrd] -= 1
                
        return ans
