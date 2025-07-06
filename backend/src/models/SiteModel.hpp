#ifndef SiteModel_hpp
#define SiteModel_hpp

#include <mutex>
#include <vector>
#include <memory>
#include "SocketModel.hpp"

enum class SiteStatus {
    IDLE,   
    ACTIVE,  
    COMPLETED
};

struct SiteStatusData {
    int siteId;
    SiteStatus siteStatus;
    std::vector<SocketStatusData> sockets;
};

class SiteModel {
public:
    int siteId;
    std::vector<std::shared_ptr<SocketModel>> sockets;
    std::mutex siteMutex;
    SiteStatus siteStatus = SiteStatus::IDLE;
    SiteModel(int id) : siteId(id) {
        for (int i = 0; i < 4; i++) { 
            auto socket = std::make_shared<SocketModel>();
            socket->socketId = id * 4 + i;
            sockets.push_back(socket);
        }
    }
};

#endif /* SiteModel_hpp */
