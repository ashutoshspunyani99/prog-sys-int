#ifndef SiteDTO_hpp
#define SiteDTO_hpp

#include "dto/SocketDTO.hpp"
#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class SiteDto : public oatpp::DTO {
  DTO_INIT(SiteDto, DTO);

  DTO_FIELD(Int32, siteId);
  DTO_FIELD(String, siteStatus);
  DTO_FIELD(Vector<oatpp::Object<SocketDto>>, sockets);
};

class ProgrammingSitesResponseDto : public oatpp::DTO {

  DTO_INIT(ProgrammingSitesResponseDto, DTO);
  DTO_FIELD(Int32, statusCode);
  DTO_FIELD(String, message);
  DTO_FIELD(Vector<oatpp::Object<SiteDto>>, data);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* SiteDTO_hpp */