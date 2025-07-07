#ifndef JobModel_hpp
#define JobModel_hpp

#include <mutex>

/// @brief Enum representing possible job statuses.
/// Defines the current state of a job in the system.
/// - IDLE: No job is currently active. This is the default state.
/// - RUNNING: A job is actively in progress.
/// - PAUSED: A job was temporarily halted and can be resumed.
/// - COMPLETED: The job has finished processing all required items.
/// - STOPPED: The job was manually stopped before completion.
enum class JobStatus {
    IDLE,   
    RUNNING,  
    PAUSED,   
    COMPLETED,  
    STOPPED  
};

/// @brief Struct representing job runtime data.
struct JobData {
    int requiredQuantity;
    int completedQuantity;
    int passedQuantity;
    int failedQuantity;
    JobStatus jobStatus; 
};

/// @brief Thread-safe job model used by the service layer.
///
/// Contains mutable job state data, protected by a mutex.
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
