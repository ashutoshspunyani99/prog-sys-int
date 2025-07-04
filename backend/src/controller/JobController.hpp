#ifndef JobController_hpp
#define JobController_hpp

#include "dto/JobDTO.hpp"
#include "dto/ResponseDTO.hpp"
#include "services/JobService.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Job Api Controller.
 */
class JobController : public oatpp::web::server::api::ApiController {
private:
    std::shared_ptr<JobService> jobService;
public:
  /**
   * Constructor with object mapper.
   * @param apiContentMappers - mappers used to serialize/deserialize DTOs.
   */
  JobController(OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers))
    : oatpp::web::server::api::ApiController(apiContentMappers),jobService(std::make_shared<JobService>())
  {}
public:
  
  ENDPOINT("POST", "api/job/start", startJob, BODY_DTO(Object<JobStartDto>, body)) {
    bool isJobStarted= jobService->startNewJob(body->requiredQuantity);
    auto resData = ResponseDto::createShared();
    if (isJobStarted) {
    resData->statusCode = 200;
    resData->message = "Job started successfully";
    return createDtoResponse(Status::CODE_200, resData);}
    else {
      resData->statusCode = 400;
      resData->message = "Job is already running or paused";
      return createDtoResponse(Status::CODE_400, resData);
    }
  }

  ENDPOINT("POST", "api/job/stop", stopJob) {
    bool isJobStopped= jobService->stopCurrentJob();
    auto resData = ResponseDto::createShared();
    if (isJobStopped) {
    resData->statusCode = 200;
    resData->message = "Job stopped successfully";
    return createDtoResponse(Status::CODE_200, resData);}
    else {
      resData->statusCode = 400;
      resData->message = "No job is currently running or paused";
      return createDtoResponse(Status::CODE_400, resData);
    }
  }

  ENDPOINT("POST", "api/job/pause", pauseJob) {
    bool isJobPaused= jobService->pauseCurrentJob();
    auto resData = ResponseDto::createShared();
    if (isJobPaused) {
    resData->statusCode = 200;
    resData->message = "Job paused successfully";
    return createDtoResponse(Status::CODE_200, resData);}
    else {
      resData->statusCode = 400;
      resData->message = "No job is currently running";
      return createDtoResponse(Status::CODE_400, resData);
    }
  }

  ENDPOINT("POST", "api/job/resume", resumeJob) {
    bool isJobResumed= jobService->resumeCurrentJob();
    auto resData = ResponseDto::createShared();
    if (isJobResumed) {
    resData->statusCode = 200;
    resData->message = "Job resumed successfully";
    return createDtoResponse(Status::CODE_200, resData);}
    else {
      resData->statusCode = 400;
      resData->message = "No job is currently paused";
      return createDtoResponse(Status::CODE_400, resData);
    }
  }
  
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* JobController_hpp */