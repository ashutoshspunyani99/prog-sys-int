#include "SiteAdapter.hpp"

SiteAdapter::SiteAdapter(std::shared_ptr<SiteService> siteService)
    : siteService(siteService) {}

ResponseWrapper<oatpp::Vector<oatpp::Int32>> SiteAdapter::getReadyToPlaceSocketsResponse() {
  auto res = ResponseDataDto<oatpp::Vector<oatpp::Int32>>::createShared();

  if (!siteService->ensureJobRunning()) {
    res->statusCode = 400;
    res->message = "No job is currently running";
    res->data = {};
    return std::make_pair(res, OatppUtils::getHttpStatus(400));
  }
  if (siteService->isJobCompleted()) {
    res->statusCode = 400;
    res->message = "Job is already completed";
    res->data = {};
    return std::make_pair(res, OatppUtils::getHttpStatus(400));
  }

  auto sockets = siteService->getReadyToPlaceSockets();
  if (sockets.empty()) {
    res->statusCode = 204;
    res->message = "No ready-to-place sockets found";
    res->data = {};
    return std::make_pair(res, OatppUtils::getHttpStatus(204));
  }
  res->statusCode = 200;
  res->message = "Ready-to-place sockets retrieved";
  res->data = OatppUtils::toOatppVector(sockets);
  return std::make_pair(res, OatppUtils::getHttpStatus(200));
}

ResponseWrapper<oatpp::Vector<oatpp::Int32>>
SiteAdapter::getReadyToPickSocketsResponse() {
  auto res = ResponseDataDto<oatpp::Vector<oatpp::Int32>>::createShared();

  if (!siteService->ensureJobRunning()) {
    res->statusCode = 400;
    res->message = "No job is currently running";
    res->data = {};
    return std::make_pair(res, OatppUtils::getHttpStatus(400));
  }
  if (siteService->isJobCompleted()) {
    res->statusCode = 400;
    res->message = "Job is already completed";
    res->data = {};
    return std::make_pair(res, OatppUtils::getHttpStatus(400));
  }

  auto sockets = siteService->getReadyToPickSockets();
  if (sockets.empty()) {
    res->statusCode = 204;
    res->message = "No ready-to-pick sockets found";
    res->data = {};
    return std::make_pair(res, OatppUtils::getHttpStatus(204));
  }
  res->statusCode = 200;
  res->message = "Ready-to-pick sockets retrieved";
  res->data = OatppUtils::toOatppVector(sockets);
  return std::make_pair(res, OatppUtils::getHttpStatus(200));
}

ResponseWrapper<oatpp::String> SiteAdapter::placeDeviceResponse(int socketId) {
  auto res = ResponseDataDto<oatpp::String>::createShared();
  if (!siteService->ensureJobRunning()) {
    res->statusCode = 400;
    res->message = "No job is currently running";
    res->data = nullptr;
    return std::make_pair(res, OatppUtils::getHttpStatus(400));
  }

  if (siteService->isJobCompleted()) {
    res->statusCode = 400;
    res->message = "Job is already completed";
    res->data = nullptr;
    return std::make_pair(res, OatppUtils::getHttpStatus(400));
  }

  bool success = siteService->placeDevice(socketId);
  if (success) {
    res->statusCode = 200;
    res->message = "Device placed successfully";
    res->data = "OK";
    return std::make_pair(res, OatppUtils::getHttpStatus(200));
  } else {
    res->statusCode = 400;
    res->message = "Failed to place device. Either invalid socket or not idle.";
    res->data = "FAILED";
    return std::make_pair(res, OatppUtils::getHttpStatus(400));
  }
}

ResponseWrapper<oatpp::String> SiteAdapter::pickDeviceResponse(int socketId) {
  auto res = ResponseDataDto<oatpp::String>::createShared();
  if (!siteService->ensureJobRunning()) {
    res->statusCode = 400;
    res->message = "No job is currently running";
    res->data = nullptr;
    return std::make_pair(res, OatppUtils::getHttpStatus(400));
  }

  if (siteService->isJobCompleted()) {
    res->statusCode = 400;
    res->message = "Job is already completed";
    res->data = nullptr;
    return std::make_pair(res, OatppUtils::getHttpStatus(400));
  }

  bool success = siteService->pickDevice(socketId);
  if (success) {
    res->statusCode = 200;
    res->message = "Device picked successfully";
    res->data = "OK";
    return std::make_pair(res, OatppUtils::getHttpStatus(200));
  } else {
    res->statusCode = 400;
    res->message =
        "Failed to pick device. Either invalid socket or not completed.";
    res->data = "FAILED";
    return std::make_pair(res, OatppUtils::getHttpStatus(400));
  }
}

ResponseWrapper<oatpp::Vector<oatpp::Object<SiteDto>>>
SiteAdapter::getProgrammingSocketsResponse(int siteId) {
  auto res =
      ResponseDataDto<oatpp::Vector<oatpp::Object<SiteDto>>>::createShared();

  auto sitesData = siteService->getSiteStatusById(siteId);
  if (sitesData.empty()) {
    res->statusCode = 204;
    res->message = "No site found with the given ID";
    res->data = {};
    return std::make_pair(res, OatppUtils::getHttpStatus(204));
  }

  auto siteDtos = oatpp::Vector<oatpp::Object<SiteDto>>::createShared();
  for (const auto &site : sitesData) {
    siteDtos->push_back(SiteDtoMapper::toDto(site));
  }

  res->statusCode = 200;
  res->message = "Sockets status retrieved successfully";
  res->data = siteDtos;

  return std::make_pair(res, OatppUtils::getHttpStatus(200));
}