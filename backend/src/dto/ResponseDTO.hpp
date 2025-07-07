#ifndef ResponseDTO_hpp
#define ResponseDTO_hpp

#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class ResponseDto : public oatpp::DTO {

  DTO_INIT(ResponseDto, DTO)

  DTO_FIELD(Int32, statusCode);
  DTO_FIELD(String, message);
};

template <typename T>
class ResponseDataDto : public oatpp::DTO {
  DTO_INIT(ResponseDataDto<T>, DTO)

  DTO_FIELD(Int32, statusCode);
  DTO_FIELD(String, message);
  DTO_FIELD(T, data); 
};

#include OATPP_CODEGEN_END(DTO)

#endif /* ResponseDTO_hpp */