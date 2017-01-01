#include <iostream>
#include "NQueens.h"

bool Board::diagonal(int lineno, int column)
{
    int xt, yt;
    // 判定从左上到右下的对角线，根据line和column的大小关系分为两种情况
    if (lineno - column < 0) {
        yt = 0;
        xt = column - lineno;
    } else {
        yt = lineno - column;
        xt = 0;
    }
    for (; yt < size_ && xt < size_; ++xt, ++yt) {
        if (board_[yt] & (0x8000 >> xt)) {
            return false;
        }
    }
    // 另一条对角线，依然两种情况
    if (lineno + column < size_) {
        yt = 0;
        xt = lineno + column;
    } else {
        yt = lineno + column - size_ + 1;
        xt = size_ - 1;
    }
    for (; xt >= 0 && yt < size_; ++yt, --xt) {
        if (board_[yt] & (0x8000 >> xt)) {
            return false;
        }
    }
    return true;
}

void print(const Board& bd)
{
    for (int i = 0; i < bd.size(); ++i) {
        for (int j = 0; j < bd.size(); ++j) {
            if (bd.get(i, j)) {
                std::cout << "X ";
            } else {
                std::cout << "0 ";
            }
        }
        std::cout << '\n';
    }
}

bool NQueenSolver::fill(unsigned depth)
{
    if (depth == board_.size()) {
        return true;
    }
    for (int i = 0; i < board_.size(); ++i) {
        if (board_.line(i, depth) && board_.diagonal(i, depth)) {
            board_.put(i, depth);
            if (fill(depth + 1)) {
                if (option_ == Print) {
                    print(board_);
                    std::cout << '\n';
                }
                ++nsolutions_;
            }
            board_.unput(i, depth);
        }
    }
    return false;
}

// 测试用例类
class NQueenTestCase {
public:
    NQueenTestCase(unsigned n, unsigned answer)
    {
        Board bd(n);
        NQueenSolver svr(bd, NQueenSolver::NoPrint);
        svr.solve();
        std::cout << n << "...";
        if (svr.solutions() == answer) {
            std::cout << "Right!\n";
        } else {
            std::cout << "Wrong...\n";
        }
    }
};

int main(void)
{
#ifdef HAVETEST
    NQueenTestCase(1, 1);
    NQueenTestCase(2, 0);
    NQueenTestCase(3, 0);
    NQueenTestCase(4, 2);
    NQueenTestCase(5, 10);
    NQueenTestCase(6, 4);
    NQueenTestCase(7, 40);
    NQueenTestCase(8, 92);
    NQueenTestCase(9, 352);
    NQueenTestCase(10, 724);
    NQueenTestCase(11, 2680);
    NQueenTestCase(12, 14200);
    NQueenTestCase(13, 73712);
    NQueenTestCase(14, 365596);
    NQueenTestCase(15, 2279184);
    NQueenTestCase(16, 14772512);
#else
    std::cout << "现有NxN的棋盘，放入N个皇后，要求所有皇后不在同一行、列和同一斜线上！\n";
    std::cout << "请输入皇后的个数：";
    unsigned n;
    std::cin >> n;
    Board bd(n);
    NQueenSolver svr(bd, NQueenSolver::Print);
    std::cout << "皇后摆法：\n";
    svr.solve();
    std::cout << "共有" << svr.solutions() << "种摆法\n";
#endif
}
