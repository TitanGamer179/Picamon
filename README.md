lex jucompiler.l
cc lex.yy.c -o jucompiler
./jucompiler -l < Factorial.java