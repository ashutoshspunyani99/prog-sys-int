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
class RobotController : public oatpp::web::server::api::ApiController {
private:
  std::shared_ptr<SiteAdapter> siteAdapter;

public:
  /**
   * Constructor with object mapper.
   * @param apiContentMappers - mappers used to serialize/deserialize DTOs.
   */
  RobotController(
      OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>,
                      apiContentMappers),
      OATPP_COMPONENT(std::shared_ptr<SiteService>, siteService))
      : oatpp::web::server::api::ApiController(apiContentMappers),
        siteAdapter(std::make_shared<SiteAdapter>(siteService)) {}

public:
  ADD_CORS(readyToPlace);
  ENDPOINT("GET", "api/robot/ready-to-place", readyToPlace) {
    auto responseWrapper = siteAdapter->getReadyToPlaceSocketsResponse();
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }

  ADD_CORS(readyToPick);
  ENDPOINT("GET", "api/robot/ready-to-pick", readyToPick) {
    auto responseWrapper = siteAdapter->getReadyToPickSocketsResponse();
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }
  
  ADD_CORS(devicePlaced);
  ENDPOINT("POST", "api/robot/device-placed", devicePlaced,QUERY(Int32, socketId)) {
    auto responseWrapper = siteAdapter->placeDeviceResponse(socketId);
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }

  ADD_CORS(devicePicked);
  ENDPOINT("POST", "api/robot/device-picked", devicePicked,QUERY(Int32, socketId)) {
    auto responseWrapper = siteAdapter->pickDeviceResponse(socketId);
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }
  
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* RobotController_hpp */