cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(Opto)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# ---- LLVM ----
set(LLVM_DIR "/opt/homebrew/opt/llvm/lib/cmake/llvm")
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Dependencies
set(RAYLIB_VERSION 4.2.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif ()
endif ()


# Our Project

add_executable(${PROJECT_NAME}
        main.cpp
        main.h
        object_view.cpp
        object_view.h
        user_input.cpp
        user_input.h
        object.cpp
        object.h
        do_then.cpp
        do_then.h
        stds.h
        globals.h
        event.cpp
        event.h
        unknown.cpp
        unknown.h
        type.cpp
        type.h
        editable_text.cpp
        editable_text.h
        declare.h declare.cpp string_.h string_.cpp integer.cpp integer.h add.cpp globals.cpp none.cpp none.h assign.h assign.cpp run.cpp
        if.cpp
        if.h
        while.cpp
        while.h
        greater_than.cpp
        greater_than.h
        arm_compile.cpp
        arm_compile.h
        repeat.cpp
        repeat.h
        loop.cpp
        loop.h
        procedure.cpp
        procedure.h
        call.cpp
        call.h
        llvm_compile.cpp
        llvm_compile.h
        test_flows.cpp
        test_flows.h
        compile.cpp
        compile.h
)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)

# Map LLVM components to libraries
llvm_map_components_to_libnames(LLVM_LIBS
        Core
        IRReader
        Support
        Target
        TargetParser
        MC
        Object
        native            # This automatically maps to AArch64 on your Mac
        nativecodegen     # Required for object file emission
        AsmPrinter        # Required for InitializeNativeTargetAsmPrinter
        AsmParser         # Required for InitializeNativeTargetAsmParser
)
target_link_libraries(${PROJECT_NAME} raylib ${LLVM_LIBS})

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()