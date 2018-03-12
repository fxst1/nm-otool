#include <Var.hpp>

using namespace	fx::computer;

void			Array::convert(const VarBase& v, const lookup_t* lookup)
{
	if (v._type == VarType::ARRAY)
	{
		Array&	tmp = (Array&)v;

		this->_data = tmp._data;
		this->_length = tmp._length;
		this->_is_matrix = tmp._is_matrix;
	}
	else if (v._type == VarType::STRING)
	{
		this->_data = std::vector<VarBase*>();
		this->_data.push_back( new String(v, lookup) );
		this->_length = 1;
		this->_is_matrix = false;
	}
	else if (v._type == VarType::FUNCTION || v._type == VarType::OBJECT)
	{
		size_t		i = 0;

		if (!lookup)
			throw (ComputerFatalException(this->toString() + ": Cannot cast to Array"));

		this->_length = lookup->location.size();
		while (i < this->_length)
		{
			this->_data.push_back( new String(lookup->location[i], lookup) );
			i++;
		}
	}
	else if (v._type == VarType::POLYNOME)
	{
		Polynome&	tmp = (Polynome&)v;
		size_t		i = 0;

		this->_length = tmp._degree;
		while (i < this->_length)
		{
			this->_data.push_back( new VarValue(tmp._data[i], lookup) );
			i++;
		}
	}
	else
		throw (ComputerFatalException(this->toString() + ": Cannot cast to " + v.typeName()));
}

std::string		Array::typeName(void)
{
	return ("Array");
}

VarBase*			Array::operator+(const VarBase& v) const
{
	Array		*tmp = new Array(v);
	VarBase		*add = NULL;

	if (v._type == VarType::DECIMAL)
		add = new VarValue(v);
	else if (v._type == VarType::FUNCTION)
		add = new Function(v);
	else if (v._type == VarType::OBJECT)
		add = new Object(v);
	else if (v._type == VarType::STRING)
		add = new String(v);

	if (add)
	{
		tmp->_length++;
		tmp->_data.push_back( add );		
	}
	else if (v._type == VarType::ARRAY)
	{
		size_t	i = 0;

		while (i < this->_length)
		{
			tmp->_data.insert( tmp->_data.end(), this->_data[i] );
			i++;
		}
		tmp->_length += this->_data.size();
	}

	return (tmp);
}

const VarBase&	Array::operator+=(const VarBase& v)
{
	Array&		tmp = (Array&)v;
	VarBase		*add = NULL;

	if (v._type == VarType::DECIMAL)
		add = new VarValue(v);
	else if (v._type == VarType::FUNCTION)
		add = new Function(v);
	else if (v._type == VarType::OBJECT)
		add = new Object(v);
	else if (v._type == VarType::STRING)
		add = new String(v);

	if (add)
	{
		this->_length++;
		this->_data.push_back( add );
	}
	else if (v._type == VarType::ARRAY)
	{
		size_t	i = 0;

		while (i < this->_length)
		{
			this->_data.insert( this->_data.end(), tmp._data[i] );
			i++;
		}
		this->_length += tmp._data.size();
	}

	return (*this);
}

VarBase*			Array::operator-(void) const
{
	this->operatorError('-');
	return (NULL);
}

VarBase*			Array::operator-(const VarBase& v) const
{
	Array		*tmp = new Array(*this);

	if (v._type == VarType::DECIMAL ||
		v._type == VarType::FUNCTION ||
		v._type == VarType::OBJECT ||
		v._type == VarType::STRING)
	{
		for (auto it = this->_data.begin();
			it != this->_data.end(); it++)
			if (*it == &v)
			{
				tmp->_data.erase( it );
				break ;
			}

		tmp->_length = this->_data.size();
	}
	else if (v._type == VarType::ARRAY)
	{
		for (auto it = this->_data.end();
					it != this->_data.begin(); it--)
					if (*it == &v)
						tmp->_data.erase( it );

		tmp->_length = tmp->_data.size();
	}

	return (tmp);
}

