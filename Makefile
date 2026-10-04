CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

DLL = PileDLL.dll
LIB = libPileDLL.dll.a
TEST = test.exe

all: $(DLL) $(TEST)

$(DLL): PileDLL.cpp PileDLL.h PileTableau.h PilePointeur.h
	$(CXX) $(CXXFLAGS) -shared -DPILEDLL_EXPORTS PileDLL.cpp -o $(DLL) -Wl,--out-implib,$(LIB)

$(TEST): test.cpp PileDLL.h $(LIB)
	$(CXX) $(CXXFLAGS) test.cpp -L. -lPileDLL -o $(TEST)

clean:
	rm -f $(DLL) $(LIB) $(TEST) *.o
