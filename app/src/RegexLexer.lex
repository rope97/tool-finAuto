%option noyywrap
%option nounput
%option noinput

%{

#include "lib/RegexDriver.hpp"
#include "RegexParser.tab.hpp"

%}

%%

\\[|?*+()] {
    return yy::parser::make_SYM_T(yytext[1]);
}

[|?*+()] {
	return *yytext;
}

[\t\n] { }

. {
	return yy::parser::make_SYM_T(*yytext);
}

%%

void RegexDriver::StringScanInit(const std::string& regex)
{
	yy_scan_string(regex.c_str());
}

void RegexDriver::StringScanDeinit()
{
	yy_delete_buffer(YY_CURRENT_BUFFER);
}
