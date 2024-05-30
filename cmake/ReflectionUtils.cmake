
set(RELFECTION_GENERATION_ROOT_TARGET _Reflection_ROOT)
add_custom_target(${RELFECTION_GENERATION_ROOT_TARGET})

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

function(zeno_declare_reflection_support target reflection_headers)
    # Call this function after all target source has been added
    set(splitor ",")

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

    add_custom_target(${REFLECTION_GENERATION_TARGET}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND 
            $<TARGET_FILE:ZenoReflect::generator> --include_dirs=\"$<JOIN:${INCLUDE_DIRS},${splitor}>,${SYSTEM_IMPLICIT_INCLUDE_DIRS}\" --pre_include_header="${LIBREFLECT_PCH_PATH}" --input_source=\"${source_paths_string}\" -o="${ZENO_REFLECTION_GENERATED_HEADERS_DIR}" --stdc++=${CMAKE_CXX_STANDARD} $<IF:$<CONFIG:Debug>,"-v",""> 
        SOURCES 
            ${REFLECTION_GENERATION_SOURCE} 
        COMMENT 
            "Generating reflection information for ${target}..."
    )
    add_dependencies(${RELFECTION_GENERATION_ROOT_TARGET} ${REFLECTION_GENERATION_TARGET})
    add_dependencies(${target} ${RELFECTION_GENERATION_ROOT_TARGET})

    target_link_libraries(${target} PUBLIC ZenoReflect::libreflect ZenoReflect::libgenerated)
endfunction()
