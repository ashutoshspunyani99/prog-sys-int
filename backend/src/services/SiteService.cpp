#include "SiteService.hpp"


SiteService::SiteService(   std::shared_ptr<JobService> jobService) : jobService(jobService) {
    for (int i = 0; i < 2; i++) { 
        sites.push_back(std::make_shared<SiteModel>(i));
    }
}

bool SiteService::ensureJobRunning()  {
    if (!jobService->isJobRunning()) {
        std::cout << "[SiteService] Operation denied. No job is currently running.\n";
        return false;
    }
    return true;
}

bool SiteService::isJobCompleted()  {
    if (!jobService->isJobRunning()) {
        std::cout << "[SiteService] Operation denied. No job is currently running.\n";
        return false;
    }
    return jobService->isJobCompleted();
}

std::vector<int> SiteService::getReadyToPlaceSockets() {
    std::vector<int> readyPlacedSockets;
    for (auto& sitePtr : sites) {
        auto& site = *sitePtr;
        std::lock_guard<std::mutex> siteLock(site.siteMutex);
        for (auto& socketPtr : site.sockets) {
            auto& socket = *socketPtr;
            std::lock_guard<std::mutex> socketLock(socket.socketMutex);
            if (!socket.isSocketPlaced && !socket.isSocketReadyForPick && !socket.isSocketPicked) {
                readyPlacedSockets.push_back(socket.socketId);
            }
        }
    }
    return readyPlacedSockets;
}

std::vector<int> SiteService::getReadyToPickSockets() {
    std::vector<int> readyPickedSockets;
    for (auto& sitePtr : sites) {
        auto& site = *sitePtr;
        std::lock_guard<std::mutex> siteLock(site.siteMutex);
        for (auto& socketPtr : site.sockets) {
            auto& socket = *socketPtr;
            std::lock_guard<std::mutex> socketLock(socket.socketMutex);
            if (socket.isSocketPlaced && socket.isSocketReadyForPick && !socket.isSocketPicked) {
                readyPickedSockets.push_back(socket.socketId);
            }
        }
    }
    return readyPickedSockets;
}

bool SiteService::placeDevice(int socketId) {
    if (socketId < 0 || socketId >= sites.size() * 4) 
        return false; 
    std::cout << "Placing device in socket: " << socketId << std::endl;
    int siteId = socketId / 4;
    int socketIndex = socketId % 4;
    std::cout << "Site ID: " << siteId << ", Socket Index: " << socketIndex << std::endl;
    std::cout << "Total Sites: " << sites.size() << std::endl;
    if (siteId < 0 || siteId >= sites.size() || socketIndex < 0 || socketIndex >= 4) 
        return false;
    std::cout << "Valid site and socket index." << std::endl;
    auto& site = *sites[siteId];
    std::lock_guard<std::mutex> siteLock(site.siteMutex);
    std::cout << "Acquired site lock for site ID: " << siteId << std::endl;
    std::cout << "Site Status: " << static_cast<int>(site.siteStatus) << std::endl;
    if (site.siteStatus != SiteStatus::IDLE)
        return false;
    std::cout << "Site is idle, proceeding to place device." << std::endl;
    auto& socket = *site.sockets[socketIndex];
    
    {
    std::lock_guard<std::mutex> socketLock(socket.socketMutex);
    if (socket.isSocketPlaced || socket.isSocketReadyForPick || socket.isSocketPicked) {

        return false;
    }
    std::cout << "Socket is ready for placement." << std::endl;
    socket.isSocketPlaced = true;
    socket.programmingResult = SocketProgrammingResult::NONE;
    }
    bool isAllSocketsPlaced = true;
    for (auto& socketPtr : site.sockets) {
        auto& s = *socketPtr;
        std::lock_guard<std::mutex> socketPlacedLock(s.socketMutex);
        if (!s.isSocketPlaced) {
            isAllSocketsPlaced = false;
            break;
        }
    }
    std::cout << "Checked all sockets for placement status." << std::endl;
    std::cout << "--------------------------------------------- " << std::endl;
    std::cout<< "Is all sockets placed: " << isAllSocketsPlaced << std::endl;
    std::cout << "--------------------------------------------- " << std::endl;
    if (isAllSocketsPlaced && site.siteStatus == SiteStatus::IDLE) {
        site.siteStatus = SiteStatus::ACTIVE;
        std::cout << "Site Status: " << static_cast<int>(site.siteStatus) << std::endl;
        std::cout << "All sockets placed, changing site status to ACTIVE." << std::endl;
        std::thread([this, siteId]() {
            siteProgramming(siteId);  
        }).detach();

    }
    std::cout << "Device placed successfully in socket: " << socketId << std::endl;
    
    return true; 
}

