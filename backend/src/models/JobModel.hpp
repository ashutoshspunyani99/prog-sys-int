#ifndef JobModel_hpp
#define JobModel_hpp

#include <mutex>

enum class JobStatus {
    IDLE,   
    RUNNING,  
    PAUSED,   
    COMPLETED,  
    STOPPED  
};

struct JobData {
    int requiredQuantity;
    int completedQuantity;
    int passedQuantity;
    int failedQuantity;
    JobStatus jobStatus; 
};

class JobModel {
public:
    std::mutex jobMutex;

    int requiredQuantity = 0;
    int completedQuantity = 0;
    int passedQuantity = 0;
    int failedQuantity = 0;
    JobStatus jobStatus = JobStatus::IDLE;
};

#endif /* JobModel_hpp */
