#include "Server.hpp"

// void a(void)
// {
// 	system("leaks server");
// }


int main(int argc, char **argv)
{
	Server server;

	// atexit(a);
	try
	{
		server.input(argc, argv);
		server.run();
	}
	catch(const std::exception& e)
	{
		close(server.getFd());
		std::cerr << e.what() << std::endl;
		return (1);
	}

	return (0);
}
