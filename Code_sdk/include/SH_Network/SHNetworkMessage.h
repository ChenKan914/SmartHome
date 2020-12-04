#ifndef SHNETWORKMESSAGE_H
#define SHNETWORKMESSAGE_H
#include <QByteArray>

#include "message.pb.h"

class SHNetworkMessage :public MessageBase
{
public:
    SHNetworkMessage();
    ~SHNetworkMessage();

    void setMessageType(MessageHeader_MessageType);
    void setMessageAlarmType(MessageBody_MessageAlarmType);
    void setMessageRoomInfo();
    void setMessageData(std::string data);
    void serializeToString(QByteArray &data);

    void setCommunity(std::string&& value);
    void setBuildingID(std::string&& value);
    void setBuildingPart(std::string&& value);
    void setCellID(std::string&& value);
    void setRoomID(std::string&& value);
    void setAttachment(std::string&& value);

    std::string getCommunity();
    std::string getBuildingID();
    std::string getBuildingPart();
    std::string getCellID();
    std::string getRoomID();
    std::string getAttachment();
    std::string getMessageData();
    MessageHeader_MessageType getMessageType();
    MessageBody_MessageAlarmType getMessageAlarmType();
    MessageRoomInfo getMessageRoomInfo();

    void mergeMessage();
    void parseMessage(QByteArray);

    QByteArray intToByte(int i);
    int byteToInt(QByteArray);


    MessageBase     *message;
    MessageHeader   *messageHeader;
    MessageBody     *messageBody;
    MessageRoomInfo *roomInfo;
};
#endif // SHNETWORKMESSAGE_H
