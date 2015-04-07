#define MAX 5000 
#define LENGTH 25

#define IsLetter(A) ( ((A>=0x41 && A<=0x5a) || (A>=0x61 && A<=0x7a)) ? 1 : 0)
#define IsNumber(A)  (A>=0x30 && A<=0x39)  ? 1 : 0


struct token{

	char type[LENGTH];
	char lexeme[LENGTH];
};


SpclChar(char);
update_token_list(char *);
AddSpclChar(char);
Generic_S1();
Expr_S2();
Expr_S3();
Constant();
Relop();
Arithop();
Cmp_Logic();
S_error();
scanner_end();
