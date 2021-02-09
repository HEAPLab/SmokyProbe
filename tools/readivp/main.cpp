
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


#include <probe_uart.h>
#include <probe_uart_safe.h>

#include "common.h"

using namespace smokyutil;

void print_help(char * execname)
{
	std::cout << std::endl;
	std::cout << "*** Firmware must be built in interactive mode (-DRUN_MODE_INTERACTIVE) ***" << std::endl;
	std::cout << std::endl;
	std::cout << "Usage: # " << execname << std::endl
	          << "    [-d] /path/to/serial/device " << std::endl
	          << "    [-c] The channel list with a comma separator (e.g, 0,1,2,3) " << std::endl
	          << "    [-n] number of samples to return" << std::endl
	          << "    [-t] period [milliseconds] between samplings" << std::endl
	          << std::endl;
}

int main(int argc, char **argv)
{
	int opt;
	char file_tty[20];
	unsigned int nr_samples = 10;
	unsigned int period_ms  = 1000;
	std::list<unsigned int> channels;

	while ((opt = getopt(argc, argv, "hd:c:n:t:")) != -1) {
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
		case 'n':
			nr_samples = atoi(optarg);
			break;
		case 't':
			period_ms = atoi(optarg);
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
		for (unsigned int ch_id = 0; ch_id < probe.get_num_channels_max(); ++ch_id)
			channels.push_back(ch_id);
	}

	std::cout << "Channel	Current	Voltage	Power " << std::endl;
	while (nr_samples--) {
		for (auto & ch_id: channels) {
			std::cout << "[" << ch_id << "]"
				<< " " << probe.get_current_A(ch_id)
				<< " " << probe.get_voltage_V(ch_id)
				<< " " << probe.get_power_W(ch_id)
				<< std::endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(period_ms));
	}

	return 0;
}
