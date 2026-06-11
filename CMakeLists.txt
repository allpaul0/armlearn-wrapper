cmake_minimum_required(VERSION 3.16)

# **********************
# GEGELATI ArmLearn-Wrapper project.
#
# Control of the WidowX robotic arm  using the Gegelati library.
#
# This CMake project creates 3 executables:
# - Training: Training the TPG on the ArmLearnWrapper learning environment
# - MultipleTraining: Create multiple trainings
# - CodeGen: Reduce the size of the trained TPG graph by generating C code, making 
# it free from the gegelati library. Removes unnecessary parts of the graph called hitchickers and unecessary instructions called introns.
# - InferenceAccuracy: Execute a TPG from C code, after code generation, on the ArmLearnWrapper learning environment
# - GrabGegelati: Control the robotic arm with gegelati.
# - GrabStandalone: Control the robotic arm without gegelati.
#
# **********************

project(GEGELATI_ArmLearn-Wrapper)

set(CMAKE_CXX_STANDARD 17)

if(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio.*")
    # Link with pthread
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

    #add libmath during non visual studio builds
    set(CMAKE_EXTRA_LIB m)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Allow additional flags to be passed at make time
# for InferenceBenchmark target
# E.g. CXXFLAGS="-DUSE_FIXEDPT" CFLAGS="-DUSE_FIXEDPT" cmake ..
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{CXXFLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} $ENV{CFLAGS}")


# Add definitions for testing purposes
if(${TESTING})
    MESSAGE("Testing mode")
    add_definitions(-DNB_GENERATIONS=10)
endif()

# Include directories for JSONcpp
include_directories("${CMAKE_SOURCE_DIR}/lib/JsonCpp")

# *******************************************
# *********** GEGELATI LIBRARY **************
# *******************************************

find_package(GEGELATI)


# includes eigen
include_directories (/usr/local/include/eigen3)

# *******************************************
# ************** Executables ****************
# *****************************************

file(GLOB_RECURSE
        armgegelati_files
        ./src/*.cpp
        ./lib/JsonCpp/*.cpp
        ./src/*.h
        ./src/armlearn/*.cpp
        ./src/armlearn/*.h)
list(REMOVE_ITEM
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/resultTester.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/resultTester.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainGrabStandalone.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainMultipleTraining.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainCodeGen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/TPG.c
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/TPG.h
        # ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/TPG_program.c
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/TPG_program.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainInferenceAccuracy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/externHeader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/externHeader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainExportLEstates.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armlearn/environment/ArmLearnWrapperInference.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armlearn/environment/ArmLearnWrapperInference.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainInferenceBenchmark.cpp
        )


# Training: Training Gegelati on the ArmLearnWrapper learning environment
add_executable(Training ${armgegelati_files})
target_link_libraries(Training ${GEGELATI_LIBRARIES} /usr/local/lib/libarmlearn.so)
target_compile_definitions(Training PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")

list(REMOVE_ITEM
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/main.cpp)
list(APPEND
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainMultipleTraining.cpp)


# MultipleTraining: Create multiple trainings
add_executable(MultipleTraining ${armgegelati_files})
target_link_libraries(MultipleTraining ${GEGELATI_LIBRARIES} /usr/local/lib/libarmlearn.so)
target_compile_definitions(MultipleTraining PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")

list(REMOVE_ITEM
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainMultipleTraining.cpp
        )
list(APPEND
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainInferenceAccuracy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armlearn/environment/ArmLearnWrapperInference.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armlearn/environment/ArmLearnWrapperInference.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/TPG.c
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/TPG.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/TPG_program.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/externHeader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/externHeader.cpp
        )


# InferenceAccuracy, Execute a TPG from C code, after code generation on the ArmLearnWrapper learning environment
add_executable(InferenceAccuracy ${armgegelati_files})
target_link_libraries(InferenceAccuracy ${GEGELATI_LIBRARIES} /usr/local/lib/libarmlearn.so)
target_compile_definitions(InferenceAccuracy PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")

list(REMOVE_ITEM
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainInferenceAccuracy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armlearn/environment/ArmLearnWrapperInference.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armlearn/environment/ArmLearnWrapperInference.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armlearn/environment/ArmLearnWrapper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armlearn/environment/ArmLearnWrapper.h
)

list(APPEND
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainInferenceBenchmark.cpp)

# InferenceBenchmark: Benchmark the inference time of a trained TPG graph on the ArmLearnWrapper learning environment
add_executable(InferenceBenchmark ${armgegelati_files})
target_link_libraries(InferenceBenchmark ${GEGELATI_LIBRARIES})
target_compile_definitions(InferenceBenchmark PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")

list(REMOVE_ITEM
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainInferenceBenchmark.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/externHeader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/externHeader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/TPG.c
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/TPG.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/TPG_program.c
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/TPG_program.h
        )
list(APPEND
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainCodeGen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armlearn/environment/ArmLearnWrapper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armlearn/environment/ArmLearnWrapper.h
)


# CodeGen: Reduce the size of the trained TPG graph by generating C code, making 
# it free from the gegelati library. Removes unnecessary parts of the graph called hitchickers. 
add_executable(CodeGen ${armgegelati_files})
target_link_libraries(CodeGen ${GEGELATI_LIBRARIES} /usr/local/lib/libarmlearn.so)
target_compile_definitions(CodeGen PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")


list(REMOVE_ITEM
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainCodeGen.cpp)
list(APPEND
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/resultTester.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/resultTester.h)
        
# GrabGegelati: Control the robotic arm with gegelati.
add_executable(armGrabGegelati ${armgegelati_files})
target_link_libraries(armGrabGegelati ${GEGELATI_LIBRARIES} /usr/local/lib/libarmlearn.so)
target_compile_definitions(armGrabGegelati PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")

# GrabStandalone: Control the robotic arm without gegelati
add_executable(GrabStandalone ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainGrabStandalone.cpp)
target_link_libraries(GrabStandalone /usr/local/lib/libarmlearn.so)


list(REMOVE_ITEM
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/resultTester.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/resultTester.h)
list(APPEND
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mainExportLEstates.cpp)


# exportLEstates: generate seeds in precalul/LE_states.h for inference of mainInferenceBenchmark.cpp
# using graph traversal
add_executable(exportLEstates ${armgegelati_files})
target_link_libraries(exportLEstates ${GEGELATI_LIBRARIES} /usr/local/lib/libarmlearn.so)
target_compile_definitions(exportLEstates PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")


# *******************************************
# *********** Directory creation ************
# *******************************************



if(NOT EXISTS ${CMAKE_BINARY_DIR}/../outLogs)
        add_custom_command(
                TARGET Training POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/../outLogs
                COMMENT "Creating outLogs directory")
endif()


if(NOT EXISTS ${CMAKE_BINARY_DIR}/../outLogs/dotfiles)
        add_custom_command(
                TARGET Training POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/../outLogs/dotfiles
                COMMENT "Creating outLogs/dotfiles directory")
endif()
