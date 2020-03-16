
CXXFLAGS = -std=c++11 -W -Wall -Ofast -fno-exceptions -fno-rtti -march=native -I../dsp
CXX = clang++ -stdlib=libc++
#CXX = g++

.PHONY: all

all: cfo sfo awgn multipath

cfo: cfo.cc
	$(CXX) $(CXXFLAGS) $< -o $@

sfo: sfo.cc
	$(CXX) $(CXXFLAGS) $< -o $@

awgn: awgn.cc
	$(CXX) $(CXXFLAGS) $< -o $@

multipath: multipath.cc
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f cfo sfo awgn multipath

