#ifndef RobotControllerTest_hpp
#define RobotControllerTest_hpp

#include "controller/RobotController.hpp"
#include "controller/JobController.hpp"
#include "app/MyApiTestClient.hpp"
#include "app/TestComponent.hpp"
#include "oatpp-test/UnitTest.hpp"

class RobotControllerTest : public oatpp::test::UnitTest {
public:
  RobotControllerTest() : UnitTest("TEST[RobotControllerTest]") {}
  void onRun() override;
};

#endif // RobotControllerTest_hpp
