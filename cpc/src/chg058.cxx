#include <map>

#include "challenge.h"
#include "graph.h"

void test0() {
    cpc::graph<char> gr;
    gr.add_edge('A', 'B', 7);
    gr.add_edge('A', 'C', 9);
    gr.add_edge('A', 'F', 14);
    gr.add_edge('B', 'C', 10);
    gr.add_edge('B', 'D', 15);
    gr.add_edge('C', 'D', 11);
    gr.add_edge('C', 'F', 2);
    gr.add_edge('D', 'E', 6);
    gr.add_edge('E', 'F', 9);
    constexpr char source = 'A';
    std::map<char, double> min_distance;
    std::map<char, char> previous;
    cpc::shortest_path(gr, source, min_distance, previous);

    for (auto const& [vertex, weight] : min_distance) {
        std::cout << source << " -> " << vertex << " : "
            << weight << '\t';
        cpc::print_path(cpc::build_path(previous, vertex));
        std::cout << std::endl;;
    }
}

int main(int, char**) {
    test0();

    return 0;
}
