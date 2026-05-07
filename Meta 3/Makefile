jucompiler: jucompiler.l jucompiler.y
	bison -y -d jucompiler.y
	flex jucompiler.l
	gcc -o jucompiler lex.yy.c y.tab.c -lfl -lm

clean:
	rm -f jucompiler lex.yy.c y.tab.c y.tab.h
