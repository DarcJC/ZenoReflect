
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
    get_target_property(target_include_dirs ${target} INCLUDE_DIRECTORIES)
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

function(print_target_properties target)
    if(NOT TARGET ${target})
      message(STATUS "There is no target named '${target}'")
      return()
    endif()

    if(NOT CMAKE_PROPERTY_LIST)
        execute_process(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)
        
        # Convert command output into a CMake list
        string(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
        string(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
        list(REMOVE_DUPLICATES CMAKE_PROPERTY_LIST)
    endif()

    foreach(property ${CMAKE_PROPERTY_LIST})
        string(REPLACE "<CONFIG>" "DEBUG" property ${property})

        get_property(was_set TARGET ${target} PROPERTY ${property} SET)
        if(was_set)
            get_target_property(value ${target} ${property})
            message("${target} ${property} = ${value}")
        endif()
    endforeach()
endfunction()

function(zeno_declare_reflection_support target)
    # Call this function after all target source has been added

    # Input sources
    get_target_property(REFLECTION_GENERATION_SOURCE ${target} SOURCES)
    get_target_property(REFLECTION_GENERATION_SOURCE_DIR ${target} SOURCE_DIR)
    list(LENGTH REFLECTION_GENERATION_SOURCE source_files_length)
    if (source_files_length EQUAL 0)
        message(WARNING "There is not source files found in target ${target}, check your calling timing")
    endif()
    set(source_paths_value ${REFLECTION_GENERATION_SOURCE})
    list(TRANSFORM source_paths_value PREPEND "${REFLECTION_GENERATION_SOURCE_DIR}/")
    list(JOIN source_paths_value "," source_paths_string)

    # Include dirs
    get_target_include_dirs_recursive(${target} include_dirs)
    list(JOIN include_dirs "," include_dirs_string)

    set(REFLECTION_GENERATION_TARGET _internal_${target}_reflect_generation)

    add_custom_target(${REFLECTION_GENERATION_TARGET}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND 
            $<TARGET_FILE:ZenoReflect::generator> --include_dirs=\"${include_dirs_string}\" --pre_include_header="${LIBREFLECT_PCH_PATH}" --input_source=\"${source_paths_string}\" -o="${ZENO_REFLECTION_GENERATED_HEADERS_DIR}" $<IF:$<CONFIG:Debug>,"-v",""> 
        SOURCES 
            ${REFLECTION_GENERATION_SOURCE} 
        COMMENT 
            "Generating reflection information"
    )
    add_dependencies(${target} ${REFLECTION_GENERATION_TARGET})
endfunction()
