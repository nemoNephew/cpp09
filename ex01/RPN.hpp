#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <string>
#include <stack>
#include <stdexcept>
#include <climits>
#include <sstream>
#include <cctype>

class RPN {
private:
    std::stack<int> _stack;

    bool isOperator(const std::string& token) const;
    void performOperation(const std::string& op);

public:
    RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& other);
    ~RPN();

    void calculate(const std::string& expression);
};

#endif
