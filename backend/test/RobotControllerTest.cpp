#include "RobotControllerTest.hpp"

#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void RobotControllerTest::onRun() {
  TestComponent component;
  oatpp::test::web::ClientServerTestRunner runner;
  runner.addController(std::make_shared<JobController>());
  runner.addController(std::make_shared<RobotController>());

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

        OATPP_LOGd("TEST", "Running: readyToPlace");
        auto readyToPlaceResponse = client->readyToPlace();
        OATPP_ASSERT(readyToPlaceResponse);
        OATPP_ASSERT(readyToPlaceResponse->getStatusCode() == 400);
        auto readyToPlaceBody = readyToPlaceResponse->readBodyToDto<
            oatpp::Object<ResponseDataDto<oatpp::Vector<oatpp::Int32>>>>(
            contentMappers->selectMapperForContent(
                readyToPlaceResponse->getHeader("Content-Type")));
        OATPP_ASSERT(readyToPlaceBody);

        OATPP_LOGd("TEST", "Running: readyToPick");
        auto readyToPickResponse = client->readyToPick();
        OATPP_ASSERT(readyToPickResponse->getStatusCode() == 400);
        auto readyToPickBody = readyToPickResponse->readBodyToDto<
            oatpp::Object<ResponseDataDto<oatpp::Vector<oatpp::Int32>>>>(
            contentMappers->selectMapperForContent(
                readyToPickResponse->getHeader("Content-Type")));
        OATPP_ASSERT(readyToPickBody);

        OATPP_LOGd("TEST", "Running: devicePlaced");
        auto placeResponse = client->devicePlaced(0);
        OATPP_ASSERT(placeResponse->getStatusCode() == 400);
        auto placeBody =
            placeResponse
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        placeResponse->getHeader("Content-Type")));
        OATPP_ASSERT(placeBody);

        OATPP_LOGd("TEST", "Running: devicePlaced with invalid socketId");
        auto invalidPlaceResponse = client->devicePlaced(-1);
        OATPP_ASSERT(invalidPlaceResponse->getStatusCode() == 400);
        auto invalidPlaceBody =
            invalidPlaceResponse
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        invalidPlaceResponse->getHeader("Content-Type")));
        OATPP_ASSERT(invalidPlaceBody);
        OATPP_ASSERT(invalidPlaceBody->message ==
                     "No job is currently running");

        OATPP_LOGd("TEST", "Running: devicePicked");
        auto pickResponse = client->devicePicked(0);
        OATPP_ASSERT(pickResponse->getStatusCode() == 400);
        auto pickBody =
            pickResponse
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        pickResponse->getHeader("Content-Type")));
        OATPP_ASSERT(pickBody);
        OATPP_ASSERT(pickBody->message == "No job is currently running");

        OATPP_LOGd("TEST", "Running: devicePicked with invalid socketId");
        auto invalidPickResponse = client->devicePicked(-1);
        OATPP_ASSERT(invalidPickResponse->getStatusCode() == 400);
        auto invalidPickBody =
            invalidPickResponse
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        invalidPickResponse->getHeader("Content-Type")));
        OATPP_ASSERT(invalidPickBody);
        OATPP_ASSERT(invalidPickBody->message == "No job is currently running");

        OATPP_LOGd("TEST", "Running: startJob");

        auto startDto = JobStartDto::createShared();
        startDto->requiredQuantity = 4;
        auto startResponse = client->startJob(startDto);
        std::cout << "Start Response: " << startResponse->getStatusCode() << std::endl;
        OATPP_ASSERT(startResponse);
        OATPP_ASSERT(startResponse->getStatusCode() == 200);
        auto startResponseBody =
            startResponse
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        startResponse->getHeader("Content-Type")));
        OATPP_ASSERT(startResponseBody);
        OATPP_ASSERT(startResponseBody->message == "Job started successfully");

        OATPP_LOGd("TEST", "Running: getJobStatus");
        auto statusResponse = client->getJobStatus();
        std::cout << "Status Response: " << statusResponse->getStatusCode() << std::endl;

        OATPP_ASSERT(statusResponse->getStatusCode() == 200);
        auto statusResponseBody = statusResponse->readBodyToDto<
            oatpp::Object<ResponseDataDto<oatpp::Object<JobStatusDto>>>>(
            contentMappers->selectMapperForContent(
                statusResponse->getHeader("Content-Type")));
        OATPP_ASSERT(statusResponseBody);
        OATPP_ASSERT(statusResponseBody->data);
        OATPP_ASSERT(statusResponseBody->data->jobStatus == "RUNNING");
        OATPP_ASSERT(statusResponseBody->data->requiredQuantity == 4);
        OATPP_ASSERT(statusResponseBody->data->completedQuantity == 0);
        OATPP_ASSERT(statusResponseBody->data->passedQuantity == 0);
        OATPP_ASSERT(statusResponseBody->data->failedQuantity == 0);  
        

        OATPP_LOGd("TEST", "Running: readyToPlace (with job)");
        auto readyToPlaceResponse2 = client->readyToPlace();
        OATPP_ASSERT(readyToPlaceResponse2->getStatusCode() == 200);
        auto readyToPlaceBody2 = readyToPlaceResponse2->readBodyToDto<
            oatpp::Object<ResponseDataDto<oatpp::Vector<oatpp::Int32>>>>(
            contentMappers->selectMapperForContent(
                readyToPlaceResponse2->getHeader("Content-Type")));
        OATPP_ASSERT(readyToPlaceBody2);
        OATPP_ASSERT(readyToPlaceBody2->data->size() > 0);
        auto socketId = readyToPlaceBody2->data[0];

        OATPP_LOGd("TEST", "Running: devicePlaced (valid)");
        auto placeResponse2 = client->devicePlaced(socketId);
        OATPP_ASSERT(placeResponse2->getStatusCode() == 200);
        auto placeBody2 =
            placeResponse2
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        placeResponse2->getHeader("Content-Type")));
        OATPP_ASSERT(placeBody2);
        OATPP_ASSERT(placeBody2->data == "OK");

       
        for (v_int32 i = 1; i < readyToPlaceBody2->data->size(); i++) {
          client->devicePlaced(readyToPlaceBody2->data[i]);
        }

        OATPP_LOGd("TEST", "Waiting for site programming to complete...");
        std::this_thread::sleep_for(
            std::chrono::seconds(25)); 

        OATPP_LOGd("TEST", "Running: readyToPick (after programming)");
        auto readyToPickResponse2 = client->readyToPick();
        OATPP_ASSERT(readyToPickResponse2->getStatusCode() == 200);
        auto readyToPickBody2 = readyToPickResponse2->readBodyToDto<
            oatpp::Object<ResponseDataDto<oatpp::Vector<oatpp::Int32>>>>(
            contentMappers->selectMapperForContent(
                readyToPickResponse2->getHeader("Content-Type")));
        OATPP_ASSERT(readyToPickBody2);
        OATPP_ASSERT(readyToPickBody2->data->size() > 0);
        auto pickSocketId = readyToPickBody2->data[0];

        OATPP_LOGd("TEST", "Running: devicePicked (valid)");
        auto pickResponse2 = client->devicePicked(pickSocketId);
        OATPP_ASSERT(pickResponse2->getStatusCode() == 200);
        auto pickBody2 =
            pickResponse2
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        pickResponse2->getHeader("Content-Type")));
        OATPP_ASSERT(pickBody2);
        OATPP_ASSERT(pickBody2->data == "OK" || pickBody2->data == "FAILED");

        OATPP_LOGd("TEST", "Running: stopJob");
        auto stopResponse = client->stopJob();
        OATPP_ASSERT(stopResponse->getStatusCode() == 200);

      },
      std::chrono::minutes(2));
}
