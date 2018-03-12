#include <Computer.hpp>

using namespace fx::computer;

int main(int argc, char **argv)
{
	Computer	c = Computer(argc, argv);

	if (c.ready)
		c.start();

	return (1);
}