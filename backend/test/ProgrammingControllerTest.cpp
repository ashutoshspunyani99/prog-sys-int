#include "ProgrammingControllerTest.hpp"

#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void ProgrammingControllerTest::onRun() {
  TestComponent component;
  oatpp::test::web::ClientServerTestRunner runner;

  runner.addController(std::make_shared<ProgrammingController>());

  runner.run(
      [this, &runner] {
        OATPP_COMPONENT(
            std::shared_ptr<oatpp::network::ClientConnectionProvider>,
            clientConnectionProvider);
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>,
                        contentMappers);

        auto client = MyApiTestClient::createShared(
            oatpp::web::client::HttpRequestExecutor::createShared(
                clientConnectionProvider),
            contentMappers->getMapper("application/json"));

        OATPP_LOGd("TEST",
                   "Running: getProgrammingSocketsBySiteId with valid siteId");

        auto validResponse = client->getProgrammingSocketsBySiteId(1);
        OATPP_ASSERT(validResponse);
        OATPP_ASSERT(validResponse->getStatusCode() == 200);
        auto bodyDto = validResponse->readBodyToDto<oatpp::Object<
            ResponseDataDto<oatpp::Vector<oatpp::Object<SiteDto>>>>>(
            contentMappers->selectMapperForContent(
                validResponse->getHeader("Content-Type")));
        OATPP_ASSERT(bodyDto);
        OATPP_ASSERT(bodyDto->data);
        OATPP_ASSERT(bodyDto->data->size() > 0);
        OATPP_ASSERT(bodyDto->data->size() == 1);
        for (const auto &site : *bodyDto->data) {
          OATPP_ASSERT(site->siteId == 1);
          OATPP_ASSERT(site->siteStatus == "IDLE");
          OATPP_ASSERT(site->sockets->size() > 0);
          for (const auto &socket : *site->sockets) {
            OATPP_ASSERT(socket->socketId > 0);
            OATPP_ASSERT(socket->isSocketPlaced == false);
            OATPP_ASSERT(socket->isSocketReadyForPick == false);
            OATPP_ASSERT(socket->isSocketPicked == false);
            OATPP_ASSERT(socket->programmingResult == "NONE");
          }
          OATPP_ASSERT(site->sockets->size() == 4);
        }
        OATPP_ASSERT(bodyDto->message ==
                     "Sockets status retrieved successfully");

        OATPP_LOGd(
            "TEST",
            "Running: getProgrammingSocketsBySiteId with invalid siteId");

        auto invalidResponse = client->getProgrammingSocketsBySiteId(-1);

        OATPP_ASSERT(invalidResponse);
        OATPP_ASSERT(invalidResponse->getStatusCode() == 204);
        auto invalidBodyDto = invalidResponse->readBodyToDto<oatpp::Object<
            ResponseDataDto<oatpp::Vector<oatpp::Object<SiteDto>>>>>(
            contentMappers->selectMapperForContent(
                invalidResponse->getHeader("Content-Type")));
        OATPP_ASSERT(invalidBodyDto);
        OATPP_ASSERT(invalidBodyDto->message ==
                     "No site found with the given ID");
      },
      std::chrono::minutes(2));
}
