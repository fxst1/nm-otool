#ifndef PARSER_HPP
# define PARSER_HPP
# include <iostream>
# include <string>
# include <list>
# include <vector>
# include <string.h>
# include <Compare.hpp>
# include <typeinfo>
# include <regex>

# define DEFINE_PARSER_SPEC(IDTYPE, ONEXEC, DATACTRL) ParserBase<ONEXEC,DATACTRL,IDTYPE>

namespace		fx
{
	/*
	**	T : Return type
	**	S : Data type
	**	I : Identifiant type
	*/

	namespace		ParserExceptionType
	{
		enum
		{
			Notice,
			Warning,
			Fatal,
			Abort
		};
	};

	class		ParserIdBase
	{
		public :

			virtual				~ParserIdBase()
			{}

			virtual bool		operator==(const std::string& s) const = 0;
			virtual bool		operator==(const char* s) const = 0;
			virtual bool		operator==(const ParserIdBase& p) const
			{
				return (this == &p);
			}
			virtual size_t		compare(const std::string& s) = 0;
			virtual size_t		compare(const char* s) = 0;
			virtual size_t		length(void) const = 0;
			virtual std::string	toString(void)
			{
				return ("ParserIdBase");
			}
	};

	class		ParserEnclosedId: public ParserIdBase
	{
		public :

			std::string				_id_o;
			std::string				_id_c;
			size_t					_length;

			ParserEnclosedId():
				_id_o(""),
				_id_c(""),
				_length(0)
			{}

			ParserEnclosedId(const ParserEnclosedId& id):
				_id_o(id._id_o),
				_id_c(id._id_c),
				_length(0)
			{}

			ParserEnclosedId(const std::string &id_open, const std::string &id_close):
				_id_o(id_open),
				_id_c(id_close),
				_length(0)
			{}

			ParserEnclosedId(const char *id_open, const char *id_close):
				_id_o(id_open),
				_id_c(id_close),
				_length(0)
			{}

			~ParserEnclosedId()
			{}

			bool	operator==(const std::string& s) const
			{
				return (this->_id_o == s || this->_id_c == s);
			}

			bool	operator==(const char *s) const
			{
				return (*this == std::string(s));
			}

			bool	operator==(const ParserIdBase& p) const
			{

				if (typeid(*this).name() == typeid(p).name())
				{
					ParserEnclosedId& tmp = (ParserEnclosedId&)p;
					return (this->_id_o == tmp._id_o &&
							this->_id_c == tmp._id_c);
				}
				return (false);
			}

			bool	operator!=(const char *s) const
			{
				return !((*this == std::string(s)));
			}

			size_t	compare(const std::string& s)
			{
				return (this->compare(s.c_str()));
			}

			size_t	compare(const char *s)
			{
				size_t	i = 0;
				int		open = 0;

				while (*s)
				{
					if (!strncmp(s, this->_id_o.c_str(), this->_id_o.length()))
						open++;
					else if (!strncmp(s, this->_id_c.c_str(), this->_id_c.length()))
						open--;
					else if (!open)
					{
						this->_length = i;
						return (i);
					}
					i++;
					s++;
				}
				this->_length = 0;
				return (i);
			}

			size_t	length(void) const
			{
				return (this->_length);
			}

			virtual std::string	toString(void)
			{
				return (this->_id_o);
			}
	};

	class		ParserRegexId: public ParserIdBase
	{
		public :

			std::string				_str;
			std::regex				_id;
			size_t					_length;

			ParserRegexId():
				_str(""),
				_id(""),
				_length(0)
			{}
			ParserRegexId(const std::string &id):
				_str("^"+id),
				_id(std::regex(this->_str)),
				_length(0)
			{}

			ParserRegexId(const char *id):
				_str("^"+std::string(id)),
				_id(this->_str),
				_length(0)
			{}

			ParserRegexId(const ParserRegexId& id):
				_str(id._str),
				_id(id._id),
				_length(0)
			{}

			~ParserRegexId()
			{}

			bool	operator==(const std::regex& s) const
			{
				return (false);
				(void)s;
			}

			bool	operator==(const std::string& s) const
			{
				return (this->_str == s);
				(void)s;
			}

			bool	operator==(const char* s) const
			{
				return (this->_str == std::string(s));
				(void)s;
			}

			bool	operator==(const ParserIdBase& p) const
			{
				if (typeid(p).name() == typeid(*this).name())
					return (this->_str == ((ParserRegexId&)p)._str);
				return (false);
			}

			bool	operator!=(const char *s) const
			{
				return (false);
				(void)s;
			}

