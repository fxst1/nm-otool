#ifndef COMPUTERTOKEN_HPP
# define COMPUTERTOKEN_HPP
# include <Var.hpp>
# include <string>
# include <cstdlib>
# include <Computer.hpp>
# define RegexDigit "[0-9]+(.[0-9]+)*"
# define RegexLiteral "[_a-zA-Z][_a-zA-Z0-9]*"
# define RegexFunction "function\\s+(" RegexLiteral ")\\s*"
# define RegexArgs "(\\s*(" RegexLiteral ")?\\s*)|(\\s*" RegexLiteral "\\s*(,\\s*" RegexLiteral "\\s*)*)"
# define RegexFunctionDecl "function(\\s*|\\s+" RegexLiteral ")\\s*\\((" RegexArgs ")\\)"
# define RegexScope ";+\\s*"

namespace		fx::computer
{
		class		Computer;

	 	class		ComputerScope: public ComputerToken
	 	{
	 		bool	_is_prefix;

	 		public :

		 		ComputerScope():
		 			ComputerToken(RegexScope, 10, true),
		 			_is_prefix(false)
		 		{}

	 			ComputerScope(const ComputerToken& m, const char *s):
		 			ComputerToken(m),
		 			_is_prefix(false)
	 			{(void)s;}

	 			std::string		toString(void) const
	 			{
	 				return ("<scope>");
	 			}

				ComputerToken*	nud(ComputerParserBase& parser, Computer& data)
				{
					ComputerToken*				tmp = NULL;

					this->_is_prefix = true;
					this->_priority = 0;
					while (!parser.end())
					{
						if (parser.getToken()->compare("}"))
							break ;
						tmp = parser.expression(data, this->_priority);
						if (tmp == NULL)
							break ;
						else if (tmp->toString() == this->toString())
							for (auto it = tmp->begin(); it != tmp->end(); it++)
								this->push_back((*it));
						else
							this->push_back(tmp);
					}
					return (this);
				}

				ComputerToken*	led(ComputerParserBase& parser, ComputerToken* left, Computer& data)
				{
					ComputerToken*				tmp = NULL;

					if (left == NULL)
						return (NULL);
					else if (left->toString() == this->toString())
						for (auto it = left->begin(); it != left->end(); it++)
							this->push_back((*it));
					else
						this->push_back(left);

					this->_priority = 0;
					while (!parser.end())
					{
						if (parser.getToken()->compare("}"))
							break ;
						tmp = parser.expression(data, this->_priority);
						if (tmp == NULL)
							break ;
						else if (tmp->toString() == this->toString())
							for (auto it = tmp->begin(); it != tmp->end(); it++)
								this->push_back((*it));
						else
							this->push_back(tmp);
					}
					return (this);
					(void)parser;
					(void)left;
					(void)data;
				}

	 			var_t			execute(Computer& data)
	 			{
	 				return (this->helperLineEval(data));
	 			}

	 			ComputerToken*	clone(const char *s)
				{
					return (new ComputerScope(*this, s));
				}
	 	};

		class		ComputerLiteral: public ComputerToken
		{
			public :

				std::string		_name;
				bool			_is_prefix;

				ComputerLiteral(std::string name = ""):
					ComputerToken(RegexLiteral, 140, true),
					_name(name),
					_is_prefix(false)
				{}

				ComputerLiteral(const ComputerToken& m, const char *s):
					ComputerToken(m),
					_name(std::string(s)),
					_is_prefix(false)
				{}

				ComputerToken	*nud(ComputerParserBase& parser, Computer& data)
				{
					this->_priority = 0;
					this->_is_prefix = true;
					return (this);
					(void)parser;
					(void)data;
				}

				ComputerToken	*led(ComputerParserBase& parser, ComputerToken* left, Computer& data)
				{
					this->push_back(left);
					return (this);
					(void)parser;
					(void)data;
				}

				var_t			execute(Computer& data)
				{
					var_t		a = (this->_name == "i") ? data.allocdVar( new VarValue(0, 1) ) : data.getVar(this->_name.c_str());
					var_t		b = NULL;

					if (this->_is_prefix)
						return (a == NULL ? data.allocdVar( new Polynome() ) : a);
					if (!a)
						throw ComputerAbortException("Undefined variable " + this->_name);
					b = this->get(0)->execute(data);
					return (data.allocdVar((*a) * (*b)));
				}

