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
    
    /// @brief Simulates the programming process for a site.
    /// @param siteId ID of the site to process.
    /// Adds delay, sets programming results, and updates site status.
    void siteProgramming(int siteId);

public:
  /// @brief Constructor for SiteService.
  /// Initializes two sites and links to the JobService.
  SiteService(std::shared_ptr<JobService> jobService);

  /// @brief Gets socket IDs that are ready for device placement.
  /// @return Vector of socket IDs ready for placing devices.
  std::vector<int> getReadyToPlaceSockets();

  /// @brief Gets socket IDs that are ready for device picking.
  /// @return Vector of socket IDs ready for picking devices.
  std::vector<int> getReadyToPickSockets();

  /// @brief Attempts to place a device in the specified socket.
  /// @param socketId ID of the socket to place the device in.
  /// @return True if successful, false if invalid or already used.
  bool placeDevice(int socketId);

  /// @brief Picks a device from the specified socket if it's ready.
  /// @param socketId ID of the socket to pick the device from.
  /// @return True if picked successfully, false otherwise.
  bool pickDevice(int socketId);

  /// @brief Ensures a job is currently running.
  /// @return True if a job is running, false otherwise.
  bool ensureJobRunning();

  /// @brief Checks if the current job is completed.
  /// @return True if the job is completed, false otherwise.
  bool isJobCompleted();

  /// @brief Retrieves the status of all sockets in the specified site.
  /// @param siteId ID of the site to query.
  /// @return A vector with one SiteStatusData entry if valid, empty otherwise.
  std::vector<SiteStatusData> getSiteStatusById(int siteId);
};

#endif /* SiteService_hpp */