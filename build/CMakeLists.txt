CMake_Minimum_Required(VERSION 2.8.9)

Set(PROJECT XTea)

Set(HEADERS
	../headers/plugininterface.h
	../headers/xtea.h 
	../headers/jobtypes.h)

Set(SOURCES ../sources/xtea.cpp)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../${PROJECT_BINARY_DIR}/lib)

Include_Directories( ../headers  ../sources)

Project(${PROJECT})

add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)

Find_Package(Qt5Core REQUIRED)

Set(CMAKE_AUTOMOC ON)
Set(CMAKE_INCLUDE_CURRENT_DIR ON)

If(MSVC)
        Add_Definitions(/W3 -fPIC)
ElseIf(CMAKE_COMPILER_IS_GNUCXX)
        Add_Definitions(-Wall -pedantic -fPIC)
Else()
        Message("Unknown compiler")
EndIf()

Add_Library(${PROJECT} SHARED ${HEADERS} ${SOURCES})

Qt5_Use_Modules(${PROJECT} Core)

