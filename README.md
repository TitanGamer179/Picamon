________________Meta 1_________________________________
lex jucompiler.l
cc lex.yy.c -o jucompiler
./jucompiler -l < Factorial.java
_______________Meta 2___________________________________
yacc -d jucompiler.y
lex jucompiler.l
cc -Wall y.tab.c lex.yy.c -o jucompiler
./jucompiler -t < Factorial.java
./jucompiler -e2 < Factorial.java
_______________Meta3___________________________________
yacc -d jucompiler.y
lex jucompiler.l
cc -o jucompiler y.tab.c lex.yy.c
./jucompiler -s < Factorial.java
./jucompiler -t < Factorial.java