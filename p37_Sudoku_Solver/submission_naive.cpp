class Solution {
public:
    bool isValid(vector<vector<char>>& existingBoard, int n, int r, int c, int newChar) {
        for (int rr = 0; rr < n; ++rr) {
            if (existingBoard[rr][c] != '.' && existingBoard[rr][c] == newChar) {
                return false;
            }
        }
        for (int cc = 0; cc < n; ++cc) {
            if (existingBoard[r][cc] != '.' && existingBoard[r][cc] == newChar) {
                return false;
            }
        }

        int boxTopLeftRow = 3*(r/3);
        int boxTopLeftCol = 3*(c/3);

        int boxBottomRightRowExclusive = boxTopLeftRow+3;
        int boxBottomRightColExclusive = boxTopLeftCol+3;
        
        for (int rr = boxTopLeftRow; rr < boxBottomRightRowExclusive; ++rr) {
            for (int cc = boxTopLeftCol; cc < boxBottomRightColExclusive; ++cc) {
                if (existingBoard[rr][cc] != '.' && existingBoard[rr][cc] == newChar) {
                    return false;
                }
            }
        }

        return true;
    }
    
    void solveSudokuExtra(vector<vector<char>>& board, int n, int r, int c, bool &found) {
        if (r == n) {
            found = true;
            return;
        }
        if (true == found) return;

        if (board[r][c] != '.') {
            // An preset digit is presented, skip.
            if (c == n-1) {
                solveSudokuExtra(board, n, r+1, 0, found);
            } else {
                solveSudokuExtra(board, n, r, c+1, found);
            }
        } else {
            for (int val = 1; val < 10; ++val) {
                char newChar = '0'+val;
                if (false == isValid(board, n, r, c, newChar)) continue;
                board[r][c] = newChar;
                if (c == n-1) {
                    solveSudokuExtra(board, n, r+1, 0, found);
                } else {
                    solveSudokuExtra(board, n, r, c+1, found);
                }
                if (true == found) break;
                board[r][c] = '.';
            }
        }
    }

    void solveSudoku(vector<vector<char>>& board) {
        int n = board.size();
        bool found = false;
        solveSudokuExtra(board, n, 0, 0, found);
    }
};
