#ifndef SocketModel_hpp
#define SocketModel_hpp

#include <mutex>

enum class SocketProgrammingResult {
    NONE,   
    PASSED,  
    FAILED
};

struct SocketStatusData {
    int socketId;
    bool isSocketPlaced;
    bool isSocketReadyForPick;
    bool isSocketPicked;
    SocketProgrammingResult programmingResult;
};

class SocketModel {
public:
    int socketId;
    std::mutex socketMutex;

    bool isSocketPlaced = false;
    bool isSocketReadyForPick = false;
    bool isSocketPicked = false;
    SocketProgrammingResult programmingResult = SocketProgrammingResult::NONE;

};

#endif /* SocketModel_hpp */
