#include "utility/JobDTOMapper.hpp"

/// @brief Converts JobData to JobStatusDto.
/// @param jobData The JobData instance containing job runtime information.
/// @return A shared pointer to a JobStatusDto containing the job status and quantities.
oatpp::Object<JobStatusDto> JobDTOMapper::toDto(const JobData& jobData) {
  auto jobDto = JobStatusDto::createShared();
  jobDto->requiredQuantity = jobData.requiredQuantity;
  jobDto->completedQuantity = jobData.completedQuantity;
  jobDto->passedQuantity = jobData.passedQuantity;
  jobDto->failedQuantity = jobData.failedQuantity;

  switch (jobData.jobStatus) {
    case JobStatus::IDLE:
      jobDto->jobStatus = "IDLE";
      break;
    case JobStatus::RUNNING:
      jobDto->jobStatus = "RUNNING";
      break;
    case JobStatus::PAUSED:
      jobDto->jobStatus = "PAUSED";
      break;
    case JobStatus::STOPPED:
      jobDto->jobStatus = "STOPPED";
      break;
    case JobStatus::COMPLETED:
      jobDto->jobStatus = "COMPLETED";
      break;
    default:
      jobDto->jobStatus = "UNKNOWN";
      break;
  }

  return jobDto;
}
