#ifndef JobDTOMapper_hpp
#define JobDTOMapper_hpp

#include "dto/JobDTO.hpp"
#include "models/JobModel.hpp"

class JobDTOMapper {
public:
  static oatpp::Object<JobStatusDto> toDto(const JobData& jobData);
};

#endif // JobDTOMapper_hpp
