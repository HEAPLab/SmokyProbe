
#include "probe_uart.h"

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <chrono>
#include <iostream>
#include <thread>

#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

namespace smokyprobe
{


void Probe_UART::print_packet_content(
    const char * msg,
    uint8_t * data,
    size_t size)
{
	std::stringstream data_stream;
	for (int i = 0; i < size; ++i) {
		data_stream << " " << static_cast<int>(data[i]);
	}
	std::string ds(data_stream.str());
	logger.debug("%s: %s", msg, ds.c_str());
}


Probe_UART::Probe_UART(std::string dev_path):
	Probe(dev_path),
	logger(log4cpp::Category::getInstance(std::string("probe_uart")))
{
	// Logger
	try {
		log4cpp::PropertyConfigurator::configure(std::string(LOGFILE));
	} catch(log4cpp::ConfigureFailure & ex) {
		std::cout << "probe_uart: missing log4cpp file!" << std::endl;
	}

	// Serial device
	this->dev_fd = open(dev_path.c_str(),
	                    O_RDWR | O_NOCTTY | O_SYNC | O_RSYNC | O_DSYNC);
	if (this->dev_fd > 0) {
		config_port();
		open_status = true;
	} else {
		perror("Probe_UART opening");
		logger.error("error while opening UART device");
	}

	// Check pending bytes....
	const struct timespec tmo_p = {
		.tv_sec = 2,
		.tv_nsec = 0
	};

	struct pollfd fds[1];
	fds[0].fd = this->dev_fd;
	fds[0].events  = 0;
	fds[0].revents = 0;

	logger.debug("checking pending bytes... ");
	int nb = 0;
	unsigned char buff[3];
	while ( ppoll(fds, 1, &tmo_p, NULL) > 0 ) {
		nb += read(this->dev_fd, buff, 3);
	}
	logger.debug("found %d pending bytes... ", nb);
}


Probe_UART::~Probe_UART()
{
	fsync(dev_fd);
	close(dev_fd);
}


void Probe_UART::test_echo(std::string const & str)
{
	char echo_byte;
	for (std::string::const_iterator it=str.begin(); it!=str.end(); ++it) {
		write(dev_fd, (char *) &(*it), 1);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		read(dev_fd, &echo_byte, 1);
		logger.debug("test_echo: %d [%c]", echo_byte, echo_byte);
	}
}

void Probe_UART::config_port()
{
	tcgetattr(this->dev_fd, &this->dev_options);
	cfsetispeed(&this->dev_options, B57600);
	cfsetospeed(&this->dev_options, B57600);
	dev_options.c_cflag |= (CLOCAL | CREAD);
	dev_options.c_cflag &= ~PARENB;
	dev_options.c_cflag &= ~CSTOPB;
	dev_options.c_cflag &= ~CSIZE;
	dev_options.c_cflag |= CS8;
	dev_options.c_cflag |= FLUSHO;
	tcsetattr(this->dev_fd, TCSANOW, &this->dev_options);
	logger.debug("config_port: serial port configured");
}


ExitCode Probe_UART::send_request(
    uint8_t channel_id,
    RequestType reqcode,
    uint8_t * data,
    uint8_t * reply_header)
{
	if (!open_status) {
		logger.error("send_request: probe device not open!");
		return ExitCode::DEVICE_NOT_OPEN;
	}

	// Header
	uint8_t request[MSG_REQUEST_LEN];
	request[MSG_POS_CHANNEL_ID]   = channel_id;
	request[MSG_POS_REQUEST_CODE] = (uint8_t) reqcode; // -48
	request[MSG_POS_DATA]         = 255;
	if (data != nullptr)
		request[MSG_POS_DATA] = *data;
	print_packet_content("send_request: request = ", request, MSG_REQUEST_LEN);

	// Send request
	ssize_t nb = write(dev_fd, request, sizeof(request));
	if (nb < 0) {
		perror("send request error");
		return ExitCode::WRITE_ERROR;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	// Receive reply header
	nb = read(dev_fd, reply_header, MSG_REPLY_HEADER_LEN);
	if (nb < 0) {
		perror("send request error");
		return ExitCode::READ_ERROR;
	}
	print_packet_content("send_request: reply_header = ",
	                     reply_header,
	                     MSG_REPLY_HEADER_LEN);

	if (reply_header[MSG_POS_REPLY_STATUS] != 0) {
		logger.error("send_request: error = %d",
		             reply_header[MSG_POS_REPLY_STATUS]);
		return ExitCode::UNKNOWN_ERROR;
	}

	return ExitCode::SUCCESS;
}


ExitCode Probe_UART::recv_data(uint8_t * reply_header, uint8_t * reply_data)
{
	// Reply data
	memset(reply_data, 0, MSG_REPLY_DATA_LEN_MAX);
	ssize_t nb = read(dev_fd, reply_data, reply_header[MSG_POS_REPLY_DATA_LEN]);
	logger.debug("recv_data: len = %d/%d",
	             nb, reply_header[MSG_POS_REPLY_DATA_LEN]);
	print_packet_content("recv_data: reply_data = ",
	                     reply_data,
	                     reply_header[MSG_POS_REPLY_DATA_LEN]);
	return ExitCode::SUCCESS;;
}


std::string Probe_UART::send_request_and_get_data(
    uint8_t channel_id, RequestType reqtype)
{
	uint8_t reply_header[MSG_REPLY_HEADER_LEN];
	auto ret = send_request(channel_id, reqtype, nullptr, reply_header);
	if (ret != ExitCode::SUCCESS) {
		return std::string("");
	}

	uint8_t reply_data[MSG_REPLY_DATA_LEN_MAX];
	recv_data(reply_header, reply_data);

	return std::string((char *)reply_data);
}


float Probe_UART::send_request_and_get_data_float(
    uint8_t channel_id, RequestType reqtype)
{
	std::string dev_data(send_request_and_get_data(channel_id, reqtype));
	return atof(dev_data.c_str());
}


std::string Probe_UART::get_info(uint8_t channel_id)
{
	return send_request_and_get_data(channel_id, GET_DEVICE_INFO);
}


DeviceStatus Probe_UART::check_channel(uint8_t channel_id)
{
	std::string dev_data(send_request_and_get_data(channel_id, CHECK_DEVICE));

	switch (dev_data[0]) {
	case 0:
		logger.debug("check_channel: %d = OK", channel_id);
		return DeviceStatus::OK;
	case 1:
		logger.warn("check_channel: %d = communication fault?", channel_id);
		return DeviceStatus::FAULT_COMM;
	case 2:
		logger.debug("check_channel: %d = manufacturing fault?", channel_id);
		return DeviceStatus::FAULT_MANUFACT;
	default:
		logger.debug("check_channel: %d = unknown value (%d)",
		             channel_id, dev_data[0]);
		return DeviceStatus::UNKNOWN_VALUE;
	}

	return DeviceStatus::OK;
}


float Probe_UART::get_current_A(uint8_t channel_id)
{
	float value = send_request_and_get_data_float(channel_id, GET_CURRENT);
	logger.debug("[%d] current: %f A", channel_id, value);
	return value;
}


float Probe_UART::get_voltage_V(uint8_t channel_id)
{
	float value = send_request_and_get_data_float(channel_id, GET_VOLTAGE);
	logger.debug("[%d] voltage: %f V", channel_id, value);
	return value;
}


float Probe_UART::get_power_W(uint8_t channel_id)
{
	float value = send_request_and_get_data_float(channel_id, GET_POWER);
	logger.debug("[%d] power: %f W", channel_id, value);
	return value;
}


std::string Probe_UART::get_all(uint8_t channel_id)
{
	return send_request_and_get_data(channel_id, GET_SAMPLES);
}


int Probe_UART::start_energy_sampling(uint8_t channel_id)
{
	uint8_t reply_header[MSG_REPLY_HEADER_LEN];
	auto ret = send_request(
	               channel_id,
	               START_ENERGY_SAMPLING,
	               nullptr,
	               reply_header);
	if (ret != ExitCode::SUCCESS) {
		return 0.0;
	}

	logger.debug("[%d] start energy sampling...", channel_id);
	return reply_header[MSG_POS_REPLY_STATUS];
}


float Probe_UART::stop_energy_sampling(uint8_t channel_id)
{
	float value = send_request_and_get_data_float(
	                  channel_id,
	                  STOP_ENERGY_SAMPLING);
	logger.debug("[%d] energy: %f J", channel_id, value);
	return value;
}

} // namespace smokyprobe
