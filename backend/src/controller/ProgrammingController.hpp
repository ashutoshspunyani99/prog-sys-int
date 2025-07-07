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
/// @brief API controller for programming site sockets.
///
/// Handles endpoints related to site socket programming and status retrieval.
class ProgrammingController : public oatpp::web::server::api::ApiController {
private:
  std::shared_ptr<SiteAdapter> siteAdapter;

public:
  /// @brief Constructor.
  /// @param apiContentMappers Mapper for DTO serialization/deserialization.
  /// @param siteService Shared instance of SiteService.
  ProgrammingController(
      OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>,
                      apiContentMappers),
      OATPP_COMPONENT(std::shared_ptr<SiteService>, siteService))
      : oatpp::web::server::api::ApiController(apiContentMappers),
        siteAdapter(std::make_shared<SiteAdapter>(siteService)) {}

public:
  /// @brief Get programming socket status by site ID.
  ///
  /// @param siteId ID of the site.
  /// @return List of socket statuses or appropriate error response.
  ADD_CORS(getSiteSockets);
  ENDPOINT("GET", "api/programming/sockets", getSiteSockets, QUERY(Int32, siteId)) {
    auto responseWrapper = siteAdapter->getProgrammingSocketsResponse(siteId);
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }
  
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* ProgrammingController_hpp */