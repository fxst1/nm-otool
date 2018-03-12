#ifndef REGEX_HPP
# define REGEX_HPP
# include <string>
# include <stdlib.h>
# include <ctype.h>
# include <exception>

namespace	fx
{

	typedef unsigned int	flag_t;

	class		Regex
	{
		char			*_regex;
		flag_t			_flags;
		int				_rec;
		int				_mrec;
		size_t			_length;

		class	Exception: public std::exception
		{
			char			*_err;

			public :

				Exception(const char *s):
					std::exception(),
					_err((char*)s)
				{}

				Exception(const std::string& s):
					std::exception(),
					_err((char*)s.c_str())
				{}

				~Exception(void)
				{}

				const char *what() const noexcept
				{
					return (this->_err);
				}
		};

		public :

			struct	regex_info
			{
				char		*start;
				size_t		len;
				int			rec;
				flag_t		flags;
				bool		(*fct)(const char *, const char *);
			};


			const static unsigned int		DefaultFlags = 0;
			const static unsigned int		SensitiveCase = 1;
			const static unsigned int		Global = 2;

			const static unsigned int		NoModifier = 0;
			const static unsigned int		ModifierMultiple = 1;
			const static unsigned int		ModifierOneOrNot = 2;
			const static unsigned int		ModifierAny = 3;

			Regex(const std::string& s = "", flag_t flags = fx::Regex::DefaultFlags, int max_recursion = 256):
				_regex((char*)s.c_str()),
				_flags(flags),
				_rec(0),
				_mrec(max_recursion)
			{}

			Regex(const char *s, flag_t flags = fx::Regex::DefaultFlags, int max_recursion = 256):
				_regex((char*)s),
				_flags(flags),
				_rec(0),
				_mrec(max_recursion)
			{}

			~Regex()
			{}

			std::string			getRegex(void) const
			{
				return (this->_regex);
			}

			bool				match(const std::string& s) const
			{
				return (this->match(s.c_str()));
			}

			bool				match(const char *s) const
			{
				struct regex_info	info;

				return (this->match(s, &info));
			}

			bool				match(const std::string& s, struct regex_info *info) const
			{
				return (this->match(s.c_str(), info));
			}

			bool				match(const char *s, struct regex_info *info) const
			{
				bool			ret = false;

				info->len = 0;
				info->start = (char*)s;
				info->flags = this->_flags;
				info->rec = this->_mrec;

				try
				{
					ret = fx::Regex::parseRegex(this->_regex, s, info);
				}
				catch (fx::Regex::Exception& e)
				{
					ret = false;
					std::cerr << e.what() << std::endl;
				}

				if (!ret)
					info->len = 0;

				return (ret);
			}

		private :

			static bool			goNextOccur(const char *s, const char *s2, struct regex_info *info)
			{
				int				open = 0;

				while (*s)
				{
					if (*s == '\\')
						s++;
					else if (*s == '(')
						open++;
					else if (*s == ')')
						open--;
					else if (open < 0 || (open == 0 && *s == '|'))
					{
						info->len = 0;
						return (fx::Regex::parseRegex(s + 1, s2, info));
					}
					s++;
				}
				return (false);
			}

			static bool			isMatchStart(const char *s1, const char *s2)
			{
				return (!*s1);
				(void)s2;
			}

			static bool			isMatchOnce(const char *s1, const char *s2)
			{
				return (!*s1 && !*s2);
			}

			static int			getModifier(const char *s1, int *modif)
			{
				*modif = fx::Regex::NoModifier;
				if (*s1 == '+')
					*modif = fx::Regex::ModifierMultiple;
				else if (*s1 == '?')
					*modif = fx::Regex::ModifierOneOrNot;
				else if (*s1 == '*')
					*modif = fx::Regex::ModifierAny;
				else
					return (0);
				return (1);
			}

			static std::string	split_token_oc(const char *tmp)
			{
				int			open = 1;
				std::string	s = "";

				while (true)
				{
					if (!*tmp)
						break ;
					else if (*tmp == '(')
						open++;
					else if (*tmp == ')')
						open--;

					if (open < 0)
						throw fx::Regex::Exception("Missing metacharacter ')'");
					else if (open != 0)
						s += *tmp;
					else
						break;
					tmp++;
				}
				return (s);
			}

