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
    JobData getJobModel();
    bool isJobRunning();
    bool isJobCompleted();
    void incrementFailedQuantity();
};

#endif /* JobService_hpp */
