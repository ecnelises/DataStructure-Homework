#include "GridCost.h"
#include <vector>
#include <limits>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <string>
#include <sstream>

Graph min_spanning_tree(Graph& g)
{
    // 构造一个顶点数相同的空图矩阵
    Graph res(g.nodes());
    // 从图g的第一个顶点开始
    std::vector<bool> in_tree(g.nodes(), false);
    std::vector<unsigned> already_in;
    // 从在already_in的所有顶点中找到权值最小且目的地不在in_tree中的边，加入res中
    in_tree.at(0) = true;
    already_in.push_back(0);
    while (already_in.size() != g.nodes()) {
        unsigned tmp_min = std::numeric_limits<unsigned>::max();
        unsigned tmp_start = std::numeric_limits<unsigned>::max();
        unsigned tmp_end = std::numeric_limits<unsigned>::max();
        for (auto i : already_in) {
            for (auto j = 0U; j < g.nodes(); ++j) {
                if (g.between(i, j) != 0 && g.between(i, j) < tmp_min && !in_tree.at(j)) {
                    tmp_min = g.between(i, j);
                    tmp_start = i;
                    tmp_end = j;
                }
            }
        }
        if (tmp_min == std::numeric_limits<unsigned>::max()) { // 不联通
            res.reset();
            return res;
        }
        already_in.push_back(tmp_end);
        in_tree.at(tmp_end) = true;
        res.between(tmp_start, tmp_end) = tmp_min;
    }
    return res;
}

int main(void)
{
    std::cout << "##\t电网造价模拟系统\t##\n"
        << "##\t\t\t\t##\n"
        << "##\tA -> 创建顶点\t\t##\n"
        << "##\tB -> 添加图边\t\t##\n"
        << "##\tC -> 生成最小生成树\t##\n"
        << "##\tD -> 显示最小生成树\t##\n"
        << "##\tE -> 退出\t\t##\n";
    std::unique_ptr<Graph> the_graph;
    char command;
    std::cout << "请选择操作：";
    while (std::cin >> command) {
        switch (command) {
        case 'A': {
            std::cout << "请输入顶点个数：";
            unsigned n_nodes;
            std::cin >> n_nodes;
            the_graph = std::make_unique<Graph>(n_nodes);
            std::cout << "生成完毕\n";
            break;
        }
        case 'B': {
            std::string tmp;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "顶点编号从1到n\n";
            do {
                std::cout << "请输入两个顶点及边（空行结尾）：";
                std::getline(std::cin, tmp);
                std::istringstream iss(tmp);
                unsigned start, end, weight;
                iss >> start >> end >> weight;
                the_graph->between(start - 1, end - 1) = weight;
            } while (tmp != "");
            break;
        }
        case 'C':
            std::cout << "最小生成树生成完毕！\n";
            break;
        case 'D': {
            auto mst = min_spanning_tree(*the_graph);
            mst.for_connected([&](int i, int j, int weight){
                std::cout << i + 1 << "->" << j + 1 << '(' << weight << ")\n";
            });
            break;
        }
        case 'E':
            std::exit(0);
            break;
        default:
            std::cout << "命令不存在！\n";
            break;
        }
        std::cout << "请选择操作：";
    }
}