#! /bin/zsh
gcc -m32 -O2 -Wall -Werror -std=gnu11 -fPIC -DPIC -c dynamic.c
gcc -m32 -std=gnu11 -fPIC -shared dynamic.o -olibdynamic.so
rm dynamic.o
gcc -m32 -O2 -std=gnu18 -Wall -Werror -Wno-pointer-sign -Werror=vla -lm -o 4 4.c