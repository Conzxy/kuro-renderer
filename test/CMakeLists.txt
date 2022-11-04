#enable_testing()
# 获取test和benchmark的所有源文件
file(GLOB KURO_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test*.cc")
file(GLOB KURO_BENCH_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*bench*.cc")

############################################
# User can determine whether to build all tests when build target all
# e.g. cmake --build */kuro/build [--target all -j 2]
############################################
# 默认将test目标从all目标中排除
set(BUILD_ALL_TESTS OFF CACHE BOOL "Don't exclude tests from target all")
message(STATUS "BUILD_ALL_TESTS = ${BUILD_ALL_TESTS}")

message(STATUS "kuro test source files: ${KURO_TEST_SOURCES}")
message(STATUS "kuro bench source files: ${KURO_BENCH_SOURCES}")

if (NOT ${BUILD_ALL_TESTS})
  message(STATUS "[The kuro test and bench files are excluded from the build of target all]")
endif (NOT ${BUILD_ALL_TESTS})

# 生成测试文件对应的可执行文件
# 
# \param SRC The variable including all test file(*.cc)
# \param ... Libraries
macro (GenTest SRC)
  foreach (kuro_test_source ${${SRC}})
    # 将扩展名去掉(*.cc -> *)
    get_filename_component(kuro_test_filename ${kuro_test_source} NAME_WE)

    if (${BUILD_ALL_TESTS})
      add_executable(${kuro_test_filename} ${kuro_test_source})
    else ()
      message(STATUS "${kuro_test_filename} is excluded from target all")
      add_executable(${kuro_test_filename} EXCLUDE_FROM_ALL ${kuro_test_source})
    endif (${BUILD_ALL_TESTS})

    target_link_libraries(${kuro_test_filename} ${ARGN})
    set_target_properties(${kuro_test_filename}
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
      COMMAND ${kuro_test_filename}
    )

    add_test(NAME ${kuro_test_filename} COMMAND ${kuro_test_filename})
  endforeach (kuro_test_source ${KURO_TEST_SOURCES})
endmacro()

############################################
# generate test and bench program
############################################
# 单元测试依赖gtest
# 基准测试依赖benchmark
# 用户可根据需要替换
# 至于其他依赖项也可以在尾部追加
GenTest(KURO_TEST_SOURCES gtest_main gtest ${KURO_LIB} Qt5::Gui Qt5::Widgets gc)
GenTest(KURO_BENCH_SOURCES benchmark benchmark_main ${KURO_LIB})