			size_t	compare(const std::string& s)
			{
				std::smatch	pieces;

				this->_length = 0;
				if (std::regex_search(s, pieces, this->_id))
				{
					if (pieces.size() > 0)
					{
						this->_length = pieces[0].str().length();
						return (this->_length);
					}
				}
				return (this->_length);
			}

			size_t	compare(const char *s)
			{
				return (this->compare(std::string(s)));
			}

			virtual std::string	toString(void)
			{
				return ("Regex");
			}

			bool	operator==(const ParserIdBase& p)
			{
				return (this == &p);
			}

			size_t	length(void) const
			{
				return (this->_length);
			}

	};

	class		ParserStringId: public ParserIdBase
	{
		public :

			std::string				_id;

			ParserStringId():
				_id("")
			{}

			ParserStringId(const std::string &id):
				_id(id)
			{}

			ParserStringId(const char *id):
				_id(std::string(id))
			{}

			ParserStringId(const ParserStringId& id):
				_id(id._id)
			{}

			~ParserStringId()
			{}

			bool	operator==(const std::string& s) const
			{
				return (s == this->_id);
			}

			bool	operator==(const char *s) const
			{
				return (strncmp(s, this->_id.c_str(), this->_id.length()) == 0);
			}

			bool	operator==(const ParserIdBase& p) const
			{
				if (typeid(p).name() == typeid(*this).name())
				{
					ParserStringId&	tmp = (ParserStringId&)p;
					return (tmp._id == this->_id);
				}
				return (false);
			}

			bool	operator!=(const char *s) const
			{
				return (strncmp(s, this->_id.c_str(), this->_id.length()) != 0);
			}

			size_t	compare(const std::string& s)
			{
				return (this->compare(s.c_str()));
			}

			size_t	compare(const char *s)
			{
				if (strncmp(s, this->_id.c_str(), this->_id.length()) == 0)
					return (this->_id.length());
				else
					return (0);
			}

			size_t	length(void) const
			{
				return (this->_id.length());
			}

			virtual std::string	toString(void)
			{
				return (this->_id);
			}

	};

	template<typename T,typename S>
	class		ParserBase;

	template<typename T,typename  S>
	class		ParserBaseException
	{
		public :

			ParserBaseException(std::string str, ParserBase<T,S> *p = NULL, bool show = true):
			s(str)
			{
				if (p)
				{
					p->err = str;
					p->type = ParserExceptionType::Fatal;					
					str += " at line " + std::to_string(p->_line);
				}

				if (show)
					std::cerr << "Exception: " << str << "\n";
			}

			~ParserBaseException()
			{

			}

			const char		*what() const noexcept
			{
				return (this->s.c_str());
			}

		protected :

			std::string		s;
			unsigned int	type;
	};

	template<typename T,typename  S>
	class		ParserBaseNoticeException: public ParserBaseException<T,S>
	{
		public:
			ParserBaseNoticeException(std::string str, ParserBase<T,S> *p = NULL):
			fx::ParserBaseException<T,S>(str, p, false)
			{
				if (p)
					p->type = ParserExceptionType::Notice;
				std::cerr << "Notice: " << str << "\n";
			}

			~ParserBaseNoticeException()
			{

			}		
	};

	template<typename T,typename  S>
	class		ParserBaseWarningException: public ParserBaseException<T,S>
	{
		public:
			ParserBaseWarningException(std::string str, ParserBase<T,S> *p = NULL):
			ParserBaseException<T,S>(str, p, false)
			{
				if (p)
					p->type = ParserExceptionType::Warning;
				std::cerr << "Warning: " << str << "\n";
			}

			~ParserBaseWarningException()
			{

			}		
	};

	template<typename T, typename S>
	class		ParserBaseAbortException: public ParserBaseException<T,S>
	{
		public:
			ParserBaseAbortException(std::string str, ParserBase<T,S> *p = NULL):
			ParserBaseException<T,S>(str, p, false)
			{
				if (p)
					p->type = ParserExceptionType::Abort;
				std::cerr << "Abort: " << str << "\n";
			}

			~ParserBaseAbortException()
			{

			}		
	};

	template<typename T,typename  S>
	class		ParserBaseFatalException: public ParserBaseException<T,S>
	{
		public :

			ParserBaseFatalException(std::string str, ParserBase<T,S> *p = NULL):
			ParserBaseException<T,S>(str, p, false)
			{
				if (p)
					p->type = ParserExceptionType::Fatal;
				std::cerr << "Fatal: " << str << "\n";
			}

			~ParserBaseFatalException()
			{

			}
	};

	template<typename T, typename S>
	class		ParserTokenBase
	{
		public :

