#pragma once

typedef struct __packed
{
    char ver;
    int sn;
    int payloadLen;
}XTransHeader;

typedef struct  __packed
{
    char frameType;
    int streamId;
    int offset;
    unsigned char checkCode[32];
    int dataLength;

}XFrameHeader;
