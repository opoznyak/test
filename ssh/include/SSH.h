#ifndef SSH_
#define SSH_
#endif

#include "SSH_utils.h"

class SSH	{
	SSH_utils *ssh_ptr_;
public:
	void connect(const std::string&, const std::string&, const std::string&);
	int execute_command(std::string&, std::string, std::string, size_t);
	void disconnect();
};
