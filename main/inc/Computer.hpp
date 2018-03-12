#ifndef COMPUTER_HPP
# define COMPUTER_HPP
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <Parser.hpp>
# include <map>
# include <Var.hpp>
# include <Program.hpp>
# include <Terminal.hpp>

namespace						fx
{
	class						Computer;
};

namespace						fx::computer
{
	class			ComputerParser: public Parser<fx::computer::VarBase*, fx::computer::Computer&>
	{
		public:

			ComputerParser(void);
	};

	class	Computer:  public Program, public Terminal
	{
		lookup_t								_lookup;

		public :

			bool								ready;
			bool								showvars;
			char								*file;
			ComputerParser						parser;
			std::map<std::string, var_t>		defined;
			std::map<std::string, var_t>		globals;
			int									temp_num;
			std::map<std::string, var_t>		temp;
			std::map<std::string, var_t>		scope;

			Computer ():
				Program(),
				Terminal(),
				ready(false),
				showvars(false),
				file(NULL),
				parser(ComputerParser()),
				defined(std::map<std::string, var_t>()),
				globals(std::map<std::string, var_t>()),
				temp_num(0),
				temp(std::map<std::string, var_t>()),
				scope(std::map<std::string, var_t>())
			{
				this->_lookup.defs = std::map<std::string, VarBase*>();
				this->_lookup.location = std::vector<std::string>();
			}

			Computer (int argc, char **argv):
				Program(argc, argv),
				Terminal(),
				ready(false),
				showvars(false),
				file(NULL),
				parser(ComputerParser()),
				defined(std::map<std::string, var_t>()),
				globals(std::map<std::string, var_t>()),
				temp_num(0),
				temp(std::map<std::string, var_t>()),
				scope(std::map<std::string, var_t>())
			{
				this->_lookup.defs = std::map<std::string, VarBase*>();
				this->_lookup.location = std::vector<std::string>();
				this->parseArgument("--verbose", &Computer::enableVerbose);
				Computer::parseArgument_static("--help", this, std::mem_fn(&Computer::help));
				Computer::parseArgument_static("--showvars", this, std::mem_fn(&Computer::showVars));
				Computer::parseArgument_static("--file", this, std::mem_fn(&Computer::useFile));
				this->ready = true;
			}

			~Computer()
			{
				for (auto it = defined.begin(); it != defined.end(); it++)
					delete it->second;

				for (auto it = temp.begin(); it != temp.end(); it++)
					delete it->second;

				for (auto it = globals.begin(); it != globals.end(); it++)
					delete it->second;
			}

			size_t					useFile(char **argv)
			{
				if (!*(argv + 1))
					this->writeError("File option expected filename");
				else
					this->file = *(argv + 1);
				std::cout << "Use file " << this->file << std::endl;
				return (1);
			}

			size_t					showVars(char **argv)
			{
				this->showvars = true;
				return (0);
				(void)argv;
			}

			size_t					help(char **&argv)
			{
				this->ready = false;
				std::cout << "HELP" << std::endl;
				return (0);
				(void)argv;
			}

			void					start()
			{
				FILE				*fp = NULL;

				if (this->file && (fp = fopen(this->file, "r")))
				{
					std::string			s = "";
					char buf[1024] = {0};
					while (fread(buf, 1, sizeof(buf), fp))
						s += buf;
					fclose(fp);

					this->load_builtin();
					execute_code(s);
				}
				else if (!this->file)
				{
					this->textMode();
					this->load_builtin();
					while (true)
					{
						write(STDOUT_FILENO, "|> ", 3);
						while (true)
						{
							char*			c = this->getInput();

							if (!this->treatInput(c))
							{
								if (c[0] == '\n')
									break ;
							}
						}

						if (!execute_code(this->getInputBuffer()))
							break ;
						this->parser.clear();
						this->resetInput();
					}
					this->reset();
				}
				else
					this->writeError("No such file " + std::string(this->file));
			}

			bool					execute_code(const std::string& s)
			{
				VarBase				*end = NULL;

				try
				{
					end = this->parser.execute(s, *this);
					if (end)
					{
						write(STDOUT_FILENO, "\t", 1);
						std::cout << end->toString() << std::endl;
						//std::cout << "=== > Vars < ===" << std::endl;
						//for (auto it = this->scope.begin(); it != this->scope.end(); it++)
						//	if (it->second)
						//		std::cout << it->first << ": " << it->second->toString() << std::endl;
					}
				}
				catch (ComputerNoticeException& e)
				{
					return (0);
				}
				catch (ComputerException& e)
				{
					return (1);
				}
				return (1);
			}

			lookup_t				*getLookup(void)
			{
				return (&this->_lookup);
			}

			void					pushLocation(std::string loc)
			{
				this->_lookup.location.push_back( loc );
			}

			void					popLocation()
			{
				this->_lookup.location.pop_back();
			}

			VarBase					*getVar(const char *name)
			{
				if (!name)
					throw ComputerException("Unexpected null literal");
				if (this->scope[name])
					return (this->scope[name]);
				else if (this->globals[name])
					return (this->globals[name]);
				return (NULL);
			}

			VarBase					*registerVar(const VarBase& v, const char *name = NULL)
			{
				VarBase				*tmp = v.clone();

				if (name)
				{
					tmp->_name = name;
					this->scope[tmp->_name] = tmp;
					return (this->scope[tmp->_name]);
				}
				else
				{
					this->temp_num++;
					tmp->_name = "temp" + std::to_string(this->temp_num);
					this->temp[tmp->_name] = tmp;
				}

				return (tmp);
			}

			VarBase					*allocdVar(VarBase* tmp, const char *name = NULL)
			{
				if (!tmp)
					return (tmp);

				if (name)
				{
					tmp->_name = name;
					this->defined[tmp->_name] = tmp;
					this->scope[tmp->_name] = tmp;
					return (this->scope[tmp->_name]);
				}
				else
				{
					this->temp_num++;
					tmp->_name = "temp" + std::to_string(this->temp_num);
					this->temp[tmp->_name] = tmp;
					this->scope[tmp->_name] = tmp;
				}

				return (tmp);
			}

			void					load_builtin(void);

		protected :

			Program				*access(void)
			{
				return (this);
			}

		private :

			void				prepare();

	};

};

# include <ComputerToken.hpp>

#endif