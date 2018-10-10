# The C++ standard we are compiling for.
CXXVERSION = -std=c++14
# All of the things we should make our compiler scream in agony for.
#CXXWARNINGS = -Wall -Wextra -Wpedantic -Wfatal-errors -Wvla -Wlogical-op -pedantic-errors
CXXWARNINGS = -Wall -Wextra -Wpedantic -Wvla -Wlogical-op -pedantic-errors
# The flags we should pass to our compiler.
CXXFLAGS_NORMAL = -O2 $(CXXWARNINGS) $(CXXVERSION)
# The flags we should pass to our compiler when debugging.
CXXFLAGS_DEBUG = -g -Og $(CXXWARNINGS) $(CXXVERSION)

# Relative path to the source directory of fondue.
src = ../../src
# Where to place our built executables.
build = build

# Our source files we're compiling.
CXX_COMPILATION_OBJECTS = delay.cpp
# A variable which will store the name of the associated executable given a compilation object.
CXX_EXECUTABLE = $(build)/$(basename $(notdir $@))
# The path of each executable we will compile.
CXX_ALL_EXECUTABLES = $(foreach exe,$(CXX_COMPILATION_OBJECTS),$(build)/$(basename $(notdir $(exe))))


# The default option: clean the build directory and clean its contents.
compile: build_folder $(CXX_COMPILATION_OBJECTS)
	
# Execute all of the executables in the build directory.
test: $(CXX_ALL_EXECUTABLES)
	
# Remove the build directory, including the executables in it.
clean:
	rm -rf $(build)


# Compiles an executable into build_directory given a certain source file.
$(CXX_COMPILATION_OBJECTS): build_folder
	$(CXX) $(CXXFLAGS_NORMAL) -o $(CXX_EXECUTABLE) $@

# Creates the build directory.
build_folder: clean
	mkdir -p $(build)

# Executes all of the executables in the build directory.
$(CXX_ALL_EXECUTABLES):
	./$@

.PHONY: clean build_folder compile test $(CXX_ALL_EXECUTABLES)
