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

class SiteAdapter {
private:
  std::shared_ptr<SiteService> siteService;

public:
  SiteAdapter(std::shared_ptr<SiteService> siteService);

  ResponseWrapper<oatpp::Vector<oatpp::Int32>> getReadyToPlaceSocketsResponse();

  ResponseWrapper<oatpp::Vector<oatpp::Int32>> getReadyToPickSocketsResponse();

  ResponseWrapper<oatpp::String> placeDeviceResponse(int socketId);

  ResponseWrapper<oatpp::String> pickDeviceResponse(int socketId);
  ResponseWrapper<oatpp::Vector<oatpp::Object<SiteDto>>> getProgrammingSocketsResponse(int siteId);
};

#endif /* SiteAdapter_hpp */