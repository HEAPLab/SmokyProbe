
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <string>

#include <chrono>
#include <thread>

#include "probe_uart.h"
#include "probe_uart_safe.h"


void print_help(char * execname) {

	std::cout << "Usage: # " << execname << std::endl
		  << "    [-d] /path/to/serial/device " << std::endl
		  << "    [-n] nr. samplings " << std::endl
		  << "    [-t] running time for energy computation " << std::endl;
}


int main(int argc, char **argv)
{
	int opt;
	char file_tty[20];
	unsigned int count = 10;
	unsigned int running_time_s = 5;

	while ((opt = getopt(argc, argv, "hd:n:t:")) != -1) {
		switch (opt) {
		case 'h':
			print_help(argv[0]);
			return 1;
		case 'd':
			strncpy(file_tty, optarg, 20);
			break;
		case 'n':
			count = atoi(optarg);
			break;
		case 't':
			running_time_s = atoi(optarg);
			break;
		default:
			print_help(argv[0]);
			return 1;
		}
	}

	smokyprobe::Probe_UART probe(file_tty);
//	smokyprobe::Probe_UART_Safe probe(file_tty);
	if (!probe.is_open()) {
		std::cout << "Probe is not ready!" << std::endl;
		return 1;
	}

	// Channel availability
	for (int j = 0; j < probe.get_num_channels_max(); ++j) {
		auto ret = probe.check_channel(j);
		if (ret == smokyprobe::DeviceStatus::OK)
			std::cout << "Channel: " << j << " = AVAILABLE" << std::endl;
		else
			std::cout << "Channel: " << j << " = NOT AVAILABLE " << (int) ret << std::endl;
	}

	// Device info
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "info: "<< probe.get_info(0) << std::endl;

	// Instantaneous measures
	std::cout << std::endl << "=== Getting " << count << " samples of measures..." << std::endl;
	for (int i = 0; i < count; ++i) {
		std::cout << "all: "<< probe.get_all(0) << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	// Enery samples
	std::cout << std::endl << "=== Getting " << count << " energy samples for " 
		  << running_time_s << "s execution..." <<  std::endl;
	for (int i = 0; i < count; ++i) {
		probe.start_energy_sampling(0);
		std::this_thread::sleep_for(std::chrono::seconds(running_time_s));
		float value = probe.stop_energy_sampling(0);
		std::cout << "energy: " << value << " J" << std::endl;
	}

	return 0;
}
