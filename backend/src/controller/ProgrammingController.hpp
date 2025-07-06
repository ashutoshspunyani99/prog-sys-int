#ifndef ProgrammingController_hpp
#define ProgrammingController_hpp

#include "dto/ResponseDTO.hpp"
#include "services/SiteService.hpp"
#include "utility/SiteDTOMapper.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Programming Api Controller.
 */
class ProgrammingController : public oatpp::web::server::api::ApiController {
private:
    std::shared_ptr<SiteService> siteService;
public:
  /**
   * Constructor with object mapper.
   * @param apiContentMappers - mappers used to serialize/deserialize DTOs.
   */
  ProgrammingController(OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers),
  OATPP_COMPONENT(std::shared_ptr<SiteService>, siteService))
    : oatpp::web::server::api::ApiController(apiContentMappers),siteService(siteService)
  {}
public:

  ADD_CORS(getSiteSockets);
  ENDPOINT("GET", "api/programming/sockets", getSiteSockets, QUERY(Int32, siteId)) {
    std::vector<SiteStatusData> sitesData = siteService->getSiteStatusById(siteId);
    auto siteResponseDto = ProgrammingSitesResponseDto::createShared();
    if (sitesData.empty()) {
      siteResponseDto->statusCode = 404;
      siteResponseDto->message = "No sites found with the given ID";
      siteResponseDto->data ={};
      return createDtoResponse(Status::CODE_404, siteResponseDto);
    }
    
     auto& site = sitesData[0];
    auto siteDto = SiteDtoMapper::toDto(site);

    siteResponseDto->data = oatpp::Vector<oatpp::Object<SiteDto>>::createShared();
    siteResponseDto->data->push_back(siteDto);
    siteResponseDto->statusCode = 200;
    siteResponseDto->message = "Sockets status retrieved successfully";
    return createDtoResponse(Status::CODE_200, siteResponseDto);
  }
  
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* ProgrammingController_hpp */