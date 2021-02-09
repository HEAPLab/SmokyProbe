
#include <chrono>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <thread>

#include <smokyprobe/probe_uart.h>
#include <smokyprobe/probe_uart_safe.h>

void print_help(char * execname)
{
	std::cout << std::endl;
	std::cout << "*** Firmware must be built in interactive mode (-DRUN_MODE_INTERACTIVE) ***" << std::endl;
	std::cout << std::endl;
	std::cout << "Usage: # " << execname << std::endl
	          << "    [-d] /path/to/serial/device " << std::endl
	          << "    [-c] The channel list with a comma separator (e.g, 0,1,2,3) " << std::endl
	          << "    [-s] Start the energy consumption sampling" << std::endl
	          << "    [-f] Stop the energy consumption sampling and print the value " << std::endl
	          << std::endl;
}


std::list<unsigned int> get_channels_from_str(const char * str)
{
	std::string ch_str(str);
	std::list<unsigned int> ch_list;

	size_t end_pos = 0;
	do {
		end_pos = ch_str.find(",");
		std::string token = ch_str.substr(0, end_pos);
		std::cout << token << std::endl;
		std::cout << "token=" << token << std::endl;
		ch_list.push_back(std::stoi(token));
		ch_str = ch_str.substr(end_pos+1);
	} while (end_pos != std::string::npos);

	return ch_list;
}


int main(int argc, char **argv)
{
	int opt;
	char file_tty[20];
	unsigned int nr_samples = 10;
	unsigned int nr_seconds = 5;
	bool stop;

	std::list<unsigned int> channels;

	while ((opt = getopt(argc, argv, "hd:c:sf")) != -1) {
		switch (opt) {
		case 'h':
			print_help(argv[0]);
			return 1;
		case 'd':
			strncpy(file_tty, optarg, 20);
			break;
		case 'c':
			channels = get_channels_from_str(optarg);
			break;
		case 's':
			stop = false;
			break;
		case 'f':
			stop = true;
			break;

		default:
			print_help(argv[0]);
			return 1;
		}
	}


	std::cout << "Opening the SmokyProbe device..." << std::endl;
	smokyprobe::Probe_UART probe(file_tty);
//	smokyprobe::Probe_UART_Safe probe(file_tty);
	if (!probe.is_open()) {
		std::cout << "Probe is not ready!" << std::endl;
		return 1;
	}

	// Smokyprobe selected channels
	if (channels.empty()) {
		std::cout << "Probing all the channels by default " << std::endl;
		for (unsigned int ch_id = 0; ch_id < probe.get_num_channels_max() - 1; ++ch_id)
			channels.push_back(ch_id);
	}

	// Start or stop energy sampling from each channel from the list
	for (auto & ch_id: channels) {
		if (!stop) {
			probe.start_energy_sampling(ch_id);
			std::cout << "[" << ch_id << "] Energy sampling started..." << std::endl;
		}
		else {
			auto e_val = probe.stop_energy_sampling(ch_id);
			std::cout << "[" << ch_id << "] Energy sampling stopped: " << e_val << " J" << std::endl;
		}
	}

	return 0;
}
