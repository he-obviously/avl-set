#include <iostream>
#include <initializer_list>
#include "cmake-build-debug/binary_search_tree.h"

int main() {
    Set<int> a;
    std::string query;
    while (std::cin >> query) {
        if (query == "exit") {
            break;
        } else if (query == "insert") {
            int x;
            std::cin >> x;
            a.insert(x);
        } else if (query == "erase") {
            int x;
            std::cin >> x;
            a.erase(x);
        } else if (query == "print") {
            for (Set<int>::iterator i = a.begin(); i != a.end(); ++i) {
                std::cout << *i << " ";
            }
            std::cout << "\n";
        } else if (query == "size") {
            std::cout << a.size() << "\n";
        } else if (query == "empty") {
            std::cout << (a.empty() ? "yes\n" : "no\n");
        } else if (query == "find") {
            int x;
            std::cin >> x;
            std::cout << (a.find(x) != a.end() ? "yes\n" : "no\n");
        } else if (query == "lb") {
            int x;
            std::cin >> x;
            Set<int>::iterator i = a.lower_bound(x);
            std::cout << (i == a.end() ? -1 : *i);
            std::cout << "\n";
        } else {
            std::cout << "repeat please\n";
        }
    }
    return 0;
}
