#include <Var.hpp>

using namespace		fx::computer;

int						VarValue::toInteger() const
{
	return (this->_re);
}

void					VarValue::convert(const VarBase& v, const lookup_t* lookup)
{
	if (v._type == VarType::STRING)
	{
		String&		tmp = (String&)v;

		this->_re = tmp.toInteger();
		this->_im = 0;
	}
	else if (v._type == VarType::DECIMAL)
	{
		VarValue&		tmp = (VarValue&)v;

		this->_re = tmp._re;
		this->_im = tmp._im;
	}
	else if (v._type == VarType::POLYNOME)
	{
		Polynome&		tmp = (Polynome&)v;
		VarBase			*v = tmp.resolve(1);

		v = tmp.resolve(1);
		*this = *v;
		delete v;
	}
	else
		throw (ComputerFatalException(this->toString() + ": Cannot cast to " + v.typeName()));
	(void)lookup;
}

VarBase*				VarValue::operator+(const VarBase& v) const
{
	VarValue*		tmp = new VarValue(v);

	tmp->_re += this->_re;
	tmp->_im += this->_im;
	return (tmp);
}

const VarBase&			VarValue::operator+=(const VarBase& v)
{
	VarValue		tmp = VarValue(v);

	this->_re += tmp._re;
	this->_im += tmp._im;
	return (*this);
}

VarBase*				VarValue::operator-(void) const
{
	VarValue		*tmp = new VarValue(-this->_re, this->_im);

	return (tmp);
}

VarBase*				VarValue::operator-(const VarBase& v) const
{
	VarValue		*tmp = new VarValue(v);

	tmp->_re = this->_re - tmp->_re;
	tmp->_im = this->_im - tmp->_im;
	return (tmp);
}

const VarBase&			VarValue::operator-=(const VarBase& v)
{
	VarValue		tmp = VarValue(v);

	this->_re -= tmp._re;
	this->_im -= tmp._im;
	return (*this);
}

VarBase*				VarValue::operator*(const VarBase& v) const
{
	VarValue				*tmp = new VarValue(v);
	double					re = tmp->_re;
	double					im = tmp->_im;

	tmp->_re = (this->_re * re) - (this->_im * im);
	tmp->_im = (this->_re * im) + (this->_im * re);
	return (tmp);
}

const VarBase&			VarValue::operator*=(const VarBase& v)
{
	VarValue				tmp = VarValue(v);
	double					re = this->_re;
	double					im = this->_im;

	this->_re = (tmp._re * re) - (tmp._im * im);
	this->_im = (tmp._re * im) + (tmp._im * re);
	return (*this);
}

VarBase*				VarValue::operator/(const VarBase& v) const
{
	VarValue*			tmp = new VarValue(v);
	double				re1 = this->_re;
	double				re2 = tmp->_re;
	double				im1 = this->_im;
	double				im2 = tmp->_re;

	if (((re2 * re2) + (im2 * im2)) == 0)
		throw FloatingPointException("Divider is 0");

	tmp->_re = ((re1 * re2) + (im1 * im2)) / ((re2 * re2) + (im2 * im2));
	tmp->_im = ((im1 * re2) + (re1 * im2)) / ((re2 * re2) + (im2 * im2));
	return (tmp);
}

const VarBase&			VarValue::operator/=(const VarBase& v)
{
	VarValue			tmp = VarValue(v);
	double				re1 = this->_re;
	double				re2 = tmp._re;
	double				im1 = this->_im;
	double				im2 = tmp._re;

	if (((re2 * re2) + (im2 * im2)) == 0)
		throw FloatingPointException("Divider is 0");

	this->_re = ((re1 * re2) + (im1 * im2)) / ((re2 * re2) + (im2 * im2));
	this->_im = ((im1 * re2) + (re1 * im2)) / ((re2 * re2) + (im2 * im2));
	return (*this);
}

VarBase*				VarValue::operator%(const VarBase& v) const
{
	VarValue*			tmp = new VarValue(v);
	double				re1 = this->_re;
	double				re2 = tmp->_re;
	double				im1 = this->_im;
	double				im2 = tmp->_re;

	if (((re2 * re2) + (im2 * im2)) == 0)
		throw FloatingPointException("Divider is 0");

	tmp->_re = ((int)((re1 * re2) + (im1 * im2))) % ((int)((re2 * re2) + (im2 * im2)));
	tmp->_im = ((int)((im1 * re2) + (re1 * im2))) % ((int)((re2 * re2) + (im2 * im2)));
	return (tmp);
}

const VarBase&			VarValue::operator%=(const VarBase& v)
{
	VarValue			tmp = VarValue(v);
	double				re1 = this->_re;
	double				re2 = tmp._re;
	double				im1 = this->_im;
	double				im2 = tmp._re;

	if (((re2 * re2) + (im2 * im2)) == 0)
		throw FloatingPointException("Divider is 0");

	this->_re = ((int)((re1 * re2) + (im1 * im2))) % ((int)((re2 * re2) + (im2 * im2)));
	this->_im = ((int)((im1 * re2) + (re1 * im2))) % ((int)((re2 * re2) + (im2 * im2)));
	return (*this);
}

const VarBase&			VarValue::operator=(const VarBase& v)
{
	this->convert(v);
	return (*this);
}

const VarBase&			VarValue::operator=(double v)
{
	this->_re = v;
	this->_im = 0;
	return (*this);
}

bool 					VarValue::operator==(const VarBase& v) const
{
	VarValue			tmp = VarValue(v);

	return (tmp._re == this->_re && tmp._im == this->_im);
}
