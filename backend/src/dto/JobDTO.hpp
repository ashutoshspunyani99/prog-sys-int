#ifndef JobDTO_hpp
#define JobDTO_hpp

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class JobStartDto : public oatpp::DTO {
  
  DTO_INIT(JobStartDto, DTO)
  DTO_FIELD(Int32, requiredQuantity); 
};

#include OATPP_CODEGEN_END(DTO)

#endif /* JobDTO_hpp */