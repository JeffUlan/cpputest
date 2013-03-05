if (MSVC)
    set(CPP_PLATFORM VisualCpp)
    include_directories(${CppUTestRootDirectory}/include/Platforms/${CPP_PLATFORM})
elseif (STD_C)
    set(CPP_PLATFORM Gcc)
else (MSVC)
    set(STD_CPP False)
    set(MEMORY_LEAK_DETECTION False)
    set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -nostdinc")
    set(CPPUTEST_LD_FLAGS "${CPPUTEST_LD_FLAGS} -nostdinc")
    set(CPPUTEST_STD_C_LIB_DISABLED 1)
    set(CPP_PLATFORM GccNoStdC)
endif (MSVC)

include(CppUTestWarningFlags)

if (NOT STD_CPP)
    set(CPPUTEST_STD_CPP_LIB_DISABLED 1)
    if (STD_C AND NOT MSVC)
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -nostdinc++")
    endif (STD_C AND NOT MSVC)
endif (NOT STD_CPP)

if (MEMORY_LEAK_DETECTION)
    if (MSVC)
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} /FI ${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h")
    else (MSVC)
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -include ${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorNewMacros.h")
        set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} -include ${CppUTestRootDirectory}/include/CppUTest/MemoryLeakDetectorMallocMacros.h")
    endif (MSVC)
else (MEMORY_LEAK_DETECTION)
    set(CPPUTEST_MEM_LEAK_DETECTION_DISABLED 1)
endif (MEMORY_LEAK_DETECTION)

if (MAP_FILE AND NOT MSVC)
    set(CPPUTEST_LD_FLAGS "${CPPUTEST_LD_FLAGS} -Wl,-map,$<.map.txt")
endif (MAP_FILE AND NOT MSVC)

if (COVERAGE AND NOT MSVC)
    set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} --coverage")
    set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} --coverage")
    set(CMAKE_BUILD_TYPE "Debug")
endif (COVERAGE AND NOT MSVC)

if (GMOCK)
    set(GMOCK_HOME $ENV{GMOCK_HOME})
    if (NOT GMOCK_HOME)
        message(FATAL_ERROR "Trying to compile with GMock support, but environment variable $GMOCK_HOME is not set")
    endif (NOT GMOCK_HOME)

    # GMock pulls in gtest.
    set(REAL_GTEST OFF)
    set(CPPUTEST_USE_REAL_GMOCK 1)
    set(CPPUTEST_USE_REAL_GTEST 1)

    include_directories(${GMOCK_HOME}/include ${GMOCK_HOME}/gtest ${GMOCK_HOME}/gtest/include)
    add_subdirectory(${GMOCK_HOME} "${CMAKE_CURRENT_BINARY_DIR}/gmock")
    set(CPPUNIT_EXTERNAL_LIBRARIES ${CPPUNIT_EXTERNAL_LIBARIES} gmock gtest)
else (GMOCK)
    include_directories(${CppUTestRootDirectory}/include/CppUTestExt/CppUTestGMock)
endif (GMOCK)

if (REAL_GTEST AND NOT CPPUTEST_USE_REAL_GTEST)
    set(GTEST_HOME $ENV{GTEST_HOME})
    if (NOT GTEST_HOME)
        message(FATAL_ERROR "Trying to compile with gtest support, but environment variable $GTEST_HOME is not set")
    endif (NOT GTEST_HOME)

    set(CPPUTEST_USE_REAL_GTEST 1)

    include_directories(${GTEST_HOME} ${GTEST_HOME}/include)
    add_subdirectory(${GTEST_HOME} "${CMAKE_CURRENT_BINARY_DIR}/gtest")
    set(CPPUNIT_EXTERNAL_LIBRARIES ${CPPUNIT_EXTERNAL_LIBARIES} gtest)
elseif (NOT CPPUTEST_USE_REAL_GTEST)
    include_directories(${CppUTestRootDirectory}/include/CppUTestExt/CppUTestGTest)
endif (REAL_GTEST AND NOT CPPUTEST_USE_REAL_GTEST)

set(CPPUTEST_C_FLAGS "${CPPUTEST_C_FLAGS} ${CPPUTEST_C_WARNING_FLAGS}")
set(CPPUTEST_CXX_FLAGS "${CPPUTEST_CXX_FLAGS} ${CPPUTEST_CXX_WARNING_FLAGS}")

if (CPPUTEST_FLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPPUTEST_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPPUTEST_CXX_FLAGS}")
    set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} ${CPPUTEST_LD_FLAGS}")
endif (CPPUTEST_FLAGS)

set(CPPUTEST_COMPILATION 1)