			static bool			parseRegex(const char *s1, const char *s2, struct regex_info *info)
			{
				if (!*s1 || !*s2)
					return (true);
				else if (!*s2)
					return (fx::Regex::goNextOccur(s1, s2, info));

				if (info->rec == 0)
					throw fx::Regex::Exception("Too much recursions");

				info->rec--;

				//printf("Regex = %s\nStr = %s (%zu)\n", s1, s2, info->len);

				if (*s1 == '\\')
				{
					s1++;
					if (*s1 == 'S' || *s1 == 's')
						return (fx::Regex::isSpaceMatch(s1 + 1, s2, info));
					else if (*s1 == 'W' || *s1 == 'w')
						return (fx::Regex::isWordMatch(s1 + 1, s2, info));
					else if (*s1 == 'D' || *s1 == 'd')
						return (fx::Regex::isDigitMatch(s1 + 1, s2, info));
					else if (*s1 == 'b')
						return (fx::Regex::isBlankMatch(s1 + 1, s2, info));
					else
						return (fx::Regex::isBackMatch(s1, s2, info));
				}
				else if (*s1 == '(')
					return (fx::Regex::isSubMatch(s1 + 1, s2, info));
				else if (*s1 == '^')
				{
					if (info->len > 1)
						throw fx::Regex::Exception("Metacharacter '^' should be at the begin of pattern");
					else
						return (Regex::parseRegex(s1 + 1, s2, info));
				}
				else if (*s1 == '$')
				{
					s1++;
					if (*s1 == '|')
					{
						if (*s2 == '\0')
							return (true);
						return (fx::Regex::parseRegex(s1, s2, info));
					}
					else if (*s1 == '\0')
						return (*s2 == '\0');
					else
						throw fx::Regex::Exception("Metacharacter '$' should be at the end of pattern");
				}
				else if (*s1 == '|')
				{
					if (!*s2)
						return (true);

					struct regex_info	i;

					i.len = 0;
					i.start = info->start;
					i.flags = info->flags;
					i.rec = info->rec;
					if (fx::Regex::parseRegex(s1 + 1, info->start, &i))
						info->len = i.len;
					return (true);
				}
				else if (*s1 == '.')
					return (fx::Regex::isAnyMatch(s1 + 1, s2, info));
				else if (*s1 == *s2)
					return (fx::Regex::isCharMatch(s1 + 1, s2, *s1, info));
				return (fx::Regex::goNextOccur(s1, s2, info));				
			}

			static bool	isSubMatch(const char *s1, const char *s2, struct regex_info *info)
			{
				std::string			sub = fx::Regex::split_token_oc(s1);
				int					modif = 0;
				struct regex_info	i;

				i.rec = info->rec;
				i.flags = info->flags;
				i.start = (char*)s2;
				s1 += sub.length() + 1;
				s1 += getModifier(s1, &modif);

				//printf("Sub = %s, Regex = %s, Str = %s (%d)\n", sub.c_str(), s1, s2, modif);

				if (modif == fx::Regex::NoModifier)
				{
					i.len = 0;
					i.start = (char*)s2;
					if (fx::Regex::parseRegex( sub.c_str(), i.start, &i ))
					{
						info->len += i.len;
						return (fx::Regex::parseRegex(s1, i.start + i.len, info));
					}
					return (fx::Regex::goNextOccur(s1, s2, info));
				}
				else if (modif == fx::Regex::ModifierMultiple)
				{
					size_t	n = 0;
					size_t	n_matches = 0;

					while (*(i.start + n))
					{
						i.len = 0;

						if (fx::Regex::parseRegex(sub.c_str(), i.start + n, &i))
						{
							n += i.len;
							n_matches++;
						}
						else
							break ;
					}

					if (n_matches < 1)
						return (fx::Regex::goNextOccur(s1, s2, info));

					info->len += n;
					return (fx::Regex::parseRegex(s1, i.start + n, info));
				}
				else if (modif == fx::Regex::ModifierOneOrNot)
				{
					i.len = 0;

					if (fx::Regex::parseRegex(sub.c_str(), i.start, &i))
					{
						info->len += i.len;
						return (fx::Regex::parseRegex(s1, i.start + i.len, info));
					}
					else
						return (fx::Regex::parseRegex(s1, i.start, info));
				}
				else if (modif == fx::Regex::ModifierAny)
				{
					size_t	n = 0;

					while (*(i.start + n))
					{
						i.len = 0;

						if (fx::Regex::parseRegex(sub.c_str(), i.start + n, &i))
						{
							info->len += i.len;
							return (fx::Regex::parseRegex(s1, i.start + i.len, info));
						}
						n++;
					}
				}

				return (fx::Regex::goNextOccur(s1, s2, info));
			}

