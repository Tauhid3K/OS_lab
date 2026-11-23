CC ?= gcc   
CFLAGS ?= -Wall -Wextra -O2
LDFLAGS ?= -lrt -lpthread

SRCS := $(wildcard *.c)
TARGETS := $(SRCS:.c=)

all: $(TARGETS) java

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

java:
	@if command -v javac >/dev/null 2>&1; then \
		echo "Compiling Java socket programs..."; \
		javac java_sockets/*.java; \
	fi

clean:
	rm -f $(TARGETS)
	rm -f java_sockets/*.class

.PHONY: all clean java
