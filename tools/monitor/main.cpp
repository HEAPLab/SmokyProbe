
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

enum class MeasureType
{
        VOLTAGE,
        CURRENT,
        POWER
};

static int print_voltage;
static int print_current;
static int print_power;
static int print_info;
static int print_energy;


static struct option long_options[] = {
	{ "status", no_argument, &print_info, 1 },
	{ "voltage", no_argument, &print_voltage, 1 },
	{ "current", no_argument, &print_current, 1 },
	{ "power", no_argument, &print_power, 1 },
	{ "energy", no_argument, &print_energy, 1 },
	{ 0, 0, 0 , 0 }
};

void print_help(char * execname)
{
	std::cout << "Usage: # " << execname << std::endl
	          << "    [-d] /path/to/serial/device " << std::endl
	          << "    [-n] number of samples for average computation" << std::endl
	          << "    [-t] running time for energy computation " << std::endl
	          << "    [-c] max channel id number " << std::endl
	          << "    [--voltage] print voltage values" << std::endl
	          << "    [--current] print current values" << std::endl
	          << "    [--power] print power values" << std::endl
	          << "    [--energy] print energy consumption for the given running time"
	          << std::endl;
}


std::list<unsigned int> get_channels_from_string(const char * str)
{
	std::string ch_str(str);
	std::list<unsigned int> ch_list;
/*
	size_t start_pos = 0;
	while () {
		auto end_pos = ch_str.find(",");
		std::string token = s.substr(start_pos, end_pos);
		std::cout << token << std::endl;
		ch_list.push_back(std:stoi(token));
	}
*/

	return ch_list;
}


int main(int argc, char **argv)
{
	int opt;
	char file_tty[20];
	unsigned int nr_samples = 10;
	unsigned int nr_seconds = 5;
	int up_to_channel = -1;

	std::list<unsigned int> channels;

	int option_index;
	while ((opt = getopt_long(
	                      argc, argv, "hd:n:t:c:",
	                      long_options,
	                      &option_index)) != -1) {
		switch (opt) {
		case 0:
			/*
				if (long_options[option_index].flag) {
					std::cout << "--"
					          << long_options[option_index].name
					          << std::endl;
				}
			*/
			break;
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
			nr_seconds = atoi(optarg);
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
	}

	print_info = 1;

	unsigned int id = up_to_channel;

	// Channel availability
	while (nr_samples--) {
//		for (unsigned int id = 0; id < up_to_channel; ++id) {
			auto ret = probe.check_channel(id);
			if (ret == smokyprobe::DeviceStatus::OK) {
				std::cout << "[" << id << "] " 
					<< probe.get_current_A(id) << " "
					<< probe.get_voltage_V(id) << " "
					<< probe.get_power_W(id) << std::endl;
			} 
/*
			else {
				std::cout << "Channel: " << id
				          << " = NOT AVAILABLE "
				          << (int) ret << std::endl;
			}
*/
			std::this_thread::sleep_for(std::chrono::seconds(1));
//		}
	}

	return 0;
}
