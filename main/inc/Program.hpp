#ifndef PROGRAM_HPP
# define PROGRAM_HPP
# include <functional>

namespace	fx
{
	class		Program
	{

		public :

			bool		_verbose;
			int			_exit;
			int			_argc;
			int			_remainargc;
			char		**_argv;
			char		**_env;

			Program(void):
				_verbose(false),
				_exit(0),
				_argc(0),
				_remainargc(0),
				_argv(NULL),
				_env(NULL)
			{};

			Program(int argc, char **argv):
				_verbose(false),
				_exit(0),
				_argc(argc),
				_remainargc(argc),
				_argv(argv),
				_env(NULL)
			{};

			Program(int argc, char **argv, char **env):
				_verbose(false),
				_exit(0),
				_argc(argc),
				_remainargc(argc),
				_argv(argv),
				_env(env)
			{};

			char				**getArgs(void)
			{
				return (this->_argv);
			}

			size_t				enableVerbose(char **argv)
			{
				std::cout << "Enable verbose" << std::endl;
				this->_verbose = true;
				(void)argv;
				return (0);
			}

			static void				parseArgument_static(std::string arg, auto obj, auto callback)
			{
				char			**argv = obj->getArgs();
				size_t			tmp = 0;

				if (!argv || obj->_remainargc == 0)
					return ;

				while (*argv)
				{
					if (std::string(*argv) == arg)
					{
						tmp = (callback)(obj, argv);
						obj->_remainargc -= (tmp + 1);
						return ;
					}
					argv++;
				}
			}

		protected :

			void						parseArgument(std::string arg, size_t (Program::*callback)(char**argv))
			{
				Program::parseArgument_static(arg, this, std::mem_fn(callback))	;
			}

			bool						unparsableArgument()
			{
				return (this->_remainargc > 0);
			}

			virtual void				writeError(const std::string s)
			{
				std::cerr << s << std::endl;
				this->help(NULL);
				this->stop();
			}

			virtual void				writeOut(const std::string s)
			{
				if (this->_verbose)
					std::cout << s << std::endl;
			}

			virtual void				stop(void)
			{
				exit(this->_exit);
			}

			virtual size_t				help(char **argv)
			{
				return (0);
				(void)argv;	
			}

/*			virtual size_t				call(size_t (fx::Program::*callback)(char**argv), char **argv)
			{
				return (this->*(callback))(argv);
			}

*/			virtual Program				*access() = 0;

	};
}

#endif