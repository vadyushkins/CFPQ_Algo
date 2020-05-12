GRAPHBLAS=deps/GraphBLAS/build/libgraphblas.a

SOURCEDIR=$(shell pwd -P)
CC_SOURCES = $(wildcard $(SOURCEDIR)/*.c)
CC_SOURCES += $(wildcard $(SOURCEDIR)/cfpq_algorithms/*.c)
CC_SOURCES += $(wildcard $(SOURCEDIR)/interpreter/*.c)
CC_SOURCES += $(wildcard $(SOURCEDIR)/grammar/*.c)
CC_SOURCES += $(wildcard $(SOURCEDIR)/graph/*.c)
CC_SOURCES += $(wildcard $(SOURCEDIR)/response/*.c)
CC_SOURCES += $(wildcard $(SOURCEDIR)/mapper/*.c)
CC_SOURCES += $(wildcard $(SOURCEDIR)/timer/*.c)

all: $(GRAPHBLAS) $(CC_SOURCES)
	gcc -o main ${CC_SOURCES} -fopenmp $(GRAPHBLAS) -lm

clean:
	rm ./main

$(GRAPHBLAS):
ifeq (,$(wildcard $(GRAPHBLAS)))
	@$(MAKE) -C deps/GraphBLAS CMAKE_OPTIONS="-DCMAKE_C_COMPILER='gcc' -DCMAKE_CXX_COMPILER='g++'" static_only
endif
.PHONY: $(GRAPHBLAS)