				std::string		toString() const
				{
					if (!this->_is_prefix)
						return ("<literal_factor="+this->_name+">");
					return ("<literal="+this->_name+">");
				}

				ComputerToken	*clone(const char *s)
				{
					return (new ComputerLiteral(*this, s));
				}
		};

	 	class		ComputerDigit: public ComputerToken
	 	{
			double 	_value;
			bool	_is_prefix;

	 		public :

				ComputerDigit(std::string s = ""):
					ComputerToken(RegexDigit, 140, true),
					_is_prefix(false)
				{
					this->_value = (double)atof(s.c_str());
				}

				ComputerDigit(const ComputerToken& m, const char *s):
					ComputerToken(m),
					_value(atof(s)),
					_is_prefix(false)
				{}

				~ComputerDigit()
				{}

				ComputerToken	*nud(ComputerParserBase& parser, Computer& data)
				{
					this->_priority = 0;
					this->_is_prefix = true;
					return (this);
					(void)parser;
					(void)data;
				}

				var_t			execute(Computer& data)
				{
					var_t		a;

					a = data.allocdVar( new fx::computer::VarValue(this->_value, 0) );
					return (a);
				}

				std::string		toString() const
				{
					if (!this->_is_prefix)
						return ("<digit_factor="+std::to_string(this->_value)+">");
					return ("<digit="+std::to_string(this->_value)+">");
				}

	 			ComputerToken	*clone(const char *s)
				{
					return (new ComputerDigit(*this, s));
				}
	 	};

		class		OperatorEqual: public ComputerTokenInfix
		{
			public :

				OperatorEqual():
				ParserTokenInfix<var_t, Computer&>("=", 80)
				{}

				OperatorEqual(const ComputerTokenInfix& m, const char *s):
					ComputerTokenInfix(m)
				{(void)s;}

				std::string			toString() const
				{
					return ("<=>");
				}

				var_t				execute(Computer& data)
				{
					var_t			a = this->get(0)->execute(data);
					var_t			b = this->get(1)->execute(data);

					if (a == NULL)
					{
						std::string	tmp = ((ComputerLiteral*)this->get(0))->_name;
						a = b->clone();
						a->_name = tmp;
						data.allocdVar( a, tmp.c_str() );
					}
					else if (b->_type == VarType::POLYNOME)
						*a = ((Polynome*)b)->resolve();
					else
						*a = *b;
					return (a);
				}

	 			ComputerToken*		clone(const char *s)
				{
					return (new OperatorEqual(*this, s));
				}
	 	};

	 	/*
	 	**	PLUS
	 	*/

	 	class		OperatorPlus: public ComputerTokenInfix
	 	{
	 		public :

	 			OperatorPlus():
	 				ComputerTokenInfix("+", 130)
	 			{

	 			}

	 			OperatorPlus(const OperatorPlus& m, const char *s):
		 			ComputerTokenInfix(m)
	 			{(void)s;}

				std::string		toString() const
				{
					return ("<+>");
				}

	 			var_t		execute(Computer& data)
	 			{
	 				var_t			a = this->get(0)->execute(data);
	 				var_t			b = this->get(1)->execute(data);

	 				if (!a)
	 					a = data.allocdVar(new Polynome(0, std::vector<VarValue>()));
	 				return (data.allocdVar(*a + *b));
	 			}

	 			ComputerToken	*clone(const char *s)
				{
					return (new OperatorPlus(*this, s));
				}
	 	};

	 	/*
	 	**	MINUS
	 	*/

	 	class		OperatorMinus: public ComputerToken
	 	{
	 		bool	_is_prefix;

	 		public :

	 			OperatorMinus():
	 				ComputerToken("-", 110),
	 				_is_prefix(false)
	 			{}

	 			OperatorMinus(const ComputerToken& m, const char *s):
		 			ComputerToken(m),
	 				_is_prefix(false)
	 			{(void)s;}

				std::string		toString() const
				{
					if (this->_is_prefix)
						return ("<- Prefix>");
					else
						return ("<- Infix>");
				}

				ComputerToken	*nud(ComputerParserBase& root, Computer& data)
				{
					this->_is_prefix = true;
					return (this->helperNud(root, data));
				}

