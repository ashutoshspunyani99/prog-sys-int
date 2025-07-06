#ifndef JobAdapter_hpp
#define JobAdapter_hpp

#include "dto/JobDTO.hpp"
#include "dto/ResponseDTO.hpp"
#include "models/SiteModel.hpp"
#include "services/JobService.hpp"
#include "utility/JobDTOMapper.hpp"
#include "utility/OatppUtils.hpp"

#include <iostream>
#include <memory>

class JobAdapter {
private:
  std::shared_ptr<JobService> jobService;

public:
  JobAdapter(std::shared_ptr<JobService> jobService);

  ResponseWrapper<oatpp::String> startJobResponse(int requiredQuantity);
  ResponseWrapper<oatpp::String> stopJobResponse();
  ResponseWrapper<oatpp::String> pauseJobResponse();
  ResponseWrapper<oatpp::String> resumeJobResponse();
  ResponseWrapper<oatpp::Object<JobStatusDto>> getJobStatusResponse();
};

#endif /* JobAdapter_hpp */