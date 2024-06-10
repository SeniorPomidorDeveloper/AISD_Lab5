COMPILLER = gcc

PATH_LIB_FILES = $(ARG)

NAME_FILE = main.c

NAME_OBJECT_FILE = main

NAME_RUN_FILE = run

all: compile_program

compile_program: assemble_lib
	# $(COMPILLER) -c -o main.o -fPIC -Wall -Wextra -g
	$(COMPILLER) *.o -L./ -l:$(NAME_OBJECT_FILE).a -o $(NAME_RUN_FILE)
	rm *.o
	rm *.a

assemble_lib: compile_lib compile_main_libs
	ar r $(NAME_OBJECT_FILE).a $(NAME_OBJECT_FILE).o *.o

compile_lib:
	$(COMPILLER) -c $(PATH_LIB_FILES)$(NAME_FILE) -o $(NAME_OBJECT_FILE).o -fPIC -Wall -Wextra -g

compile_main_libs:
	make -f $(PATH_LIB_FILES)./libs/graph/makefile ARG=$(PATH_LIB_FILES)libs/graph/ 'compile_lib'
	make -f $(PATH_LIB_FILES)./libs/interface/makefile ARG=$(PATH_LIB_FILES)libs/interface/ 'compile_lib'
	make -f $(PATH_LIB_FILES)./libs/export/makefile ARG=$(PATH_LIB_FILES)libs/export/ 'compile_lib'