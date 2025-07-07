#ifndef SiteAdapter_hpp
#define SiteAdapter_hpp

#include "services/SiteService.hpp"
#include "dto/ResponseDTO.hpp"
#include "models/SiteModel.hpp"
#include "utility/OatppUtils.hpp"
#include "dto/SiteDTO.hpp" 
#include "utility/SiteDTOMapper.hpp"

#include <iostream>
#include <memory>
#include <thread>
#include <vector>

/// @brief Adapter for handling site-related API responses.
///
/// Connects API calls to SiteService and returns standardized responses.
class SiteAdapter {
private:
  std::shared_ptr<SiteService> siteService;

public:
  SiteAdapter(std::shared_ptr<SiteService> siteService);

  /// Gets socket IDs ready for placing devices.
  ///
  /// @return A list of socket IDs or appropriate error/status response.
  ResponseWrapper<oatpp::Vector<oatpp::Int32>> getReadyToPlaceSocketsResponse();

  /// Gets socket IDs ready for picking devices.
  ///
  /// @return A list of socket IDs or appropriate error/status response.
  ResponseWrapper<oatpp::Vector<oatpp::Int32>> getReadyToPickSocketsResponse();

  /// Attempts to place a device into a socket.
  ///
  /// @param socketId The ID of the target socket.
  /// @return Success or failure message with HTTP status.
  ResponseWrapper<oatpp::String> placeDeviceResponse(int socketId);

  /// Attempts to pick a device from a socket.
  ///
  /// @param socketId The ID of the target socket.
  /// @return Success or failure message with HTTP status.
  ResponseWrapper<oatpp::String> pickDeviceResponse(int socketId);

  /// Retrieves socket statuses for a specific site.
  ///
  /// @param siteId The ID of the site to query.
  /// @return A list of SiteDto objects representing socket status.
  ResponseWrapper<oatpp::Vector<oatpp::Object<SiteDto>>> getProgrammingSocketsResponse(int siteId);
};

#endif /* SiteAdapter_hpp */