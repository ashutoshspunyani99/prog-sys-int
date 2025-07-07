#ifndef SiteModel_hpp
#define SiteModel_hpp

#include <mutex>
#include <vector>
#include <memory>
#include "SocketModel.hpp"

/// @brief Represents the current status of a site.
///
/// - IDLE: The site is not actively processing.
/// - ACTIVE: The site is actively running and managing sockets.
/// - COMPLETED: The site has finished processing all devices.
enum class SiteStatus {
    IDLE,   
    ACTIVE,  
    COMPLETED
};

/// @brief Data structure holding the runtime status of a site.
struct SiteStatusData {
    int siteId;
    SiteStatus siteStatus;
    std::vector<SocketStatusData> sockets;
};

/// @brief Thread-safe model representing a programming site.
/// 
/// Contains a collection of sockets and their statuses, protected by a mutex.
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
