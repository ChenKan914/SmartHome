#include "SHNetworkMessage.h"

SHNetworkMessage::SHNetworkMessage()
{
    message = new MessageBase();
    messageHeader = new MessageHeader();
    messageBody = new MessageBody();
    roomInfo = new MessageRoomInfo();
}
SHNetworkMessage::~SHNetworkMessage()
{

}
void SHNetworkMessage::setMessageType(MessageHeader_MessageType type)
{
    messageHeader->set_type(type);
}

void SHNetworkMessage::setMessageAlarmType(MessageBody_MessageAlarmType alarmType)
{
    messageBody->set_alarm(alarmType);
}

void SHNetworkMessage::setMessageRoomInfo()
{
    messageBody->set_allocated_roominfo(roomInfo);
}

void SHNetworkMessage::setMessageData(std::string data)
{
    messageBody->set_data(data);
}

void SHNetworkMessage::mergeMessage()
{
    messageBody->set_allocated_roominfo(roomInfo);
    message->set_allocated_header(messageHeader);
    message->set_allocated_body(messageBody);
}

MessageHeader_MessageType SHNetworkMessage::getMessageType()
{
    return message->header().type();
}

MessageBody_MessageAlarmType SHNetworkMessage::getMessageAlarmType()
{
    return message->body().alarm();
}

MessageRoomInfo SHNetworkMessage::getMessageRoomInfo()
{
     return message->body().roominfo();
}

void SHNetworkMessage::parseMessage(QByteArray buf)
{
    std::string receiveMsg = buf.toStdString();
    message->ParseFromString(receiveMsg);
    *messageHeader = message->header();
    *messageBody = message->body();
    *roomInfo = message->body().roominfo();
}

void SHNetworkMessage::serializeToString(QByteArray &data)
{
    std::string str = "";
    message->SerializeToString(&str);
    QByteArray bytetostring;
    data.append(bytetostring.fromStdString(str));
}

void SHNetworkMessage::setCommunity(std::string &&value)
{
    roomInfo->set_community(value);
}

void SHNetworkMessage::setBuildingID(std::string &&buildingID)
{
    roomInfo->set_building_id(buildingID);
}

void SHNetworkMessage::setBuildingPart(std::string &&buildingPart)
{
    roomInfo->set_building_part(buildingPart);
}

void SHNetworkMessage::setCellID(std::string  &&cellID)
{
    roomInfo->set_cell_id(cellID);
}

void SHNetworkMessage::setRoomID(std::string &&roomID)
{
    roomInfo->set_room_id(roomID);
}

void SHNetworkMessage::setAttachment(std::string&& attachment)
{
    roomInfo->set_attachment(attachment);
}

std::string SHNetworkMessage::getCommunity()
{
    return roomInfo->community();
}
std::string SHNetworkMessage::getBuildingID()
{
    return roomInfo->building_id();
}
std::string SHNetworkMessage::getBuildingPart()
{
    return roomInfo->building_part();
}
std::string SHNetworkMessage::getCellID()
{
    return roomInfo->cell_id();
}
std::string SHNetworkMessage::getRoomID()
{
    return roomInfo->room_id();
}
std::string SHNetworkMessage::getAttachment()
{
    return roomInfo->attachment();
}
std::string SHNetworkMessage::getMessageData()
{
    return messageBody->data();
}
QByteArray SHNetworkMessage::intToByte(int i)
{
    QByteArray abyte0;
    abyte0.resize(4);
    abyte0[3] = (uchar) (0x000000ff & i);
    abyte0[2] = (uchar) ((0x0000ff00 & i) >> 8);
    abyte0[1] = (uchar) ((0x00ff0000 & i) >> 16);
    abyte0[0] = (uchar) ((0xff000000 & i) >> 24);
    return abyte0;
}

int SHNetworkMessage::byteToInt(QByteArray bytes)
{
    int addr = bytes[3] & 0x000000ff;
    addr |= ((bytes[2] << 8) & 0x0000ff00);
    addr |= ((bytes[1] << 16) & 0x00ff0000);
    addr |= ((bytes[0] << 24) & 0xff000000);
    return  addr;
}
