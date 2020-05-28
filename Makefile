.PHONY: clean

quicksort: main.c
	cc main.c -o mmints

clean:
	rm mmints
