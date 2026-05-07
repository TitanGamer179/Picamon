%token A B C SEMICOLON
%%
List : List Stmt | List Var | ;
Stmt : A SEMICOLON | error SEMICOLON ;
Var : B SEMICOLON | error SEMICOLON ;
