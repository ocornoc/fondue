CXXVERSION = -std=c++14
CXXWARNINGS = -Wall -Wextra -Wpedantic -Wfatal-errors -Wvla -Wlogical-op -pedantic-errors
CXXFLAGS_NORMAL = -O2 -pthread $(CXXWARNINGS) $(CXXVERSION)
CXXFLAGS_DEBUG = -g $(CXXFLAGS_NORMAL)

src = ../../src
build = build

CXX_COMPILATION_OBJECTS = single_type.cpp multi_type.cpp
CXX_EXECUTABLE = $(build)/$(basename $(notdir $@))
CXX_ALL_EXECUTABLES = $(foreach exe,$(CXX_COMPILATION_OBJECTS),$(build)/$(basename $(notdir $(exe))))



compile: $(CXX_COMPILATION_OBJECTS)
	
test: $(CXX_ALL_EXECUTABLES)
	
clean:
	rm -rf $(build)



$(CXX_COMPILATION_OBJECTS): build_folder clean
	$(CXX) $(CXXFLAGS_NORMAL) -o $(CXX_EXECUTABLE) $@

build_folder: clean
	mkdir -p $(build)

$(CXX_ALL_EXECUTABLES):
	./$@

.PHONY: clean build_folder compile test $(CXX_ALL_EXECUTABLES)
