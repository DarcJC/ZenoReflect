option(REFLECTION_USE_PREBUILT_BINARY "" OFF)

set(RELFECTION_GENERATION_ROOT_TARGET _Reflection_ROOT CACHE INTERNAL "Reflection generator dependencies for all targets")
if(NOT TARGET _Reflection_ROOT)
    add_custom_target(${RELFECTION_GENERATION_ROOT_TARGET})
endif()

macro(make_absolute_paths out_var)
    set(result_list)
    foreach(path ${ARGN})
        get_filename_component(abs_path "${path}" ABSOLUTE)
        list(APPEND result_list "${abs_path}")
    endforeach()
    set(${out_var} "${result_list}")
endmacro()

function (zeno_get_target_property target property result)
    # IDK why with get_target_property only might lead to NOT_FOUND result
    get_property(was_set TARGET ${target} PROPERTY ${property} SET)
    if(was_set)
        get_target_property(value ${target} ${property})
        set(${result} ${value} PARENT_SCOPE)
    else()
        set(${result} "")
    endif()
endfunction(zeno_get_target_property)

function (get_target_include_dirs_recursive target result)
    # Get include dirs from a target recursive
    # Only PUBLIC libraries will be traversed
    get_target_property(target_include_dirs ${target} INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)
    set(${result} ${target_include_dirs})
    set(${target}_get_target_include_dirs_recursive_is_traversed "yes")

    zeno_get_target_property(${target} INTERFACE_LINK_LIBRARIES target_deps)

    foreach(dep ${target_deps})
        if (TARGET ${dep} AND NOT DEFINED ${dep}_get_target_include_dirs_recursive_is_traversed)
            get_target_include_dirs_recursive(${dep} dep_include_dirs)
            list(APPEND ${result} ${dep_include_dirs})
        endif()
    endforeach()
    
    list(REMOVE_DUPLICATES ${result})
    set(${result} ${${result}} PARENT_SCOPE)
endfunction(get_target_include_dirs_recursive)

function(get_prebuilt_generator generator_path)
    set(GENERATOR_PATH "${CMAKE_BINARY_DIR}/ReflectGenerator-Prebuilt.exe")

    if (NOT EXISTS ${GENERATOR_PATH})
        file(DOWNLOAD "https://zenustech.oss-cn-beijing.aliyuncs.com/zenobuild/ReflectGenerator.exe" ${GENERATOR_PATH}
                STATUS download_status
                SHOW_PROGRESS
        )
    endif ()
    if (NOT EXISTS "${CMAKE_BINARY_DIR}/zlib1.dll")
        file(DOWNLOAD "https://zenustech.oss-cn-beijing.aliyuncs.com/zenobuild/zlib1.dll" "${CMAKE_BINARY_DIR}/zlib1.dll"
                STATUS download_status
                SHOW_PROGRESS
        )
    endif ()
    if (NOT EXISTS "${CMAKE_BINARY_DIR}/zstd.dll")
        file(DOWNLOAD "https://zenustech.oss-cn-beijing.aliyuncs.com/zenobuild/zstd.dll" "${CMAKE_BINARY_DIR}/zstd.dll"
                STATUS download_status
                SHOW_PROGRESS
        )
    endif ()
    set(generator_path ${GENERATOR_PATH})
    RETURN(PROPAGATE generator_path)
endfunction()

