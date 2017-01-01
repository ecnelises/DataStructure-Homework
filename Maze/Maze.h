#ifndef MAZE_H
#define MAZE_H

#include <stack>
#include <vector>
#include <tuple>

struct Point {
    Point(int r, int c) : row(r), column(c) {}
    int row;
    int column;
};

class Maze {
    friend void input_maze(Maze& mz);
public:
    Maze(int rows, int columns, Point start = {0, 0}) : col(columns), row(rows),
    map(columns * rows, 0), start_place(start) {}
    ~Maze() = default;
    bool success() const
    {
        return !success_path.empty();
    }
    const std::vector<Point>& getPath() const
    {
        return success_path;
    }
    void findWay();
private:
    static const int Uninitialized = 0;
    static const int Unexplored = 1;
    static const int Wall = 2;
    static const int ToBeMarked = 3;
    static const int BadWay = 4;
    bool is_destination(int r, int c) const
    {
        return (c == col - 1) || (r == row - 1);
    }
    bool valid_block(int r, int c) const;
    // 不同的标记
    // 0 -> 未初始化
    // 1 -> 未探索过的路
    // 2 -> 墙
    // 3 -> 待标记的路
    // 4 -> 确定的死路
    std::stack<std::tuple<Point, int, int>> path;
    std::vector<Point> success_path;
    std::vector<int> map;
    int col;
    int row;
    Point start_place;
};

#endif // MAZE_H