bool SiteService::pickDevice(int socketId) {

    std::cout << "Picking device from socket: " << socketId << std::endl;
    if (socketId < 0 || socketId >= sites.size() * 4) 
    return false; 
    std::cout << "Valid socket ID." << std::endl;
int siteId = socketId / 4;
int socketIndex = socketId % 4;
std::cout << "Site ID: " << siteId << ", Socket Index: " << socketIndex << std::endl;
if (siteId < 0 || siteId >= sites.size() || socketIndex < 0 || socketIndex >= 4) 
    return false;
std::cout << "Valid site and socket index." << std::endl;
auto& site = *sites[siteId];
std::lock_guard<std::mutex> siteLock(site.siteMutex);
std::cout << "Acquired site lock for site ID: " << siteId << std::endl;
std::cout << "Site Status: " << static_cast<int>(site.siteStatus) << std::endl;
if (site.siteStatus != SiteStatus::COMPLETED)
    return false;
auto& socket = *site.sockets[socketIndex];

{
std::lock_guard<std::mutex> socketLock(socket.socketMutex);

    if (!socket.isSocketPlaced || !socket.isSocketReadyForPick || socket.isSocketPicked) 
    return false;
if(socket.programmingResult==  SocketProgrammingResult::PASSED) {
    std::cout << "Socket programming passed, ready to pick." << std::endl;
    jobService->incrementCompletedQuantity();
    
}
else if (socket.programmingResult == SocketProgrammingResult::FAILED) {
    std::cout << "Socket programming failed, cannot pick." << std::endl;
    jobService->incrementFailedQuantity();
}

std::cout << "Socket is ready for picking." << std::endl;
socket.isSocketPicked = true;
}
bool isAllSocketsPickedUp = true;

for (auto& socketPtr : site.sockets) {
    auto& s = *socketPtr;
    std::lock_guard<std::mutex> socketPickedLock(s.socketMutex);
    // std::cout << "Checking socket ID: " << s.socketId << std::endl;
    // std::cout << "Socket Placed: " << s.isSocketPlaced << ", Socket Ready For Pick: " << s.isSocketReadyForPick << std::endl;
    // std::cout << "--------------------------------------------- " << std::endl;
    // std::cout << "Socket Status: " << static_cast<int>(s.programmingResult) << std::endl;
    // std::cout << "--------------------------------------------- " << std::endl;
    if (!s.isSocketPicked) {
        // std::cout << "Socket ID: " << s.socketId << " is not picked yet." << std::endl;
        isAllSocketsPickedUp = false;
        break;
    }
}
    std::cout << "--------------------------------------------- " << std::endl;
    std::cout<< "Is all sockets picked: " << isAllSocketsPickedUp << std::endl;
    std::cout << "--------------------------------------------- " << std::endl;
if (isAllSocketsPickedUp && site.siteStatus == SiteStatus::COMPLETED) {
    for (auto& socketPtr : site.sockets) {
        auto& s = *socketPtr;
        std::lock_guard<std::mutex> socketPickedLock(s.socketMutex);
        s.isSocketPlaced = false;
        s.isSocketReadyForPick = false;
        s.isSocketPicked = false;
        s.programmingResult = SocketProgrammingResult::NONE;
        std::cout << "Resetting socket ID: " << s.socketId << " to initial state." << std::endl;
    }
    site.siteStatus = SiteStatus::IDLE;
    std::cout << "Site reset to IDLE, ready for next batch." << std::endl;

}

    return true; 
}

