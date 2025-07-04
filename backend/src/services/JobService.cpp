#include "JobService.hpp"

JobService::JobService() {
    job = std::make_shared<JobModel>();
}

bool JobService::startNewJob(int requiredQuantity) {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    if (job->status == JobStatus::RUNNING || job->status == JobStatus::PAUSED) {
        return false; 
    }
    job->requiredQuantity = requiredQuantity;
    job->completedQuantity = 0;
    job->status = JobStatus::RUNNING;
    return true;
}

bool JobService::stopCurrentJob() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    if (job->status == JobStatus::RUNNING || job->status == JobStatus::PAUSED) {
        job->requiredQuantity = 0;
        job->completedQuantity = 0;
        job->status = JobStatus::STOPPED;
        return true; 
    }
    return false;
}

bool JobService::pauseCurrentJob() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    if (job->status == JobStatus::RUNNING) {
        job->status = JobStatus::PAUSED;
        return true; 
    }
    return false;
}

bool JobService::resumeCurrentJob() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    if (job->status == JobStatus::PAUSED) {
        job->status = JobStatus::RUNNING;
        return true; 
    }
    return false;
}



