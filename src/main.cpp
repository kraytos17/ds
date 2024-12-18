#include <map>
#include <print>
#include <string>

struct Edge {
    long v1{};
    long v2{};
};

int main(int, char**) {
    std::println("Hello, from ds!");
    std::println("{0}", __cpp_concepts);

    std::map<std::string, Edge> m;
    m["edge"] = Edge(1, 2);
}
