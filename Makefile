# La variable CC indica el ejecutable para el compilador C (usualmente son gcc o clang)
CC=gcc
# La variable CFLAGS indica las banderas para el compilador C
CFLAGS=-Wall -Wextra -std=gnu99 -ggdb -c -g 		# el -g es para permitirle al compilador agarrar la inf. del debugger
# La variable LDLIBS indica las banderas para la parte del "loader", usualmente librerías "-lxxxx"
# LDLIBS=

# Este sería el ejecutable, sus dependencias son los .o
minish: minish.o linea2argv.o builtin_getenv.o temp.o ejecutar.o externo.o builtin_lookup.o builtin_cd.o builtin_pid.o builtin_gid.o builtin_uid.o builtin_setenv.o builtin_unsetenv.o

# Acá van los .o, make sabe que cada .o depende del .c correspondiente, acá se agrega la dependencia al .h
minish.o: minish.h
linea2argv.o: minish.h
builtin_getenv.o: minish.h
externo.o: minish.h
builtin_lookup.o: minish.h
temp.o: minish.h
builtin_cd.o: minish.h
builtin_uid.o:minish.h
builtin_pid.o: minish.h
builtin_gid.o: minish.h
builtin_setenv.o: minish.h
builtin_unsetenv.o: minish.h

clean: rm -f *.o minish