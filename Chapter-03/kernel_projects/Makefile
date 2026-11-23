KERNEL_BUILD := /lib/modules/$(shell uname -r)/build
BUILD_DIR := /tmp/oslab_chapter3_kernel_build

MODULES := kernel_pid_info kernel_list_tasks kernel_color_list kernel_collatz

all: $(MODULES)

$(MODULES):
	mkdir -p "$(BUILD_DIR)"
	cp "$(CURDIR)/$@.c" "$(BUILD_DIR)/$@.c"
	printf 'obj-m += $@.o\n' > "$(BUILD_DIR)/Makefile"
	make -C "$(KERNEL_BUILD)" M="$(BUILD_DIR)" LLVM=1 modules
	cp "$(BUILD_DIR)/$@.ko" "$(CURDIR)/$@.ko"

clean:
	rm -rf "$(BUILD_DIR)"
	rm -f $(foreach m,$(MODULES),"$(CURDIR)/$(m).ko")

.PHONY: all clean $(MODULES)
