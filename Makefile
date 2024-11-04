CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I ./tests

unit_test: unit_test_driver.o

	$(CXX) unit_test_driver.o