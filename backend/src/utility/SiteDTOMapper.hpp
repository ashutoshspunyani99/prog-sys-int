#ifndef SiteDTOMapper_hpp
#define SiteDTOMapper_hpp

#include "models/SiteModel.hpp"
#include "dto/SiteDTO.hpp"

/// @brief Mapper class for converting SiteStatusData to SiteDto.
class SiteDtoMapper {
public:
  static oatpp::Object<SiteDto> toDto(const SiteStatusData& site);
};

#endif /* SiteDTOMapper_hpp */