#ifndef ProgrammingControllerTest_hpp
#define ProgrammingControllerTest_hpp

#include "oatpp-test/UnitTest.hpp"
#include "dto/JobDTO.hpp"
#include "app/MyApiTestClient.hpp"
#include "app/TestComponent.hpp"
#include "controller/ProgrammingController.hpp"

class ProgrammingControllerTest : public oatpp::test::UnitTest {
public:
  ProgrammingControllerTest() : UnitTest("TEST[ProgrammingControllerTest]") {}
  void onRun() override;
};

#endif // ProgrammingControllerTest_hpp