function(zeno_declare_reflection_support target reflection_headers)
    set(generator_path)
    if (REFLECTION_USE_PREBUILT_BINARY AND WIN32)
        get_prebuilt_generator(generator_path)
    endif()

    set(INTERMEDIATE_FILE_BASE_DIR "${CMAKE_BINARY_DIR}/intermediate")

    # Call this function after all target source has been added
    set(splitor ",")

    set(INTERMEDIATE_FILE_DIR "${INTERMEDIATE_FILE_BASE_DIR}/${target}")
    set(INTERMEDIATE_ALL_IN_ONE_FILE "${INTERMEDIATE_FILE_DIR}/${target}.generated.cpp")
    file(WRITE "${INTERMEDIATE_ALL_IN_ONE_FILE}" "// TBD by reflection generator\n")
    target_sources(${target} PRIVATE "${INTERMEDIATE_ALL_IN_ONE_FILE}")

    # Input sources
    get_target_property(REFLECTION_GENERATION_SOURCE ${target} SOURCES)
    get_target_property(REFLECTION_GENERATION_SOURCE_DIR ${target} SOURCE_DIR)
    list(LENGTH REFLECTION_GENERATION_SOURCE source_files_length)
    if (source_files_length EQUAL 0)
        message(WARNING "There is not source files found in target ${target}, check your calling timing")
    endif()
    set(source_paths_value ${REFLECTION_GENERATION_SOURCE})
    list(JOIN reflection_headers ${splitor} source_paths_string)

    # Include dirs
    set(INCLUDE_DIRS $<LIST:REMOVE_DUPLICATES,$<TARGET_PROPERTY:${target},INCLUDE_DIRECTORIES>>)
    # Obtain compiler built-in include paths
    list(JOIN CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "," SYSTEM_IMPLICIT_INCLUDE_DIRS)

    set(REFLECTION_GENERATION_TARGET _internal_${target}_reflect_generation)
    set(TIMESTAMP_FILE "${CMAKE_BINARY_DIR}/timestamp/${target}_timestamp")
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/timestamp")

    if (${GENERATE_ON_BUILD_DIR})
        set(REFLECTION_GENERATED_DIR ${CMAKE_BINARY_DIR}/intermediate/${target})
    else()
        set(REFLECTION_GENERATED_DIR ${ZENO_REFLECTION_GENERATED_HEADERS_DIR})
    endif()

    if (REFLECTION_USE_PREBUILT_BINARY AND WIN32)
        add_custom_command(
            OUTPUT ${TIMESTAMP_FILE}
            COMMAND ${CMAKE_COMMAND} -E touch ${TIMESTAMP_FILE}
            COMMAND ${generator_path}
                --include_dirs=\"$<JOIN:${INCLUDE_DIRS},${splitor}>,${SYSTEM_IMPLICIT_INCLUDE_DIRS}\"
                --pre_include_header="${LIBREFLECT_PCH_PATH}"
                --input_source=\"${source_paths_string}\"
                --header_output="${REFLECTION_GENERATED_DIR}"
                --stdc++=${CMAKE_CXX_STANDARD}
                $<IF:$<CONFIG:Debug>,-v,>
                --generated_source_path="${INTERMEDIATE_ALL_IN_ONE_FILE}"
                --target_name="${target}"
            DEPENDS ${reflection_headers}
            COMMENT "Generating reflection information for ${target}..."
        )
    else ()
        add_custom_command(
            OUTPUT ${TIMESTAMP_FILE}
            COMMAND ${CMAKE_COMMAND} -E touch ${TIMESTAMP_FILE}
            COMMAND $<TARGET_FILE:ZenoReflect::generator>
                --include_dirs=\"$<JOIN:${INCLUDE_DIRS},${splitor}>,${SYSTEM_IMPLICIT_INCLUDE_DIRS}\"
                --pre_include_header="${LIBREFLECT_PCH_PATH}"
                --input_source=\"${source_paths_string}\"
                --header_output="${REFLECTION_GENERATED_DIR}"
                --stdc++=${CMAKE_CXX_STANDARD}
                $<IF:$<CONFIG:Debug>,-v,>
                --generated_source_path="${INTERMEDIATE_ALL_IN_ONE_FILE}"
                --target_name="${target}"
            DEPENDS ${reflection_headers}
            COMMENT "Generating reflection information for ${target}..."
        )
    endif ()

    add_custom_target(${REFLECTION_GENERATION_TARGET}
        DEPENDS ${TIMESTAMP_FILE}
    )

    add_dependencies(${RELFECTION_GENERATION_ROOT_TARGET} ${REFLECTION_GENERATION_TARGET})
    add_dependencies(${target} ${RELFECTION_GENERATION_ROOT_TARGET})

    target_link_libraries(${target} PUBLIC ZenoReflect::libreflect ZenoReflect::libgenerated)
endfunction()


