get_filename_component(TESTS_PATH "." ABSOLUTE)

file(GLOB_RECURSE TEST_CXX_SOURCES  ${TESTS_PATH}/*.cpp)

add_executable(fiber-tests ${TEST_CXX_SOURCES})
target_link_libraries(fiber-tests simple-fibers)