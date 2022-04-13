# Compiler
CC = gcc

# Compile options
CFLAGS = -Wall -g3 -Werror 


# .o files needed
OBJS = main.o  Vector.o

# Executable
EXEC = main


$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

clean:
	@rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC)

time: $(EXEC)
	time ./$(EXEC)

valgrind: $(EXEC)
	valgrind --leak-check=full --track-origins=yes -s ./$(EXEC)