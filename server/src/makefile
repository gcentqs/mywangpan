RM := rm -rf
CC := gcc
CFLAGS := -g -Wall -pthread
OUT := server
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

$(OUT):$(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) -g
%.o:%.c
	$(CC) -c $^ -o $@  
.PHONY:rebuild clean
rebuild:clean all
clean:
	$(RM) $(OBJS) $(OUT) 