void SiteService::siteProgramming(int siteId) {
    if (siteId < 0 || siteId >= sites.size()) 
        return; 
        {
            auto& site = *sites[siteId];
            std::lock_guard<std::mutex> siteLock(site.siteMutex);
    
            std::cout << "[Site " << siteId << "] Starting programming in 20 seconds...\n";
    
            if (site.siteStatus != SiteStatus::ACTIVE) {
                std::cout << "[Site " << siteId << "] Site is not active, skipping programming.\n";
                return;
            }
        } 
    std::this_thread::sleep_for(std::chrono::seconds(20));

    auto& site = *sites[siteId];

        for (auto& socketPtr : site.sockets) {
            auto& socket = *socketPtr;
        std::lock_guard<std::mutex> socketLock(socket.socketMutex);
        if (socket.isSocketPlaced && !socket.isSocketReadyForPick) {
            socket.isSocketReadyForPick = true;
            bool value = rand() % 10 < 8; 
            socket.programmingResult = value ? SocketProgrammingResult::PASSED : SocketProgrammingResult::FAILED;
            std::cout << "[Socket " << socket.socketId << "] Programming result: "
                      << (value ? "PASSED" : "FAILED") << "\n";
            std::cout << "[Socket " << socket.socketId << "] Result: "
            << (value ? "PASSED" : "FAILED") << "\n";
        }
    }

    {
        std::lock_guard<std::mutex> siteLock(site.siteMutex);
        site.siteStatus = SiteStatus::COMPLETED;
        std::cout << "[Site " << siteId << "] Programming completed. Status set to COMPLETED.\n";
    }
}

// std::vector<SiteModel> SiteService::getSitesStatus() {
//     std::vector<SiteModel> status;
//     for (auto& site : sites) {
//         std::lock_guard<std::mutex> siteLock(site.siteMutex);
//         status.push_back(site);
//     }
//     return status;
// }

std::vector<SiteStatusData> SiteService::getSiteStatusById(int siteId) {
    std::vector<SiteStatusData> siteStatusResult;
    if (siteId < 0 || siteId >= sites.size()) 
        return siteStatusResult; 
    SiteStatusData statusData;
    auto& site = *sites[siteId];
    std::lock_guard<std::mutex> siteLock(site.siteMutex);
    statusData.siteId = site.siteId;
    statusData.siteStatus = site.siteStatus;
    std::cout << "[Status API] Site " << siteId << " status: " 
    << static_cast<int>(site.siteStatus) << std::endl;
    for (auto& socketPtr : site.sockets) {
        auto& socket = *socketPtr;
        std::lock_guard<std::mutex> socketLock(socket.socketMutex);
        SocketStatusData socketStatus;
        socketStatus.socketId = socket.socketId;
        socketStatus.isSocketPlaced = socket.isSocketPlaced;
        socketStatus.isSocketReadyForPick = socket.isSocketReadyForPick;
        socketStatus.isSocketPicked = socket.isSocketPicked;
        socketStatus.programmingResult = socket.programmingResult;
        std::cout << "[Socket " << socket.socketId << "] Placed=" << socket.isSocketPlaced
        << ", ReadyForPick=" << socket.isSocketReadyForPick
        << ", Picked=" << socket.isSocketPicked
        << ", Result=" << static_cast<int>(socket.programmingResult) << std::endl;
        statusData.sockets.push_back(socketStatus);
    }
    siteStatusResult.push_back(statusData);
    return siteStatusResult;
    
}