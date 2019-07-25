
#ifndef __SMOKYPROBE_UART__
#define __SMOKYPROBE_UART__

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include "config.h"
#include "probe.h"
#include "types.h"

#include <termios.h>

#define LOGFILE "/home/giuseppe//Development/embedded/workspace_ac6/SmokyProbe/libsmokyprobe-dev/logprobe.properties"

namespace smokyprobe
{

/**
 * @class Probe_UART
 * @author Politecnico di Milano
 * @date 23/07/19
 * @file uart_probe.h
 * @brief This class provide access to the data coming from
 * 	the probing device.
 */
class Probe_UART: public Probe
{

public:

	/**
	 * @brief Constructor
	 * @param dev_path Path to serial device (e.g., "/dev/ttyACM0")
	 */
	Probe_UART(std::string dev_path);

	Probe_UART() = delete;

	Probe_UART(Probe_UART & other) = delete;

	Probe_UART & operator=(Probe_UART & other) = delete;

	/**
	 * @brief Destructor
	 */
	virtual ~Probe_UART();

	/**
	 * @brief Check the device has been opened
	 * @return true or false accordingly
	 */
	bool is_open() const noexcept override {
		return open_status;
	}

	/**
	 * @brief Check a device channel availability
	 * @param channel_id the channel id number
	 * @return (@see DeviceStatus)
	 * 	SUCCESS if the channel is available.
	 * 	FAULT_COMM if a communication fault has occurred while accessing
	 *	probe sensors.
	 * 	FAULT_MANUFACT if a manufacturing fault has been reported at probe
	 *	sensor level.
	 */
	DeviceStatus check_channel(uint8_t channel_id) override;

	/**
	 * @brief Get information about the probe sensor
	 * @param channel_id the channel id number
	 * @return a description string
	 */
	std::string get_info(uint8_t channel_id) override;

	/**
	 * @brief Get the max number of probing devices (channels)
	 * @return a positive integer number
	 */
	unsigned int get_num_channels_max() const noexcept override {
		return NR_CHANNELS_MAX;
	}

	/**
	 * @brief Get the instantaneous current read
	 * @param channel_id the channel id number
	 * @return the measured current in Ampere
	 */
	float get_current_A(uint8_t channel_id) override;

	/**
	 * @brief Get the instantaneous voltage read
	 * @param channel_id the channel id number
	 * @return the measured current in Volt
	 */
	float get_voltage_V(uint8_t channel_id) override;

	/**
	 * @brief Get the instantaneous power read
	 * @param channel_id the channel id number
	 * @return the measured current in Watt
	 */
	float get_power_W(uint8_t channel_id) override;

	/**
	 * @brief Get the instantaneous read of current, voltage, power and
	 * shunt voltage
	 * @param channel_id the channel id number
	 * @return the measured values
	 */
	std::string get_all(uint8_t channel_id) override;

	/**
	 * @brief Start the energy measure
	 * @param channel_id the channel id number
	 * @return
	 */
	int start_energy_sampling(uint8_t channel_id) override;

	/**
	 * @brief Stop the energy measure
	 * @param channel_id the channel id number
	 * @return the measured energy value in Joule
	 */
	float stop_energy_sampling(uint8_t channel_id) override;

	/**
	 * @brief Simple echo test
	 * @param str a string to send to the device
	 */
	void test_echo(std::string const & str) override;


private:

	/// Logger
	log4cpp::Category & logger;

	///	Serial port device descriptor
	int dev_fd;

	/// Serial port opening status
	bool open_status = false;

	/// Serial port configuration options
	struct termios dev_options;

protected:

	/**
	 * @brief Configure the serial port
	 */
	void config_port();

	/**
	 * @brief Send the request from the host to the device
	 * @param channel_id the channel of the probing device
	 * @param reqcode the code of the request
	 * @param data the optional data to append to the request
	 * @param reply_header the header of the reply message
	 * @return SUCCESS for successful termination.
	 * 	WRITE_ERROR if an error occurred during request transmission.
	 *  READ_ERROR if an error occurred during header reply reception.
	 * 	UNKNOWN_ERROR if an unknown return code has been received.
	 */
	ExitCode send_request(
	    uint8_t channel_id,
	    RequestType reqcode,
	    uint8_t * data,
	    uint8_t * reply_header);

	/**
	 * @brief Receive the actual data as reply of a request
	 * @param reply_header The reply header previously received
	 * @param reply_data The actual reply data
	 * @return SUCCESS for successful termination.
	 */
	ExitCode recv_data(uint8_t * reply_header, uint8_t * reply_data);

	/**
	 * @brief Send a request and get the reply in string form
	 * @param channel_id the channel of the probing device
	 * @param reqtype the request type
	 * @return a string object containing the reply
	 */
	std::string send_request_and_get_data(
	    uint8_t channel_id,
	    RequestType reqtype);

	/**
	 * @brief Send a request and get the reply as floating point value
	 * @param channel_id the channel of the probing device
	 * @param reqtype the request type
	 * @return a floating point value (measure)
	 */
	float send_request_and_get_data_float(
	    uint8_t channel_id,
	    RequestType reqtype);

	/**
	 * @brief Print the content of a data packet
	 * @param msg a prefix text message
	 * @param data the data packet
	 * @param size the size of the data packet
	 */
	void print_packet_content(
	    const char * msg,
	    uint8_t * data,
	    size_t size);

};

} // namespace smokyprobe

#endif // __SMOKYPROBE_UART__
