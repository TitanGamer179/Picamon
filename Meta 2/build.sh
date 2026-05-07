#!/bin/bash
bison -dv -o y_new.tab.c jucompiler.y && \
cp y_new.tab.c y.tab.c && cp y_new.tab.h y.tab.h && \
gcc -o jucompiler lex.yy.c y.tab.c -ll && echo "BUILD OK"
