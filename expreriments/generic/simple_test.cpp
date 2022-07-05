#include <iostream>
#include <iterator>
#include <ostream>
#include <concepts>

#include <vector>
#include <array>
#include <set>
#include <unordered_set>

//template <template<class> class Container, class T>
//requires std::ranges::common_range<Container<T>>
//void printContainer(const Container<T>& container)
void printContainer(const std::ranges::common_range auto & container)
{
    for(const auto& item : container)
        std::cout << item << " ";
    std::cout << std::endl;
}

template <template<class> class Container>
requires std::ranges::common_range<Container<int>>
void printIntContainer(const Container<int>& container)
{
    for(const auto& item : container)
        std::cout << item << " ";
    std::cout << std::endl;
}

int main() {
    std::vector<int> vec {1,2,3,4,45};
    std::set<int> set {32,3,1,2,4};
    std::array<int, 2> arr {1,2};

    printContainer(vec);
    printContainer(set);
    printContainer(arr);

    printIntContainer(vec);
    printIntContainer(set);
}
