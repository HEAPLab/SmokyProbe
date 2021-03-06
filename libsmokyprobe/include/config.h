

#ifndef __SMOKYPROBE_CONFIG__
#define __SMOKYPROBE_CONFIG__

/*
#define MSG_REQUEST_SIZE     3
#define MSG_REQUEST_CHANNEL  0
#define MSG_REQUEST_CODE     1
#define MSG_REQUEST_DATA     2

#define MSG_REPLY_HEADER_SIZE     3

#define MSG_REPLY_CODE            MSG_REQUEST_CODE
#define MSG_REPLY_CHANNEL         MSG_REQUEST_CHANNEL
#define MSG_REPLY_HEADER_DATALEN  2

#define MSG_REPLY_DATA_MAX_SIZE  100
*/

#define NR_CHANNELS_MAX          4

#define MSG_REQUEST_LEN         3
#define MSG_REPLY_HEADER_LEN    4
#define MSG_REPLY_DATA_LEN_MAX  100

#define MSG_POS_CHANNEL_ID      0
#define MSG_POS_REQUEST_CODE    1
#define MSG_POS_DATA            2

#define MSG_POS_REPLY_CODE      MSG_POS_REQUEST_CODE
#define MSG_POS_REPLY_STATUS    2
#define MSG_POS_REPLY_DATA_LEN  3

#define REPLY_STATUS_OK         100

#endif // __SMOKYPROBE_CONFIG__
