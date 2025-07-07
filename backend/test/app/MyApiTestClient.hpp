
#ifndef MyApiTestClient_hpp
#define MyApiTestClient_hpp

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/macro/codegen.hpp"
#include "dto/JobDTO.hpp" 

/* Begin Api Client code generation */
#include OATPP_CODEGEN_BEGIN(ApiClient)

/**
 * Test API client.
 * Use this client to call application APIs.
 */
class MyApiTestClient : public oatpp::web::client::ApiClient {

  API_CLIENT_INIT(MyApiTestClient)
  API_CALL("GET", "/", getRoot)
  API_CALL("POST", "api/job/start", startJob,
           BODY_DTO(Object<JobStartDto>, body))
  API_CALL("POST", "api/job/pause", pauseJob)
  API_CALL("POST", "api/job/resume", resumeJob)
  API_CALL("POST", "api/job/stop", stopJob)
  API_CALL("GET", "api/job/status", getJobStatus)

  // Robot API
  API_CALL("GET", "api/robot/ready-to-place", readyToPlace)
  API_CALL("POST", "api/robot/device-placed", devicePlaced,
           QUERY(Int32, socketId))
  API_CALL("GET", "api/robot/ready-to-pick", readyToPick)
  API_CALL("POST", "api/robot/device-picked", devicePicked,
           QUERY(Int32, socketId))

  API_CALL("GET", "api/programming/sockets", getProgrammingSocketsBySiteId,
           QUERY(Int32, siteId))
};

/* End Api Client code generation */
#include OATPP_CODEGEN_END(ApiClient)

#endif // MyApiTestClient_hpp