#include <Var.hpp>

using namespace		fx::computer;

VarBase*			Polynome::deg2(void)
{
	Array			*a = NULL;
	VarValue		desc = VarValue(0, 0);
	VarValue		fact = VarValue(4, 0);

	desc = this->_data[1];
	desc *= desc;
	fact *= this->_data[0];
	fact *= this->_data[2];
	desc -= fact;
	a = new Array();
	fact = VarValue(2, 0);
	fact *= this->_data[0];
	a->_data[0] = this->_data[1] - desc.sqrt();
	a->_data[1] = this->_data[1] + desc.sqrt();
	*a->_data[0] /= fact;
	*a->_data[1] /= fact;
	*a->_data[0] *= VarValue(-1, 0);
	*a->_data[1] *= VarValue(-1, 0);
	return ((VarBase*)a);
}

VarBase*			Polynome::to_function(const VarValue& x)
{
	return (this);
	(void)x;
}

VarBase*			Polynome::resolve(int degree)
{
	VarValue		*v = NULL;
	VarValue		desc = VarValue(0, 0);

	if (degree >= 0)
	{
		if ((int)this->_degree != degree)
			throw ComputerFatalException(this->toString() + ": Cannot resolve polynome (deg" + std::to_string(degree) + ")");
		if (degree == 0)
			return (new VarValue(this->_data[0]));
		else if (degree == 1)
		{
			v = new VarValue(-this->_data[0]);
			*v /= this->_data[1];
			return (v);
		}
		else if (degree == 2)
			return (this->deg2());
		else
			throw ComputerFatalException(this->toString() + ": Cannot resolve polynome (deg" + std::to_string(degree) + ")");
		return (NULL);
	}
	else if (degree == 0)
		return (new VarValue(this->_data[0]));
	else if (degree == 1)
	{
		v = new VarValue(-this->_data[0]);
		*v /= this->_data[1];
		return (v);
	}
	else if (degree == 2)
		return (this->deg2());
	else
		throw ComputerFatalException(this->toString() + ": Cannot resolve polynome (deg" + std::to_string(degree) + ")");
	return (NULL);
}

VarBase*				Polynome::operator+(const VarBase& v) const
{
	Polynome*		tmp = new Polynome(v);
	size_t			i = 0;

	while (i < this->_data.size())
	{
		if (tmp->_data.size() <= i)
			tmp->_data[i] = 0;
		tmp->_data[i] += this->_data[i];
		i++;
	}
	tmp->_degree = tmp->_data.size();
	return (tmp);
}

const VarBase&			Polynome::operator+=(const VarBase& v)
{
	Polynome		tmp = Polynome(v);
	size_t			i = 0;

	while (i < tmp._data.size())
	{
		if (this->_data.size() <= i)
			this->_data[i] = 0;
		this->_data[i] += tmp._data[i];
		i++;
	}
	this->_degree = this->_data.size();
	return (*this);
}

VarBase*				Polynome::operator-(void) const
{
	Polynome*		tmp = new Polynome(0, std::vector<VarValue>());
	size_t			i = 0;

	while (i < this->_data.size())
	{
		if (tmp->_data.size() <= i)
			tmp->_data[i] = 0;
		tmp->_data[i] = this->_data[i];
		tmp->_data[i] *= VarValue(-1, 0);
		i++;
	}
	tmp->_degree = tmp->_data.size();
	return (tmp);
}

VarBase*				Polynome::operator-(const VarBase& v) const
{
	Polynome*		tmp = new Polynome(v);
	size_t			i = 0;

	while (i < this->_data.size())
	{
		if (tmp->_data.size() <= i)
			tmp->_data[i] = 0;
		tmp->_data[i] -= this->_data[i];
		i++;
	}
	tmp->_degree = tmp->_data.size();
	return (tmp);
}

const VarBase&			Polynome::operator-=(const VarBase& v)
{
	Polynome		tmp = Polynome(v);
	size_t			i = 0;

	while (i < tmp._data.size())
	{
		if (this->_data.size() <= i)
			this->_data[i] = 0;
		this->_data[i] -= tmp._data[i];
		i++;
	}
	this->_degree = this->_data.size();
	return (*this);
}

VarBase*				Polynome::operator*(const VarBase& v) const
{
	Polynome*		tmp = new Polynome(v);
	size_t			i = 0;

	while (i < this->_data.size())
	{
		if (tmp->_data.size() <= i)
			tmp->_data[i] = 0;
		tmp->_data[i] *= this->_data[i];
		i++;
	}
	tmp->_degree = tmp->_data.size();
	return (tmp);
}

const VarBase&			Polynome::operator*=(const VarBase& v)
{
	Polynome		tmp = Polynome(v);
	size_t			i = 0;

	while (i < tmp._data.size())
	{
		if (this->_data.size() <= i)
			this->_data[i] = 0;
		this->_data[i] += tmp._data[i];
		i++;
	}
	this->_degree = this->_data.size();
	return (*this);
}

VarBase*				Polynome::operator/(const VarBase& v) const
{
	Polynome*		tmp = new Polynome(v);
	size_t			i = 0;

	while (i < this->_data.size())
	{
		if (tmp->_data.size() <= i)
			tmp->_data[i] = 0;
		tmp->_data[i] /= this->_data[i];
		i++;
	}
	tmp->_degree = i;
	return (tmp);
}

const VarBase&			Polynome::operator/=(const VarBase& v)
{
	Polynome		tmp = Polynome(v);
	size_t			i = 0;

	while (i < tmp._data.size())
	{
		if (this->_data.size() <= i)
			this->_data[i] = 0;
		this->_data[i] /= tmp._data[i];
		i++;
	}
	this->_degree = i;
	return (*this);
}

VarBase*				Polynome::operator%(const VarBase& v) const
{
	Polynome*		tmp = new Polynome(v);
	size_t			i = 0;

	while (i < this->_data.size())
	{
		if (tmp->_data.size() <= i)
			tmp->_data[i] = 0;
		tmp->_data[i] %= this->_data[i];
		i++;
	}
	tmp->_degree = i;
	return (tmp);
}

const VarBase&			Polynome::operator%=(const VarBase& v)
{
	Polynome		tmp = Polynome(v);
	size_t			i = 0;

	while (i < tmp._data.size())
	{
		if (this->_data.size() <= i)
			this->_data[i] = 0;
		this->_data[i] %= tmp._data[i];
		i++;
	}
	this->_degree = i;
	return (*this);
}

const VarBase&			Polynome::operator=(const VarBase& v)
{
	this->convert(v);
	return (*this);
}

bool 					Polynome::operator==(const VarBase& v) const
{
	return (v == *this);
}