			const static int		UseGlobal = 0;
			const static int		UseLedOnly = 1;
			const static int		UseNudOnly = 2;

			const static int		RequestAll = 0;
			const static int		RequestNud = 1;
			const static int		RequestLed = 2;
			const static int		RequestGlobal = 3;

			int						_line;
			int						_col;

			ParserTokenBase<T,S>(const ParserTokenBase<T,S>& id):
				_line(id._line),
				_col(id._col),
				_id(id._id),
				_priority(id._priority),
				_mode(id._mode),
				_request(id._request),
				_temp(id._temp),
				_alloc_d(false),
				_nodes(std::list<ParserTokenBase<T,S>*>())
			{}

			ParserTokenBase<T,S>(const ParserEnclosedId& id, int priority = 0):
				_line(0),
				_col(0),
				_id(new ParserEnclosedId(id)),
				_priority(priority),
				_mode(ParserTokenBase<T,S>::UseGlobal),
				_request(ParserTokenBase<T,S>::RequestAll),
				_temp(false),
				_alloc_d(true),
				_nodes(std::list<ParserTokenBase<T,S>*>())
			{}

			ParserTokenBase<T,S>(const ParserIdBase& id, int priority = 0):
				_line(0),
				_col(0),
				_id((ParserIdBase*)&id),
				_priority(priority),
				_mode(ParserTokenBase<T,S>::UseGlobal),
				_request(ParserTokenBase<T,S>::RequestAll),
				_temp(false),
				_alloc_d(false),
				_nodes(std::list<ParserTokenBase<T,S>*>())
			{}

			ParserTokenBase<T,S>(const ParserStringId& id, int priority = 0):
				_line(0),
				_col(0),
				_id((ParserIdBase*)&id),
				_priority(priority),
				_mode(ParserTokenBase<T,S>::UseGlobal),
				_request(ParserTokenBase<T,S>::RequestAll),
				_temp(false),
				_alloc_d(false),
				_nodes(std::list<ParserTokenBase<T,S>*>())
			{}

			ParserTokenBase<T,S>(const ParserRegexId& id, int priority = 0):
				_line(0),
				_col(0),
				_id((ParserIdBase*)&id),
				_priority(priority),
				_mode(ParserTokenBase<T,S>::UseGlobal),
				_request(ParserTokenBase<T,S>::RequestAll),
				_temp(false),
				_alloc_d(false),
				_nodes(std::list<ParserTokenBase<T,S>*>())
			{}

			ParserTokenBase<T,S>(const std::string& id, int priority = 0, bool is_regex = false)
			{
				this->_line = 0;
				this->_col = 0;
				this->_mode = (ParserTokenBase<T,S>::UseGlobal);
				this->_request = (ParserTokenBase<T,S>::RequestAll);
				this->_temp = false;
				this->_alloc_d = true;
				if (is_regex)
					this->_id = new ParserRegexId(id);
				else
					this->_id = new ParserStringId(id);
				this->_priority = priority;
				this->_nodes = std::list<ParserTokenBase<T,S>*>();
			}

			ParserTokenBase<T,S>(const char *id, int priority = 0, bool is_regex = false)
			{
				this->_line = 0;
				this->_col = 0;
				this->_mode = (ParserTokenBase<T,S>::UseGlobal);
				this->_request = (ParserTokenBase<T,S>::RequestAll);
				this->_temp = false;
				this->_alloc_d = true;
				if (is_regex)
					this->_id = new ParserRegexId(id);
				else
					this->_id = new ParserStringId(id);
				this->_priority = priority;
				this->_nodes = std::list<ParserTokenBase<T,S>*>();
			}

			virtual ~ParserTokenBase<T,S>()
			{
				this->_nodes.clear();
				if (this->_alloc_d)
					delete this->_id;
			}

			ParserTokenBase<T,S>*					helperLineNud(ParserBase<T,S>& parser, S data)
			{
				ParserTokenBase<T,S>*				tmp = NULL;

				while (true)
				{
					tmp = parser.expression(data, this->_priority);
					if (tmp == NULL)
						break ;
					this->push_back(tmp);
				}

				return (this);
			}

			ParserTokenBase<T,S>*					helperLineLed(ParserBase<T,S>& parser, ParserTokenBase<T,S>* left, S data)
			{
				ParserTokenBase<T,S>*				tmp = NULL;

				this->push_back(left);
				while (true)
				{
					tmp = parser.expression(data, this->_priority);
					if (tmp == NULL)
						break ;
					this->push_back(tmp);
				}
				return (this);
			}

