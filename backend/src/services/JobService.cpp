#include "JobService.hpp"

JobService::JobService() {
    job = std::make_shared<JobModel>();
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
        return false; 
    }
 
    if (job->jobStatus == JobStatus::RUNNING || job->jobStatus == JobStatus::PAUSED) {
        return false; 
    }

    job->requiredQuantity = requiredQuantity;
    job->completedQuantity = 0;
    job->passedQuantity = 0;
    job->failedQuantity = 0;
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
    std::cout<< "Job Passed Quantity: " 
              << job->passedQuantity << std::endl;
    std::cout<< "Job Failed Quantity: " 
              << job->failedQuantity << std::endl;
    if (job->jobStatus == JobStatus::RUNNING) {
        job->completedQuantity++;
        job->passedQuantity++;
        if (job->completedQuantity >= job->requiredQuantity) {
            job->jobStatus = JobStatus::COMPLETED;
        }
    }
}
void JobService::incrementFailedQuantity() {
    std::lock_guard<std::mutex> lock(job->jobMutex);
    std::cout << "Incrementing Failed quantity for job. Current status: "
              << static_cast<int>(job->jobStatus) << std::endl;
    std::cout << "Failed Quantity before increment: "
              << job->failedQuantity << std::endl;
    std::cout << "Required Quantity: " << job->requiredQuantity << std::endl;
    std::cout << "Completed Quantity: " << job->completedQuantity << std::endl;
    std::cout << "Job Passed Quantity: " << job->passedQuantity << std::endl;
    if (job->jobStatus == JobStatus::RUNNING) {
        job->failedQuantity++;
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
    std::cout << "Job Status: " << static_cast<int>(jobData.jobStatus) << std::endl;
    std::cout << "Required Quantity: " << jobData.requiredQuantity << std::endl;
    std::cout << "Completed Quantity: " << jobData.completedQuantity << std::endl;
    std::cout << "Passed Quantity: " << jobData.passedQuantity << std::endl;
    std::cout << "Failed Quantity: " << jobData.failedQuantity << std::endl;
    return jobData;
}




