#include "lib/RegexDriver.hpp"

std::unique_ptr<RegexAST> RegexDriver::Parse(const std::string& regex)
{
	StringScanInit(regex);
	yy::parser parser(*this);
	int res = parser();
	StringScanDeinit();
	if (res != 0)
		return nullptr;
	return std::move(mAST);
}
