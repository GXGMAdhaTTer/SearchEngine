#include "../../../Include/RoundRobin.h"
#include <iostream>
#include <nlohmann/json.hpp>
using namespace nlohmann;
using std::cout;
using std::endl;

RoundRobin::RoundRobin()
    : _timeOut(stoul(GetPath("TIMEOUT"))), _timeIndex(0), _slotMap(_timeOut, unordered_set<TcpConnectionPtr>()), _curSlotTable() {
    cout << "RoundRobin" << endl;
}

void RoundRobin::timeUpdate() {
    _timeIndex = (_timeIndex + 1) % _timeOut;
}

void RoundRobin::clearSlot() {
    cout << "clearSlot " << _timeIndex << endl;
    for (auto& conn : _slotMap[_timeIndex]) {
        json myJ;
        myJ["msgID"] = 666;
        myJ["msg"] = "已超时断开连接";
        conn->send(myJ.dump());
        conn->shutDown();
        _curSlotTable.erase(conn);
    }
    _slotMap[_timeIndex].clear();
}
void RoundRobin::attachToSlot(const TcpConnectionPtr& conn) {
    size_t slotIndexToAttach = (_timeOut + _timeIndex - 1) % _timeOut;
    _slotMap[slotIndexToAttach].insert(conn);
    _curSlotTable.insert({conn, slotIndexToAttach});
}

void RoundRobin::updateSlot(const TcpConnectionPtr& conn) {
    size_t slotIndexToAttach = (_timeOut + _timeIndex - 1) % _timeOut;
    _slotMap[_curSlotTable[conn]].erase(conn);
    _slotMap[slotIndexToAttach].insert(conn);
    _curSlotTable[conn] = slotIndexToAttach;
}

void RoundRobin::removeFromSlot(const TcpConnectionPtr& conn) {
    _slotMap[_curSlotTable[conn]].erase(conn);
    _curSlotTable.erase(conn);
}
