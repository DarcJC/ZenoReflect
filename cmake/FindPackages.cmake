
message(CHECK_START "[Reflection] Starting to locate dependencies")

if (MSVC)
    find_package(LLVM REQUIRED CONFIG)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
    message(STATUS "Using libclang ${LIBCLANG_LIBRARY}")

    include_directories(${LLVM_INCLUDE_DIRS})
    link_directories(${LLVM_LIBRARY_DIR})
    add_definitions(${LLVM_DEFINITIONS})
else()
    find_package(Clang REQUIRED CONFIG)
    include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
    link_directories(${CLANG_LIBRARY_DIR})
    add_definitions(${LLVM_DEFINITIONS})
    add_definitions(${CLANG_DEFINITIONS})
    set(LIBCLANG_LIBRARY libclang)
endif(MSVC)

include_directories(${CMAKE_SOURCE_DIR}/thirdparty/argparse/include)
message(STATUS "Added argparse header into include dir")

find_package(fmt REQUIRED)

message(CHECK_PASS "[Reflection] Found all dependencies successfully")
