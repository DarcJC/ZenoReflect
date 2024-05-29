
message(CHECK_START "[Reflection] Starting to locate dependencies")

find_package(Clang REQUIRED CONFIG)
find_package(LLVM REQUIRED CONFIG COMPONENTS Headers)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
link_directories(${CLANG_LIBRARY_DIR} ${LLVM_LIBRARY_DIR})

add_definitions(${CLANG_DEFINITIONS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST}) 

set(LIBCLANG_LIBRARY clang clang-cpp)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using libraries: ${LIBCLANG_LIBRARY}")

include_directories(${CMAKE_SOURCE_DIR}/thirdparty/argparse/include)
message(STATUS "Added argparse header into include dir")

find_package(fmt REQUIRED)

message(CHECK_PASS "[Reflection] Found all dependencies successfully")

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../thirdparty/bitsery)
