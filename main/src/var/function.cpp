#include <Var.hpp>
#include <Computer.hpp>

using namespace		fx::computer;

VarBase*			Function::call(std::vector<VarBase*>& db, Computer& data)
{
	size_t								i = 0;
	size_t								n = this->_args.size();
	VarBase*							ret = NULL;
	std::map<std::string, VarBase*>		backup = data.scope;

	data.scope = std::map<std::string, VarBase*>();
	while (i < n)
	{
		//std::cout << this->_args[i] << " = " << db[i]->toString() << std::endl;
		data.scope[this->_args[i]] = db[i];
		i++;
	}

	if (this->_name.length() == 0)
		data.pushLocation("anonymous");
	else
		data.pushLocation(this->_name);

	ret = this->_content->execute(data);

	data.scope = backup;
	data.popLocation();
	return (ret);
}

/*
VarBase				Function::operator+(const VarBase& v) const
{
	return (this->operatorError());
	(void)v;
}

const VarBase&		Function::operator+=(const VarBase& v)
{
	return (this->operatorError());
	(void)v;
}

VarBase				Function::operator-(void) const
{
	return (this->operatorError());
}

VarBase				Function::operator-(const VarBase& v) const
{
	return (this->operatorError());
	(void)v;
}

const VarBase&		Function::operator-=(const VarBase& v)
{
	return (this->operatorError());
	(void)v;
}

VarBase				Function::operator*(const VarBase& v) const
{
	return (this->operatorError());
	(void)v;
}

const VarBase&		Function::operator*=(const VarBase& v)
{
	return (this->operatorError());
	(void)v;
}

VarBase				Function::operator/(const VarBase& v) const
{
	return (this->operatorError());
	(void)v;
}

const VarBase&		Function::operator/=(const VarBase& v)
{
	return (this->operatorError());
	(void)v;
}

VarBase				Function::operator%(const VarBase& v) const
{
	return (this->operatorError());
	(void)v;
}

const VarBase&		Function::operator%=(const VarBase& v)
{
	return (this->operatorError());
	(void)v;
}
*/