function(zeno_generate_reflection_on_builddir target reflection_headers)
    set(generator_path "${CMAKE_BINARY_DIR}/ReflectGenerator-Prebuilt.exe")
    if (REFLECTION_USE_PREBUILT_BINARY AND WIN32)
        get_prebuilt_generator(generator_path)
    endif()

    set(INTERMEDIATE_FILE_BASE_DIR "${CMAKE_BINARY_DIR}/intermediate")

    # Call this function after all target source has been added
    set(splitor ",")

    set(INTERMEDIATE_FILE_DIR "${INTERMEDIATE_FILE_BASE_DIR}/${target}")
    set(INTERMEDIATE_ALL_IN_ONE_FILE "${INTERMEDIATE_FILE_DIR}/${target}.generated.cpp")
    file(WRITE "${INTERMEDIATE_ALL_IN_ONE_FILE}" "// TBD by reflection generator\n")
    target_sources(${target} PRIVATE "${INTERMEDIATE_ALL_IN_ONE_FILE}")

    # Input sources
    get_target_property(REFLECTION_GENERATION_SOURCE ${target} SOURCES)
    get_target_property(REFLECTION_GENERATION_SOURCE_DIR ${target} SOURCE_DIR)
    list(LENGTH REFLECTION_GENERATION_SOURCE source_files_length)
    if (source_files_length EQUAL 0)
        message(WARNING "There is not source files found in target ${target}, check your calling timing")
    endif()
    set(source_paths_value ${REFLECTION_GENERATION_SOURCE})
    list(JOIN reflection_headers ${splitor} source_paths_string)

    # Include dirs
    set(INCLUDE_DIRS $<LIST:REMOVE_DUPLICATES,$<TARGET_PROPERTY:${target},INCLUDE_DIRECTORIES>>)
    # Obtain compiler built-in include paths
    list(JOIN CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "," SYSTEM_IMPLICIT_INCLUDE_DIRS)

    set(REFLECTION_GENERATION_TARGET _internal_${target}_reflect_generation)

    if (REFLECTION_USE_PREBUILT_BINARY AND WIN32)
        add_custom_target(${REFLECTION_GENERATION_TARGET}
            WORKING_DIRECTORY
                ${CMAKE_CURRENT_BINARY_DIR}
            COMMAND
                ${generator_path} --include_dirs=\"$<JOIN:${INCLUDE_DIRS},${splitor}>,${SYSTEM_IMPLICIT_INCLUDE_DIRS}\" --pre_include_header="${LIBREFLECT_PCH_PATH}" --input_source=\"${source_paths_string}\" --header_output="${INTERMEDIATE_FILE_DIR}" --stdc++=${CMAKE_CXX_STANDARD} $<IF:$<CONFIG:Debug>,-v,> --generated_source_path="${INTERMEDIATE_ALL_IN_ONE_FILE}" --target_name="${target}"
            SOURCES
                ${reflection_headers}
            COMMENT
                "Generating reflection information for ${target}..."
        )
    else ()
        add_custom_target(${REFLECTION_GENERATION_TARGET}
            WORKING_DIRECTORY
                ${CMAKE_CURRENT_BINARY_DIR}
            COMMAND
                $<TARGET_FILE:ZenoReflect::generator> --include_dirs=\"$<JOIN:${INCLUDE_DIRS},${splitor}>,${SYSTEM_IMPLICIT_INCLUDE_DIRS}\" --pre_include_header="${LIBREFLECT_PCH_PATH}" --input_source=\"${source_paths_string}\" --header_output="${INTERMEDIATE_FILE_DIR}" --stdc++=${CMAKE_CXX_STANDARD} $<IF:$<CONFIG:Debug>,-v,> --generated_source_path="${INTERMEDIATE_ALL_IN_ONE_FILE}" --target_name="${target}"
            SOURCES
                ${reflection_headers}
            COMMENT
                "Generating reflection information for ${target}..."
        )
    endif ()
    add_dependencies(${RELFECTION_GENERATION_ROOT_TARGET} ${REFLECTION_GENERATION_TARGET})
    add_dependencies(${target} ${RELFECTION_GENERATION_ROOT_TARGET})

    target_link_libraries(${target} PUBLIC ZenoReflect::libreflect ZenoReflect::libgenerated)
endfunction()
