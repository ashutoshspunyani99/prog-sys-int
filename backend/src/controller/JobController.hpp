#ifndef JobController_hpp
#define JobController_hpp

#include "dto/JobDTO.hpp"
#include "dto/ResponseDTO.hpp"
#include "adapters/JobAdapter.hpp"
#include "utility/JobDTOMapper.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Job Api Controller.
 */
class JobController : public oatpp::web::server::api::ApiController {
private:
    std::shared_ptr<JobAdapter> jobAdapter;

  public:
    /**
     * Constructor with object mapper.
     * @param apiContentMappers - mappers used to serialize/deserialize DTOs.
     */
    JobController(
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>,
                        apiContentMappers),
        OATPP_COMPONENT(std::shared_ptr<JobService>, jobService))
        : oatpp::web::server::api::ApiController(apiContentMappers),
          jobAdapter(std::make_shared<JobAdapter>(jobService)) {}

  public:
    ENDPOINT("POST", "api/job/start", startJob,
             BODY_DTO(Object<JobStartDto>, body)) {
      auto responseWrapper =
          jobAdapter->startJobResponse(body->requiredQuantity);
      return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }

  ENDPOINT("POST", "api/job/stop", stopJob) {
    auto responseWrapper = jobAdapter->stopJobResponse();
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }

  ENDPOINT("POST", "api/job/pause", pauseJob) {
    auto responseWrapper = jobAdapter->pauseJobResponse();
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }

  ENDPOINT("POST", "api/job/resume", resumeJob) {
    auto responseWrapper = jobAdapter->resumeJobResponse();
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }

    ENDPOINT("GET", "api/job/status", getJobStatus) {
      auto responseWrapper =
          jobAdapter->getJobStatusResponse();
      return createDtoResponse(responseWrapper.second, responseWrapper.first);
    }

  
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* JobController_hpp */