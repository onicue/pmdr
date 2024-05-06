SRCS = src/main.c src/utils.c src/server.c src/client.c

OBJS = $(SRCS:.c=.o)

TARGET = pmdr

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -pthread $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) -pthread $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
