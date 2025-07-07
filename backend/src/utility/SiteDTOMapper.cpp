#include "SiteDTOMapper.hpp"
/// @brief converting SiteStatusData to SiteDto.
oatpp::Object<SiteDto> SiteDtoMapper::toDto(const SiteStatusData& site) {
  auto siteDto = SiteDto::createShared();
  siteDto->siteId = site.siteId;

  switch (site.siteStatus) {
    case SiteStatus::IDLE:      siteDto->siteStatus = "IDLE"; break;
    case SiteStatus::ACTIVE:    siteDto->siteStatus = "ACTIVE"; break;
    case SiteStatus::COMPLETED: siteDto->siteStatus = "COMPLETED"; break;
  }

  auto socketDtoList = oatpp::Vector<oatpp::Object<SocketDto>>::createShared();
  for (auto& socket : site.sockets) {
    auto socketDto = SocketDto::createShared();
    socketDto->socketId = socket.socketId;
    socketDto->isSocketPlaced = socket.isSocketPlaced;
    socketDto->isSocketReadyForPick = socket.isSocketReadyForPick;
    socketDto->isSocketPicked = socket.isSocketPicked;

    switch (socket.programmingResult) {
      case SocketProgrammingResult::NONE:
        socketDto->programmingResult = "NONE"; break;
      case SocketProgrammingResult::PASSED:
        socketDto->programmingResult = "PASSED"; break;
      case SocketProgrammingResult::FAILED:
        socketDto->programmingResult = "FAILED"; break;
    }

    socketDtoList->push_back(socketDto);
  }

  siteDto->sockets = socketDtoList;
  return siteDto;
}