.PHONY: all linecard format cpplint clean run

SRC_DIR = ..

STD=-std=c++11
CXXFLAGS = -g -O0 -I. -I$(SRC_DIR) -pthread

USE_GCC=

ifeq ($(USE_GCC),)
CXX = clang++
LIBS = -lc++
else
CXX = g++
LIBS = -lstdc++
endif

LIBS += -lgtest -lpthread


FORMAT  = clang-format
CPPLINT = cpplint

SRCS = \
	$(SRC_DIR)/chipset_a_driver.cc \
	$(SRC_DIR)/chipset_b_driver.cc \
	$(SRC_DIR)/chipset_c_driver.cc \
	$(SRC_DIR)/chipset_driver.cc \
	$(SRC_DIR)/network_driver.cc \
	mock_chipset_driver_a.cc \
	mock_chipset_driver_b.cc \
	test_cases.cc \
	test_main.cc

INCS = \
	$(SRC_DIR)/chipset_driver.h \
	$(SRC_DIR)/network_driver.h \
	$(SRC_DIR)/simple_init_chain.h \
	mock_chipset_driver_a.h \
	mock_chipset_driver_b.h \
	mock_chipset_driver_cfg.h

all: test_runner

test_runner: $(SRCS) $(INCS) Makefile
	$(CXX) -o $@ $(CXXFLAGS) $(SRCS) $(LIBS)


format: $(SRCS) $(INCS) Makefile
	$(FORMAT) --style=google -i $(SRCS) $(INCS)

cpplint: $(SRCS) $(INCS) Makefile
	$(CPPLINT) $(SRCS) $(INCS)

# We need a local copy of simiple_init_chain.h
$(SRC_DIR)/simple_init_chain.h:
	cd ..; $(MAKE) sources

clean:
	rm -rf test_runner *~ *.dSYM test_runner

run: test_runner
	./test_runner


