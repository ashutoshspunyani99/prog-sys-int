#ifndef SiteService_hpp
#define SiteService_hpp

#include "models/SiteModel.hpp"
#include "utility/OatppUtils.hpp"
#include "JobService.hpp"
#include "dto/ResponseDTO.hpp"

#include <memory>
#include <vector>
#include <thread>
#include <iostream>

class SiteService {
private:
    std::vector<std::shared_ptr<SiteModel>> sites;
    std::shared_ptr<JobService> jobService;
    void siteProgramming(int siteId);

public:
    SiteService(std::shared_ptr<JobService> jobService);
    std::vector<int> getReadyToPlaceSockets();
    std::vector<int> getReadyToPickSockets();
    bool placeDevice(int socketId);
    bool pickDevice(int socketId);
    bool ensureJobRunning();
    bool isJobCompleted();
    std::vector<SiteStatusData> getSiteStatusById(int siteId);
};

#endif /* SiteService_hpp */