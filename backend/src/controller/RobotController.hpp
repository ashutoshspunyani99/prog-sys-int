#ifndef RobotController_hpp
#define RobotController_hpp

#include "dto/ResponseDTO.hpp"
#include "dto/SocketDTO.hpp"
#include "adapters/SiteAdapter.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Robot Api Controller.
 */
/// @brief API controller for robot-related socket operations.
///
/// Provides endpoints for robot to interact with sockets (place/pick devices).
class RobotController : public oatpp::web::server::api::ApiController {
private:
  std::shared_ptr<SiteAdapter> siteAdapter;

public:
  /// @brief Constructor.
  /// @param apiContentMappers Mapper for DTO serialization/deserialization.
  /// @param siteService Shared instance of SiteService.
  RobotController(
      OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>,
                      apiContentMappers),
      OATPP_COMPONENT(std::shared_ptr<SiteService>, siteService))
      : oatpp::web::server::api::ApiController(apiContentMappers),
        siteAdapter(std::make_shared<SiteAdapter>(siteService)) {}

public:
  /// @brief Get sockets ready to place devices.
  ///
  /// @return List of socket IDs or appropriate status message.
  ADD_CORS(readyToPlace);
  ENDPOINT("GET", "api/robot/ready-to-place", readyToPlace) {
    auto responseWrapper = siteAdapter->getReadyToPlaceSocketsResponse();
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }

  /// @brief Get sockets ready to pick devices.
  ///
  /// @return List of socket IDs or appropriate status message.
  ADD_CORS(readyToPick);
  ENDPOINT("GET", "api/robot/ready-to-pick", readyToPick) {
    auto responseWrapper = siteAdapter->getReadyToPickSocketsResponse();
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }

  /// @brief Notify system that a device has been placed in a socket.
  ///
  /// @param socketId ID of the socket where the device was placed.
  /// @return Operation result message with status code.
  ADD_CORS(devicePlaced);
  ENDPOINT("POST", "api/robot/device-placed", devicePlaced,QUERY(Int32, socketId)) {
    auto responseWrapper = siteAdapter->placeDeviceResponse(socketId);
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }

  /// @brief Notify system that a device has been picked from a socket.
  ///
  /// @param socketId ID of the socket where the device was picked from.
  /// @return Operation result message with status code.
  ADD_CORS(devicePicked);
  ENDPOINT("POST", "api/robot/device-picked", devicePicked,QUERY(Int32, socketId)) {
    auto responseWrapper = siteAdapter->pickDeviceResponse(socketId);
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }
  
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* RobotController_hpp */