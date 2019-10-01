
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>

#include <chrono>
#include <thread>

#include "probe_uart.h"
#include "probe_uart_safe.h"


void print_help(char * execname)
{
	std::cout << "Usage: # " << execname << std::endl
	          << "    [-d] /path/to/serial/device " << std::endl
	          << "    [-n] nr. samplings " << std::endl
	          << "    [-t] running time for energy computation "
	          << "    [-c] max channel id number " << std::endl;

}


void print_insta_voltage(smokyprobe::Probe_UART & probe, int max_channel_id)
{
	// Instantaneous measures
//	std::cout << "Print voltage values from 0 to "
//	                << max_channel_id << std::endl;
	for (int id = 0; id < max_channel_id; ++id) {
		if (id > 0) std::cout << ",";
		std::cout << probe.get_voltage_V(id);
//		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::cout << std::endl;
}

void print_avg_voltage(
        smokyprobe::Probe_UART & probe,
        unsigned int max_channel_id,
        unsigned int nr_samples)
{
	std::vector<float> avg_values(max_channel_id + 1);
	for (auto & ch_value : avg_values)
		ch_value = 0.0;

	for (unsigned int i = 0; i < nr_samples; ++i) {
		for (int id = 0; id <= max_channel_id; ++id) {
			avg_values[id] += probe.get_voltage_V(id) / nr_samples;
		}
	}

	for (int id = 0; id <= max_channel_id; ++id) {
		if (id > 0) std::cout << ",";
		std::cout << avg_values[id];
	}
	std::cout << std::endl;
}


void print_energy(
        smokyprobe::Probe_UART & probe,
        unsigned int channel_id,
        unsigned int nr_samples,
        unsigned int running_time_s)
{
	// Energy samples
	std::cout << std::endl << "=== Getting "
	          << nr_samples << " energy samples for "
	          << running_time_s << "s execution..." <<  std::endl;

	for (int i = 0; i < nr_samples; ++i) {
		probe.start_energy_sampling(channel_id);
		std::this_thread::sleep_for(std::chrono::seconds(running_time_s));
		float value = probe.stop_energy_sampling(channel_id);
		std::cout << "energy: " << value << " J" << std::endl;
	}
}

int main(int argc, char **argv)
{
	int opt;
	char file_tty[20];
	unsigned int nr_samples = 10;
	unsigned int running_time_s = 5;
	int up_to_channel = -1;

	while ((opt = getopt(argc, argv, "hd:n:t:c:")) != -1) {
		switch (opt) {
		case 'h':
			print_help(argv[0]);
			return 1;
		case 'd':
			strncpy(file_tty, optarg, 20);
			break;
		case 'n':
			nr_samples = atoi(optarg);
			break;
		case 't':
			running_time_s = atoi(optarg);
			break;
		case 'c':
			up_to_channel = atoi(optarg);
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

	if (up_to_channel < 0) {
		up_to_channel = probe.get_num_channels_max() - 1;
//		std::cout << "# Scanning up to channel: "
//		          << up_to_channel << std::endl;
	}
	/*
		// Channel availability
		for (unsigned int id = 0; id < up_to_channel; ++id) {
			auto ret = probe.check_channel(id);
			if (ret == smokyprobe::DeviceStatus::OK) {
				std::cout << "Channel: " << id << " = AVAILABLE" << std::endl;
	//			std::cout << "info: " << probe.get_info(id) << std::endl;
			} else {
				std::cout << "Channel: " << id << " = NOT AVAILABLE "
				          << (int) ret << std::endl;
			}

	//		std::this_thread::sleep_for(std::chrono::seconds(2));
		}
	*/
	print_avg_voltage(probe, up_to_channel, nr_samples);

	return 0;
}
