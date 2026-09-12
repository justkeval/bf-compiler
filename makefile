bf: main.c compiler.c
	gcc main.c -o bf
# 	clang -std=c18 -O0 -Wall -Wextra -Wpedantic -fsanitize=address,undefined -g main.c -o bf

clean:
	rm bf

.PHONY: clean