#include <Computer.hpp>

using namespace		fx::computer;

static VarBase*		builtin_echo(Args args, Computer& data)
{
	std::string		s = "";

	for (auto it = args.begin(); it != args.end(); it++)
		if (!(*it))
			s += s.length() > 0 ? ", (nil)" : "(nil)";
		else
		{
			String	tmp = String(*it);

			s += s.length() > 0 ? ", " + tmp._data : tmp._data;
		}
	std::cout << s << std::endl;
	return (data.allocdVar(new String(s)));
	(void)data;
}

static VarBase*		builtin_input(Args args, Computer& data)
{
	std::string		s = "";
	char			c = 0;

	write(STDOUT_FILENO,  "|> ", 3);
	while (read(STDIN_FILENO, &c, sizeof(c)) > 0 && c != '\n')
	{
		s += c;
		c = 0;
	}
	return (data.allocdVar(new String(s)));
	(void)args;
	(void)data;
}

static VarBase*		builtin_exit(Args args, Computer& data)
{
	throw ComputerNoticeException("Exiting");
	return (NULL);
	(void)args;
	(void)data;
}

void				Computer::load_builtin(void)
{
	this->globals["exit"] = new FunctionSys(&builtin_exit);
	this->globals["echo"] = new FunctionSys(&builtin_echo);
	this->globals["input"] = new FunctionSys(&builtin_input);
}
