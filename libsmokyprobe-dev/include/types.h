
#ifndef __SMOKYPROBE_TYPES_
#define __SMOKYPROBE_TYPES_

namespace smokyprobe
{

enum class ExitCode
{
    SUCCESS,
    DEVICE_NOT_OPEN,
    READ_ERROR,
    WRITE_ERROR,
    UNKNOWN_ERROR
};

enum RequestType {
    NOP,
    CHECK_DEVICE,
    GET_DEVICE_INFO,
    GET_CURRENT,
    GET_VOLTAGE,
    GET_VOLTAGE_SHUNT,
    GET_POWER,
    GET_SAMPLES,
    START_ENERGY_SAMPLING,
    STOP_ENERGY_SAMPLING
};


enum class DeviceStatus
{
    OK,
    FAILED_REQUEST,
    FAULT_COMM,
    FAULT_MANUFACT,
    UNKNOWN_VALUE
};


} // namespace smokyprobe


#endif // __SMOKYPROBE_TYPES_
