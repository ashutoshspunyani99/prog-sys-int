#include "JobService.hpp"

JobService::JobService() {
    job = std::make_shared<JobModel>();
}

bool JobService::isJobCompleted() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    return job->jobStatus == JobStatus::COMPLETED;
}
bool JobService::isJobPaused() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    return job->jobStatus == JobStatus::PAUSED;
}
bool JobService::isJobStopped() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    return job->jobStatus == JobStatus::STOPPED;
}

bool JobService::isJobRunning() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    return job->jobStatus == JobStatus::RUNNING;
}

bool JobService::startNewJob(int requiredQuantity) {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    if (requiredQuantity <= 0) {
        return false; 
    }
 
    if (job->jobStatus == JobStatus::RUNNING || job->jobStatus == JobStatus::PAUSED) {
        return false; 
    }

    job->requiredQuantity = requiredQuantity;
    job->completedQuantity = 0;
    job->jobStatus = JobStatus::RUNNING;
    return true;
}

bool JobService::stopCurrentJob() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    if (job->jobStatus == JobStatus::RUNNING || job->jobStatus == JobStatus::PAUSED) {
        job->requiredQuantity = 0;
        job->completedQuantity = 0;
        job->jobStatus = JobStatus::STOPPED;
        return true; 
    }
    return false;
}

bool JobService::pauseCurrentJob() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    if (job->jobStatus == JobStatus::RUNNING) {
        job->jobStatus = JobStatus::PAUSED;
        return true; 
    }
    return false;
}

bool JobService::resumeCurrentJob() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    if (job->jobStatus == JobStatus::PAUSED) {
        job->jobStatus = JobStatus::RUNNING;
        return true; 
    }
    return false;
}

void JobService::incrementCompletedQuantity() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    std::cout << "Incrementing completed quantity for job. Current status: " 
              << static_cast<int>(job->jobStatus) << std::endl;
    std::cout << "Completed Quantity before increment: " 
              << job->completedQuantity << std::endl;
    std::cout << "Required Quantity: "  
              << job->requiredQuantity << std::endl;    
    if (job->jobStatus == JobStatus::RUNNING) {
        job->completedQuantity++;
        if (job->completedQuantity >= job->requiredQuantity) {
            job->jobStatus = JobStatus::COMPLETED;
        }
    }
}

JobStatus JobService::getJobStatus() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    return job->jobStatus;
}           

int JobService::getRequiredQuantity() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    return job->requiredQuantity;
}

int JobService::getCompletedQuantity() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    return job->completedQuantity;
}

JobData JobService::getJobModel() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    JobData jobData;
    jobData.requiredQuantity = job->requiredQuantity;
    jobData.completedQuantity = job->completedQuantity;
    jobData.jobStatus = job->jobStatus;
    std::cout << "Job Status: " << static_cast<int>(jobData.jobStatus) << std::endl;
    std::cout << "Required Quantity: " << jobData.requiredQuantity << std::endl;
    std::cout << "Completed Quantity: " << jobData.completedQuantity << std::endl;
    return jobData;
}




