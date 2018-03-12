#ifndef VAR_HPP
# define VAR_HPP
# include <Parser.hpp>
# include <stdlib.h>
# include <vector>
# include <map>
# include <exception>
# include <string.h>
# include <math.h>

namespace						fx
{
	namespace					computer
	{
		namespace				VarType
		{
			enum
			{
				POLYNOME,
				DECIMAL,
				ARRAY,
				FUNCTION,
				STRING,
				OBJECT
			};
		};

		class					Computer;
		class					VarBase;
		class					VarValue;
		class					String;
		class					Array;
		class					Polynome;
		class					Function;
		class					Object;

		using					var_t = VarBase*;
		using					array_t = Array*;
		using					decimal_t = VarValue*;

		using 					ComputerParserBase = ParserBase<VarBase*, Computer&>;
		using					ComputerToken = fx::ParserToken<var_t, Computer&>;
		using					ComputerTokenInfix = fx::ParserTokenInfix<var_t, Computer&>;
		using					ComputerTokenInfixR = fx::ParserTokenInfixR<var_t, Computer&>;
		using					ComputerTokenPrefix = fx::ParserTokenPrefix<var_t, Computer&>;
		using					ComputerTokenLiteral = fx::ParserTokenLiteral<var_t, Computer&>;
		using					ComputerTokenTemp = fx::ParserTokenBase<var_t, Computer&>;
		using					ComputerTokenScope = fx::ParserTokenScope<var_t, Computer&>;

		using					ComputerException = fx::ParserBaseException<var_t, Computer&>;
		using					ComputerWarningException = fx::ParserBaseWarningException<var_t, Computer&>;
		using					ComputerNoticeException = fx::ParserBaseNoticeException<var_t, Computer&>;
		using					ComputerAbortException = fx::ParserBaseAbortException<var_t, Computer&>;
		using					ComputerFatalException = fx::ParserBaseFatalException<var_t, Computer&>;

		using					FloatingPointException = fx::ParserBaseFatalException<var_t, Computer&>;
		using					CastException = fx::ParserBaseFatalException<var_t, Computer&>;

		typedef	struct
		{
			std::vector< std::string >							location;
			std::map<std::string, VarBase*>						defs;
		}														lookup_t;

		class					VarBase
		{
			public :

				std::string		_name;
				unsigned int	_type;
				bool			_ref;
				bool			_unknow;

				VarBase(std::string name = "", unsigned int type = 0, bool ref = false):
					_name(name),
					_type(type),
					_ref(ref),
					_unknow(false)
				{}

				VarBase(const VarBase& v, const lookup_t* lookup = NULL):
					_name(v._name),
					_type(v._type),
					_ref(v._ref),
					_unknow(v._unknow)
				{}

				VarBase(const VarBase* v):
					_name(v->_name),
					_type(v->_type),
					_ref(v->_ref),
					_unknow(v->_unknow)
				{}

				virtual VarBase			*clone() const;
				static VarBase			*doop(const VarBase& a, const VarBase& b, const std::string& op);

				void					setUnknow(bool u)
				{
					this->_unknow = u;
				}

				bool					getUnknow(void)
				{
					return (this->_unknow);
				}

				void					setName(std::string name)
				{
					this->_name = name;
				}

				void					setRef(bool ref)
				{
					this->_ref = ref;
				}

				virtual ~VarBase()
				{}

				virtual std::string		toString() const
				{
					return (this->_name + " (" + this->typeName() + ")");					
				}

				virtual std::string		typeName(void) const
				{
					return ("VarBase");
				}

				virtual void			convert(unsigned int type)
				{
					(void)type;
				}

				virtual void			convert(const VarBase& v, const lookup_t* lookup = NULL)
				{
					std::cout << "BASE CONVERT" << std::endl;
					(void)v;
				}

				const VarBase&			operatorError(const char&op = '?') const
				{
					throw (ComputerFatalException(this->toString() + ": Undefined "+op+" operator"));
					return (*this);
				}

				virtual VarBase*		operator+(const VarBase& v) const {return ((VarBase*)&v);}
				virtual VarBase*		operator-(void) const {return ((VarBase*)this);}
				virtual VarBase*		operator-(const VarBase& v) const {return ((VarBase*)&v);}
				virtual VarBase*		operator*(const VarBase& v) const {return ((VarBase*)&v);}
				virtual VarBase*		operator/(const VarBase& v) const {return ((VarBase*)&v);}
				virtual VarBase*		operator%(const VarBase& v) const {return ((VarBase*)&v);}
				virtual const VarBase&	operator+=(const VarBase& v){return (v);}
				virtual const VarBase&	operator-=(const VarBase& v){return (v);}
				virtual const VarBase&	operator*=(const VarBase& v){return (v);}
				virtual const VarBase&	operator/=(const VarBase& v){return (v);}
				virtual const VarBase&	operator%=(const VarBase& v){return (v);}
				virtual const VarBase&	operator=(const VarBase& v){return (v);}
				virtual bool 			operator==(const VarBase& v) const {return (v == *this);}

