#pragma once
#include <string>

using namespace std;

class EntryPoint
{
    public:
    enum EntryProtocol
    {
        ProtocolTCP,
        ProtocolUDP,
        ProtocolHTTP,
        ProtocolTCPS,
        ProtocolUDPS,
    };
private:
    EntryProtocol entryProtocol;
    string addr;
    int point;

};