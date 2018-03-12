#include <Var.hpp>

using namespace fx::computer;

int					String::toInteger()
{
	return (atoi(this->_data.c_str()));
}

void				String::convert(const VarBase& v, const lookup_t* lookup)
{
	size_t			i = 0;

	if (v._type == VarType::STRING)
		return ;
	else if (v._type == VarType::DECIMAL)
	{
		VarValue&		tmp = (VarValue&)v;

		this->_data = std::to_string(tmp._re);
		if (tmp._im > 0)
			this->_data += (" " + std::to_string(tmp._im) + "i");
	}
	else if (v._type == VarType::ARRAY)
	{
		Array&			tmp = (Array&)v;

		while (i < tmp._data.size())
		{
			String		s = String(tmp._data[i]);

			if (this->_data == "")
				this->_data += s._data;
			else
				this->_data += (", " + s._data);
			i++;
		}
	}
	else if (v._type == VarType::POLYNOME)
	{
		Polynome&			tmp = (Polynome&)v;

		while (i < tmp._data.size())
		{
			String		s = String(tmp._data[i]);

			if (this->_data != "")
				this->_data += " ";
			this->_data += "(" + s._data + ")x**";
			this->_data += std::to_string(tmp._data.size() - i - 1);
			i++;
		}
	}
	else if (v._type == VarType::FUNCTION || v._type == VarType::OBJECT)
		this->_data = this->toString();
	else
		throw (ComputerFatalException(this->toString() + ": Cannot cast to " + v.typeName()));
	(void)lookup;
}

void				String::convert(unsigned int type)
{
	(void)type;
}

VarBase*				String::operator+(const VarBase& v) const
{
	String		*tmp = new String(this);

	tmp->append(v);
	return (tmp);
}

const VarBase&		String::operator+=(const VarBase& v)
{
	this->append(v);
	return (*this);
}

const VarBase&		String::operator=(const VarBase& v)
{
	return (v);
}

bool 				String::operator==(const VarBase& v) const
{
	return (v == *this);
}

void				String::append(const VarBase& s)
{
	String			tmp = String(s);

	this->convert(tmp);
	this->_data += tmp._data;
}