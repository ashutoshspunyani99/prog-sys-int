#ifndef JobDTOMapper_hpp
#define JobDTOMapper_hpp

#include "dto/JobDTO.hpp"
#include "models/JobModel.hpp"

/// @brief Mapper class for converting JobData to JobStatusDto.
class JobDTOMapper {
public:
  static oatpp::Object<JobStatusDto> toDto(const JobData& jobData);
};

#endif // JobDTOMapper_hpp