				virtual int				toInteger() const
				{
					throw (ComputerFatalException(this->toString() + ": Integer conversion failed"));
					return (0);
				}
		};

		class						VarValue: public VarBase
		{

			public :

				double				_re;
				double				_im;

				VarValue(double re = 0, double im = 0, std::string name = "", bool ref = false):
					VarBase(name, VarType::DECIMAL, ref),
					_re(re),
					_im(im)
				{}

				VarValue(const VarBase& v, const lookup_t* lookup = NULL):
					VarBase(v),
					_re(0),
					_im(0)
				{
					this->convert(v);
				}

				VarValue(const VarBase* v):
					VarBase(v),
					_re(0),
					_im(0)
				{
					std::cout << "------------" << std::endl;
					this->convert(*v);
				}

				std::string				toString() const override
				{
					if (this->_im > 0)
						return ("<complex "+this->_name+"> " + std::to_string(this->_re) + " + " + std::to_string(this->_im) + "i");
					else if (this->_im < 0)
						return ("<complex "+this->_name+"> " + std::to_string(this->_re) + " - " + std::to_string(-this->_im) + "i");
					return ("<real "+this->_name+"> " + std::to_string(this->_re));
				}
				int						toInteger() const  override;
				//VarBase				*doop(const VarBase& a, const char& c);
				void					convert(const VarBase& v, const lookup_t* lookup = NULL);

				virtual VarBase*		operator+(const VarBase& v) const;
				virtual VarBase*		operator-(void) const;
				virtual VarBase*		operator-(const VarBase& v) const;
				virtual VarBase*		operator*(const VarBase& v) const;
				virtual VarBase*		operator/(const VarBase& v) const;
				virtual VarBase*		operator%(const VarBase& v) const;
				virtual const VarBase&	operator+=(const VarBase& v);
				virtual const VarBase&	operator-=(const VarBase& v);
				virtual const VarBase&	operator*=(const VarBase& v);
				virtual const VarBase&	operator/=(const VarBase& v);
				virtual const VarBase&	operator%=(const VarBase& v);
				virtual const VarBase&	operator=(const VarBase& v);
				virtual const VarBase&	operator=(double v);
				virtual bool 			operator==(const VarBase& v) const;

				const VarValue*			sqrt(void)
				{
					VarValue*			tmp = new VarValue(0, 0);
					double				del = (this->_re * this->_re) + (this->_im * this->_im);

					del = (this->_re + ::sqrt(del)) / 2;
					tmp->_re = ::sqrt(tmp->_re);
					tmp->_im = (this->_re / 2) / tmp->_re;
					return(tmp);
				}

				VarValue*				pow(const VarValue& v)
				{
					VarValue			*tmp = new VarValue(v);
					int					n = (int)tmp->_re;

					if (tmp->_im != 0)
						throw ComputerAbortException("Undefined pow operation on complex");
					else if (n < 0)
						throw ComputerAbortException("Undefined pow operation on negative value");

					tmp->_re = (n == 0);
					while (n--)
						(*tmp) *= (*this);
					return(tmp);
					(void)v;
				}

				VarBase*				clone(void) const
				{
					//std::cout << "CLONE" << std::endl;
					return ( new VarValue(this->_re, this->_im) );
				}
		};

		class						String: public VarBase
		{
			public :

				std::string			_data;

				String(std::string value = "", std::string name = "", bool ref = false):
					VarBase(name, VarType::STRING, ref),
					_data(value)
				{}

				String(const VarBase& v, const lookup_t* lookup = NULL):
					VarBase(v),
					_data("")
				{
					this->convert(v);
				}

				String(const VarBase* v, const lookup_t* lookup = NULL):
					VarBase(v),
					_data("")
				{
					this->convert(*v);
				}

				int					toInteger();

				void				convert(const VarBase& v, const lookup_t* lookup = NULL);

				void				convert(unsigned int type);


				virtual VarBase*		operator+(const VarBase& v) const;
				virtual const VarBase&	operator+=(const VarBase& v);
				virtual const VarBase&	operator=(const VarBase& v);
				virtual bool 			operator==(const VarBase& v) const;