const VarBase&	Array::operator-=(const VarBase& v)
{
	if (v._type == VarType::DECIMAL ||
		v._type == VarType::FUNCTION ||
		v._type == VarType::OBJECT ||
		v._type == VarType::STRING)
	{
		for (std::vector<VarBase*>::iterator it = this->_data.begin();
			it != this->_data.end(); it++)
			if (*it == &v)
			{
				this->_data.erase( it );
				break ;
			}
		
		this->_length = this->_data.size();
	}
	else if (v._type == VarType::ARRAY)
	{
		for (std::vector<VarBase*>::iterator it = this->_data.end();
			it != this->_data.begin(); it--)
			if (*it == &v)
				this->_data.erase( it );

		this->_length = this->_data.size();
	}

	return (*this);
}

VarBase*			Array::operator*(const VarBase& v) const
{
	Array		*tmp = new Array(*this);

	if (v._type == VarType::DECIMAL ||
		v._type == VarType::STRING)
	{
		int				d = v.toInteger();
		auto			collec = this->_data;

		while (--d)
			for (auto it = collec.begin(); it != collec.end(); it++)
				tmp->_data.push_back(*it);
	}
	else
		throw (ComputerFatalException(this->toString() + ": Divider should be a convertible to Decimal"));
	return (tmp);
}

const VarBase&		Array::operator*=(const VarBase& v)
{
	if (v._type == VarType::DECIMAL ||
		v._type == VarType::STRING)
	{
		int			d = v.toInteger();
		auto		collec = this->_data;

		while (--d)
			for (auto it = collec.begin(); it != collec.end(); it++)
				this->_data.push_back(*it);
	}
	else
		throw (ComputerFatalException(this->toString() + ": Divider should be a convertible to Decimal"));
	return (*this);
}


VarBase*			Array::operator/(const VarBase& v) const
{
	Array		*tmp = new Array(v);

	if (v._type == VarType::DECIMAL ||
		v._type == VarType::STRING)
	{
		tmp->split(v.toInteger());
	}
	else
		throw (ComputerFatalException(this->toString() + ": Divider should be a convertible to Decimal"));
	return (tmp);
}

const VarBase&	Array::operator/=(const VarBase& v)
{
	if (v._type == VarType::DECIMAL ||
		v._type == VarType::STRING)
	{
		this->split(v.toInteger());
	}
	else
		throw (ComputerFatalException(this->toString() + ": Divider should be a convertible to Decimal"));
	return (*this);
}

VarBase*			Array::operator%(const VarBase& v) const
{
	Array		*tmp = new Array(v);

	if (v._type == VarType::DECIMAL ||
		v._type == VarType::STRING)
	{
		tmp->split(v.toInteger());
	}
	else
		throw (ComputerFatalException(this->toString() + ": Divider should be a convertible to Decimal"));
	return (tmp);
}

const VarBase&	Array::operator%=(const VarBase& v)
{
	if (v._type == VarType::DECIMAL ||
		v._type == VarType::STRING)
	{
		this->split(v.toInteger());
	}
	else
		throw (ComputerFatalException(this->toString() + ": Divider should be a convertible to Decimal"));
	return (*this);
}

const VarBase&	Array::operator=(const VarBase& v)
{
	this->convert(v);
	return (*this);
}

bool			Array::operator==(const VarBase& v) const
{
	return (v == *this);
}

void			Array::split(int divider)
{
	std::vector<VarBase*>	ntab = std::vector<VarBase*>();
	Array*					ntmptab = NULL;
	VarBase					*tmp = NULL;

	try
	{
		if (divider < 0)
		{
			size_t		i = 0;

			while (!this->_data.empty())
			{
				tmp = this->_data.back();

				if (!ntmptab)
					ntmptab = new Array();
				ntmptab->_data.push_back( tmp );
				ntmptab->_length++;

				if (i % divider == 0)
				{
					if (ntmptab)
						ntab.push_back( ntmptab );
					ntmptab = NULL;
				}

				this->_data.pop_back();
				i++;
			}

			if (ntmptab)
				ntab.push_back( ntmptab );

			this->_length = ntab.size();
			this->_data = ntab;
		}
		else
			throw (FloatingPointException(this->toString() + ": Floating point exception"));
	}
	catch (const ComputerFatalException& e)
	{
		throw (ComputerFatalException(this->toString() + ": Divider should be a convertible to Decimal"));
	}
}
