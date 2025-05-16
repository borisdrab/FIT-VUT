CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic

# Target executables
TARGET_PROJ2 = proj2  

# Default rule (build both programs)
all: $(TARGET_PROJ2) $(TARGET_PREP)

# Rule to build proj2 executable
$(TARGET_PROJ2): proj2.c  
	$(CC) $(CFLAGS) -o $(TARGET_PROJ2) proj2.c

# Clean rule to remove generated files
clean:
	rm -f $(TARGET_PROJ2) $(TARGET_PREP)
