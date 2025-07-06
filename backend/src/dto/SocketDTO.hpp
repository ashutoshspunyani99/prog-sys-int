#ifndef ScoketDTO_hpp
#define ScoketDTO_hpp

#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class SocketDto : public oatpp::DTO {
  DTO_INIT(SocketDto, DTO);

  DTO_FIELD(Int32, socketId);
  DTO_FIELD(Boolean, isSocketPlaced);
  DTO_FIELD(Boolean, isSocketReadyForPick);
  DTO_FIELD(Boolean, isSocketPicked);
  DTO_FIELD(String, programmingResult);
};

class SocketListDto : public oatpp::DTO {
  DTO_INIT(SocketListDto, DTO)

  DTO_FIELD(Int32, statusCode);
  DTO_FIELD(String, message);
  DTO_FIELD(Vector<Int32>, socketIds);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* ScoketDTO_hpp */