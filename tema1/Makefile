CC=gcc

CFLAGS = -I. -Wall
OUTPUTNAME = market

OBJS = market.o dl_list.o vector.o
default: build

build: $(OBJS)
	$(CC) -o $(OUTPUTNAME) $(OBJS) $(CFLAGS)

debug: $(OBJS)
	$(CC) -g -o $(OUTPUTNAME) $(OBJS) $(CFLAGS)

opt: $(OBJS)
	$(CC) -O3 -o $(OUTPUTNAME) $(OBJS) $(CFLAGS)

.PHONY: clean
clean:
	-@rm *.o 2>/dev/null || true
	-@rm $(OUTPUTNAME) 2>/dev/null || true