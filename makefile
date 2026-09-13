OUT = bf

build:
	gcc -I. -std=c18 -O0 -Wall -Wextra -Wpedantic -fsanitize=address,undefined -g main.c -o $(OUT)

build-release:
	gcc -I. -O3 main.c -o $(OUT)

clean:
	rm bf

.PHONY: clean build build-debug