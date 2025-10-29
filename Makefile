CC = gcc
CFLAGS = -Wall -Iinclude -I/opt/homebrew/include # Chemin pour SDL macos, à enlever quand on testera sur linux.
LDFLAGS = -L/opt/homebrew/lib -lSDL3 -lSDL3_image

SRC = src/main.c \
	  src/displayer.c \
	  src/button.c

OBJ = $(SRC:src/%.c=build/%.o)
EXEC = build/programme

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*.o $(EXEC)

run:
	./$(EXEC)

