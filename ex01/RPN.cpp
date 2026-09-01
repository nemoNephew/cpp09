#include "RPN.hpp"
#include <sstream>
#include <cstdlib>

RPN::RPN() {}

RPN::RPN(const RPN& other) { *this = other; }

RPN& RPN::operator=(const RPN& other) {
    if (this != &other) { this->_stack = other._stack; }
    return *this;
}

RPN::~RPN() {}

bool RPN::isOperator(const std::string& token) const {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

void RPN::performOperation(const std::string& op) {
    if (_stack.size() < 2) throw std::runtime_error("Error");
    
    int right = _stack.top(); _stack.pop();
    int left = _stack.top(); _stack.pop();
    
    if (op == "+") _stack.push(left + right);
    else if (op == "-") _stack.push(left - right);
    else if (op == "*") _stack.push(left * right);
    else if (op == "/") {
        if (right == 0) throw std::runtime_error("Error: Division by zero");
        _stack.push(left / right);
    }
}

void RPN::calculate(const std::string& expression) {
    std::istringstream iss(expression);
    std::string token;
    
    try {
        while (iss >> token) {
            if (isOperator(token)) {
                performOperation(token);
            } else {
                bool isNum = true;
                for (size_t i = 0; i < token.length(); ++i) {
                    if (!isdigit(token[i]) && !(i == 0 && token[i] == '-' && token.length() > 1)) {
                        isNum = false;
                        break;
                    }
                }
                if (!isNum || token.length() > 2) throw std::runtime_error("Error"); // Numbers must be < 10
                _stack.push(std::atoi(token.c_str()));
            }
        }
        if (_stack.size() != 1) throw std::runtime_error("Error");
        std::cout << _stack.top() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
