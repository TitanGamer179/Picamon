%token CLASS IDENTIFIER LBRACE RBRACE PUBLIC STATIC SEMICOLON
%%
Program : CLASS IDENTIFIER LBRACE ProgramElements RBRACE ;
ProgramElements : /* empty */ | ProgramElements MethodDecl | ProgramElements FieldDecl | ProgramElements SEMICOLON ;
MethodDecl : PUBLIC STATIC IDENTIFIER LBRACE RBRACE | error RBRACE ;
FieldDecl : PUBLIC STATIC IDENTIFIER SEMICOLON | error SEMICOLON ;
