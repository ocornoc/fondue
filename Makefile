CPPC = g++
CPPWARNINGS = -Wall -Wextra -Wpedantic -Wfatal-errors -Wvla -Wlogical-op -pedantic-errors
CPPFLAGS_NORMAL = -O2 -pthread $(CPPWARNINGS)
CPPFLAGS_DEBUG = -g $(CPPFLAGS_NORMAL)

src = src
build = build

CPP_COMPILATION_OBJECTS = test.cpp
CPP_OUTP = $(build)/a.out
ACCESSIBLE_CPP_OUTP = a.out

#lmao i am really struggling to remember how to make makefiles
#its been a while

compile: build_folder clean
	$(CPPC) $(CPPFLAGS_DEBUG) -o $(CPP_OUTP) $(CPP_COMPILATION_OBJECTS)
	mv $(CPP_OUTP) $(ACCESSIBLE_CPP_OUTP)

clean: build_folder
	rm -f $(CPP_OUTP) $(ACCESSIBLE_CPP_OUTP)

build_folder:
	mkdir -p $(build)