			T										helperLineEval(S data)
			{
				T									tmp = NULL;
				T									ltmp = NULL;
				size_t								i = 0;

				while (i < this->size())
				{
					tmp = this->get(i)->execute(data);
					if (!tmp)
						break ;
					else
						ltmp = tmp;	
					i++;
				}
				return (ltmp);
			}

			ParserTokenBase<T,S>*					helperNud(ParserBase<T,S>& parser, S data)
			{
				this->push_back(parser.expression(data, this->_priority));
				return (this);
			}

			ParserTokenBase<T,S>*					helperLed(ParserBase<T,S>& parser, ParserTokenBase<T,S>* left, S data)
			{
				this->push_back(left);
				this->push_back(parser.expression(data, this->_priority));
				return (this);
			}

			const ParserIdBase						&getId(void) const
			{
				return (*this->_id);
			}

			bool									compare(const std::string& s) const
			{
				return (this->_id->compare(s));
			}

			bool									compare(const char *s) const
			{
				return (this->_id->compare(s));
			}

			size_t									length(void) const
			{
				return (this->_id->length());
			}

			int										getPriority() const
			{
				return (this->_priority);
			}

			virtual void							debug(int rec = 0) const
			{
				int									i = 0;

				while (i < rec)
				{
					std::cout << "\t";
					i++;
				}

				std::cout << this->toString() << std::endl;

				for (auto it = this->_nodes.begin(); it != this->_nodes.end(); it++)
					(*it)->debug(rec + 1);
			}

			void									setTemp(bool v)
			{
				this->_temp = v;
			}

			bool									isTemp(void) const
			{
				return (this->_temp);
			}

			void									setMode(int mode)
			{
				this->_mode = mode;
			}

			int										getMode(void) const
			{
				return (this->_mode);
			}

			void									setRequest(int request)
			{
				this->_request = request;
			}

			int										getRequest(void) const
			{
				return (this->_request);
			}

			virtual ParserTokenBase<T,S>*			nud(ParserBase<T,S>& root, S data)
			{
				throw ParserBaseException<T,S>("Syntax error: Undefined token " + this->toString(), &root);
				//return (this);					
				(void)root;
				(void)data;
			}

			virtual ParserTokenBase<T,S>*			led(ParserBase<T,S>& root, ParserTokenBase<T,S>* left, S data)
			{
				throw ParserBaseException<T,S>("Syntax error: Unexpected token " + this->toString(), &root);
				//return (this);					
				(void)root;
				(void)left;
				(void)data;
			}

			/******/

			typename std::list<ParserTokenBase<T,S>*>::iterator						begin()
			{
				return (this->_nodes.begin());
			}

			typename std::list<ParserTokenBase<T,S>*>::iterator						end()
			{
				return (this->_nodes.end());
			}

			bool									empty()
			{
				return (this->_nodes.empty());
			}

			ParserTokenBase<T,S>*					get(size_t index)
			{
				size_t		i = 0;

				for (auto it = this->_nodes.begin(); it != this->_nodes.end() ; it++)
					if (i == index)
						return (*it);
					else
						i++;
				return (NULL);
			}

			ParserTokenBase<T,S>*					front()
			{
				return (this->_nodes.front());
			}

			ParserTokenBase<T,S>*					back()
			{
				return (this->_nodes.back());
			}

			void									push_front(const ParserTokenBase<T,S>* val)
			{
				(this->_nodes.push_front((ParserTokenBase<T,S>*)val));
			}

			void									push_back(const ParserTokenBase<T,S>* val)
			{
				(this->_nodes.push_back((ParserTokenBase<T,S>*)val));
			}

			void									pop_back(const ParserTokenBase<T,S>* val)
			{
				this->_nodes.pop_back((ParserTokenBase<T,S>*)val);
			}

			size_t									size(void)
			{
				return (this->_nodes.size());
			}

			void									merge(std::list<ParserTokenBase<T,S>*>& lst)
			{
				this->_nodes.merge( lst );
			}

			void									merge(std::list<ParserTokenBase<T,S>*>& lst, fx::Compare<ParserTokenBase<T,S>> comp)
			{
				this->_nodes.merge( lst, comp );
			}

			void									remove(const ParserTokenBase<T,S>* val)
			{
				this->_nodes.remove(val);
			}

			void									remove_if(bool (*fct)(const int &))
			{
				this->_nodes.remove_if(fct);
			}

			void									swap(std::list<ParserTokenBase<T,S>*>& lst)
			{
				this->_nodes.swap(lst);
			}

			bool									operator==(const ParserTokenBase<T,S>* ref)
			{
				return (this->_id == ref._id);
			}

