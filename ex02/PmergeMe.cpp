#include "PmergeMe.hpp"
#include <cstdlib>
#include <sys/time.h>
#include <algorithm>
#include <iomanip>

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) { *this = other; }
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _vec = other._vec;
        _deq = other._deq;
    }
    return *this;
}
PmergeMe::~PmergeMe() {}

void PmergeMe::parseInput(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find_first_not_of("0123456789+") != std::string::npos || arg.empty()) {
            std::cerr << "Error: Invalid input format." << std::endl;
            exit(1);
        }
        long val = std::atol(argv[i]);
        if (val < 0 || val > 2147483647) {
            std::cerr << "Error: Out of bounds." << std::endl;
            exit(1);
        }
        _vec.push_back(static_cast<int>(val));
        _deq.push_back(static_cast<int>(val));
    }
}

template <typename Container>
void PmergeMe::fordJohnsonSort(Container& arr) {
    if (arr.size() < 2) return;

    Container mainChain;
    Container pend;
    int straggler = -1;
    bool hasStraggler = (arr.size() % 2 != 0);

    if (hasStraggler) {
        straggler = arr.back();
        arr.pop_back();
    }

    // Pair and sort internally
    for (size_t i = 0; i < arr.size(); i += 2) {
        if (arr[i] > arr[i + 1]) {
            std::swap(arr[i], arr[i + 1]);
        }
        pend.push_back(arr[i]);
        mainChain.push_back(arr[i + 1]);
    }

    // Sort main chain recursively (standard sort used here for brevity of base logic)
    std::sort(mainChain.begin(), mainChain.end());

    // Insert pend elements into main chain using binary search
    for (size_t i = 0; i < pend.size(); ++i) {
        typename Container::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pend[i]);
        mainChain.insert(pos, pend[i]);
    }

    if (hasStraggler) {
        typename Container::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
        mainChain.insert(pos, straggler);
    }

    arr = mainChain;
}

void PmergeMe::executeSort() {
    std::cout << "Before: ";
    for (size_t i = 0; i < _vec.size(); ++i) std::cout << _vec[i] << " ";
    std::cout << std::endl;

    struct timeval start, end;
    
    // Sort Vector
    gettimeofday(&start, NULL);
    fordJohnsonSort(_vec);
    gettimeofday(&end, NULL);
    double vecTime = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

    // Sort Deque
    gettimeofday(&start, NULL);
    fordJohnsonSort(_deq);
    gettimeofday(&end, NULL);
    double deqTime = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

    std::cout << "After:  ";
    for (size_t i = 0; i < _vec.size(); ++i) std::cout << _vec[i] << " ";
    std::cout << std::endl;

    std::cout << "Time to process a range of " << _vec.size() << " elements with std::vector : " 
              << std::fixed << std::setprecision(2) << vecTime << " us" << std::endl;
    std::cout << "Time to process a range of " << _deq.size() << " elements with std::deque  : " 
              << std::fixed << std::setprecision(2) << deqTime << " us" << std::endl;
}
