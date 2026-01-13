/*
 * Parser interpretera maszyny wirtualnej do projektu z JFTT2025
 *
 * Autor: Maciek Gębala
 * http://ki.pwr.edu.pl/gebala/
 * 2025-11-15
 * 
 * Modified by Adam Kostrzewski
*/
%code requires { 
#include<vector> 
#include<utility>
#include "instructions.hpp"

#define YYSTYPE var_t
}
%{
#include <iostream>
#include <utility>
#include <vector>
#include <print>

#include "instructions.hpp"
#include "colors.hpp"

extern int yylineno;
int yylex();
void yyset_in(FILE* in_str);
void yyerror(std::vector<std::pair<int,var_t>>& program, char const *s);

%}
%parse-param { std::vector<std::pair<int,var_t>>& program }
%token COM_0
%token COM_1
%token JUMP_0
%token JUMP_1
%token STOP
%token REG
%token NUMBER
%token ERROR
%%
input 
	: input line
	| %empty
	;

line 
	: COM_0	  		{ program.push_back(std::make_pair($1,0));   }
	| COM_1 REG	  	{ program.push_back(std::make_pair($1,$2));  }
	| COM_1 NUMBER  { program.push_back(std::make_pair($1,$2));  }
	| JUMP_0        { program.push_back(std::make_pair($1,0));   }
	| JUMP_1 NUMBER { program.push_back(std::make_pair($1,$2));  }
	| STOP          { program.push_back(std::make_pair($1,0));   }
	| ERROR         { yyerror(program, "Symbol not recognised"); }
	;
%%

void yyerror(std::vector<std::pair<int, var_t>>& program, char const *s)
{
	std::println(std::cerr, "{}Line {}: {}{}", cRed, yylineno, s, cReset);
	std::exit(-1);
}

void run_parser(std::vector<std::pair<int,var_t>>& program, FILE* data ) 
{
	std::println("{}Reading the code{}", cBlue, cReset);
	yyset_in(data);
	yyparse(program);
	std::println("{}Finished reading the code (instructions: {}){}", cBlue, program.size(), cReset);
}

