learn:
	gcc -Wall -Werror -fsanitize=address -o learn learn.c
clean:
	rm -f learn
