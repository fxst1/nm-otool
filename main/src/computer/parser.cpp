#include <Computer.hpp>

using namespace fx::computer;

ComputerParser::ComputerParser(void):
	Parser<fx::computer::VarBase*, fx::computer::Computer&>()
{
	this->_g_symbols.push_back( new OpenBodyToken() );
	this->_g_symbols.push_back( new CloseBodyToken() );
	this->_g_symbols.push_back( new CallDefinition() );

	//Operators
	this->_g_symbols.push_back( new OperatorPow() );
	this->_g_symbols.push_back( new OperatorMultiply() );
	this->_g_symbols.push_back( new OperatorDivide() );
	this->_g_symbols.push_back( new OperatorModulo() );
	this->_g_symbols.push_back( new OperatorPlus() );
	this->_g_symbols.push_back( new OperatorMinus() );
	this->_g_symbols.push_back( new OperatorEqual() );
	this->_g_symbols.push_back( new OpenParenthesis() );
	this->_g_symbols.push_back( new CloseParenthesis() );

	//Initializer
	this->_g_symbols.push_back( new ComputerLiteral() );
	this->_g_symbols.push_back( new ComputerDigit() );
	this->_g_symbols.push_back( new ArrayOpen() );
	this->_g_symbols.push_back( new CloseInitializeArray() );
	this->_g_symbols.push_back( new ListSeparator() );

	//Other
	this->_g_symbols.push_back( new ComputerScope() );
}