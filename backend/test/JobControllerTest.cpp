#include "JobControllerTest.hpp"

#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

void JobControllerTest::onRun() {
  TestComponent component;
  oatpp::test::web::ClientServerTestRunner runner;

  runner.addController(std::make_shared<JobController>());

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
                   "Running: pauseJob when no job is running (expect 400)");
        auto pauseBeforeStartResponse = client->pauseJob();
        OATPP_ASSERT(pauseBeforeStartResponse->getStatusCode() == 400);

        auto pauseErrorBody =
            pauseBeforeStartResponse
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        pauseBeforeStartResponse->getHeader("Content-Type")));

        OATPP_ASSERT(pauseErrorBody);
        OATPP_ASSERT(pauseErrorBody->message == "No job is currently running");

        OATPP_LOGd("TEST",
                   "Running: stopJob when no job is running (expect 400)");
        auto stopBeforeStartResponse = client->stopJob();
        OATPP_ASSERT(stopBeforeStartResponse->getStatusCode() == 400);

        auto stopErrorBody =
            stopBeforeStartResponse
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        stopBeforeStartResponse->getHeader("Content-Type")));

        OATPP_ASSERT(stopErrorBody);
        OATPP_ASSERT(stopErrorBody->message ==
                     "No job is currently running or paused")

        OATPP_LOGd("TEST",
                   "Running: resumeJob when no job is running (expect 400)");
        auto resumeBeforeStartResponse = client->resumeJob();
        OATPP_ASSERT(resumeBeforeStartResponse->getStatusCode() == 400);

        auto resumeErrorBody =
            resumeBeforeStartResponse
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        resumeBeforeStartResponse->getHeader("Content-Type")));

        OATPP_ASSERT(resumeErrorBody);
        OATPP_ASSERT(resumeErrorBody->message == "No job is currently paused")

        OATPP_LOGd("TEST", "Running: startJob");
        OATPP_LOGd("TEST", "Running: startJob with valid quantity");
        auto validDto = JobStartDto::createShared();
        validDto->requiredQuantity = 3;
        auto startResponse = client->startJob(validDto);
        OATPP_ASSERT(startResponse->getStatusCode() == 200);
        auto startResponseBody =
            startResponse
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        startResponse->getHeader("Content-Type")));
        OATPP_ASSERT(startResponseBody);
        OATPP_ASSERT(startResponseBody->message == "Job started successfully");

        OATPP_LOGd("TEST", "Running: startJob when thete is already a job "
                           "running (expect 400)");
        auto secondValidDto = JobStartDto::createShared();
        secondValidDto->requiredQuantity = 5;
        auto secondStartResponse = client->startJob(secondValidDto);
        OATPP_ASSERT(secondStartResponse->getStatusCode() == 400);
        auto invalidSecondStartResponseBody =
            secondStartResponse
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        secondStartResponse->getHeader("Content-Type")));
        OATPP_ASSERT(invalidSecondStartResponseBody);
        OATPP_ASSERT(invalidSecondStartResponseBody->message ==
                     "Job is already running or paused");

        OATPP_LOGd("TEST", "Running: startJob with invalid quantity");
        auto invalidDto = JobStartDto::createShared();
        invalidDto->requiredQuantity = -5;
        auto invalidStartResponse = client->startJob(invalidDto);

        OATPP_LOGd("TEST", "Running: pauseJob");
        auto pauseResponse = client->pauseJob();
        OATPP_ASSERT(pauseResponse->getStatusCode() == 200);
        auto pauseResponseBody =
            pauseResponse
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        pauseResponse->getHeader("Content-Type")));
        OATPP_ASSERT(pauseResponseBody);
        OATPP_ASSERT(pauseResponseBody->message == "Job paused successfully");

        OATPP_LOGd("TEST", "Running: resumeJob");
        auto resumeResponse = client->resumeJob();
        OATPP_ASSERT(resumeResponse->getStatusCode() == 200);
        auto resumeResponseBody =
            resumeResponse
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        resumeResponse->getHeader("Content-Type")));
        OATPP_ASSERT(resumeResponseBody);
        OATPP_ASSERT(resumeResponseBody->message == "Job resumed successfully");

        OATPP_LOGd("TEST", "Running: stopJob");
        auto stopResponse = client->stopJob();
        OATPP_ASSERT(stopResponse->getStatusCode() == 200);
        auto stopResponseBody =
            stopResponse
                ->readBodyToDto<oatpp::Object<ResponseDataDto<oatpp::String>>>(
                    contentMappers->selectMapperForContent(
                        stopResponse->getHeader("Content-Type")));
        OATPP_ASSERT(stopResponseBody);
        OATPP_ASSERT(stopResponseBody->message == "Job stopped successfully");

        OATPP_LOGd("TEST", "Running: getJobStatus");
        auto statusResponse = client->getJobStatus();
        OATPP_ASSERT(statusResponse->getStatusCode() == 200);
        auto statusResponseBody = statusResponse->readBodyToDto<
            oatpp::Object<ResponseDataDto<oatpp::Object<JobStatusDto>>>>(
            contentMappers->selectMapperForContent(
                statusResponse->getHeader("Content-Type")));
        OATPP_ASSERT(statusResponseBody);
        OATPP_ASSERT(statusResponseBody->data->jobStatus == "STOPPED");
        OATPP_ASSERT(statusResponseBody->data->requiredQuantity == 0);
        OATPP_ASSERT(statusResponseBody->data->completedQuantity == 0);
        OATPP_ASSERT(statusResponseBody->data->passedQuantity == 0);
        OATPP_ASSERT(statusResponseBody->data->failedQuantity == 0);
      },
      std::chrono::minutes(2));
}
