#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other) {
    *this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        this->_vec = other._vec;
        this->_deq = other._deq;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::parse(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.empty() || arg.find_first_not_of("0123456789") != std::string::npos) {
            std::cerr << "Error" << std::endl;
            exit(1);
        }
        long val = std::atol(arg.c_str());
        if (val < 0 || val > 2147483647) {
            std::cerr << "Error" << std::endl;
            exit(1);
        }
        _vec.push_back(static_cast<int>(val));
        _deq.push_back(static_cast<int>(val));
    }
}

std::vector<int> PmergeMe::generateJacobsthal(int max) {
    std::vector<int> jacob;
    jacob.push_back(0);
    jacob.push_back(1);
    int last = 1;
    int last2 = 0;
    while (true) {
        int next = last + 2 * last2;
        jacob.push_back(next);
        last2 = last;
        last = next;
        if (next > max) break;
    }
    return jacob;
}

std::vector<int> PmergeMe::generateInsertionSequence(int size) {
    std::vector<int> seq;
    if (size == 0) return seq;
    
    std::vector<int> jacob = generateJacobsthal(size);
    int lastJacob = 1;
    seq.push_back(0);
    
    for (size_t i = 3; i < jacob.size(); ++i) {
        int currentJacob = jacob[i];
        int start = currentJacob - 1;
        if (start >= size) start = size - 1;
        for (int j = start; j >= lastJacob; --j) {
            seq.push_back(j);
        }
        lastJacob = currentJacob;
    }
    return seq;
}

void PmergeMe::mergePairsVec(std::vector<std::pair<int, int> >& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    std::vector<std::pair<int, int> > L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].first <= R[j].first) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void PmergeMe::mergeSortPairsVec(std::vector<std::pair<int, int> >& arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSortPairsVec(arr, l, m);
    mergeSortPairsVec(arr, m + 1, r);
    mergePairsVec(arr, l, m, r);
}

void PmergeMe::fordJohnsonSort(std::vector<int>& arr) {
    if (arr.size() < 2) return;
    
    std::vector<std::pair<int, int> > pairs;
    int straggler = -1;
    bool hasStraggler = (arr.size() % 2 != 0);
    
    if (hasStraggler) straggler = arr.back();
    
    for (size_t i = 0; i < arr.size() - (hasStraggler ? 1 : 0); i += 2) {
        if (arr[i] > arr[i + 1]) pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }
    
    if (!pairs.empty()) {
        mergeSortPairsVec(pairs, 0, pairs.size() - 1);
    }
    
    std::vector<int> mainChain;
    std::vector<int> pend;
    
    for (size_t i = 0; i < pairs.size(); ++i) {
        mainChain.push_back(pairs[i].first);
        pend.push_back(pairs[i].second);
    }
    
    if (!pend.empty()) {
        mainChain.insert(mainChain.begin(), pend[0]);
    }
    
    std::vector<int> seq = generateInsertionSequence(pend.size());
    
    for (size_t i = 1; i < seq.size(); ++i) {
        int target = pend[seq[i]];
        std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), target);
        mainChain.insert(pos, target);
    }
    
    if (hasStraggler) {
        std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
        mainChain.insert(pos, straggler);
    }
    
    arr = mainChain;
}

void PmergeMe::mergePairsDeq(std::deque<std::pair<int, int> >& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    std::deque<std::pair<int, int> > L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].first <= R[j].first) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void PmergeMe::mergeSortPairsDeq(std::deque<std::pair<int, int> >& arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSortPairsDeq(arr, l, m);
    mergeSortPairsDeq(arr, m + 1, r);
    mergePairsDeq(arr, l, m, r);
}

void PmergeMe::fordJohnsonSort(std::deque<int>& arr) {
    if (arr.size() < 2) return;
    
    std::deque<std::pair<int, int> > pairs;
    int straggler = -1;
    bool hasStraggler = (arr.size() % 2 != 0);
    
    if (hasStraggler) straggler = arr.back();
    
    for (size_t i = 0; i < arr.size() - (hasStraggler ? 1 : 0); i += 2) {
        if (arr[i] > arr[i + 1]) pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }
    
    if (!pairs.empty()) {
        mergeSortPairsDeq(pairs, 0, pairs.size() - 1);
    }
    
    std::deque<int> mainChain;
    std::deque<int> pend;
    
    for (size_t i = 0; i < pairs.size(); ++i) {
        mainChain.push_back(pairs[i].first);
        pend.push_back(pairs[i].second);
    }
    
    if (!pend.empty()) {
        mainChain.insert(mainChain.begin(), pend[0]);
    }
    
    std::vector<int> seq = generateInsertionSequence(pend.size());
    
    for (size_t i = 1; i < seq.size(); ++i) {
        int target = pend[seq[i]];
        std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), target);
        mainChain.insert(pos, target);
    }
    
    if (hasStraggler) {
        std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
        mainChain.insert(pos, straggler);
    }
    
    arr = mainChain;
}

void PmergeMe::sortAndDisplay() {
    std::cout << "Before: ";
    for (size_t i = 0; i < _vec.size(); ++i) {
        if (i == 5 && _vec.size() > 10) {
            std::cout << "[...]";
            break;
        }
        std::cout << _vec[i] << " ";
    }
    std::cout << std::endl;

    struct timeval start, end;

    gettimeofday(&start, NULL);
    fordJohnsonSort(_vec);
    gettimeofday(&end, NULL);
    double vecTime = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);
    fordJohnsonSort(_deq);
    gettimeofday(&end, NULL);
    double deqTime = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

    std::cout << "After:  ";
    for (size_t i = 0; i < _vec.size(); ++i) {
        if (i == 5 && _vec.size() > 10) {
            std::cout << "[...]";
            break;
        }
        std::cout << _vec[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Time to process a range of " << _vec.size() << " elements with std::vector : " 
              << std::fixed << std::setprecision(5) << vecTime << " us" << std::endl;
    std::cout << "Time to process a range of " << _deq.size() << " elements with std::deque  : " 
              << std::fixed << std::setprecision(5) << deqTime << " us" << std::endl;
}
