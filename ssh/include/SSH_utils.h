#ifndef SSH_UTILS
#define SSH_UTILS
#endif

#include <iostream>
#include <libssh/libssh.h>
#include <cstring>
#include <queue>
#include <thread>
#include <atomic>
#include <chrono>

class SSH_utils	{
		const std::string user_;
		const std::string host_;
		const std::string password_;

		ssh_session my_ssh_session_;
		ssh_channel channel_;

		std::atomic<bool> shutdown_flag_;
		std::queue<char> q_buffer_;

		bool print_output_;
		std::string buf_;

		int verify_knownhost();
	public:
		SSH_utils (const std::string &user,
				   const std::string &host,
				   const std::string &password);

		void enable_print_output();
		void reader();
		int execute_command(const std::string &, const std::string &, size_t);
		std::string get_buffer();
		void shutdown();
		void disconnect ();
	};
