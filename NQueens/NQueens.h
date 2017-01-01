#ifndef N_QUEENS_H
#define N_QUEENS_H

#include <cstdint>
#include <algorithm>

class Board {
public:
    Board(unsigned n) : size_(n)
    {
        std::fill(std::begin(board_), std::end(board_), 0);
    }
    bool diagonal(int x, int y);
    bool line(unsigned lineno, unsigned column)
    {
        return board_[lineno] == 0;
    }
    // 因为从左向右一列一列摆，不再需要判定列的惟一性
    // bool column(unsigned x, unsigned y);
    void put(unsigned lineno, unsigned column)
    {
        board_[lineno] |= 0x8000 >> column;
    }
    void unput(unsigned lineno, unsigned column)
    {
        board_[lineno] ^= 0x8000 >> column;
    }
    bool get(unsigned lineno, unsigned column) const
    {
        return board_[lineno] & (0x8000 >> column);
    }
    unsigned size(void) const
    {
        return size_;
    }
private:
    unsigned size_;
    // 16皇后问题有14772512个解，暂作上限
    std::uint16_t board_[16];
};

class NQueenSolver {
public:
    enum PrintOption {
        Print, NoPrint
    };
    NQueenSolver(Board& bd, PrintOption po) : board_(bd), nsolutions_(0),
        option_(po) {}
    void solve(void)
    {
        fill(0);
    }
    unsigned long solutions(void) const
    {
        return nsolutions_;
    }
    bool any(void) const
    {
        return nsolutions_ != 0;
    }
private:
    PrintOption option_;
    bool fill(unsigned depth);
    unsigned long nsolutions_;
    Board& board_;
};

#endif // N_QUEENS_H