#ifndef JobControllerTest_hpp
#define JobControllerTest_hpp

#include "oatpp-test/UnitTest.hpp"
#include "dto/JobDTO.hpp"
#include "app/MyApiTestClient.hpp"
#include "app/TestComponent.hpp"
#include "controller/JobController.hpp"

class JobControllerTest : public oatpp::test::UnitTest {
public:
  JobControllerTest() : UnitTest("TEST[JobControllerTest]") {}
  void onRun() override;
};

#endif // JobControllerTest_hpp
