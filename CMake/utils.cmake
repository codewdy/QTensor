macro (new_library library_name source_dir)
  file(GLOB_RECURSE src_list RELATIVE ${PROJECT_SOURCE_DIR} ${source_dir}/*.cc)
  add_library(${library_name} OBJECT ${src_list})
  set(OBJECTS ${OBJECTS} $<TARGET_OBJECTS:${library_name}>)
endmacro ()

macro (new_test test_name source_dir)
  file(GLOB_RECURSE src_list RELATIVE ${PROJECT_SOURCE_DIR} ${source_dir}/*.cc)
  add_executable(${test_name} ${src_list} ${OBJECTS})
  target_link_libraries(${test_name} ${LIBRARY} ${TEST_LIBRARY})
  add_test(NAME ${test_name} COMMAND ${test_name})
endmacro ()

macro (new_executable executable_name source_dir)
  file(GLOB_RECURSE src_list RELATIVE ${PROJECT_SOURCE_DIR} ${source_dir}/*.cc)
  add_executable(${test_name} ${src_list} ${OBJECTS})
  target_link_libraries(${executable_name} ${LIBRARY} ${TEST_LIBRARY})
endmacro ()

