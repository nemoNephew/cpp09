#include "PmergeMe.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Error: Provide sequence of positive integers." << std::endl;
        return 1;
    }
    
    PmergeMe sorter;
    sorter.parseInput(argc, argv);
    sorter.executeSort();
    
    return 0;
}
