#!/bin/bash
yacc -d jucompiler.y
lex jucompiler.l
cc -o jucompiler y.tab.c lex.yy.c