			virtual T								execute(S data)
			{
				throw ParserBaseException<T,S>("Parse error: Undefined execution " + this->toString());
				return (T());
				(void)data;
			}

			virtual T								execute(void)
			{
				throw ParserBaseException<T,S>("Parse error: Undefined execution " + this->toString());
				return (T());
			}

			virtual ParserTokenBase<T,S>			*clone(const char *s)
			{
				return (NULL);
				(void)s;
			}

			virtual std::string						toString() const
			{
				return (this->_id->toString());
			}

		protected :

			ParserIdBase*							_id;
			int										_priority;
			int										_mode;
			int										_request;
			bool									_temp;

		private :

			bool									_alloc_d;
			std::list<ParserTokenBase<T,S>*>		_nodes;

	};

	template<typename T, typename S>
	class		ParserTokenBasePrefix: public ParserTokenBase<T,S>
	{
		public :

			ParserTokenBasePrefix<T,S> (const ParserTokenBase<T,S>& id):
				ParserTokenBase<T,S>(id)
			{
				this->setMode(ParserTokenBase<T,S>::UseNudOnly);
			}

			ParserTokenBasePrefix<T,S> (const std::string& id, int priority = 0, bool is_regex = false):
				ParserTokenBase<T,S>(id, priority, is_regex)
			{
				this->setMode(ParserTokenBase<T,S>::UseNudOnly);
			}

			ParserTokenBasePrefix<T,S> (const char *id, int priority = 0, bool is_regex = false):
				ParserTokenBase<T,S>(id, priority, is_regex)
			{
				this->setMode(ParserTokenBase<T,S>::UseNudOnly);
			}

			ParserTokenBasePrefix<T,S> (const ParserIdBase& id, int priority = 0):
				ParserTokenBase<T,S>(id, priority)
			{
				this->setMode(ParserTokenBase<T,S>::UseNudOnly);
			}

			virtual ~ParserTokenBasePrefix<T,S>()
			{}

			virtual ParserTokenBase<T,S>*				nud(ParserBase<T,S>& parser, S data)
			{
				this->push_back(parser.expression(data, this->_priority));
				return (this);
			}

	};

	template<typename T, typename S>
	class		ParserTokenBaseInfix: public ParserTokenBase<T,S>
	{
		public :

			ParserTokenBaseInfix<T,S> (const ParserTokenBase<T,S>& id):
				ParserTokenBasePrefix<T,S>(id)
			{
				this->setMode(ParserTokenBase<T,S>::UseLedOnly);
			}

			ParserTokenBaseInfix<T,S> (const std::string& id, int priority = 0, bool is_regex = false):
				ParserTokenBase<T,S>(id, priority, is_regex)
			{
				this->setMode(ParserTokenBase<T,S>::UseLedOnly);
			}

			ParserTokenBaseInfix<T,S> (const char *id, int priority = 0, bool is_regex = false):
				ParserTokenBase<T,S>(id, priority, is_regex)
			{
				this->setMode(ParserTokenBase<T,S>::UseLedOnly);
			}

			ParserTokenBaseInfix<T,S> (const ParserIdBase& id, int priority = 0):
				ParserTokenBase<T,S>(id, priority)
			{
				this->setMode(ParserTokenBase<T,S>::UseLedOnly);
			}

			ParserTokenBase<T,S>*				led(ParserBase<T,S>& parser, ParserTokenBase<T,S>* left, S data)
			{
				this->push_back(left);
				this->push_back(parser.expression(data, this->_priority));
				return (this);
				(void)data;
			}

	};


	template<typename T, typename S>
	class		ParserTokenBaseInfixR: public ParserTokenBase<T,S>
	{
		public :

			ParserTokenBaseInfixR<T,S> (const ParserTokenBase<T,S>& id):
				ParserTokenBase<T,S>(id)
			{
				this->setMode(ParserTokenBase<T,S>::UseLedOnly);
			}

			ParserTokenBaseInfixR<T,S> (const std::string& id, int priority = 0, bool is_regex = false):
				ParserTokenBase<T,S>(id, priority, is_regex)
			{
				this->setMode(ParserTokenBase<T,S>::UseLedOnly);
			}

			ParserTokenBaseInfixR<T,S> (const char *id, int priority = 0, bool is_regex = false):
				ParserTokenBase<T,S>(id, priority, is_regex)
			{
				this->setMode(ParserTokenBase<T,S>::UseLedOnly);
			}

			ParserTokenBaseInfixR<T,S> (const ParserIdBase& id, int priority = 0):
				ParserTokenBase<T,S>(id, priority)
			{
				this->setMode(ParserTokenBase<T,S>::UseLedOnly);
			}

