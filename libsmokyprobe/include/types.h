
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

/*
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
*/

typedef char RequestType;

#define NOP                    'x'
#define CHECK_DEVICE           'C'
#define GET_DEVICE_INFO        'D'
#define GET_CURRENT            'I'
#define GET_VOLTAGE            'V'
#define GET_VOLTAGE_SHUNT      'v'
#define GET_POWER              'P'
#define GET_SAMPLES            'S'
#define START_ENERGY_SAMPLING  'E'
#define STOP_ENERGY_SAMPLING   'e'

#define itoch(x) x + 48
#define chtoi(x) x - 48

#define INCLUDE_NO_DATA        '-'

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
