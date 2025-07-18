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
/// @brief API controller for job-related operations.
///
/// Handles HTTP endpoints for managing job lifecycle.
class JobController : public oatpp::web::server::api::ApiController {
private:
    std::shared_ptr<JobAdapter> jobAdapter;

  public:
    /// @brief Constructor.
    /// @param apiContentMappers Mapper for DTO serialization/deserialization.
    /// @param jobService Shared instance of JobService.
    JobController(
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>,
                        apiContentMappers),
        OATPP_COMPONENT(std::shared_ptr<JobService>, jobService))
        : oatpp::web::server::api::ApiController(apiContentMappers),
          jobAdapter(std::make_shared<JobAdapter>(jobService)) {}

  public:
    /// @brief Start a new job.
    /// @param body Request body containing required quantity.
    /// @return HTTP response with success or failure message.
    ADD_CORS(startJob);
    ENDPOINT("POST", "api/job/start", startJob,
             BODY_DTO(Object<JobStartDto>, body)) {
      auto responseWrapper =
          jobAdapter->startJobResponse(body->requiredQuantity);
      return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }

  /// @brief Stop the current job.
  /// @return HTTP response with result of stop action.
  ADD_CORS(stopJob);
  ENDPOINT("POST", "api/job/stop", stopJob) {
    auto responseWrapper = jobAdapter->stopJobResponse();
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }

  /// @brief Pause the current job.
  /// @return HTTP response with result of pause action.
  ADD_CORS(pauseJob);
  ENDPOINT("POST", "api/job/pause", pauseJob) {
    auto responseWrapper = jobAdapter->pauseJobResponse();
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }

  /// @brief Resume a paused job.
  /// @return HTTP response with result of resume action.
  ADD_CORS(resumeJob);
  ENDPOINT("POST", "api/job/resume", resumeJob) {
    auto responseWrapper = jobAdapter->resumeJobResponse();
    return createDtoResponse(responseWrapper.second, responseWrapper.first);
  }

  /// @brief Get status of the current job.
  /// @return HTTP response with job status information.
  ADD_CORS(getJobStatus);
    ENDPOINT("GET", "api/job/status", getJobStatus) {
      auto responseWrapper =
          jobAdapter->getJobStatusResponse();
      return createDtoResponse(responseWrapper.second, responseWrapper.first);
    }
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* JobController_hpp */