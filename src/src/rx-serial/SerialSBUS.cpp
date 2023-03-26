#include "SerialSBUS.h"
#include "CRSF.h"
#include "device.h"
#include "logging.h"

#define SBUS_FLAG_SIGNAL_LOSS       (1 << 2)
#define SBUS_FLAG_FAILSAFE_ACTIVE   (1 << 3)

void SerialSBUS::setLinkQualityStats(uint16_t lq, uint16_t rssi)
{
    // unsupported
}

void SerialSBUS::sendLinkStatisticsToFC()
{
    // unsupported
}

uint32_t SerialSBUS::sendRCFrameToFC(bool frameAvailable, uint32_t *channelData)
{
    crsf_channels_s PackedRCdataOut;
    
    PackedRCdataOut.ch0 = fmap(channelData[0], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);
    PackedRCdataOut.ch1 = fmap(channelData[1], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);
    PackedRCdataOut.ch2 = fmap(channelData[2], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);
    PackedRCdataOut.ch3 = fmap(channelData[3], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);

    PackedRCdataOut.ch4 = fmap(channelData[5], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);
    PackedRCdataOut.ch5 = fmap(channelData[6], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);
    PackedRCdataOut.ch6 = fmap(channelData[7], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 176, 848); // Ch 7 appears to be control mode https://forum.dji.com/forum.php?mod=viewthread&tid=227897&page=2#pid2784111
    PackedRCdataOut.ch7 = fmap(channelData[8], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);

    PackedRCdataOut.ch8 = fmap(channelData[9], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);
    PackedRCdataOut.ch9 = fmap(channelData[10], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);
    PackedRCdataOut.ch10 = fmap(channelData[11], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);
    PackedRCdataOut.ch11 = fmap(channelData[12], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);

    PackedRCdataOut.ch12 = fmap(channelData[13], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);
    PackedRCdataOut.ch13 = fmap(channelData[14], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);
    PackedRCdataOut.ch14 = fmap(channelData[15], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);
    PackedRCdataOut.ch15 = fmap(channelData[4], CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 352, 1696);
    
    DBG("%d, ", PackedRCdataOut.ch0);
    DBG("%d, ", PackedRCdataOut.ch1);
    DBG("%d, ", PackedRCdataOut.ch2);
    DBG("%d, ", PackedRCdataOut.ch3);
    DBG("%d, ", PackedRCdataOut.ch4);
    DBG("%d, ", PackedRCdataOut.ch5);
    DBG("%d, ", PackedRCdataOut.ch6);
    DBGLN("%d", PackedRCdataOut.ch7);

    uint8_t extraData = 0;
    if (!frameAvailable)
    {
        extraData |= SBUS_FLAG_SIGNAL_LOSS;
    }

#if !defined(DEBUG_SBUS)
    _outputPort->write(0x0F);    // HEADER
    _outputPort->write((byte *)&PackedRCdataOut, RCframeLength);
    _outputPort->write((uint8_t)extraData);    // ch 17, 18, lost packet, failsafe
    _outputPort->write((uint8_t)0x00);    // FOOTER
#endif
    return 9;   // callback is every 9ms
}

void SerialSBUS::sendMSPFrameToFC(uint8_t* data)
{
    // unsupported
}
