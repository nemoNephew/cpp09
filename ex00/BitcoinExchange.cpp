#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) {
    *this = other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other) {
        this->_db = other._db;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::trim(std::string& str) const {
    size_t first = str.find_first_not_of(" \t");
    if (std::string::npos == first) {
        str = "";
        return;
    }
    size_t last = str.find_last_not_of(" \t");
    str = str.substr(first, (last - first + 1));
}

bool BitcoinExchange::isValidDate(const std::string& date) const {
    if (date.length() != 10 || date[4] != '-' || date[7] != '-') return false;
    
    int year, month, day;
    char dash1, dash2;
    std::istringstream ss(date);
    ss >> year >> dash1 >> month >> dash2 >> day;
    
    if (ss.fail() || !ss.eof() || month < 1 || month > 12 || day < 1 || day > 31) return false;
    
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (day > (isLeap ? 29 : 28)) return false;

    }
    return true;
}

bool BitcoinExchange::isValidValue(const std::string& valueStr, float& value) const {
    char* end;
    double tempVal = std::strtod(valueStr.c_str(), &end);
    
    if (valueStr.c_str() == end) return false;

    if (*end != '\0' && !(*end == 'f' && *(end + 1) == '\0')) return false;

    if (tempVal < 0) {
        std::cerr << "Error: not a positive number." << std::endl;
        return false;
    }
    if (tempVal > 1000) {
        std::cerr << "Error: too large a number." << std::endl;
        return false;
    }
    
    value = static_cast<float>(tempVal);
    return true;
}

void BitcoinExchange::loadDatabase(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open database file " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos) continue;

        std::string date = line.substr(0, commaPos);
        std::string rateStr = line.substr(commaPos + 1);

        char* end;
        double rate = std::strtod(rateStr.c_str(), &end);

        if (rateStr.c_str() != end && *end == '\0') {
            _db[date] = static_cast<float>(rate);
        }
    }
}

void BitcoinExchange::processInput(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t pipePos = line.find('|');
        if (pipePos == std::string::npos) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string date = line.substr(0, pipePos);
        std::string valueStr = line.substr(pipePos + 1);
        
        trim(date);
        trim(valueStr);

        if (!isValidDate(date)) {
            std::cerr << "Error: bad input => " << (date.empty() ? line : date) << std::endl;
            continue;
        }

        float value;
        if (!isValidValue(valueStr, value)) {
            if (valueStr.empty() || (value >= 0 && value <= 1000)) {
                std::cerr << "Error: bad input => " << line << std::endl;
            }
            continue;
        }

        if (_db.empty()) {
            std::cerr << "Error: database is empty." << std::endl;
            return;
        }

        std::map<std::string, float>::const_iterator it = _db.lower_bound(date);
        
        if (it == _db.end() || it->first != date) {
            if (it == _db.begin()) {
                std::cerr << "Error: no valid older date found for => " << date << std::endl;
                continue;
            }
            --it;
        }
        std::cout << date << " => " << value << " = " << (value * it->second) << std::endl;
    }
}

