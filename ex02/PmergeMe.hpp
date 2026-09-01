#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <string>

class PmergeMe {
private:
    std::vector<int> _vec;
    std::deque<int> _deq;

    template <typename Container>
    void sortPairs(Container& arr);

    template <typename Container>
    void fordJohnsonSort(Container& arr);

public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void parseInput(int argc, char** argv);
    void executeSort();
};

#endif