				ComputerToken	*led(ComputerParserBase& root, ComputerToken *left, Computer& data)
				{
					return (this->helperLed(root, left, data));
				}

	 			var_t			execute(Computer& data)
	 			{
	 				if (this->_is_prefix)
	 					return (this->prefixEval(data));
	 				else
	 					return (this->infixEval(data));

	 			}

	 			ComputerToken	*clone(const char *s)
				{
					return (new OperatorMinus(*this, s));
				}

			private :

				var_t			infixEval(Computer& data)
				{
	 				var_t		a = this->get(0)->execute(data);
	 				var_t		b = this->get(1)->execute(data);

	 				if (!a)
	 					a = data.allocdVar(new Polynome(0, std::vector<VarValue>()));
	 				return (data.allocdVar(*a - *b));
				}

				var_t			prefixEval(Computer& data)
				{
	 				var_t		a = this->get(0)->execute(data);

	 				if (!a)
	 					a = data.allocdVar(new Polynome(0, std::vector<VarValue>()));
	 				return (data.allocdVar(-(*a)));
				}
	 	};

	 	class		OperatorPow: public ComputerTokenInfixR
	 	{
	 		public :

	 			OperatorPow():
	 				ComputerTokenInfixR("**", 140)
	 			{}

	 			OperatorPow(const ComputerTokenInfixR& m, const char *s):
		 			ComputerTokenInfixR(m)
	 			{ (void)s; }

	 			~OperatorPow()
	 			{}

	 			var_t			execute(Computer& data)
	 			{
	 				var_t		a = this->get(0)->execute(data);
	 				var_t		b = this->get(1)->execute(data);

	 				return (data.allocdVar(((VarValue*)a)->pow(*b)));
	 			}

				std::string		toString() const
				{
					return ("<**>");
				}

	 			ComputerToken	*clone(const char *s)
				{
					return (new OperatorPow(*this, s));
				}
	 	};

	 	class		OperatorMultiply: public ComputerTokenInfix
	 	{
	 		public :

	 			OperatorMultiply():
	 				ComputerTokenInfix("*", 140)
	 			{}

	 			OperatorMultiply(const ComputerTokenInfix& m, const char *s):
		 			ComputerTokenInfix(m)
	 			{
	 				(void)s;
	 			}

	 			~OperatorMultiply()
	 			{}

	 			var_t			execute(Computer& data)
	 			{
	 				var_t		a = this->get(0)->execute(data);
	 				var_t		b = this->get(1)->execute(data);

	 				if (!a)
	 					a = data.allocdVar(new Polynome(0, std::vector<VarValue>()));
	 				return (data.allocdVar((*a) * (*b)));
	 			}

				std::string		toString() const
				{
					return ("<*>");
				}

	 			ComputerToken	*clone(const char *s)
				{
					return (new OperatorMultiply(*this, s));
				}
	 	};

	 	class		OperatorDivide: public ComputerTokenInfix
	 	{
	 		public :

	 			OperatorDivide():
	 			ComputerTokenInfix("/", 120)
	 			{

	 			}

	 			OperatorDivide(const ComputerTokenInfix& m, const char *s):
		 			ComputerTokenInfix(m)
	 			{(void)s;}

				std::string		toString() const
				{
					return ("</>");
				}

	 			var_t		execute(Computer& data)
	 			{
	 				var_t			a = this->get(0)->execute(data);
	 				var_t			b = this->get(1)->execute(data);

	 				if (!a)
	 					a = data.allocdVar(new Polynome(0, std::vector<VarValue>()));
	 				return (data.allocdVar(*a / *b));
	 			}

	 			ComputerToken	*clone(const char *s)
				{
					return (new OperatorDivide(*this, s));
				}
	 	};

		class		OperatorModulo: public ComputerTokenInfix
		{
			public :

				OperatorModulo():
				ComputerTokenInfix("%", 120)
				{

				}

				OperatorModulo(const ComputerTokenInfix& m, const char *s):
					ComputerTokenInfix(m)
				{(void)s;}

				std::string		toString() const
				{
					return ("<%>");
				}

				var_t		execute(Computer& data)
				{
					var_t			a = this->get(0)->execute(data);
					var_t			b = this->get(1)->execute(data);

					return (data.allocdVar(*a % *b));
				}

