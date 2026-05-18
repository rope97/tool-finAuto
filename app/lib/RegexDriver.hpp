#ifndef REGEX_DRIVER_HPP
#define REGEX_DRIVER_HPP

#include <memory>

#include "RegexParser.tab.hpp"
#include "RegexAST.hpp"

// Class for controlling the regex lexer and parser.
class RegexDriver
{
	// We want the parser to be able to access the driver's
	// ast member field (the parser actually creates and sets m_ast).
	friend class yy::parser;
public:
    // Special character representing epsilon transitions.
    inline static const char epsilon = '$';
	// Returns the built AST for the passed regex if parsing
	// was successful, otherwise returns a null pointer.
    std::unique_ptr<RegexAST> Parse(const std::string& regex);
private:
	// These are defined in the lexer file. An alternative to these
	// would be using extern declarations for the needed functions,
	// which can get ugly.
    void StringScanInit(const std::string& regex);
    void StringScanDeinit();
	
    std::unique_ptr<RegexAST> mAST;
};

// By default, yylex's signature is int yylex(void);
// so we have to redefine it.
#define YY_DECL yy::parser::symbol_type yylex(RegexDriver& driver)
// We have to declare yylex for the parser (we could have done this
// in the parser instead, but this way it's cleaner).
YY_DECL;

#endif // REGEX_DRIVER_HPP
