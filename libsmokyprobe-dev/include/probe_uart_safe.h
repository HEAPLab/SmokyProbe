

#ifndef __SMOKYPROBE_UART_SAFE__
#define __SMOKYPROBE_UART_SAFE__

#include "probe_uart.h"
#include "smart_semaphore.h"

namespace smokyprobe
{

/**
 * @class Probe_UART_Safe
 * @author Politecnico di Milano
 * @date 25/07/19
 * @file probe_uart_safe.h
 * @brief Process-safe version of the Probe_UART
 */
class Probe_UART_Safe: public Probe
{

public:

	Probe_UART_Safe(std::string dev_path)
		: Probe(dev_path)
		, __probe(dev_path) {
		this->sem = sem_open("/smokyprobe-uart", O_CREAT, 0644, 1);
	}

	~Probe_UART_Safe() {
		sem_close(this->sem);
	}

	bool is_open() const noexcept override {
		return __probe.is_open();
	}

	DeviceStatus check_channel(uint8_t channel_id) override {
		smart_semaphore s(sem);
		return __probe.check_channel(channel_id);
	}

	std::string get_info(uint8_t channel_id) override {
		smart_semaphore s(sem);
		return __probe.get_info(channel_id);

	}

	unsigned int get_num_channels_max() const noexcept override {
		return __probe.get_num_channels_max();
	}

	float get_current_A(uint8_t channel_id) override {
		smart_semaphore s(sem);
		return __probe.get_current_A(channel_id);
	}

	float get_voltage_V(uint8_t channel_id) override {
		smart_semaphore s(sem);
		return __probe.get_voltage_V(channel_id);
	}

	float get_power_W(uint8_t channel_id) override {
		smart_semaphore s(sem);
		return __probe.get_power_W(channel_id);
	}

	std::string get_all(uint8_t channel_id) override {
		smart_semaphore s(sem);
		return __probe.get_all(channel_id);
	}

	int start_energy_sampling(uint8_t channel_id) override {
		smart_semaphore s(sem);
		return __probe.start_energy_sampling(channel_id);
	}

	float stop_energy_sampling(uint8_t channel_id) override {
		smart_semaphore s(sem);
		return __probe.stop_energy_sampling(channel_id);
	}

	void test_echo(std::string const & str) override {
		smart_semaphore s(sem);
		__probe.test_echo(str);
	}

private:

	Probe_UART __probe;

	sem_t * sem;

};

}

#endif // __SMOKYPROBE_UART_SAFE__