				ComputerToken	*clone(const char *s)
				{	
					return (new OperatorModulo(*this, s));
				}
		};

		/*
		**	()
		*/

		class		OpenParenthesis: public ComputerToken
		{
			bool						_is_prefix;
			std::vector<ComputerToken*>	_args;

			public :

				OpenParenthesis():
					ComputerToken("(", 90),
					_is_prefix(false),
					_args(std::vector<ComputerToken*>())
				{}

				OpenParenthesis(const OpenParenthesis& m, const char *s):
					ComputerToken(m),
					_is_prefix(false),
					_args(std::vector<ComputerToken*>())
				{ (void)s; }

				~OpenParenthesis(void)
				{}

				std::string		toString() const
				{
					if (this->_is_prefix)
						return ("<(...) Prefix Open>");
					return ("<call>");
				}

				var_t			execute(Computer& data)
				{
					size_t					i = 0;
					Function*				fct = NULL;
					std::vector<VarBase*>	send = std::vector<VarBase*>();

					if (this->_is_prefix)
						return (this->get(0)->execute(data));

					while (i < this->_args.size())
					{
						send.push_back( this->_args[i]->execute(data) );
						i++;
					}

					fct = (Function*)this->get(0)->execute(data);
					if (fct)
						return (fct->call(send, data));
					else
						throw ComputerAbortException("Undefined function " + ((ComputerLiteral*)this->get(0))->_name);
					return (fct);
				}

				ComputerToken*	nud(ComputerParserBase& root, Computer& data)
				{
					ComputerToken	*ret = NULL;

					this->_is_prefix = true;
					this->_priority = 0;
					ret = root.expression(data);

					if (!root.getToken() || !root.getToken()->compare(")"))
						throw ComputerAbortException("Unclosed (");

					root.advance();
					return (ret);
				}

				ComputerToken*	led(ComputerParserBase& root, ComputerToken* left, Computer& data)
				{
					this->push_back(left);
					if (!root.getToken()->compare(")"))
						while (true)
						{
							if (root.getToken()->compare(")"))
								break ;
							this->_args.push_back( (ComputerToken*)root.expression(data) );
							if (root.getToken()->compare(")"))
								break ;
							else if (root.getToken()->compare(","))
								root.advance();
							else
								throw ComputerAbortException("')' or ',' Expected");
						}
					root.advance();
					return (this);
				}

				ComputerToken*	clone(const char *s)
				{
					return (new OpenParenthesis(*this, s));
				}	 		
		};

		class		CloseParenthesis: public ComputerToken
		{
			public :

				CloseParenthesis():
					ComputerToken(")", 0)
				{}

				CloseParenthesis(const ComputerToken& m, const char *s):
		 			ComputerToken(m)
				{ (void)s; }

				std::string		toString() const
				{
					return ("<(...) Prefix Close>");
				}

				ComputerToken	*clone(const char *s)
				{
					return (new CloseParenthesis(*this, s));
					(void)s;
				}
		};

		/*
		**	[]
		*/

		class		ArrayOpen: public ComputerToken
		{
			bool							_is_prefix;
			std::vector<ComputerToken*>		_undef;

			public :

				ArrayOpen():
					ComputerToken("[", 90),
					_is_prefix(false),
					_undef(std::vector<ComputerToken*>())
				{}

				ArrayOpen(const ComputerToken& m, const char *s):
					ComputerToken(m),
					_is_prefix(false),
					_undef(std::vector<ComputerToken*>())
				{ (void)s; }

				std::string		toString() const
				{
					std::string	s = "";

					if (this->_is_prefix)
					{
						for (auto it = this->_undef.begin(); it != this->_undef.end(); it++)
						{
							if (s == "")
								s = (*it)->toString();
							else
								s += ", " + (*it)->toString();
						}
						return ("<array=["+s+"]>");
					}
					return ("<[access array]>");
				}

				ComputerToken*	nud(ComputerParserBase &root, Computer &data)
				{
					this->_is_prefix = true;
					this->_priority = 0;
					if (!root.getToken()->compare("]"))
						while (true)
						{
							if (root.getToken()->compare("]"))
								break ;
							this->_undef.push_back( (ComputerToken*)root.expression(data, this->_priority) );
							if (root.getToken()->compare("]"))
								break ;
							else if (root.getToken()->compare(","))
								root.advance();
							else
								throw ComputerAbortException("']' or ',' Expected");
						}
					root.advance();
					std::cout << root.getToken()->toString() << std::endl;
					return (this);
				}

