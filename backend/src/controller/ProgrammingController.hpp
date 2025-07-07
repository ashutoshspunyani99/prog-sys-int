#ifndef ProgrammingController_hpp
#define ProgrammingController_hpp

#include "dto/ResponseDTO.hpp"
#include "adapters/SiteAdapter.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Programming Api Controller.
 */
class ProgrammingController : public oatpp::web::server::api::ApiController {
private:
  std::shared_ptr<SiteAdapter> siteAdapter;

public:
  /**
   * Constructor with object mapper.
   * @param apiContentMappers - mappers used to serialize/deserialize DTOs.
   */
  ProgrammingController(
      OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>,
                      apiContentMappers),
      OATPP_COMPONENT(std::shared_ptr<SiteService>, siteService))
      : oatpp::web::server::api::ApiController(apiContentMappers),
        siteAdapter(std::make_shared<SiteAdapter>(siteService)) {}

public:

  ADD_CORS(getSiteSockets);
  ENDPOINT("GET", "api/programming/sockets", getSiteSockets, QUERY(Int32, siteId)) {
    auto responseWrapper = siteAdapter->getProgrammingSocketsResponse(siteId);
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }
  
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* ProgrammingController_hpp */