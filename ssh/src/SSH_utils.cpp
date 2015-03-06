#include "SSH_utils.h"

SSH_utils::SSH_utils (const std::string &user, const std::string &host, const std::string &password):
			user_(user),
			host_(host),
			password_(password),
			print_output_ (false)	{

			int rc;

			// Open session and set options
			my_ssh_session_ = ssh_new();
			if (my_ssh_session_ == NULL)
				exit(EXIT_FAILURE);

			ssh_options_set(my_ssh_session_, SSH_OPTIONS_HOST, host_.c_str());
			ssh_options_set(my_ssh_session_, SSH_OPTIONS_USER, user_.c_str());

			// Connect to server
			rc = ssh_connect(my_ssh_session_);
			if (rc != SSH_OK)	{
				std::cerr << "Error connecting to " + host_ << std::endl;
				exit(EXIT_FAILURE);
			}

			// Verify the server's identity
			if (verify_knownhost() < 0)	{
				exit(EXIT_FAILURE);
			}

			rc = ssh_userauth_password(my_ssh_session_, user_.c_str(), password_.c_str());
			if (rc != SSH_AUTH_SUCCESS)	{
				std::cerr << "Error authenticating with password: " + std::string(password) << std::endl;
				std::cerr << std::string(ssh_get_error(my_ssh_session_)) << std::endl;
				exit(EXIT_FAILURE);
			}

			//open channel
			channel_ = ssh_channel_new(my_ssh_session_);
			if (channel_ == NULL)
				exit(EXIT_FAILURE);

			rc = ssh_channel_open_session(channel_);
			if (rc != SSH_OK)	{
				exit(EXIT_FAILURE);
			}

			rc = ssh_channel_request_shell(channel_);
			if (rc != SSH_OK)	{
				exit(EXIT_FAILURE);
			}
		}

int SSH_utils::verify_knownhost()	{
	int state = ssh_is_server_known(my_ssh_session_);

	ssh_key key;
	int rc;

	if (ssh_get_publickey(my_ssh_session_, &key) != SSH_OK)	{
		std::cerr << "Error: Can't get public key";
	}
	unsigned char *hash;
	size_t hlen;
	rc = ssh_get_publickey_hash(key, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);
	if (rc < 0)
		return 1;

	switch (state)	{
		case SSH_SERVER_KNOWN_OK:
			std::cout << "Host key for server is known" << std::endl;
			break;
		case SSH_SERVER_KNOWN_CHANGED:
			ssh_print_hexa("Host key for server changed: it is now: ", hash, hlen); /* remove old key and renew it */
			return 1;
		case SSH_SERVER_NOT_KNOWN:	{
			char *hexa = ssh_get_hexa(hash, hlen);
			std::cout << "The server is unknown. Do you trust the host key?: " + std::string(hexa) << std::endl;

			std::string buf;
			char temp;
			while (temp != '\n')	{
				temp = std::cin.get();
				buf += temp;
			}
			const char *buf_char_array = buf.c_str();

			if (buf.length() == 1)	{
				return 1;
			}
			if (strncasecmp(buf_char_array, "yes", 3) != 0)	{
				return 1;
			}
			if (ssh_write_knownhost(my_ssh_session_) < 0)	{
				return 1;
			}
			break;
		}
		case SSH_SERVER_ERROR:
			std::cerr << std::string(ssh_get_error(my_ssh_session_)) << std::endl;
			return 1;
		default:
			std::cerr << "Unknown Error" << std::endl;
			return 1;
	}
	return 0;
}

void SSH_utils::enable_print_output()	{
	print_output_ = true;
}

void SSH_utils::reader()	{
	char temp;
	shutdown_flag_.store(false, std::memory_order_relaxed);
	while (!shutdown_flag_.load(std::memory_order_relaxed))	{
		ssh_channel_read(channel_, &temp, sizeof(temp), 0);
		q_buffer_.push(temp);
	}
}

int SSH_utils::execute_command(const std::string &command, const std::string &string_to_wait, size_t timeout)
{
    char temp;
    buf_ = "";
	size_t npos = -1;

    ssh_channel_write(channel_, command.c_str(), command.length());
    ssh_channel_write(channel_, "\n", 1);

    using namespace std::chrono;
    time_point<steady_clock> t1 = steady_clock::now();

    while (q_buffer_.empty())	{}	//wait for first chars in buffer

    while (!q_buffer_.empty())	{
    	temp = q_buffer_.front();
    	q_buffer_.pop();

    	if (print_output_) 	{
    		std::cout << temp;
    	}

    	buf_ += temp;

    	if (buf_.find(string_to_wait) != npos && string_to_wait != "")	{
    		break;
    	}

    	time_point<steady_clock> t2 = steady_clock::now();
    	duration<int> time_span = duration_cast<duration<int>>(t2 - t1);
    	if (time_span.count() == timeout)	{
    		return SSH_ERROR;
    	}
    }
    return SSH_OK;
}

std::string SSH_utils::get_buffer()	{
	return buf_;
}

void SSH_utils::shutdown()	{
	shutdown_flag_.store(true, std::memory_order_relaxed);
}

void SSH_utils::disconnect ()	{
	ssh_channel_close(channel_);
	ssh_channel_free(channel_);
	ssh_disconnect(my_ssh_session_);
	ssh_free(my_ssh_session_);
}
