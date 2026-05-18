#ifndef REGEX_AST_HPP
#define REGEX_AST_HPP

#include <memory>
#include <vector>
#include <map>
#include <set>

class RegexAST
{
public:
	virtual ~RegexAST() { }
    virtual std::pair<std::map<std::pair<unsigned, char>, std::set<unsigned>>, unsigned> Compile(unsigned startState) const = 0;
};

class AlternationAST : public RegexAST
{
public:
	AlternationAST(std::unique_ptr<RegexAST> left, std::unique_ptr<RegexAST> right);
    std::pair<std::map<std::pair<unsigned, char>, std::set<unsigned>>, unsigned> Compile(unsigned startState) const;
private:
    std::unique_ptr<RegexAST> mLeft, mRight;
};

class ConcatenationAST : public RegexAST
{
public:
	ConcatenationAST(std::unique_ptr<RegexAST> left, std::unique_ptr<RegexAST> right);
    std::pair<std::map<std::pair<unsigned, char>, std::set<unsigned>>, unsigned> Compile(unsigned startState) const;
private:
    std::unique_ptr<RegexAST> mLeft, mRight;
};

class ZeroOrOneAST : public RegexAST
{
public:
	ZeroOrOneAST(std::unique_ptr<RegexAST> operand);
    std::pair<std::map<std::pair<unsigned, char>, std::set<unsigned>>, unsigned> Compile(unsigned startState) const;
private:
    std::unique_ptr<RegexAST> mOperand;
};

class ZeroOrMoreAST : public RegexAST
{
public:
	ZeroOrMoreAST(std::unique_ptr<RegexAST> operand);
    std::pair<std::map<std::pair<unsigned, char>, std::set<unsigned>>, unsigned> Compile(unsigned startState) const;
private:
    std::unique_ptr<RegexAST> mOperand;
};

class OneOrMoreAST : public RegexAST
{
public:
	OneOrMoreAST(std::unique_ptr<RegexAST> operand);
    std::pair<std::map<std::pair<unsigned, char>, std::set<unsigned>>, unsigned> Compile(unsigned startState) const;
private:
    std::unique_ptr<RegexAST> mOperand;
};

class SymbolAST : public RegexAST
{
public:
	SymbolAST(char symbol);
    std::pair<std::map<std::pair<unsigned, char>, std::set<unsigned>>, unsigned> Compile(unsigned startState) const;
private:
    char mSymbol;
};

#endif // REGEX_AST_HPP
