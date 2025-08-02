# ----------------------------------------------
#  Makefile – UAV Simulation Project
# ----------------------------------------------
#  CXX      : C++ compiler to use
#  CXXFLAGS : compilation flags
#     -std=c++17   -> use the C++17 standard
#     -Wall        -> enable all compiler warnings
#     -Isrc        -> add “src/” to the compiler’s header-search path
#  SRC      : list of all *.cpp source files
#  OBJ      : same files, but with .o extension (automatic substitution)
#  TARGET   : final executable name
# ----------------------------------------------

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Isrc

SRC  = src/main.cpp src/SimParams.cpp src/SimCmds.cpp src/UAV.cpp src/Simulation.cpp
OBJ  = $(SRC:.cpp=.o)
TARGET = uavsim

# -------- default goal --------
# “make” or “make all” builds the executable.
all: $(TARGET)

# -------- link stage ----------
# $(OBJ) are prerequisites; $@ is the left-hand side (target name),
# $^ expands to the full list of prerequisites.
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# -------- compile stage -------
# Pattern rule: how to build any *.o from its *.cpp counterpart.
# $< is the first prerequisite (the .cpp file).
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# -------- housekeeping --------
# “make clean” removes all object files and the executable.
clean:
	rm -f $(OBJ) $(TARGET)
