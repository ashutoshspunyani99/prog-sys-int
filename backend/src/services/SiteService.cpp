#include "SiteService.hpp"


SiteService::SiteService(   std::shared_ptr<JobService> jobService) : jobService(jobService) {
    for (int i = 0; i < 2; i++) { 
        sites.push_back(std::make_shared<SiteModel>(i));
    }
    std::cout << "[INFO] SiteService: Initialized with " << sites.size()
              << " sites." << std::endl;
}

bool SiteService::ensureJobRunning()  {
    if (!jobService->isJobRunning()) {
         std::cout << "[WARN] SiteService: Operation denied. No job is currently running." << std::endl;;
        return false;
    }
    return true;
}

bool SiteService::isJobCompleted()  {
    if (!jobService->isJobRunning()) {
         std::cout << "[WARN] SiteService: Operation denied. No job is currently running." << std::endl;;
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
    int siteId = socketId / 4;
    int socketIndex = socketId % 4;
    if (siteId < 0 || siteId >= sites.size() || socketIndex < 0 || socketIndex >= 4) 
        return false;
    auto& site = *sites[siteId];
    std::lock_guard<std::mutex> siteLock(site.siteMutex);
    if (site.siteStatus != SiteStatus::IDLE)
        return false;
    auto& socket = *site.sockets[socketIndex];
    
    {
    std::lock_guard<std::mutex> socketLock(socket.socketMutex);
    if (socket.isSocketPlaced || socket.isSocketReadyForPick || socket.isSocketPicked) {

        return false;
    }
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
    if (isAllSocketsPlaced && site.siteStatus == SiteStatus::IDLE) {
        site.siteStatus = SiteStatus::ACTIVE;
        std::cout << "[INFO] SiteService: All sockets placed in site " << siteId
                  << ". Status set to ACTIVE." << std::endl;
        std::thread([this, siteId]() {
            siteProgramming(siteId);  
        }).detach();

    }
    std::cout << "[INFO] SiteService: Device placed in socket " << socketId
              << ". Socket status updated." << std::endl;    
    return true; 
}

bool SiteService::pickDevice(int socketId) {

    if (socketId < 0 || socketId >= sites.size() * 4) 
    return false; 
int siteId = socketId / 4;
int socketIndex = socketId % 4;
if (siteId < 0 || siteId >= sites.size() || socketIndex < 0 || socketIndex >= 4) 
    return false;
auto& site = *sites[siteId];
std::lock_guard<std::mutex> siteLock(site.siteMutex);
if (site.siteStatus != SiteStatus::COMPLETED)
    return false;
auto& socket = *site.sockets[socketIndex];

{
std::lock_guard<std::mutex> socketLock(socket.socketMutex);

    if (!socket.isSocketPlaced || !socket.isSocketReadyForPick || socket.isSocketPicked) 
    return false;
if(socket.programmingResult==  SocketProgrammingResult::PASSED) {
    
    jobService->incrementCompletedQuantity();
    
}
else if (socket.programmingResult == SocketProgrammingResult::FAILED) {
    jobService->incrementFailedQuantity();
}

socket.isSocketPicked = true;
}
bool isAllSocketsPickedUp = true;

for (auto& socketPtr : site.sockets) {
    auto& s = *socketPtr;
    std::lock_guard<std::mutex> socketPickedLock(s.socketMutex);
    if (!s.isSocketPicked) {
        isAllSocketsPickedUp = false;
        break;
    }
}
if (isAllSocketsPickedUp && site.siteStatus == SiteStatus::COMPLETED) {
    for (auto& socketPtr : site.sockets) {
        auto& s = *socketPtr;
        std::lock_guard<std::mutex> socketPickedLock(s.socketMutex);
        s.isSocketPlaced = false;
        s.isSocketReadyForPick = false;
        s.isSocketPicked = false;
        s.programmingResult = SocketProgrammingResult::NONE;
    }
    site.siteStatus = SiteStatus::IDLE;
    std::cout << "[INFO] SiteService: Site " << siteId
              << " reset to IDLE after all devices picked." << std::endl;
}

    return true; 
}

void SiteService::siteProgramming(int siteId) {
    if (siteId < 0 || siteId >= sites.size()) 
        return; 
        {
            auto& site = *sites[siteId];
            std::lock_guard<std::mutex> siteLock(site.siteMutex);
    
    
            if (site.siteStatus != SiteStatus::ACTIVE) {
              std::cout << "[WARN] SiteService: Programming aborted. Site "
                        << siteId << " not active." << std::endl;
              return;
            }
            std::cout << "[INFO] SiteService: Programming started for Site "
                      << siteId << "." << std::endl;
        } 
    std::this_thread::sleep_for(std::chrono::seconds(5)); // Simulate programming delay

    auto& site = *sites[siteId];

        for (auto& socketPtr : site.sockets) {
            auto& socket = *socketPtr;
        std::lock_guard<std::mutex> socketLock(socket.socketMutex);
        if (socket.isSocketPlaced && !socket.isSocketReadyForPick) {
            socket.isSocketReadyForPick = true;
            bool value = rand() % 10 < 8; 
            socket.programmingResult = value ? SocketProgrammingResult::PASSED : SocketProgrammingResult::FAILED;
            std::cout << "[INFO] SiteService: Socket " << socket.socketId
                      << " programming " << (value ? "PASSED" : "FAILED") << "."
                      << std::endl;
        }
    }

    {
        std::lock_guard<std::mutex> siteLock(site.siteMutex);
        site.siteStatus = SiteStatus::COMPLETED;
        std::cout << "[INFO] SiteService: Site " << siteId
                  << " programming complete. Status = COMPLETED." << std::endl;
    }
}


std::vector<SiteStatusData> SiteService::getSiteStatusById(int siteId) {
    std::vector<SiteStatusData> siteStatusResult;
    if (siteId < 0 || siteId >= sites.size()) 
        return siteStatusResult; 
    SiteStatusData statusData;
    auto& site = *sites[siteId];
    std::lock_guard<std::mutex> siteLock(site.siteMutex);
    statusData.siteId = site.siteId;
    statusData.siteStatus = site.siteStatus;
    for (auto& socketPtr : site.sockets) {
        auto& socket = *socketPtr;
        std::lock_guard<std::mutex> socketLock(socket.socketMutex);
        SocketStatusData socketStatus;
        socketStatus.socketId = socket.socketId;
        socketStatus.isSocketPlaced = socket.isSocketPlaced;
        socketStatus.isSocketReadyForPick = socket.isSocketReadyForPick;
        socketStatus.isSocketPicked = socket.isSocketPicked;
        socketStatus.programmingResult = socket.programmingResult;
        statusData.sockets.push_back(socketStatus);
    }
    siteStatusResult.push_back(statusData);
    return siteStatusResult;
    
}