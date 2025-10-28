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
# - InferenceEnv: Execute a TPG from C code, after code generation, on the ArmLearnWrapper learning environment
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
# E.g. CXXFLAGS="-DUSE_FIXEDPT" CFLAGS="-DUSE_FIXEDPT" cmake
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
        )
list(REMOVE_ITEM
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/resultTester.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/resultTester.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainGrabStandalone.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainMultipleTraining.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainCodeGen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/codeGenArmlearn.c
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/codeGenArmlearn.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/codeGenArmlearn_program.c
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/codeGenArmlearn_program.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainInferenceEnv.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/externHeader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/externHeader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainSeedsGraphTraversal.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armLearnWrapper_InferenceEnv.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armLearnWrapper_InferenceEnv.cpp
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
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armLearnWrapper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armLearnWrapper.h)
list(APPEND
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/codeGenArmlearn.c
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/codeGenArmlearn.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/codeGenArmlearn_program.c
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/codeGenArmlearn_program.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainInferenceEnv.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/externHeader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/externHeader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armLearnWrapper_InferenceEnv.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armLearnWrapper_InferenceEnv.cpp
        )


# InferenceEnv, Execute a TPG from C code, after code generation on the ArmLearnWrapper learning environment
add_executable(InferenceEnv ${armgegelati_files})
target_link_libraries(InferenceEnv ${GEGELATI_LIBRARIES} /usr/local/lib/libarmlearn.so)
target_compile_definitions(InferenceEnv PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")


# armGrabGegelati: Control the robotic arm with gegelati.
list(REMOVE_ITEM
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/codeGenArmlearn.c
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/codeGenArmlearn.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/codeGenArmlearn_program.c
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/codeGenArmlearn_program.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainInferenceEnv.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/externHeader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/codegen/externHeader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armLearnWrapper_InferenceEnv.h
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armLearnWrapper_InferenceEnv.cpp
        )
list(APPEND
        armgegelati_files
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/mainCodeGen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armLearnWrapper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/./src/armLearnWrapper.h)


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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mainSeedsGraphTraversal.cpp)


# PreCalcul: generate seeds in PreCalcul/seeds_nbActionsToTerminal.h for inference of mainInferenceBenchmark.cpp
# using graph traversal
add_executable(PreCalcul ${armgegelati_files})
target_link_libraries(PreCalcul ${GEGELATI_LIBRARIES} /usr/local/lib/libarmlearn.so)
target_compile_definitions(PreCalcul PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")


# *******************************************
# *********** Directory creation ************
# *******************************************



if(NOT EXISTS ${CMAKE_BINARY_DIR}/../outLogs)
        add_custom_command(
                TARGET ${PROJECT_NAME}
                COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/../outLogs
                COMMENT "Creating outLogs directory")
endif()


if(NOT EXISTS ${CMAKE_BINARY_DIR}/../outLogs/dotfiles)
        add_custom_command(
                TARGET ${PROJECT_NAME}
                COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/../outLogs/dotfiles
                COMMENT "Creating outLogs/dotfiles directory")
endif()
