OBJECTS  = pendu.c
OBJECTS += logic.c
OBJECTS += words.c

INC = include/

program: 
	gcc -I$(INC) $(OBJECTS) -o $@

clean:
	-rm -f program