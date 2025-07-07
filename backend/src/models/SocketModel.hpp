#ifndef SocketModel_hpp
#define SocketModel_hpp

#include <mutex>

/// @brief Represents the result of a socket's programming operation.
///
/// - NONE: No result yet (default state).
/// - PASSED: Programming was successful.
/// - FAILED: Programming failed.
enum class SocketProgrammingResult {
    NONE,   
    PASSED,  
    FAILED
};

/// @brief Runtime status data for a socket.
///
/// Contains basic state flags and programming result.
struct SocketStatusData {
    int socketId;
    bool isSocketPlaced;
    bool isSocketReadyForPick;
    bool isSocketPicked;
    SocketProgrammingResult programmingResult;
};

/// @brief Thread-safe model representing a socket in the system.
///
/// Contains mutable state data, protected by a mutex.
class SocketModel {
public:
    int socketId;
    std::mutex socketMutex;

    bool isSocketPlaced = false;
    bool isSocketReadyForPick = false;
    bool isSocketPicked = false;
    SocketProgrammingResult programmingResult = SocketProgrammingResult::NONE;

};

#endif /* SocketModel_hpp */
