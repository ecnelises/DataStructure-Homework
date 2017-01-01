#include "Maze.h"
#include <iostream>

bool Maze::valid_block(int r, int c) const
{
    if (r < 0 || c < 0) {
        return false;
    }
    if (r >= row || c >= col) {
        return false;
    }
    if (map.at(r * col + c) == Unexplored) {
        return true;
    }
    return false;
}

void Maze::findWay()
{
    int id_gen = 1;
    int cur_id = 1;
    Point current = start_place;
    if (valid_block(start_place.row, start_place.column)) {
        path.push({start_place, 0, 1});
    }
    bool forwardable = false;
    while (!path.empty()) {
        // 寻找四个方向可以走的路，压入栈中，然后把当前位置设置成这些
        forwardable = false;
        if (valid_block(current.row - 1, current.column)) {
            forwardable = true;
            path.push({Point(current.row - 1, current.column), cur_id, ++id_gen});
        }
        if (valid_block(current.row, current.column - 1)) {
            forwardable = true;
            path.push({Point(current.row, current.column - 1), cur_id, ++id_gen});
        }
        if (valid_block(current.row + 1, current.column)) {
            forwardable = true;
            path.push({Point(current.row + 1, current.column), cur_id, ++id_gen});
        }
        if (valid_block(current.row, current.column + 1)) {
            forwardable = true;
            path.push({Point(current.row, current.column + 1), cur_id, ++id_gen});
        }
        if (is_destination(current.row, current.column)) {
            // 找到了终点
            break;
        } else if (forwardable) {
            map.at(current.row * col + current.column) = ToBeMarked;
            current = std::get<0>(path.top());
            cur_id = std::get<2>(path.top());
        } else {
            // 回退
            map.at(current.row * col + current.column) = BadWay;
            path.pop();
            current = std::get<0>(path.top());
            cur_id = std::get<2>(path.top());
        }
    }
    while (!path.empty()) {
        if (cur_id == std::get<2>(path.top())) {
            success_path.push_back(std::get<0>(path.top()));
            cur_id = std::get<1>(path.top());
        }
        path.pop();
    }
}

void input_maze(Maze& mz)
{
    std::cout << "请输入迷宫，1表示墙，0表示路：\n";
    int tmp = -1;
    for (auto i = 0; i < mz.row * mz.col; ++i) {
        std::cin >> tmp;
        if (tmp == 0 || tmp == 1) {
            mz.map.at(i) = tmp + 1;
        } else {
            std::cout << "请正确输入0或1！\n";
            --i;
        }
    }
}

std::ostream& operator << (std::ostream& o, const Point& p)
{
    o << '(' << p.row << ", " << p.column << ')';
    return o;
}

int main(void)
{
    int mrow = 0, mcol = 0;
    std::cout << "请输入迷宫的行数：";
    std::cin >> mrow;
    std::cout << "请输入迷宫的列数：";
    std::cin >> mcol;
    int srow = 0, scol = 0;
    std::cout << "请输入迷宫的起点行和起点列（0开始）：";
    std::cin >> srow >> scol;
    Maze the_maze(mrow, mcol, {srow, scol});
    input_maze(the_maze);
    the_maze.findWay();
    if (the_maze.getPath().empty()) {
        std::cout << "无路可走！\n";
    } else {
        std::cout << "路径是：";
        std::cout << *(the_maze.getPath().crbegin());
        for (auto i = the_maze.getPath().crbegin() + 1;
             i != the_maze.getPath().crend(); ++i) {
            std::cout << "->" << *i;
        }
        std::cout << '\n';
    }
}