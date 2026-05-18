#include "lib/RegexAST.hpp"
#include "lib/RegexDriver.hpp"

// Constructors
AlternationAST::AlternationAST(std::unique_ptr<RegexAST> left, std::unique_ptr<RegexAST> right)
    : mLeft(std::move(left)), mRight(std::move(right))
{ }
	
ConcatenationAST::ConcatenationAST(std::unique_ptr<RegexAST> left, std::unique_ptr<RegexAST> right)
    : mLeft(std::move(left)), mRight(std::move(right))
{ }
	
ZeroOrOneAST::ZeroOrOneAST(std::unique_ptr<RegexAST> operand)
    : mOperand(std::move(operand))
{ }
	
ZeroOrMoreAST::ZeroOrMoreAST(std::unique_ptr<RegexAST> operand)
    : mOperand(std::move(operand))
{ }
	
OneOrMoreAST::OneOrMoreAST(std::unique_ptr<RegexAST> operand)
    : mOperand(std::move(operand))
{ }
	
SymbolAST::SymbolAST(char symbol)
    : mSymbol(symbol)
{ }

// Compile methods
std::pair<std::map<std::pair<unsigned, char>, std::set<unsigned>>, unsigned> AlternationAST::Compile(unsigned startState) const
{
	/* l|r */
    auto [transitionMapLeft, endStateLeft] = mLeft->Compile(startState + 1);
    auto [transitionMapRight, endStateRight] = mRight->Compile(endStateLeft + 1);

	transitionMapRight.merge(transitionMapLeft);

	/* eps transitions */
    transitionMapRight[{startState, RegexDriver::epsilon}].insert(startState + 1); /* load l */
    transitionMapRight[{startState, RegexDriver::epsilon}].insert(endStateLeft + 1); /* load r */
    transitionMapRight[{endStateLeft, RegexDriver::epsilon}].insert(endStateRight + 1); /* end l */
    transitionMapRight[{endStateRight, RegexDriver::epsilon}].insert(endStateRight + 1); /* end r */

	return {transitionMapRight, endStateRight + 1};
}

std::pair<std::map<std::pair<unsigned, char>, std::set<unsigned>>, unsigned> ConcatenationAST::Compile(unsigned startState) const
{
	/* lr */
    auto [transitionMapLeft, endStateLeft] = mLeft->Compile(startState);
    auto [transitionMapRight, endStateRight] = mRight->Compile(endStateLeft);

	transitionMapRight.merge(transitionMapLeft);

	return {transitionMapRight, endStateRight};
}

std::pair<std::map<std::pair<unsigned, char>, std::set<unsigned>>, unsigned> ZeroOrOneAST::Compile(unsigned startState) const
{
	/* op? */
    auto [transitionMap, endState] = mOperand->Compile(startState + 1);

	/* eps transition */
    transitionMap[{startState, RegexDriver::epsilon}].insert(endState + 1); /* 0 */
    transitionMap[{startState, RegexDriver::epsilon}].insert(startState + 1); /* load */
    transitionMap[{endState, RegexDriver::epsilon}].insert(endState + 1); /* end */

	return {transitionMap, endState + 1};
}

std::pair<std::map<std::pair<unsigned, char>, std::set<unsigned>>, unsigned> ZeroOrMoreAST::Compile(unsigned startState) const
{
	/* op* */
    auto [transitionMap, endState] = mOperand->Compile(startState + 1);

	/* eps transition */
    transitionMap[{startState, RegexDriver::epsilon}].insert(endState + 1); /* 0 */
    transitionMap[{startState, RegexDriver::epsilon}].insert(startState + 1); /* load */
    transitionMap[{endState, RegexDriver::epsilon}].insert(startState + 1); /* more */
    transitionMap[{endState, RegexDriver::epsilon}].insert(endState + 1); /* end */

	return {transitionMap, endState + 1};
}

std::pair<std::map<std::pair<unsigned, char>, std::set<unsigned>>, unsigned> OneOrMoreAST::Compile(unsigned startState) const
{
	/* op+ */
    auto [transitionMap, endState] = mOperand->Compile(startState + 1);

	/* eps transition */
    transitionMap[{startState, RegexDriver::epsilon}].insert(startState + 1); /* load */
    transitionMap[{endState, RegexDriver::epsilon}].insert(startState + 1); /* more */
    transitionMap[{endState, RegexDriver::epsilon}].insert(endState + 1); /* end */

	return {transitionMap, endState + 1};
}

std::pair<std::map<std::pair<unsigned, char>, std::set<unsigned>>, unsigned> SymbolAST::Compile(unsigned startState) const
{
	
	std::map<std::pair<unsigned, char>, std::set<unsigned>> transitionMap;
    transitionMap[{startState, mSymbol}].insert(startState + 1);

	return {transitionMap, startState + 1};
}
