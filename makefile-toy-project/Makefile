CC = gcc
CFLAGS = -Wall -O -g

TARGET_DIR = ./bin/
TARGET_NAME = toy_system

DIRS = ./ ./system/ ./ui/ ./web_server/

TARGET = $(addprefix $(TARGET_DIR), $(TARGET_NAME))
SRCS = $(foreach dir, $(DIRS), $(wildcard $(dir)*.c))
OBJS = $(SRCS:.c=.o)

INC_DIR = $(foreach dir, $(DIRS), -I$(dir))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INC_DIR) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) $(INC_DIR) -o $@ -c $<

clean:
	@ rm -f $(TARGET) $(OBJS)