			ParserTokenBase<T,S>*				led(ParserBase<T,S>& parser, ParserTokenBase<T,S>* left, S data)
			{
				this->push_back(left);
				this->push_back(parser.expression(data, this->_priority - 1));
				return (this);
				(void)data;
			}

	};

	template<typename T, typename S>
	class		ParserTokenBaseLiteral: public ParserTokenBase<T,S>
	{
		public :

			ParserTokenBaseLiteral<T,S> (const ParserTokenBase<T,S>& id):
				ParserTokenBase<T,S>(id)
			{
				this->setMode(ParserTokenBase<T,S>::UseNudOnly);
//				this->setRequest(ParserTokenBase<T,S>::RequestLed);
			}

			ParserTokenBaseLiteral<T,S> (const std::string& id, int priority = 0):
				ParserTokenBase<T,S>(id, priority, true)
			{
				this->setMode(ParserTokenBase<T,S>::UseNudOnly);
//				this->setRequest(ParserTokenBase<T,S>::RequestLed);
			}

			ParserTokenBaseLiteral<T,S> (const char *id, int priority = 0):
				ParserTokenBase<T,S>(id, priority, true)
			{
				this->setMode(ParserTokenBase<T,S>::UseNudOnly);
//				this->setRequest(ParserTokenBase<T,S>::RequestLed);
			}

			ParserTokenBaseLiteral<T,S> (const ParserRegexId& id, int priority = 0):
				ParserTokenBase<T,S>(id, priority)
			{
				this->setMode(ParserTokenBase<T,S>::UseNudOnly);
//				this->setRequest(ParserTokenBase<T,S>::RequestLed);
			}

			//virtual ~ParserTokenBaseLiteral<T,S>()
			//{}

			ParserTokenBase<T,S>*				nud(ParserBase<T,S>& parser, S data)
			{
				return (this);
				(void)parser;
				(void)data;
			}

			ParserTokenBase<T,S>*				led(ParserBase<T,S>& parser, ParserTokenBase<T,S>* left, S data)
			{
				return (this);
				(void)parser;
				(void)left;
				(void)data;
			}

	};

	template<typename T, typename S>
	class		ParserTokenBaseTemp: public ParserTokenBase<T,S>
	{
		public :

			ParserTokenBaseTemp<T,S> (const ParserTokenBase<T,S>& id):
				ParserTokenBase<T,S>(id)
			{
				this->setTemp(true);
			}

			ParserTokenBaseTemp<T,S> (const std::string& id, int priority = 0):
				ParserTokenBase<T,S>(id, priority, true)
			{
				this->setTemp(true);
			}

			ParserTokenBaseTemp<T,S> (const char *id, int priority = 0):
				ParserTokenBase<T,S>(id, priority, true)
			{
				this->setTemp(true);
			}

			ParserTokenBaseTemp<T,S> (const ParserRegexId& id, int priority = 0):
				ParserTokenBase<T,S>(id, priority)
			{
				this->setTemp(true);
			}

			//virtual ~ParserTokenBaseLiteral<T,S>()
			//{}

			ParserTokenBase<T,S>*				nud(ParserBase<T,S>& parser, S data)
			{
				return (this);
				(void)parser;
				(void)data;
			}

			ParserTokenBase<T,S>*				led(ParserBase<T,S>& parser, ParserTokenBase<T,S>* left, S data)
			{
				return (this);
				(void)parser;
				(void)left;
				(void)data;
			}

	};

	template<typename T, typename S>
	class		ParserTokenScope: public ParserTokenBase<T,S>
	{
		public :

			ParserTokenScope<T,S> (const ParserTokenBase<T,S>& id):
				ParserTokenBase<T,S>(id)
			{
				this->setMode(ParserTokenBase<T,S>::UseGlobal);
				this->setRequest(ParserTokenBase<T,S>::RequestNud);
			}

			ParserTokenScope<T,S> (const std::string& id, int priority = 0):
				ParserTokenBase<T,S>(id, priority, false)
			{
				this->setMode(ParserTokenBase<T,S>::UseGlobal);
				this->setRequest(ParserTokenBase<T,S>::RequestNud);
			}

			ParserTokenScope<T,S> (const char *id, int priority = 0):
				ParserTokenBase<T,S>(id, priority, false)
			{
				this->setMode(ParserTokenBase<T,S>::UseGlobal);
				this->setRequest(ParserTokenBase<T,S>::RequestNud);
			}

			ParserTokenScope<T,S> (const ParserStringId& id, int priority = 0):
				ParserTokenBase<T,S>(id, priority)
			{
				this->setMode(ParserTokenBase<T,S>::UseGlobal);
				this->setRequest(ParserTokenBase<T,S>::RequestNud);
			}

