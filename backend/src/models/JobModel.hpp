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

class JobModel {
public:
    std::mutex jobMutex;

    int requiredQuantity = 0;
    int completedQuantity = 0;
    JobStatus status = JobStatus::IDLE;
};

#endif /* JobModel_hpp */
