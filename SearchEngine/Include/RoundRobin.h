#ifndef __ROUNDROBIN__
#define __ROUNDROBIN__
#include <cstdio>
#include "TcpConnection.h"
#include "Configuration.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::unordered_map;
using std::unordered_set;
using std::vector;

using TcpConnectionPtr = shared_ptr<TcpConnection>;

class RoundRobin {
public:
    RoundRobin();
    void timeUpdate();
    void initSlotMap();
    void clearSlot();
    void attachToSlot(const TcpConnectionPtr& conn);
    void updateSlot(const TcpConnectionPtr& conn);
    void removeFromSlot(const TcpConnectionPtr& conn);

private:
    size_t _timeOut;
    size_t _timeIndex;
    vector<unordered_set<TcpConnectionPtr>> _slotMap;
    unordered_map<TcpConnectionPtr, size_t> _curSlotTable;
};

#endif