			//virtual ~ParserTokenBaseLiteral<T,S>()
			//{}

			ParserTokenBase<T,S>*				nud(ParserBase<T,S>& parser, S data)
			{
				ParserTokenBase<T,S>			*tmp = NULL;

				while (true)
				{
					tmp = parser.expression(data, this->_priority);
					if (tmp == NULL)
						break ;
					this->push_back(tmp);
				}

				return (this);
			}

			ParserTokenBase<T,S>*				led(ParserBase<T,S>& parser, ParserTokenBase<T,S>* left, S data)
			{
				ParserTokenBase<T,S>			*tmp = NULL;

				this->push_back(left);
				while (true)
				{
					tmp = parser.expression(data, this->_priority);
					if (tmp == NULL)
						break ;
					this->push_back(tmp);
				}

				return (this);
			}

			T									execute(S data)
			{
				T			tmp = NULL;
				T			ltmp = NULL;
				size_t		i = 0;

				while (i < this->size())
				{
					tmp = this->get(i)->execute(data);
					if (!tmp)
						break ;
					else
						ltmp = tmp;	
					i++;
				}

				return (ltmp);
			}
	};

	template<typename T, typename S>
	class		ParserBase
	{
		public :

			int				_line;
			int				_col;
			int				priority;
			unsigned int	type;
			std::string		err;

			typename std::vector<ParserTokenBase<T,S>*>::iterator	it;
			std::list<ParserTokenBase<T,S>*>	_g_symbols;
			std::list<ParserTokenBase<T,S>*>	_nud_symbols;
			std::list<ParserTokenBase<T,S>*>	_led_symbols;
			std::vector<ParserTokenBase<T,S>*>	_unused;
			std::vector<ParserTokenBase<T,S>*>	_tokens;
			ParserTokenBase<T,S>*				_root;

			ParserBase<T,S>(void):
				_line(0),
				_col(0),
				priority(0),
				_g_symbols(std::list<ParserTokenBase<T,S>*>()),
				_nud_symbols(std::list<ParserTokenBase<T,S>*>()),
				_led_symbols(std::list<ParserTokenBase<T,S>*>()),
				_unused(std::vector<ParserTokenBase<T,S>*>()),
				_tokens(std::vector<ParserTokenBase<T,S>*>())
			{
				this->_root = NULL;
			}

			ParserBase<T,S>(const ParserBase<T,S>& p):
				_line(p._line),
				_col(p._col),
				priority(0),
				_g_symbols(std::list<ParserTokenBase<T,S>*>()),
				_nud_symbols(std::list<ParserTokenBase<T,S>*>()),
				_led_symbols(std::list<ParserTokenBase<T,S>*>()),
				_unused(std::vector<ParserTokenBase<T,S>*>()),
				_tokens(std::vector<ParserTokenBase<T,S>*>())
			{
				this->_root = NULL;
				for (auto it = p._g_symbols.begin(); it != p._g_symbols.end(); it++)
					this->_g_symbols.push_back((*it)->clone(""));
			}

			virtual ~ParserBase<T,S>()
			{
				for (auto it = this->_g_symbols.begin(); it != this->_g_symbols.end(); it++)
					if (*it != NULL)
						delete (*it);

				for (auto it = this->_led_symbols.begin(); it != this->_led_symbols.end(); it++)
					if (*it != NULL)
						delete (*it);

				for (auto it = this->_nud_symbols.begin(); it != this->_nud_symbols.end(); it++)
					if (*it != NULL)
						delete (*it);
				
				this->_g_symbols.clear();
				this->_led_symbols.clear();
				this->_nud_symbols.clear();
				for (auto it = this->_tokens.begin(); it != this->_tokens.end(); it++)
					if (*it != NULL)
						delete (*it);
			}

			void							garbage_temp(ParserTokenBase<T,S>* tok = NULL)
			{
				if (!tok)
					tok = this->getToken();

				if (tok && tok->isTemp())
					this->_unused.push_back(tok);
			}

			virtual ParserTokenBase<T,S>*	expression(S data, int priority = 0)
			{
				ParserTokenBase<T,S>*		tok = NULL;
				ParserTokenBase<T,S>*		left = NULL;

				this->priority = (int)priority;

				if (this->it == this->_tokens.end())
					return (NULL);

			//	while (this->it != this->_tokens.end())
				{
					tok = *(this->it);
					this->advance();
					left = tok->nud(*this, data);
			//		if (left)
			//			break ;
				}

				while (left && this->it != this->_tokens.end() &&
						priority < (*this->it)->getPriority())
				{
					tok = *(this->it);
					this->advance();
					left = tok->led(*this, left, data);
				}
				return (left);
			}

