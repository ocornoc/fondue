#!/bin/make
MY_DIR = $(abspath .)
TEST_MKFL_DIRS := $(sort $(abspath $(dir $(wildcard $(MY_DIR)/*/Makefile))))
TEST_MKFLS := $(foreach mkfl_dir,$(TEST_MKFL_DIRS),$(mkfl_dir)/Makefile)

TEST_FUNCTIONS := $(foreach mkfl_dir,$(TEST_MKFL_DIRS),test_$(mkfl_dir))

test: $(TEST_FUNCTIONS)
	@echo Done testing.
	@echo Tested makefiles\: $(TEST_MKFLS)

GET_PATH_FROM_TEST_FUNCTION = $(subst test_,,$@)

$(TEST_FUNCTIONS): $(TEST_MKFL_DIRS) $(TEST_MKFLS)
	$(MAKE) -C $(GET_PATH_FROM_TEST_FUNCTION)
	$(MAKE) -C $(GET_PATH_FROM_TEST_FUNCTION) test

.PHONY: test $(TEST_FUNCTIONS)
