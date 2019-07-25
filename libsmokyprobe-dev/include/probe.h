

#ifndef __SMOKYPROBE_IF_
#define __SMOKYPROBE_IF_

#include <cstdint>
#include <string>

#include "types.h"

namespace smokyprobe
{

class Probe
{

public:

	Probe(std::string dev_path) { }

	virtual ~Probe() { }

	virtual bool is_open() const noexcept {
		return false;
	}

	virtual	unsigned int get_num_channels_max() const noexcept {
		return 0;
	}

	virtual std::string get_info(uint8_t channel_id) {
		return std::string("probe");
	}

	virtual DeviceStatus check_channel(uint8_t channel_id) {
		return DeviceStatus::OK;
	}

	virtual float get_current_A(uint8_t channel_id) {
		return 0.0;
	}

	virtual float get_voltage_V(uint8_t channel_id) {
		return 0.0;
	}

	virtual float get_power_W(uint8_t channel_id) {
		return 0.0;
	}

	virtual std::string get_all(uint8_t channel_id) {
		return "<missing-values>";
	}

	virtual int start_energy_sampling(uint8_t channel_id) {
		return 0;
	}

	virtual float stop_energy_sampling(uint8_t channel_id) {
		return 0.0;
	}

	virtual void test_echo(std::string const & str) { }

};

} // namespace smokyprobe


#endif // __SMOKYPROBE_IF_
