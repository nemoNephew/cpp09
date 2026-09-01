#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <sys/time.h>
#include <cstdlib>
#include <algorithm>
#include <iomanip>

class PmergeMe {
private:
    std::vector<int> _vec;
    std::deque<int>  _deq;

    std::vector<int> generateJacobsthal(int max);
    std::vector<int> generateInsertionSequence(int size);

    void mergePairsVec(std::vector<std::pair<int, int> >& arr, int l, int m, int r);
    void mergeSortPairsVec(std::vector<std::pair<int, int> >& arr, int l, int r);
    void fordJohnsonSort(std::vector<int>& arr);

    void mergePairsDeq(std::deque<std::pair<int, int> >& arr, int l, int m, int r);
    void mergeSortPairsDeq(std::deque<std::pair<int, int> >& arr, int l, int r);
    void fordJohnsonSort(std::deque<int>& arr);

public:
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void parse(int argc, char** argv);
    void sortAndDisplay();
};

#endif
