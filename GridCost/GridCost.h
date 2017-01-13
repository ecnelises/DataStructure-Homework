#ifndef GRID_COST_H
#define GRID_COST_H

#include <vector>
#include <limits>
#include <algorithm>

class Graph {
public:
    Graph(unsigned nodes) : in_rep(nodes * nodes, 0), n_nodes(nodes) {}
    int& between(unsigned r, unsigned s)
    {
        return in_rep.at(n_nodes * std::min(r, s) + std::max(r,s));
    }
    const int& between(unsigned r, unsigned s) const
    {
        return in_rep.at(n_nodes * std::min(r, s) + std::max(r, s));
    }
    unsigned nodes(void) const
    {
        return n_nodes;
    }
    void reset(void)
    {
        std::fill(std::begin(in_rep), std::end(in_rep), 0);
    }
    template<typename F>
    void for_connected(F fn)
    {
        for (auto i = 0; i < n_nodes; ++i) {
            for (auto j = 0; j < i; ++j) {
                if (between(j, i) != 0) {
                    fn(j, i, between(j, i));
                }
            }
        }
    }
private:
    unsigned n_nodes;
    std::vector<int> in_rep;
};

Graph min_spanning_tree(const Graph& g);

#endif // GRID_COST_H