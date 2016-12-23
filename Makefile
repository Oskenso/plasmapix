CC = clang
CC_FLAGS = -Wall -std=c11 -O2 -march=native

EXEC = plasmapix
#-lpng
LDFLAGS = -lm

$(EXEC): plasmapix.c
	$(CC) $(CC_FLAGS) plasmapix.c -o $(EXEC) $(LDFLAGS)


.PHONY: clean
clean:
	rm -f $(OBJECTS)
