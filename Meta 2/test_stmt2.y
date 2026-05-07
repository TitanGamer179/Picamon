%token LBRACE RBRACE SEMICOLON A
%%
Block : LBRACE StatementList RBRACE ;
StatementList : /* empty */ | StatementList Statement | StatementList error RBRACE ;
Statement : A SEMICOLON | error SEMICOLON ;
