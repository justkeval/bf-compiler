OUT = bf

build:
	gcc -std=c18 -O0 -Wall -Wextra -Wpedantic -fsanitize=address,undefined -g main.c -o $(OUT)

build-release:
	gcc -O3 main.c -o $(OUT)

clean:
	rm bf

.PHONY: clean build build-debug