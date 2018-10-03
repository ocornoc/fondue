CPPC = g++
CPPVERSION = -std=c++17
CPPWARNINGS = -Wall -Wextra -Wpedantic -Wfatal-errors -Wvla -Wlogical-op -pedantic-errors
CPPFLAGS_NORMAL = -O2 -pthread $(CPPWARNINGS) $(CPPVERSION)
CPPFLAGS_DEBUG = -g $(CPPFLAGS_NORMAL)

src = ../../src
build = build

CPP_COMPILATION_OBJECTS = single_type.cpp multi_type.cpp

CPP_EXECUTABLE = $(build)/$(basename $(notdir $@))



compile: $(CPP_COMPILATION_OBJECTS)
	

$(CPP_COMPILATION_OBJECTS): build_folder clean
	$(CPPC) $(CPPFLAGS_DEBUG) -o $(CPP_EXECUTABLE) $@

clean:
	rm -rf $(build)

build_folder: clean
	mkdir -p $(build)



.PHONY: $(CPP_COMPILATION_OBJECTS) clean builder_folder compile
