#!/bin/make
# Travis mandates a shebang ^

# The directory of the makefile.
MY_DIR = $(abspath .)
# The absolute paths to the direct subdirectories which contain makefiles.
# Duplicates (which there shouldn't be) are removed by sort.
TEST_MKFL_DIRS = $(sort $(abspath $(dir $(wildcard $(MY_DIR)/*/Makefile))))
# The absolute paths to the makefiles in the direct subdirectories.
TEST_MKFLS = $(foreach mkfl_dir,$(TEST_MKFL_DIRS),$(mkfl_dir)/Makefile)
# If JOBS isn't defined, set it to 2.
# This controls how many parallel tasks are ran for sub-makefiles.
JOBS ?= 2

# Tests all of the direct subdirectories' makefiles and prints all of the tested ones.
test: $(TEST_MKFL_DIRS)
	@echo Done testing.
	@echo Tested makefiles\: $(TEST_MKFLS)


# Calls make to enter the given subdirectory and to then compile and test.
$(TEST_MKFL_DIRS): $(TEST_MKFLS)
	$(MAKE) -j $(JOBS) -C $@
	$(MAKE) -j $(JOBS) -C $@ test

.PHONY: test $(TEST_MKFL_DIRS)
