#ifndef JobService_hpp
#define JobService_hpp

#include "models/JobModel.hpp"
#include <memory>


class JobService {
private:
    std::shared_ptr<JobModel> job;

public:
    JobService();
    bool startNewJob(int requiredQuantity);
    bool stopCurrentJob();
    bool pauseCurrentJob();
    bool resumeCurrentJob();
};

#endif /* JobService_hpp */
