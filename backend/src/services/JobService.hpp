#ifndef JobService_hpp
#define JobService_hpp

#include "models/JobModel.hpp"
#include <memory>
#include <iostream>


class JobService {
private:
    std::shared_ptr<JobModel> job;

public:
    JobService();
    bool startNewJob(int requiredQuantity);
    bool stopCurrentJob();
    bool pauseCurrentJob();
    bool resumeCurrentJob();
    void incrementCompletedQuantity();
    JobStatus getJobStatus();
    int getRequiredQuantity();
    int getCompletedQuantity();
    JobData getJobModel();
    bool isJobRunning();
    bool isJobPaused();
    bool isJobStopped();
    bool isJobCompleted();

};

#endif /* JobService_hpp */
