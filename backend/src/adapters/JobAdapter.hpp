#ifndef JobAdapter_hpp
#define JobAdapter_hpp

#include "dto/JobDTO.hpp"
#include "dto/ResponseDTO.hpp"
#include "models/SiteModel.hpp"
#include "services/JobService.hpp"
#include "utility/JobDTOMapper.hpp"
#include "utility/OatppUtils.hpp"

#include <iostream>
#include <memory>

/// @brief Adapter for handling job-related API responses.
///
/// Connects API calls to JobService and returns standardized responses.
class JobAdapter {
private:
  std::shared_ptr<JobService> jobService;

public:
  JobAdapter(std::shared_ptr<JobService> jobService);
  /// Starts a new job with the specified required quantity.
  ///
  /// @param requiredQuantity The number of items the job should process.
  /// @return A ResponseWrapper containing a message indicating success or
  /// failure, and the appropriate HTTP status code.
  ResponseWrapper<oatpp::String> startJobResponse(int requiredQuantity);

  /// Stops the currently running or paused job, if any.
  ///
  /// @return A ResponseWrapper containing a message indicating success or
  /// failure, and the appropriate HTTP status code.
  ResponseWrapper<oatpp::String> stopJobResponse();

  /// Pauses the currently running job, if any.
  ///
  /// @return A ResponseWrapper containing a message indicating success or
  /// failure, and the appropriate HTTP status code.
  ResponseWrapper<oatpp::String> pauseJobResponse();

  /// Resumes a currently paused job, if any.
  ///
  /// @return A ResponseWrapper containing a message indicating success or
  /// failure, and the appropriate HTTP status code.
  ResponseWrapper<oatpp::String> resumeJobResponse();

  // Retrieves the status of the current job.
  ///
  /// @return A ResponseWrapper containing the job status data in a
  /// JobStatusDto,
  ///         a success message, and the HTTP 200 status code.
  ResponseWrapper<oatpp::Object<JobStatusDto>> getJobStatusResponse();
};

#endif /* JobAdapter_hpp */