				void				append(const VarBase& s);
				VarBase*			clone(void) const
				{
					return (new String(*this));
				}

				std::string				toString() const
				{
					return (this->_data);
				}
		};

		class					Array: public VarBase
		{
			public :

				Array(std::string name = "", bool ref = false):
					VarBase(name, VarType::ARRAY, ref),
					_is_matrix(false),
					_length(0),
					_data(std::vector<VarBase*>())
				{}

				Array(const VarBase& v, const lookup_t* lookup = NULL):
					VarBase(v),
					_is_matrix(false),
					_length(0),
					_data(std::vector<VarBase*>())

				{
					this->convert(v, lookup);
				}


				~Array()
				{
					size_t		i = 0;

					while (i < this->_length)
					{
						delete this->_data[i];
						i++;
					}
				}

				void					convert(const VarBase& v, const lookup_t* lookup = NULL);

				std::string				typeName(void);
				virtual VarBase*		operator+(const VarBase& v) const;
				virtual VarBase*		operator-(void) const;
				virtual VarBase*		operator-(const VarBase& v) const;
				virtual VarBase*		operator*(const VarBase& v) const;
				virtual VarBase*		operator/(const VarBase& v) const;
				virtual VarBase*		operator%(const VarBase& v) const;
				virtual const VarBase&	operator+=(const VarBase& v);
				virtual const VarBase&	operator-=(const VarBase& v);
				virtual const VarBase&	operator*=(const VarBase& v);
				virtual const VarBase&	operator/=(const VarBase& v);
				virtual const VarBase&	operator%=(const VarBase& v);
				virtual const VarBase&	operator=(const VarBase& v);
				virtual bool 			operator==(const VarBase& v) const;

//				virtual					operator Polynome() const;

				std::string		toString(void) const
				{
					std::string	s = "Array "+this->_name+" ("+std::to_string(this->_length)+") :[\n";

					for (auto it = this->_data.begin(); it != this->_data.end(); it++)
					{
						s += "\t" + (*it)->toString() + "\n";
					}
					s += "]";
					return (s);
				}

				void			split(int divider);
				VarBase*		clone(void) const
				{
					return (new Array(*this));
				}


				bool					_is_matrix;
				size_t					_length;
				std::vector<VarBase*>	_data;

		};

		class					Function: public VarBase
		{
			public :

				std::vector<std::string>			_args;
				ComputerToken*						_content;
				lookup_t*							_ctx;
				bool								_va;

				Function(std::vector<std::string>& args, ComputerToken* parser, lookup_t *ctx = NULL, std::string name = "", bool ref = false):
					VarBase(name, VarType::FUNCTION, ref),
					_args(args),
					_content(parser),
					_ctx(ctx),
					_va(false)
				{}

				Function(ComputerToken* parser, lookup_t *ctx = NULL, std::string name = "", bool ref = false):
					VarBase(name, VarType::FUNCTION, ref),
					_args(std::vector<std::string>()),
					_content(parser),
					_ctx(ctx),
					_va(false)
				{}

				Function(const VarBase& v, const lookup_t* ctx = NULL):
					VarBase(v),
					_args(std::vector<std::string>()),
					_content(NULL),
					_ctx((lookup_t*)ctx),
					_va(false)
				{
					this->convert(v, ctx);
				}

				~Function(void)
				{}

				virtual VarBase*	call(std::vector<VarBase*>& args, Computer& data);
				void				convert(const VarBase& v, const lookup_t *lookup = NULL)
				{
					(void)v;
					(void)lookup;
				}

				VarBase*			clone(void) const
				{
					return (new Function(*this));
				}

		};

		typedef VarBase*	(*Syscall)(std::vector<VarBase*>&, Computer&);

		typedef
		std::vector<VarBase*>&									Args;

		class					FunctionSys: public Function
		{
			public :

				Syscall		_fct;

				FunctionSys(Syscall call, lookup_t *ctx = NULL, std::string name = "", bool ref = false):
					Function(NULL, ctx, name, ref),
					_fct(call)
				{}

				~FunctionSys(void)
				{}

				VarBase*			call(std::vector<VarBase*>& args, Computer& data)
				{
					return ((*this->_fct)(args, data));
				}

				VarBase*			clone(void) const
				{
					return (new FunctionSys(*this));
				}
		};

		class					Object: public VarBase
		{
			public :

				std::map<std::string, VarBase*>		_attrs;
				std::map<std::string, Function*>	_methods;
				lookup_t*							_ctx;

