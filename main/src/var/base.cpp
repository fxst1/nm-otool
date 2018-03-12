#include <Var.hpp>

using namespace fx::computer;
/*	
VarBase*			VarBase::clone(const VarBase& v)
{
	if (v._type == VarType::DECIMAL)
	{
		std::cout << "CLONE DECIMAL" << std::endl;
		return (new Decimal(v));
	}
	else if (v._type == VarType::STRING)
		return (new String(v));
	std::cout << "CLONE BASE" << std::endl;
	return (new VarBase(v));
}*/

VarBase*			VarBase::clone(void) const
{
	printf("Clone base\n");
	if (this->_type == VarType::DECIMAL)
	{
		return (new VarValue(this));
	}

	return (NULL);
}

VarBase*			VarBase::doop(const VarBase& a, const VarBase& b, const std::string& op)
{
	if (op == "*")
		return (new VarValue(a * b));
	else if (op == "/")
		return (new VarValue(a * b));
	else if (op == "%")
		return (new VarValue(a * b));
	else if (op == "+")
		return (new VarValue(a * b));
	else if (op == "-")
		return (new VarValue(a * b));
	return (new VarBase());
}