				ComputerToken*	led(ComputerParserBase& root, ComputerToken* left, Computer& data)
				{
					this->push_back( left );
					this->push_back( (ComputerToken*)root.expression(data) );
					std::cout << root.getToken()->toString() << std::endl;
					if (root.getToken()->compare("]"))
						root.advance();
					else
						throw ComputerAbortException("']' Expected");
					return (this);
				}

				var_t			execute(Computer& data)
				{
					if (this->_is_prefix)
						return (this->evalArray(data));
					return (this->evalIndex(data));
				}

				ComputerToken	*clone(const char *s)
				{
					return (new ArrayOpen(*this, s));
				}

			private :

				var_t			evalIndex(Computer& data)
				{
					array_t			a = (array_t)this->get(0)->execute(data);
					decimal_t		b = (decimal_t)this->get(1)->execute(data);						

					return (a->_data[b->_re]);			
				}

				var_t			evalArray(Computer& data)
				{
					size_t		i = 0;
					array_t		a = new Array();

					while (i < this->_undef.size())
					{
						a->_data.push_back( this->_undef[i]->execute(data) );
						i++;
					}
					return (data.allocdVar(a));					
				}
		};

		class		CloseInitializeArray: public ComputerTokenTemp
		{
			public :

				CloseInitializeArray():
					ComputerTokenTemp("]", 0)
				{}

				CloseInitializeArray(const ComputerTokenTemp& m, const char *s):
					ComputerTokenTemp(m)
				{ (void)s; }

				ComputerToken*	nud(ComputerParserBase& root, Computer& data)
				{
					return (this);
					(void)root;
					(void)data;
				}

				ComputerToken*	led(ComputerParserBase& root, ComputerToken& left, Computer& data)
				{
					return (this);
					(void)root;
					(void)left;
					(void)data;
				}

				std::string		toString() const
				{
					return ("<[Array] Close>");
				}

				ComputerToken	*clone(const char *s)
				{
					return (new CloseInitializeArray(*this, s));
					(void)s;
				}
		};

		class		ListSeparator: public ComputerTokenTemp
		{
			public :

				ListSeparator():
					ComputerTokenTemp(",")
				{}

				ListSeparator(const ComputerTokenTemp& m, const char *s):
					ComputerTokenTemp(m)
				{ (void)s; }

				std::string		toString() const
				{
					return ("<,>");
				}

				ComputerToken	*clone(const char *s)
				{
					return (new ListSeparator(*this, s));
				}
	 	};

		class		OpenBodyToken: public ComputerToken
		{
			bool							_is_prefix;

			public :

				OpenBodyToken(void):
					ComputerToken("{", 10),
					_is_prefix(false)
				{}

				OpenBodyToken(const ComputerToken& m, const char *s):
					ComputerToken(m),
					_is_prefix(false)
				{ (void)s; }

				ComputerToken*	nud(ComputerParserBase& root, Computer& data)
				{
					return ( (ComputerToken*)root.expression(data) );
				}

				ComputerToken*	led(ComputerParserBase& root, ComputerToken* left, Computer& data)
				{
					if (!root.getToken()->compare("}"))
						while (!root.end())
						{
							if (root.getToken()->compare("}"))
								break ;
							this->push_back( (ComputerToken*)root.expression(data, this->_priority) );
							if (root.getToken()->compare("}"))
								break ;
							root.advance();
						}
					if (root.end())
						throw ComputerException("Unclosed '}'", &root);
					root.advance();
					return (this);
					(void)left;
				}

				var_t			execute(Computer& data)
				{
					return (this->helperLineEval(data));
				}

				std::string		toString() const
				{
					if (this->_is_prefix == true)
						return ("<body>");
					return ("<corps>");
				}

				ComputerToken*	clone(const char *s)
				{
					return (new OpenBodyToken(*this, s));
				}
		};

		class		CloseBodyToken: public ComputerTokenTemp
		{
			public :

				CloseBodyToken(void):
					ComputerTokenTemp("}", -1)
				{}

				CloseBodyToken(const ComputerTokenTemp& m, const char *s):
					ComputerTokenTemp(m)
				{ (void)s; }

