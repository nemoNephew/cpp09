src = BitcoinExchange.cpp main.cpp 
obj = $(src:.cpp=.o)
hdr = BitcoinExchange.hpp
name = BitcoinExchange
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

all: $(name)
$(name) : $(obj) $(hdr)
	$(CXX) $(CXXFLAGS) -o $(name) $(obj)

clean:
	rm -f $(obj)
fclean: clean
	rm -f $(name)

re: fclean all
.PHONY: all re clean fclean
.SECONDARY: $(obj)