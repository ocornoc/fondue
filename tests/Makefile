#!/bin/make
MY_DIR = $(abspath .)
TEST_MKFL_DIRS = $(sort $(abspath $(dir $(wildcard $(MY_DIR)/*/Makefile))))
TEST_MKFLS = $(foreach mkfl_dir,$(TEST_MKFL_DIRS),$(mkfl_dir)/Makefile)


test: $(TEST_MKFL_DIRS)
	@echo Done testing.
	@echo Tested makefiles\: $(TEST_MKFLS)


$(TEST_MKFL_DIRS): $(TEST_MKFLS)
	$(MAKE) -C $@
	$(MAKE) -C $@ test

.PHONY: test $(TEST_MKFL_DIRS)