				ComputerToken*	nud(ComputerParserBase& root, Computer& data)
				{
					return (this);
					(void)root;
					(void)data;
				}

				ComputerToken*	led(ComputerParserBase& root, ComputerToken& left, Computer& data)
				{
					return (this);
					(void)root;
					(void)left;
					(void)data;
				}

				std::string		toString() const
				{
					return ("<close body>");
				}

				ComputerToken	*clone(const char *s)
				{
					return (new CloseBodyToken(*this, s));
				}
		};

		class		CallDefinition: public ComputerToken
		{
			std::vector<std::string>		_args;
			std::string						_name;
			Function*						_fct;

			public :

				CallDefinition(void):
					ComputerToken(RegexFunctionDecl, 0, true),
					_args(std::vector<std::string>()),
					_name("")
				{}

				CallDefinition(const ComputerToken& m, const char *s):
					ComputerToken(m),
					_args(std::vector<std::string>()),
					_name("")
				{
					s += 8;
					while (isspace(*s))
						s++;
					while (isdigit(*s) || isalpha(*s) || *s == '_')
					{
						this->_name += *s;
						s++;
					}
					while (isspace(*s))
						s++;
					std::cout << "|>" << std::string(s) << "<|" << std::endl;
					if (*s == '(')
					{
						s++;
						std::string	tmp = "";
						while (*s)
						{
							while (isspace(*s))
								s++;
							while (isdigit(*s) || isalpha(*s) || *s == '_')
							{
								tmp += *s;
								s++;
							}
							while (isspace(*s))
								s++;
							if (*s && *s != ',' && *s != ')')
								throw ComputerException("Cannot parse argument " + std::string(s));
							else if (*s)
								s++;
							if (tmp.length() != 0)
							{
								this->_args.push_back(tmp);
								std::cout << "|> push:" << std::string(tmp) << "<|" << std::endl;
							}
							tmp = "";
						}
					}
					else
						throw ComputerException("Expected argument list");
				}

				ComputerToken*	nud(ComputerParserBase& root, Computer& data)
				{
					ComputerToken	*ret = NULL;

					if (root.getToken()->compare("{"))
						ret = root.expression(data);
					else
						throw ComputerAbortException("'{' (open body) Expected", &root);
					root.advance();
					this->_fct = new Function(
						this->_args,
						ret,
						data.getLookup(),
						this->_name
					);
					ret->debug();
					data.allocdVar(this->_fct, this->_name.c_str());
					return (this);
					(void)root;
					(void)data;
				}

				var_t			execute(Computer& data)
				{
					return (this->_fct);
					(void)data;
				}

				std::string		toString() const
				{
					return ("<function_declaration: "+this->_name+">");
				}

				ComputerToken	*clone(const char *s)
				{
					return (new CallDefinition(*this, s));
				}
		};

		class		CastToken: public ComputerToken
		{
			std::string		_typename;

			public :

				CastToken(std::string name = ""):
					ComputerToken("\\((\\s)*(Integer|Polynome)?(\\s)*\\)", 0, true),
					_typename(name)
				{}

				CastToken(const ComputerToken& m, const char *s):
					ComputerToken(m),
					_typename("")
				{
					std::string		str = "";

					if (*s)
					{
						s++;
						while (*(s + 1))
						{
							if (!isspace(*s))
								this->_typename += *s;
							s++;
						}						
					}
				}

				ComputerToken*	nud(ComputerParserBase& root, Computer& data)
				{
					this->push_back(root.expression(data));
					return (this);
					(void)root;
					(void)data;
				}

				var_t			execute(Computer& data)
				{
					var_t		a = this->get(0)->execute(data);

					std::cerr << "Cast to " << a->toString() << " to " << this->_typename << std::endl;
					if (this->_typename == "Integer")
						return ( data.allocdVar(new VarValue(a)) );
					else if (this->_typename == "Polynome")
						return ( data.allocdVar(new Polynome(a)) );
					std::cerr << "Cannot cast " << a->_name << " to " << this->_typename << std::endl;
					return (a);
				}

				std::string		toString() const
				{
					return ("<cast_to: "+this->_typename+">");
				}

				ComputerToken	*clone(const char *s)
				{
					return (new CastToken(*this, s));
				}
		};
};

#endif