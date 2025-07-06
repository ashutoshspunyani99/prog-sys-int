#include "JobAdapter.hpp"

JobAdapter::JobAdapter(std::shared_ptr<JobService> jobService)
    : jobService(jobService) {}

ResponseWrapper<oatpp::String>
JobAdapter::startJobResponse(int requiredQuantity) {
  auto res = ResponseDataDto<oatpp::String>::createShared();
  if (jobService->startNewJob(requiredQuantity)) {
    res->statusCode = 200;
    res->message = "Job started successfully";
    return {res, OatppUtils::getHttpStatus(200)};
  } else {
    res->statusCode = 400;
    res->message = "Job is already running or paused";
    return {res, OatppUtils::getHttpStatus(400)};
  }
}

ResponseWrapper<oatpp::String> JobAdapter::stopJobResponse() {
  auto res = ResponseDataDto<oatpp::String>::createShared();
  if (jobService->stopCurrentJob()) {
    res->statusCode = 200;
    res->message = "Job stopped successfully";
    return {res, OatppUtils::getHttpStatus(200)};
  } else {
    res->statusCode = 400;
    res->message = "No job is currently running or paused";
    return {res, OatppUtils::getHttpStatus(400)};
  }
}

ResponseWrapper<oatpp::String> JobAdapter::pauseJobResponse() {
  auto res = ResponseDataDto<oatpp::String>::createShared();
  if (jobService->pauseCurrentJob()) {
    res->statusCode = 200;
    res->message = "Job paused successfully";
    return {res, OatppUtils::getHttpStatus(200)};
  } else {
    res->statusCode = 400;
    res->message = "No job is currently running";
    return {res, OatppUtils::getHttpStatus(400)};
  }
}

ResponseWrapper<oatpp::String> JobAdapter::resumeJobResponse() {
  auto res = ResponseDataDto<oatpp::String>::createShared();
  if (jobService->resumeCurrentJob()) {
    res->statusCode = 200;
    res->message = "Job resumed successfully";
    return {res, OatppUtils::getHttpStatus(200)};
  } else {
    res->statusCode = 400;
    res->message = "No job is currently paused";
    return {res, OatppUtils::getHttpStatus(400)};
  }
}

ResponseWrapper<oatpp::Object<JobStatusDto>>
JobAdapter::getJobStatusResponse() {
  auto jobData = jobService->getJobModel();
  auto jobDto = JobDTOMapper::toDto(jobData);
  auto res = ResponseDataDto<oatpp::Object<JobStatusDto>>::createShared();
  res->statusCode = 200;
  res->message = "Job status retrieved successfully";
  res->data = jobDto;
  return {res, OatppUtils::getHttpStatus(200)};
}