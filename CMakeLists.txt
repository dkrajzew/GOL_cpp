include_directories("./")
set(examplelibs optionslib)
set(testerlibs optionslib)

add_subdirectory(utils)

add_executable(tester tester.cpp)
set_target_properties(tester PROPERTIES OUTPUT_NAME tester${BINARY_SUFFIX})
set_target_properties(tester PROPERTIES OUTPUT_NAME_DEBUG tester${BINARY_SUFFIX}D)
target_link_libraries(tester ${testerlibs})

add_executable(example example.cpp)
set_target_properties(example PROPERTIES OUTPUT_NAME example${BINARY_SUFFIX})
set_target_properties(example PROPERTIES OUTPUT_NAME_DEBUG example${BINARY_SUFFIX}D)
target_link_libraries(example ${examplelibs})

configure_file(config.h.cmake config.h)

