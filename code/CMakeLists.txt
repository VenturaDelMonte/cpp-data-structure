#
# Builds the core library
#
project(containers CXX)

include(cmake/environment.cmake)

# enable ctest support for testing
enable_testing()

# States that CMake required version must be >= 2.6
cmake_minimum_required(VERSION 2.6)

# Create a variable containing all .cpp files:
file(GLOB_RECURSE project_srcs 	src/*.cpp src/*.def src/*.inc src/*.cxx)
file(GLOB_RECURSE project_incs 	include/containers/*.h include/containers/*.def include/containers/*.inc )

# list all include paths
include_directories( ${project_inc_dir} )

# Build the code as a library
add_library(containers ${LINKING_TYPE} ${project_srcs} ${project_incs})

#set(USE_VALGRIND OFF)
#set(CONDUCT_MEMORY_CHECKS OFF)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

# build executables
file(GLOB_RECURSE executables containers/*.cxx)
foreach ( exec_file ${executables})

        get_filename_component( exec_name ${exec_file} NAME_WE )

        # Build dummy executable
        add_executable( ${exec_name} ${exec_file}  )

	target_link_libraries(${case_name} containers)
	target_link_libraries(${case_name} ${pthread_lib})
	
endforeach(exec_file)


# Build unit tests
set ( ut_prefix  ut_ )
file(GLOB_RECURSE test_cases test/*.cc)
foreach ( case_file ${test_cases})
	get_filename_component( case_name ${case_file} NAME_WE )
	
	set ( case_name ${ut_prefix}${case_name} )
	add_executable(${case_name} ${case_file})

	target_link_libraries(${case_name} containers)
	target_link_libraries(${case_name} ${pthread_lib})

	add_unit_test(${case_name} ${ut_prefix})
endforeach(case_file)

