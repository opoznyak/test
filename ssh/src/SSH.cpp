#include "SSH.h"

void SSH::connect(const std::string &user, const std::string &host, const std::string &password)	{
	ssh_ptr_ = new SSH_utils(user, host, password);
}

int SSH::execute_command(std::string &buf, std::string command, std::string string_to_wait, size_t timeout)	{
	std::thread t1(&SSH_utils::reader, ssh_ptr_);
	int exit_code = ssh_ptr_->execute_command(command, string_to_wait, timeout);
	buf = ssh_ptr_->get_buffer();
	ssh_ptr_->shutdown();
	t1.detach();
	return exit_code;
}

void SSH::disconnect()	{
	ssh_ptr_->disconnect();
	delete ssh_ptr_;
}

int main()	{
	SSH ssh;
	std::string buffer;
	ssh.connect("root", "10.10.10.177", "Atl@5plAtform");
}
