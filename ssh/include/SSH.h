#ifndef SSH_
#define SSH_
#endif

#include "SSH_utils.h"

class SSH	{
	SSH_utils *ssh_ptr_;
public:
	void connect(const std::string&, const std::string&, const std::string&);
	int execute_command(std::string&, std::string, std::string string_to_wait="", size_t timeout=4);
	void disconnect();
};
