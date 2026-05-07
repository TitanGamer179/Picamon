%token LBRACE RBRACE SEMICOLON A
%%
Statement : LBRACE StatementList RBRACE | error RBRACE ;
StatementList : /* empty */ | StatementList Statement ;
