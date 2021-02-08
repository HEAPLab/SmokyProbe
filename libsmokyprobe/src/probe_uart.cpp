
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
    std::string const & msg,
    uint8_t * data,
    size_t size)
{
	std::stringstream data_stream;
	for (size_t i = 0; i < size; ++i) {
		data_stream << "[" << data[i] << "]";
	}
	std::string ds(data_stream.str());
	logger.debug("%s: %s", msg.c_str(), ds.c_str());
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
		return;
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
	logger.info("Probe_UART ready");
}


Probe_UART::~Probe_UART()
{
	// Close file descriptors
	fsync(dev_fd);
	close(dev_fd);
	logger.shutdown();
}


void Probe_UART::test_echo(std::string const & str)
{
	char echo_byte;
	for (std::string::const_iterator it=str.begin(); it!=str.end(); ++it) {
		ssize_t nb = write(dev_fd, (char *) &(*it), 1);
		if (nb < 0) {
			perror("test_echo");
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		nb = read(dev_fd, &echo_byte, 1);
		if (nb < 0) {
			perror("test_echo");
			return;
		}
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


/*** Data transmission functions ***/


ExitCode Probe_UART::send_to_device(request_pkt & request)
{
	// Check device status
	if (!open_status) {
		logger.error("send_to_device: probe device not open!");
		return ExitCode::DEVICE_NOT_OPEN;
	}

	ssize_t nb = write(dev_fd, request.data(), sizeof(request.data()));
	if (nb < 0) {
		perror("send_to_device: error");
		return ExitCode::WRITE_ERROR;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	return ExitCode::SUCCESS;
}


ExitCode Probe_UART::recv_from_device(
	reply_h_pkt & reply_header,
	reply_d_pkt & reply_data)
{
	// Receive reply header
	ssize_t nb = read(dev_fd, reply_header.data(), MSG_REPLY_HEADER_LEN);
	if (nb < 0) {
		perror("recv_from_device:" );
		return ExitCode::READ_ERROR;
	}

	logger.debug("recv_from_device: header length  = %d", nb);
	print_packet_content("recv_from_device: reply_header = ",
	                     reply_header.data(),
	                     MSG_REPLY_HEADER_LEN);

	if (reply_header[MSG_POS_REPLY_STATUS] != REPLY_STATUS_OK) {
		logger.error("recv_from_device: error = %d",
		             reply_header[MSG_POS_REPLY_STATUS]);
		return ExitCode::READ_ERROR;
	}

	// Receive reply data
	nb = read(dev_fd, reply_data.data(), reply_header[MSG_POS_REPLY_DATA_LEN]);
	if (nb < 0) {
		perror("recv_from_device:" );
		return ExitCode::UNKNOWN_ERROR;
	}

	logger.debug("recv_from_device: len = %d/%d",
	             nb, reply_header[MSG_POS_REPLY_DATA_LEN]);
	print_packet_content("recv_from_device: reply_data = ",
	                     reply_data.data(),
	                     reply_header[MSG_POS_REPLY_DATA_LEN]);

	return ExitCode::SUCCESS;
}


std::string Probe_UART::send_request_and_get_data(
    uint8_t channel_id,
    RequestType reqcode,
    uint8_t * data)
{
	// Create and send the request
	request_pkt request;
	request[MSG_POS_CHANNEL_ID]   = itoch(channel_id);
	request[MSG_POS_REQUEST_CODE] = reqcode;
	request[MSG_POS_DATA]         = INCLUDE_NO_DATA;
	if (data != nullptr)
		request[MSG_POS_DATA] = *data;
	print_packet_content("send_request_and_get_data: request = ", request.data(), MSG_REQUEST_LEN);
	send_to_device(request);

	reply_h_pkt reply_header;
	reply_d_pkt reply_data;
	recv_from_device(reply_header, reply_data);

	return std::string((char *) reply_data.data());
}


float Probe_UART::send_request_and_get_data_float(
    uint8_t channel_id,
    RequestType reqcode,
    uint8_t * data)
{
	std::string dev_data(send_request_and_get_data(channel_id, reqcode, data));
	return atof(dev_data.c_str());
}


/*** Functionalities ***/


std::string Probe_UART::get_line()
{
	char buffer[MSG_REPLY_DATA_LEN_MAX];
	int nb = read(dev_fd, buffer, MSG_REPLY_DATA_LEN_MAX);
	if (nb < 0) {
		perror("get_line");
		return "";
	}
	return std::string(buffer);
}



std::string Probe_UART::get_info(uint8_t channel_id)
{
	return send_request_and_get_data(channel_id, GET_DEVICE_INFO);
}


DeviceStatus Probe_UART::check_channel(uint8_t channel_id)
{
	std::string dev_data(send_request_and_get_data(channel_id, CHECK_DEVICE));

	switch (dev_data[0]) {
	case '0':
		logger.debug("check_channel: %d = OK", channel_id);
		return DeviceStatus::OK;
	case '1':
		logger.warn("check_channel: %d = communication fault?", channel_id);
		return DeviceStatus::FAULT_COMM;
	case '2':
		logger.debug("check_channel: %d = manufacturing fault?", channel_id);
		return DeviceStatus::FAULT_MANUFACT;
	default:
		logger.debug("check_channel: %d = unknown value (%c)",
		             channel_id, dev_data[0]);
		return DeviceStatus::UNKNOWN_VALUE;
	}

	return DeviceStatus::OK;
}


float Probe_UART::get_current_A(uint8_t channel_id)
{
	float value = send_request_and_get_data_float(channel_id, GET_CURRENT);
	logger.info("[%d] current: %f A", channel_id, value);
	return value;
}


float Probe_UART::get_voltage_V(uint8_t channel_id)
{
	float value = send_request_and_get_data_float(channel_id, GET_VOLTAGE);
	logger.info("[%d] voltage: %f V", channel_id, value);
	return value;
}


float Probe_UART::get_power_W(uint8_t channel_id)
{
	float value = send_request_and_get_data_float(channel_id, GET_POWER);
	logger.info("[%d] power: %f W", channel_id, value);
	return value;
}


std::string Probe_UART::get_all(uint8_t channel_id)
{
	return send_request_and_get_data(channel_id, GET_SAMPLES);
}


int Probe_UART::start_energy_sampling(uint8_t channel_id)
{
	send_request_and_get_data(channel_id, START_ENERGY_SAMPLING);
	logger.info("[%d] start energy sampling...", channel_id);
	return 0;
}


float Probe_UART::stop_energy_sampling(uint8_t channel_id)
{
	float value = send_request_and_get_data_float(
	                  channel_id,
	                  STOP_ENERGY_SAMPLING);
	logger.info("[%d] energy: %f J", channel_id, value);
	return value;
}


/***/

} // namespace smokyprobe
