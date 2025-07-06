#ifndef RobotController_hpp
#define RobotController_hpp

#include "dto/ResponseDTO.hpp"
#include "dto/SocketDTO.hpp"
#include "services/SiteService.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Robot Api Controller.
 */
class RobotController : public oatpp::web::server::api::ApiController {
private:
    std::shared_ptr<SiteService> siteService;
public:
  /**
   * Constructor with object mapper.
   * @param apiContentMappers - mappers used to serialize/deserialize DTOs.
   */
  RobotController(OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers),
  OATPP_COMPONENT(std::shared_ptr<SiteService>, siteService))
    : oatpp::web::server::api::ApiController(apiContentMappers),siteService(siteService)
  {}
public:
  
  ENDPOINT("GET", "api/robot/ready-to-place", readyToPlace) {
    auto socketListDto = SocketListDto::createShared();
    auto readyToPickSockets = siteService->getReadyToPlaceSockets();
    if (readyToPickSockets.empty()) {
      socketListDto->statusCode = 404;
      socketListDto->message = "No sockets ready to place devices";
      socketListDto->socketIds = {};
      return createDtoResponse(Status::CODE_404, socketListDto);
    }
    socketListDto->socketIds = oatpp::Vector<oatpp::Int32>::createShared();
    for (const auto &socketId : readyToPickSockets) {
      socketListDto->socketIds->push_back(socketId);
    }
    socketListDto->statusCode = 200;
    socketListDto->message = "Ready to place sockets retrieved successfully";
    return createDtoResponse(Status::CODE_200, socketListDto);
  }

  ENDPOINT("GET", "api/robot/ready-to-pick", readyToPick) {
    auto socketListDto = SocketListDto::createShared();
    auto readyToPickSockets = siteService->getReadyToPickSockets();
    if (readyToPickSockets.empty()) {
      socketListDto->statusCode = 404;
      socketListDto->message = "No sockets ready to pick devices";
      socketListDto->socketIds = {};
      return createDtoResponse(Status::CODE_404, socketListDto);
    }
    socketListDto->socketIds = oatpp::Vector<oatpp::Int32>::createShared();
    for (const auto& socketId : readyToPickSockets) {
      socketListDto->socketIds->push_back(socketId);
    }
    socketListDto->statusCode = 200;
    socketListDto->message = "Ready to pick sockets retrieved successfully";
    return createDtoResponse(Status::CODE_200, socketListDto);
  }
  
  ENDPOINT("POST", "api/robot/device-placed", devicePlaced,QUERY(Int32, socketId)) {
    auto respsoneDto = ResponseDto::createShared();
    bool isPlaced = siteService->placeDevice(socketId);
    if (!isPlaced) {
      respsoneDto->statusCode = 400;
      respsoneDto->message = "Failed to place device. Socket may already be occupied or not ready.";
      return createDtoResponse(Status::CODE_400, respsoneDto);
    }
    respsoneDto->statusCode = 200;
    respsoneDto->message = "Device placed successfully";
    return createDtoResponse(Status::CODE_200, respsoneDto);
  }

  ENDPOINT("POST", "api/robot/device-picked", devicePicked,QUERY(Int32, socketId)) {
    auto respsoneDto = ResponseDto::createShared();
    bool isPicked = siteService->pickDevice(socketId);
    if (!isPicked) {
      respsoneDto->statusCode = 400;
      respsoneDto->message = "Failed to pick device. Socket may not be ready or not occupied.";
      return createDtoResponse(Status::CODE_400, respsoneDto);
    }
    respsoneDto->statusCode = 200;
    respsoneDto->message = "Device picked successfully";
    return createDtoResponse(Status::CODE_200, respsoneDto);
  }
  
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* RobotController_hpp */