			static bool	isClassMatch(const char *s1, const char *s2, int (*match)(int), struct regex_info *info)
			{
				int		modif = 0;

				s1 += fx::Regex::getModifier(s1, &modif);
				if (modif == fx::Regex::NoModifier)
				{
					if ((*match)(*s2))
					{
						info->len++;
						return (fx::Regex::parseRegex(s1, s2 + 1, info));
					}
					else
						return (fx::Regex::goNextOccur(s1, s2, info));
				}
				else if (modif == fx::Regex::ModifierMultiple)
				{
					size_t	n = 0;

					while ((*match)(*s2))
					{
						n++;
						s2++;
					}

					//printf("(%zu) %s\n", n, s2);

					if (n > 0)
					{
						info->len += n;
						return (fx::Regex::parseRegex(s1, s2, info));
					}
					else
						return (fx::Regex::goNextOccur(s1, s2, info));
				}
				else if (modif == fx::Regex::ModifierOneOrNot)
				{
					if ((*match)(*s2))
					{
						info->len++;
						return (fx::Regex::parseRegex(s1, s2 + 1, info));
					}
					else
						return (fx::Regex::parseRegex(s1, s2, info));
				}
				else if (modif == fx::Regex::ModifierAny)
				{
					size_t	tmp = info->len;

					while ((*match)(*s2))
					{
						info->len = tmp;
						if (fx::Regex::parseRegex(s1, s2, info))
							return (true);
						s2++;
					}
				}

				return (fx::Regex::goNextOccur(s1, s2, info));
			}

			static bool	isCharMatch(const char *s1, const char *s2, const char c, struct regex_info *info)
			{
				int		modif = 0;

				s1 += fx::Regex::getModifier(s1, &modif);
				if (modif == fx::Regex::NoModifier)
				{
					if (c == *s2)
					{
						info->len++;
						return (fx::Regex::parseRegex(s1, s2 + 1, info));
					}
					else
						return (fx::Regex::goNextOccur(s1, s2, info));
				}
				else if (modif == fx::Regex::ModifierMultiple)
				{
					size_t	n = 0;

					while (c == *s2)
					{
						n++;
						s2++;
					}
					if (n > 0)
					{
						info->len += n;
						return (fx::Regex::parseRegex(s1, s2, info));
					}
					else
						return (fx::Regex::goNextOccur(s1, s2, info));
				}
				else if (modif == fx::Regex::ModifierOneOrNot)
				{
					if (c == *s2)
					{
						info->len++;
						return (fx::Regex::parseRegex(s1, s2 + 1, info));
					}
					else
						return (fx::Regex::parseRegex(s1, s2, info));
				}
				else if (modif == fx::Regex::ModifierAny)
				{
					size_t	tmp = info->len;

					while (c == *s2)
					{
						info->len = tmp;
						if (fx::Regex::parseRegex(s1, s2, info))
							return (true);
						s2++;
					}
				}
				return (fx::Regex::goNextOccur(s1, s2, info));
			}

			static bool	isSpaceMatch(const char *s1, const char *s2, struct regex_info *info)
			{
				return (fx::Regex::isClassMatch(s1, s2, &isspace, info));
			}

			static bool	isWordMatch(const char *s1, const char *s2, struct regex_info *info)
			{
				return (fx::Regex::isClassMatch(s1, s2, &isalpha, info));
			}

			static bool	isDigitMatch(const char *s1, const char *s2, struct regex_info *info)
			{
				return (fx::Regex::isClassMatch(s1, s2, &isdigit, info));
			}

			static bool	isAlnumMatch(const char *s1, const char *s2, struct regex_info *info)
			{
				return (fx::Regex::isClassMatch(s1, s2, &isalnum, info));
			}

			static bool	isBlankMatch(const char *s1, const char *s2, struct regex_info *info)
			{
				return (fx::Regex::isClassMatch(s1, s2, &isblank, info));
			}

			static bool	isBackMatch(const char *s1, const char *s2, struct regex_info *info)
			{
				if ((*s1 == 'n' && *s2 == '\n') ||
					(*s1 == 'r' && *s2 == '\r') ||
					(*s1 == 't' && *s2 == '\t') ||
					(*s1 == *s2))
				{
					info->len++;
//					return (fx::Regex::parseRegex(s1 + 1, s2 + 1, info));
					return (fx::Regex::isCharMatch(s1 + 1, s2, *s2, info));
				}
				return (fx::Regex::goNextOccur(s1, s2, info));
			}

			static bool	isAnyMatch(const char *s1, const char *s2, struct regex_info *info)
			{
				return (fx::Regex::isClassMatch(s1, s2, &isprint, info));
			}
	};

};

#endif