				Object(std::map<std::string, VarBase*>& attrs, std::map<std::string, Function*>& methods, lookup_t* ctx, std::string name = "", bool ref = false):
					VarBase(name, VarType::OBJECT, ref),
					_attrs(attrs),
					_methods(methods),
					_ctx(ctx)
				{}

				Object(const VarBase& v, const lookup_t *lookup = NULL):
					VarBase(v),
					_attrs(std::map<std::string, VarBase*>()),
					_methods(std::map<std::string, Function*>()),
					_ctx((lookup_t*)lookup)
				{
					this->convert(v, lookup);
				}

				void			convert(const VarBase& v, const lookup_t *lookup = NULL)
				{
					(void)v;
					(void)lookup;
				}

				VarBase*		clone(void) const
				{
					return (new Object(*this));
				}
		};

		class					Polynome: public VarBase
		{
			public :

				size_t						_degree;
				std::vector<VarValue>		_data;

				Polynome(size_t degree, std::vector<VarValue>& vars, std::string name = "", bool ref = false):
					VarBase(name, VarType::POLYNOME, ref),
					_degree(degree),
					_data(vars)
				{}

				Polynome(size_t degree = 0, std::vector<VarValue> vars = std::vector<VarValue>(), std::string name = "", bool ref = false):
					VarBase(name, VarType::POLYNOME, ref),
					_degree(degree),
					_data(vars)
				{}

				Polynome(const VarBase& v, const lookup_t *lookup = NULL):
					VarBase(v),
					_degree(0),
					_data(std::vector<VarValue>())
				{
					this->convert(v, lookup);
				}

				Polynome(const VarBase* v, const lookup_t *lookup = NULL):
					VarBase(v),
					_degree(0),
					_data(std::vector<VarValue>())
				{
					if (v)
						this->convert(*v, lookup);
				}

				~Polynome(void)
				{}

				virtual VarBase*		operator+(const VarBase& v) const;
				virtual VarBase*		operator-(void) const;
				virtual VarBase*		operator-(const VarBase& v) const;
				virtual VarBase*		operator*(const VarBase& v) const;
				virtual VarBase*		operator/(const VarBase& v) const;
				virtual VarBase*		operator%(const VarBase& v) const;
				virtual const VarBase&	operator+=(const VarBase& v);
				virtual const VarBase&	operator-=(const VarBase& v);
				virtual const VarBase&	operator*=(const VarBase& v);
				virtual const VarBase&	operator/=(const VarBase& v);
				virtual const VarBase&	operator%=(const VarBase& v);
				virtual const VarBase&	operator=(const VarBase& v);
				virtual bool 			operator==(const VarBase& v) const;

				std::string		toString(void) const
				{
					std::string	s = "Polynome "+std::to_string(this->_degree);

					s += " : [";
					for (auto it = this->_data.begin(); it != this->_data.end(); it++)
						s += " " + (*it).toString();
					s += "]";
					return (s);
				}

				VarBase*		to_function(const VarValue& x);
				VarBase*		deg2(void);
				VarBase*		resolve(int degree = -1);
				void			convert(const VarBase& v, const lookup_t *lookup = NULL)
				{
					if (v._type == VarType::DECIMAL)
					{
						VarValue& tmp = (VarValue&)v;

						this->_degree = 1;
						this->_data.push_back(tmp);
					}
					else if (v._type == VarType::ARRAY)
					{
						size_t		i = 0;
						Array&		tmp = (Array&)v;

						this->_degree = 0;
						while (i < tmp._data.size())
						{
							VarValue tmp2 = VarValue(tmp._data[i]);
							std::cout << tmp2.toString() << std::endl;
							this->_data.push_back(tmp2);
							this->_degree++;
							i++;
						}
					}
					else
						throw ComputerFatalException(this->toString() + ": Cannot cast to Polynome");
				}

				VarBase*		clone(void) const
				{
					return (new Polynome(*this));
				}
		};

		class					Complex: public VarBase
		{
			public :

				double			_re;
				double			_im;

				Complex(double re = 0, double im = 1, std::string name = "", bool ref = false):
					VarBase(name, VarType::POLYNOME, ref),
					_re(re),
					_im(im)
				{}

				Complex(const VarBase& v, const lookup_t *lookup = NULL):
					VarBase(v),
					_re(0),
					_im(1)
				{
					this->convert(v, lookup);
				}

				~Complex(void)
				{}

				void		convert(const VarBase& v, const lookup_t *lookup = NULL)
				{
					(void)v;
					(void)lookup;
				}

				VarBase		*clone(void) const
				{
					return (new Complex(*this));
				}
		};

	};
};

#endif