			bool							end()
			{
				return (this->it == this->_tokens.end());
			}

			void							advance()
			{
				if (this->it != this->_tokens.end())
					this->it++;
				else
					std::cerr << "LAST TOKEN" << std::endl;
			}

			ParserTokenBase<T,S>*			getToken(void)
			{
				if (this->it != this->_tokens.end())
					return (*this->it);
				return (NULL);
			}

			void							find(const std::string id)
			{
				while (this->it != this->_tokens.end() &&
						!(*this->it)->compare(id))
					this->it++;
			}

			void							useToken(ParserTokenBase<T,S>* tok)
			{
				if (tok)
					this->_g_symbols.push_back( tok );
			}

			std::list<ParserTokenBase<T,S>*>					copyTokens(void) const
			{
				std::list<ParserTokenBase<T,S>*>				lst = std::list<ParserTokenBase<T,S>*>();

				for (auto it = this->_g_symbols.begin(); it != this->_g_symbols.end(); it++)
					lst.push_back( (*it)->clone("") );
				return (lst);
			}

			void							init(const std::string s)
			{
				this->tokenize(s);
				this->it = this->_tokens.begin();
			}

			T								execute(const std::string s, S data)
			{
				this->tokenize(s);
				this->it = this->_tokens.begin();
				this->_root = this->expression(data);
				if (this->_root)
				{
					//std::cout << "================" << std::endl;
					//this->_root->debug();
					//std::cout << "================" << std::endl;
					if (this->it != this->_tokens.end())
						throw ParserBaseException<T,S> ("Cannot parse `" + (*this->it)->toString() + "`", this);
					return (this->_root->execute( data ));
				}
				return (NULL);
			}

			void							clear(void)
			{
				this->_root = NULL;
				for (auto it = this->_tokens.begin(); it != this->_tokens.end(); it++)
					delete (*it);
				this->_tokens.clear();
			}

		private :

			void							addToken(ParserTokenBase<T,S>* tok)
			{
				if (tok)
					this->_tokens.push_back( tok );
			}

			size_t							parseSymbol(const char *str)
			{
				size_t						tmp = 0;

				for (auto it = this->_g_symbols.begin(); it != this->_g_symbols.end(); it++)
				{
					tmp = (*it)->compare(str);
					if (tmp > 0)
					{
						std::string				s = std::string(str).substr(0, (*it)->length()).c_str();
						char*					stmp = (char*)s.c_str();
						ParserTokenBase<T,S>*	tok = (*it)->clone(s.c_str());

						tok->_line = this->_line;
						tok->_col = this->_col;
						while (*stmp)
						{
							if (*stmp == '\n')
							{
								this->_line++;
								this->_col = 0;						
							}
							else
								this->_col++;
							stmp++;
						}
						this->addToken(tok);
						return ((*it)->length());
					}
				}
				return (0);
			}

		protected :

			virtual void	tokenize(const char *str)
			{
				if (this->_g_symbols.size() == 0)
					throw ParserBaseFatalException<T,S>("No symbols");
				this->_line = 1;
				this->_col = 1;
				while (*str)
				{
					while (*str == '\n' || *str == '\t' || *str == ' ')
					{
						if (*str == '\n')
						{
							this->_line++;
							this->_col = 0;						
						}
						else
							this->_col++;
						str++;
					}
					if (*str)
					{
						size_t		len = this->parseSymbol(str);

						if (!len)
						{
							std::string s = "Cannot parse `";
							s += *str;
							s += "` (";
							s += std::to_string(*str);
							s += ")";

							throw ParserBaseFatalException<T,S>(s);
						}
						else
							str += (len <= strlen(str)) ? len : strlen(str);
					}
				}
			}

			virtual void	tokenize(const std::string str)
			{
				this->tokenize(str.c_str());
			}

	};

	template<typename T, typename S>
	using Parser = ParserBase<T,S>;

	template<typename T, typename S>
	using ParserToken = ParserTokenBase<T,S>;

	template<typename T, typename S>
	using ParserTokenPrefix = ParserTokenBasePrefix<T,S>;

	template<typename T, typename S>
	using ParserTokenInfix = ParserTokenBaseInfix<T,S>;

	template<typename T, typename S>
	using ParserTokenInfixR = ParserTokenBaseInfixR<T,S>;

	template<typename T, typename S>
	using ParserTokenLiteral = ParserTokenBaseLiteral<T,S>;

};


#endif