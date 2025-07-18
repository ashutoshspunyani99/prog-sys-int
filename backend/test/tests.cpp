
#include "MyControllerTest.hpp"
#include "JobControllerTest.hpp"
#include "ProgrammingControllerTest.hpp"
#include "RobotControllerTest.hpp"


#include <iostream>

    void
    runTests() {
  OATPP_RUN_TEST(MyControllerTest);
  OATPP_RUN_TEST(JobControllerTest);
  OATPP_RUN_TEST(ProgrammingControllerTest);
  OATPP_RUN_TEST(RobotControllerTest);
}

int main() {

  oatpp::Environment::init();

  runTests();

  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::Environment::getObjectsCreated() << "\n\n";

  OATPP_ASSERT(oatpp::Environment::getObjectsCount() == 0);

  oatpp::Environment::destroy();

  return 0;
}