cmake_minimum_required(VERSION 3.20)

set(project_name prog_sys_int) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 17)

add_library(${project_name}-lib
        src/AppComponent.hpp
        src/adapters/JobAdapter.hpp
        src/adapters/JobAdapter.cpp
        src/adapters/SiteAdapter.hpp
        src/adapters/SiteAdapter.cpp
        src/controller/MyController.cpp
        src/controller/MyController.hpp
        src/controller/JobController.hpp
        src/controller/ProgrammingController.hpp
        src/controller/RobotController.hpp
        src/dto/DTOs.hpp
        src/dto/JobDTO.hpp
        src/dto/ResponseDTO.hpp
        src/dto/SiteDTO.hpp
        src/dto/SocketDTO.hpp
        src/models/JobModel.hpp
        src/models/SiteModel.hpp
        src/models/SocketModel.hpp
        src/services/JobService.hpp
        src/services/JobService.cpp
        src/services/SiteService.hpp
        src/services/SiteService.cpp
        src/utility/SiteDTOMapper.hpp
        src/utility/SiteDTOMapper.cpp
        src/utility/OatppUtils.hpp
        src/utility/OatppUtils.cpp
        src/utility/JobDTOMapper.cpp
        src/utility/JobDTOMapper.hpp

)

## link libs

find_package(oatpp 1.4.0 REQUIRED)

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
)

target_include_directories(${project_name}-lib PUBLIC src)

## add executables

add_executable(${project_name}-exe
        src/App.cpp
        test/app/MyApiTestClient.hpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)

add_executable(${project_name}-test
        test/tests.cpp
        test/app/TestComponent.hpp
        test/app/MyApiTestClient.hpp
        test/MyControllerTest.cpp
        test/MyControllerTest.hpp
        test/JobControllerTest.cpp
        test/JobControllerTest.hpp
        test/ProgrammingControllerTest.cpp
        test/ProgrammingControllerTest.hpp
        test/RobotControllerTest.cpp
        test/RobotControllerTest.hpp
)

target_link_libraries(${project_name}-test ${project_name}-lib)
add_dependencies(${project_name}-test ${project_name}-lib)

set_target_properties(${project_name}-lib ${project_name}-exe ${project_name}-test PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

enable_testing()
add_test(project-tests ${project_name}-test)