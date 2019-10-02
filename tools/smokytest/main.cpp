
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

#include "probe_uart.h"
#include "probe_uart_safe.h"

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

void print_insta_measure(
        smokyprobe::Probe_UART & probe,
        MeasureType measure_type,
        int max_channel_id)
{
	for (int id = 0; id < max_channel_id; ++id) {
		if (id > 0) std::cout << ",";

		switch (measure_type) {
		case MeasureType::CURRENT:
			std::cout << probe.get_current_A(id);
			break;
		case MeasureType::VOLTAGE:
			std::cout << probe.get_voltage_V(id);
			break;
		case MeasureType::POWER:
			std::cout << probe.get_power_W(id);
			break;
		}
		std::cout << std::endl;
	}
}

void print_avg_measure(
        smokyprobe::Probe_UART & probe,
        MeasureType measure_type,
        unsigned int max_channel_id,
        unsigned int nr_samples)
{
	std::vector<float> avg_values(max_channel_id + 1);
	for (auto & ch_value : avg_values)
		ch_value = 0.0;

	for (unsigned int i = 0; i < nr_samples; ++i) {
		for (int id = 0; id <= max_channel_id; ++id) {
			switch (measure_type) {
			case MeasureType::CURRENT:
				avg_values[id] += probe.get_current_A(id)
				                  / nr_samples;
				break;
			case MeasureType::VOLTAGE:
				avg_values[id] += probe.get_voltage_V(id)
				                  / nr_samples;
				break;
			case MeasureType::POWER:
				avg_values[id] += probe.get_power_W(id)
				                  / nr_samples;
				break;
			}
		}
	}

	for (int id = 0; id <= max_channel_id; ++id) {
		if (id > 0) std::cout << ",";
		std::cout << avg_values[id];
	}
	std::cout << std::endl;
}


void print_energy_for_nsec_exec(
        smokyprobe::Probe_UART & probe,
        unsigned int max_channel_id,
        unsigned int nr_seconds)
{
	std::list<std::thread> tlist;

	for (int id = 0; id <= max_channel_id; ++id) {
		tlist.push_back(
		std::thread([&] () {
			probe.start_energy_sampling(id);
			std::this_thread::sleep_for(
			        std::chrono::seconds(nr_seconds));
			float value = probe.stop_energy_sampling(id);
			std::cout << "[" << id << "] energy: "
			<< value << " J" << std::endl;
		}));
	}

	for (auto & t : tlist)
		t.join();
}

int main(int argc, char **argv)
{
	int opt;
	char file_tty[20];
	unsigned int nr_samples = 10;
	unsigned int nr_seconds = 5;
	int up_to_channel = -1;

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

	// Channel availability
	if (print_info) {
		std::cout << "# Scanning up to channel: " << up_to_channel
		          << std::endl;
		for (unsigned int id = 0; id < up_to_channel; ++id) {
			auto ret = probe.check_channel(id);
			if (ret == smokyprobe::DeviceStatus::OK) {
				std::cout << "Channel: " << id
				          << " = AVAILABLE" << std::endl;
				std::cout << "info: " << probe.get_info(id)
				          << std::endl;
			} else {
				std::cout << "Channel: " << id
				          << " = NOT AVAILABLE "
				          << (int) ret << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	// Values
	if (print_voltage) {
		print_avg_measure(
		        probe, MeasureType::VOLTAGE, up_to_channel, nr_samples);
	}

	if (print_current) {
		print_avg_measure(
		        probe, MeasureType::CURRENT, up_to_channel, nr_samples);
	}

	if (print_power) {
		print_avg_measure(
		        probe, MeasureType::POWER, up_to_channel, nr_samples);
	}

	if (print_energy) {
		print_energy_for_nsec_exec(probe, up_to_channel, nr_seconds);

	}

	return 0;
}
