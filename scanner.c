#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"scanner.h"


struct token token_list[MAX];
char buf,lex[LENGTH],stack;
int fd,fd2,j,list_index=0,line_count=1;


keyword(char *temp){

	if( (strcmp(temp,"for"))==0 || (strcmp(temp,"while"))==0 ){
		strcpy(token_list[list_index-1].type,"LOOP");
	}else

	if( (strcmp(temp,"char"))==0 || (strcmp(temp,"short"))==0 || (strcmp(temp,"int"))==0){
		strcpy(token_list[list_index-1].type,"DEFINITION");
	
	}else

	if( (strcmp(temp,"if"))==0 || (strcmp(temp,"switch"))==0 ){
		strcpy(token_list[list_index-1].type,"CONDITIONAL");
		
	}else
	if( (strcmp(temp,"else"))==0 ){
		strcpy(token_list[list_index-1].type,"ENDIF");
		
	}
	
}




SpclChar(char temp){

	if(temp==';' || temp=='<' || temp=='>' || temp=='!'
			|| temp=='=' || temp=='|' || temp=='&'
			|| temp=='(' || temp==')' || temp=='{'
			|| temp=='}' || temp==':' || temp=='#'
			|| temp=='%' || temp=='&' || temp=='*'
			|| temp=='/' || temp=='-' || temp=='+'
			|| temp==',' || temp=='\''|| temp==' '
			|| temp=='\t'|| temp=='\n'|| temp=='['
			|| temp==']'){
		return 1;
	}else{
	 return 0;
	 }
}


update_token_list(char *temp){

	lex[j]='\0';
	strcpy(token_list[list_index].lexeme,lex);
	strcpy(token_list[list_index++].type,temp);
	j=0;
}


AddSpclTok(char temp){

	switch(temp){
	//delimiters
		case'\n':
			buf=NULL;
			line_count++;
			Generic_S1();
			break;
		case'\t':
			buf=NULL;
			Generic_S1();
			break;
		case' ':
			buf=NULL;
			Generic_S1();
			break;
			
	//if punctuation save it and start over
		case';':
			lex[j++]=temp;
			update_token_list("SEMICOLON");
			buf=NULL;
			Generic_S1();
			break;
		case'(':
                        lex[j++]=temp;
                        update_token_list("LEFTPAREN");
                        buf=NULL;
                        Generic_S1();
                        break;
		case')':
                        lex[j++]=temp;
                        update_token_list("RIGHTPAREN");
                        buf=NULL;
                        Generic_S1();
                        break;
		case'{':
                        lex[j++]=temp;
                        update_token_list("BLOCKBEGIN");
                        buf=NULL;
                        Generic_S1();
                        break;
		case'}':
                        lex[j++]=temp;
                        update_token_list("BLOCKEND");
                        buf=NULL;
                        Generic_S1();
                        break;
		case'[':
                        lex[j++]=temp;
                        update_token_list("L_SQR_BRACKET");
                        buf=NULL;
                        Generic_S1();
                        break;
		case']':
                        lex[j++]=temp;
                        update_token_list("R_SQR_BRACKET");
                        buf=NULL;
                        Generic_S1();
                        break;
	//Possible Relop
		case'=':
			stack=temp;
			Relop();
			break;
		case'<':
			stack=temp;
			Relop();
			break;
		case'>':
			stack=temp;
			Relop();
			break;
		case'!':
			stack=temp;
			Relop();
			break;
	//Possible bitwise / logical comparison
		case'|':
			stack=temp;
			Cmp_Logic();
			break;
               	case'&':
			stack=temp;
			Cmp_Logic();
			break;
	//Possible Arithop
		case'+':
			stack=temp;
			Arithop();
			break;
		case'-':
			stack=temp;
			Arithop();
			break;
		case'*':
			stack=temp;
			Arithop();
			break;
		case'/':
			stack=temp;
			Arithop();
			break;
	//Unrecognized Token
		default:
			S_error();
			break;
	}

}


Generic_S1(){
		
	if(buf==NULL){
		if((read(fd,&buf,1))==-1){
			printf("error reading from file in Generic_S1\n");
		}
	}

	if(buf==NULL){
		scanner_end();
	}

	if(IsLetter(buf)){
		Expr_S2();
	}else
	if(IsNumber(buf)){
		Expr_S3();
	}else
	if(SpclChar(buf)){
		AddSpclTok(buf);
	}else
	 S_error();
}


Expr_S2(){

	if(buf==NULL){
		if((read(fd,&buf,1)) ==-1){
			printf("error reading from file in Expr_S2\n");
		}
	}

	if(buf==NULL)scanner_end();

	if(IsLetter(buf)){
		lex[j++]=buf;
		if((read(fd,&buf,1))==-1) printf("error reading char in Expr_S2\n");
                Expr_S2();
        }else
        if(SpclChar(buf) ){
		Identifier();
        }else
	
	if(IsNumber(buf)){
		Identifier();
	}else
	 S_error();
        
}


