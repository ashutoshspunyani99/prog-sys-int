#include "JobService.hpp"

JobService::JobService() {
  job = std::make_shared<JobModel>();
  std::cout
      << "[INFO] JobService: Initialized JobService with default job state."
      << std::endl;
}

bool JobService::isJobCompleted() {
  std::lock_guard<std::mutex> lock(job->jobMutex);
  return job->jobStatus == JobStatus::COMPLETED;
}

bool JobService::isJobRunning() {
  std::lock_guard<std::mutex> lock(job->jobMutex);
  return job->jobStatus == JobStatus::RUNNING;
}

bool JobService::startNewJob(int requiredQuantity) {
  std::lock_guard<std::mutex> lock(job->jobMutex);
  if (requiredQuantity <= 0) {
    std::cout
        << "[WARN] JobService: Invalid job start request. Quantity must be > 0."
        << std::endl;
    return false;
  }

  if (job->jobStatus == JobStatus::RUNNING ||
      job->jobStatus == JobStatus::PAUSED) {
    std::cout
        << "[WARN] JobService: Cannot start job. Another job is already active."
        << std::endl;
    return false;
  }

  job->requiredQuantity = requiredQuantity;
  job->completedQuantity = 0;
  job->passedQuantity = 0;
  job->failedQuantity = 0;
  job->jobStatus = JobStatus::RUNNING;
  std::cout << "[INFO] JobService: New job started. Required quantity: "
            << requiredQuantity << std::endl;
  return true;
}

bool JobService::stopCurrentJob() {
  std::lock_guard<std::mutex> lock(job->jobMutex);
  if (job->jobStatus == JobStatus::RUNNING ||
      job->jobStatus == JobStatus::PAUSED) {
    job->requiredQuantity = 0;
    job->completedQuantity = 0;
    job->passedQuantity = 0;
    job->failedQuantity = 0;
    job->jobStatus = JobStatus::STOPPED;
    std::cout << "[INFO] JobService: Job stopped." << std::endl;
    return true;
  }
  return false;
}

bool JobService::pauseCurrentJob() {
  std::lock_guard<std::mutex> lock(job->jobMutex);
  if (job->jobStatus == JobStatus::RUNNING) {
    job->jobStatus = JobStatus::PAUSED;
    std::cout << "[INFO] JobService: Job paused." << std::endl;
    return true;
  }
  return false;
}

bool JobService::resumeCurrentJob() {
  std::lock_guard<std::mutex> lock(job->jobMutex);
  if (job->jobStatus == JobStatus::PAUSED) {
    job->jobStatus = JobStatus::RUNNING;
    std::cout << "[INFO] JobService: Job resumed." << std::endl;
    return true;
  }
  return false;
}

void JobService::incrementCompletedQuantity() {
  std::lock_guard<std::mutex> lock(job->jobMutex);
  if (job->jobStatus == JobStatus::RUNNING) {
    job->completedQuantity++;
    job->passedQuantity++;
    if (job->completedQuantity >= job->requiredQuantity) {
      job->jobStatus = JobStatus::COMPLETED;
      std::cout << "[INFO] JobService: Job completed successfully."
                << std::endl;
    } else {
      std::cout << "[INFO] JobService: Job in progress. Completed: "
                << job->completedQuantity
                << ", Required: " << job->requiredQuantity << std::endl;
    }
  }
}
void JobService::incrementFailedQuantity() {
  std::lock_guard<std::mutex> lock(job->jobMutex);
  if (job->jobStatus == JobStatus::RUNNING) {
    job->failedQuantity++;
    std::cout
        << "[DEBUG] JobService: Incremented failed quantity. Total failed: "
        << job->failedQuantity << std::endl;
  }
}

JobData JobService::getJobModel() {
  std::lock_guard<std::mutex> lock(job->jobMutex);
  JobData jobData;
  jobData.requiredQuantity = job->requiredQuantity;
  jobData.completedQuantity = job->completedQuantity;
  jobData.passedQuantity = job->passedQuantity;
  jobData.failedQuantity = job->failedQuantity;
  jobData.jobStatus = job->jobStatus;
  std::cout << "[DEBUG] JobService: Job status requested. Status: "
            << static_cast<int>(jobData.jobStatus) << std::endl;
  return jobData;
}
