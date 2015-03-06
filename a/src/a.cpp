#include "a.h"

int main()	{
	SSH ssh;
	std::string buffer;
	ssh.connect("root", "10.10.10.177", "Atl@5plAtform");
//	ssh.execute_command(buffer, "pwd", "\n");
//	std::cout << buffer;
//	ssh.execute_command(buffer, "ps", "");
//	std::cout << buffer;
//	ssh.disconnect();
	return 0;
}


