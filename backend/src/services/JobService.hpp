#ifndef JobService_hpp
#define JobService_hpp

#include "models/JobModel.hpp"
#include <memory>
#include <iostream>


class JobService {
private:
    std::shared_ptr<JobModel> job;

public:
  /// @brief Constructor for JobService.
  /// Initializes a new job model instance.
  JobService();

  /// @brief Starts a new job with the given required quantity.
  /// @param requiredQuantity The number of units to process.
  /// @return True if job was successfully started, otherwise false.
  bool startNewJob(int requiredQuantity);

  /// @brief Stops the current running or paused job.
  /// @return True if job was successfully stopped, otherwise false.
  bool stopCurrentJob();

  /// @brief Pauses the currently running job.
  /// @return True if job was paused, otherwise false.
  bool pauseCurrentJob();

  /// @brief Resumes a paused job.
  /// @return True if job was resumed, otherwise false.
  bool resumeCurrentJob();

  /// @brief Increments the completed and passed quantity if job is running.
  /// Marks job as COMPLETED if required quantity is reached.
  void incrementCompletedQuantity();

  /// @brief Retrieves a copy of the current job data.
  /// @return A JobData struct with the current job state.
  JobData getJobModel();

  /// @brief Checks if a job is currently running.
  /// @return True if job status is RUNNING, otherwise false.
  bool isJobRunning();

  /// @brief Checks if the job is completed.
  /// @return True if job status is COMPLETED, otherwise false.
  bool isJobCompleted();
  
  /// @brief Increments the failed quantity if job is running.
  void incrementFailedQuantity();
};

#endif /* JobService_hpp */
