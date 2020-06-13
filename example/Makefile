.PHONY: all format tidy cpplint clean run sources

STD=-std=c++11
CXXFLAGS = -g -O0 -I.

USE_GCC=

ifeq ($(USE_GCC),)
CXX = clang++
LIBS = -lc++
else
CXX = g++
LIBS = -lstdc++
endif

FORMAT  = clang-format
TIDY    = clang-tidy
CPPLINT = cpplint

SRCS = \
	chipset_a_driver.cc \
	chipset_b_driver.cc \
	chipset_c_driver.cc \
	chipset_driver.cc \
	linecard.cc \
	network_driver.cc

INCS = \
	chipset_driver.h \
	network_driver.h \
	simple_init_chain.h



all: linecard

linecard: $(SRCS) $(INCS) Makefile
	$(CXX) -o $@ $(CXXFLAGS) $(SRCS) $(LIBS)

format: $(SRCS) $(INCS) Makefile
	$(FORMAT) --style=google -i $(SRCS) $(INCS)
	cd unit_test; $(MAKE) format
	cd ex_unit_test; $(MAKE) format

tidy: $(SRCS) $(INCS) Makefile
	$(TIDY) --fix -extra-arg-before=-xc++ $(SRCS) $(INCS) -- $(CXXFLAGS) $(STD)

cpplint: $(SRCS) $(INCS) Makefile
	$(CPPLINT) $(SRCS) $(INCS)
	cd unit_test; $(MAKE) cpplint
	cd ex_unit_test; $(MAKE) cpplint

# We need a local copy of simiple_init_chain.h
simple_init_chain.h:
	rm -rf simple_init_chain
	git clone https://github.com/alex4747-pub/simple_init_chain.git
	cp simple_init_chain/include/simple_init_chain.h .
	rm -rf simple_init_chain

clean:
	rm -rf linecard *~ *.dSYM simple_init_chain.h simple_init_chain

run: linecard
	./linecard

sources: simple_init_chain.h