Expr_S3(){

	if(buf==NULL){
		if(read(fd,&buf,1)==-1){
			printf("error reading from file in Expr_S3\n");
		}
	}

	if(buf==NULL)scanner_end();
	
	if(IsNumber(buf)){
		lex[j++]=buf;
		if(read(fd,&buf,1)==-1) printf("error reading char in Expr_S3\n");
		Expr_S3();
	}
	
	if(SpclChar(buf) || IsLetter(buf)){
		Constant();
	}else
	 S_error();
	
}


Identifier(){
	
	update_token_list("ID");
	keyword(token_list[list_index-1].lexeme);
	Generic_S1();

}


Constant(){
	
	update_token_list("CONST");
	Generic_S1();
}


Cmp_Logic(){
	if((read(fd,&buf,1))!=0){
		lex[j++]=stack;
		if(IsLetter(buf)){
			if(stack=='&'){
				update_token_list("BITWISE");
				Generic_S1();
			}else
			if(stack=='|'){
				update_token_list("BITWISE");
				Generic_S1();
			}
		}else
		if(stack=='&'){
			if(buf=='&'){
				lex[j++]=buf;
				update_token_list("CMP");
				buf=NULL;
				Generic_S1();
			}
		}else
		if(stack=='|'){
			if(buf=='|'){
				lex[j++]=buf;
				update_token_list("CMP");
				buf==NULL;
				Generic_S1();
			}
		}else S_error();
	}
}


Relop(){
	if((read(fd,&buf,1))!=0){
		lex[j++]=stack;
		if(IsLetter(buf) || IsNumber(buf)){
			if(stack=='='){
				update_token_list("ASSIGN");
				Generic_S1();
			}else
			if(stack=='<'){
				update_token_list("RELOP");
				Generic_S1();
			}else
			if(stack=='>'){
				update_token_list("RELOP");
				Generic_S1();
			}else S_error;
		}else
		if(stack=='='){
			if(buf=='='){
				lex[j++]=buf;
				update_token_list("RELOP");
				buf=NULL;
				Generic_S1();
				
			}else S_error();
		}else
		if(stack=='<'){
			if(buf=='='){
				lex[j++]=buf;
				update_token_list("RELOP");
				buf=NULL;
				Generic_S1();
			}else S_error();
		}else
		if(stack=='>'){
			if(buf=='='){
				lex[j++]=buf;
				update_token_list("RELOP");
				buf=NULL;
				Generic_S1();
			}else S_error();
		}else
		if(stack=='!'){
			if(buf=='='){
				lex[j++]=buf;
				update_token_list("RELOP");
				buf=NULL;
				Generic_S1();
			}else S_error();
		}else S_error();

	}
	
}


Arithop(){
	if((read(fd,&buf,1))!=0){
		lex[j++]=stack;
		if(IsLetter(buf) || IsNumber(buf)){
			if(stack=='+'){
				update_token_list("Arithop");
				Generic_S1();
			}else
			if(stack=='-'){
				update_token_list("Arithop");
				Generic_S1();
			}else
			if(stack=='*'){
				update_token_list("Arithop");
				Generic_S1();
			}else
			if(stack=='/'){
				update_token_list("Arithop");
				Generic_S1();
			}else S_error();

		}else S_error();
	}
}


S_error(){
	int i=0;
	printf("error:in line %d unexpected '%c' token near '%s' token\n"
				,line_count,buf,token_list[list_index-1].lexeme);
	printf("recognized tokens: ");
	for(i=0;i<list_index;i++)
		printf("<%s,%s>\n",token_list[i].lexeme,token_list[i].type);
	_exit(0);
}


scanner_end(){
	char buffer[MAX]="<html>\n <head>\n  <title>Scanner</title>"
			"\n <link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\">\n </head>"
			"\n <body>\n  <h1>CLexer</h1>\n  <table width=30%>"
			"\n   <tr><td>TOKEN</td><td>TYPE</td></tr>";
	int fd2,n,temp;
		
	if((fd2=creat("token.html",S_IRWXU))==-1){
		printf("error creating token.html\n");
		_exit(0);
	}
	
	for(temp=0;temp<list_index;temp++){
		
		strcat(buffer,"\n   <tr><td>");
		strcat(buffer,token_list[temp].lexeme);
		strcat(buffer,"</td><td>");
		strcat(buffer,token_list[temp].type);
		strcat(buffer,"</td></tr>");
	
	}
	
	strcat(buffer,"\n  </table>\n </body>\n</html>");
	
	write(fd2,buffer,strlen(buffer));

	_exit(0);
}


main(int argc,char **argv){

	if((fd = open(argv[1],0))==-1){
		printf("error opening file\n");
		_exit(0);
	}
	
	j=0;

	buf=NULL;
	Generic_S1();

}
