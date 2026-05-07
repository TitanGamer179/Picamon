%token CLASS IDENTIFIER LBRACE RBRACE PUBLIC STATIC SEMICOLON
%%
Program : CLASS IDENTIFIER LBRACE ProgramElements RBRACE ;
ProgramElements : /* empty */ | ProgramElements MethodDecl | ProgramElements FieldDecl | ProgramElements SEMICOLON | ProgramElements error RBRACE ;
MethodDecl : PUBLIC STATIC IDENTIFIER LBRACE RBRACE ;
FieldDecl : PUBLIC STATIC IDENTIFIER SEMICOLON | error SEMICOLON ;
