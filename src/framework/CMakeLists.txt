# add framework cmake modules
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake;${CMAKE_MODULE_PATH}")
MESSAGE(STATUS ${CMAKE_MODULE_PATH})

# framework options
OPTION(NO_CONSOLE "Disables console window on Windows platform" OFF)
OPTION(HANDLE_EXCEPTIONS "Generate crash reports" OFF)
OPTION(USE_OPENGL_ES2 "Use OpenGL ES 2.0 (for mobiles devices)" OFF)
OPTION(USE_GCC47 "Use experimental gcc 4.7" OFF)

# set debug as default build type
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)

# find needed libraries
SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost COMPONENTS system REQUIRED)

IF(USE_OPENGL_ES2)
    FIND_PACKAGE(OpenGLES2 REQUIRED)
    FIND_PACKAGE(EGL REQUIRED)
    SET(OPENGL_INCLUDE_DIR ${OPENGLES_INCLUDE_DIR} ${EGL_INCLUDE_DIR})
    SET(OPENGL_LIBRARIES ${OPENGLES_LIBRARY} ${EGL_LIBRARY})
    ADD_DEFINITIONS(-DOPENGL_ES2)
ELSE(USE_OPENGL_ES2)
    FIND_PACKAGE(OpenGL REQUIRED)
ENDIF(USE_OPENGL_ES2)

FIND_PACKAGE(Lua REQUIRED)
FIND_PACKAGE(PhysFS REQUIRED)
FIND_PACKAGE(GMP REQUIRED)
FIND_PACKAGE(ZLIB REQUIRED)

# setup compiler options
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CXX_WARNS "-Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable -Wno-switch -Wno-missing-field-initializers")
    SET(CMAKE_CXX_FLAGS "-std=gnu++0x -pipe ${CXX_WARNS}")
    SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -ggdb3 -fno-inline")
    SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O1 -g -ggdb -fno-inline")
    SET(CMAKE_CXX_LINK_FLAGS "-static-libgcc -static-libstdc++ -Wl,--as-needed")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    ADD_DEFINITIONS(-D_DEBUG)
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")

MESSAGE(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
IF(USE_OPENGL_ES2)
    MESSAGE(STATUS "Renderer: OpenGL ES 2")
ELSE(USE_OPENGL_ES2)
    MESSAGE(STATUS "Renderer: OpenGL")
ENDIF(USE_OPENGL_ES2)

IF(HANDLE_EXCEPTIONS)
    ADD_DEFINITIONS(-DHANDLE_EXCEPTIONS)
    MESSAGE(STATUS "Generate crash reports: ON")
ELSE(HANDLE_EXCEPTIONS)
    MESSAGE(STATUS "Generate crash reports: OFF")
ENDIF(HANDLE_EXCEPTIONS)

IF(USE_GCC47)
    SET(CMAKE_C_COMPILER gcc-4.7)
    SET(CMAKE_CXX_COMPILER g++-4.7)
ENDIF(USE_GCC47)

IF(WIN32)
    SET(framework_SOURCES ${framework_SOURCES} ${CMAKE_CURRENT_LIST_DIR}/platform/win32window.cpp)
    SET(ADDITIONAL_LIBRARIES ws2_32 mswsock)

    IF(CMAKE_COMPILER_IS_GNUCXX)
        ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501)
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)

    IF(NO_CONSOLE)
        IF(CMAKE_COMPILER_IS_GNUCXX)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthread")
            SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -mwindows")
        ENDIF(CMAKE_COMPILER_IS_GNUCXX)
        MESSAGE(STATUS "Disable windows console: ON")
    ELSE(NO_CONSOLE)
        MESSAGE(STATUS "Disable windows console: OFF")
    ENDIF(NO_CONSOLE)
ELSE(WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -rdynamic")
    IF(USE_GCC47)
        ADD_DEFINITIONS(-D_GLIBCXX__PTHREADS)
    ENDIF(USE_GCC47)
    SET(ADDITIONAL_LIBRARIES X11 dl)
    SET(framework_SOURCES ${framework_SOURCES} ${CMAKE_CURRENT_LIST_DIR}/platform/x11window.cpp)
ENDIF(WIN32)


INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${PHYSFS_INCLUDE_DIR}
    ${GMP_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    "${CMAKE_CURRENT_LIST_DIR}/.."
)

SET(framework_LIBRARIES
    ${Boost_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${LUA_LIBRARIES}
    ${PHYSFS_LIBRARY}
    ${GMP_LIBRARY}
    ${ZLIB_LIBRARY}
    ${ADDITIONAL_LIBRARIES}
)


SET(framework_SOURCES ${framework_SOURCES}
    # framework
    ${CMAKE_CURRENT_LIST_DIR}/application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/luafunctions.cpp

    # framework core
    ${CMAKE_CURRENT_LIST_DIR}/core/logger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/clock.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/configmanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/resourcemanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/eventdispatcher.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/modulemanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/module.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/clock.cpp

    # framework net
    ${CMAKE_CURRENT_LIST_DIR}/net/connection.cpp
    ${CMAKE_CURRENT_LIST_DIR}/net/inputmessage.cpp
    ${CMAKE_CURRENT_LIST_DIR}/net/outputmessage.cpp
    ${CMAKE_CURRENT_LIST_DIR}/net/protocol.cpp
    ${CMAKE_CURRENT_LIST_DIR}/net/rsa.cpp
    ${CMAKE_CURRENT_LIST_DIR}/net/server.cpp

    # framework platform
    ${CMAKE_CURRENT_LIST_DIR}/platform/platformwindow.cpp

    # framework graphics
    ${CMAKE_CURRENT_LIST_DIR}/graphics/font.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/fontmanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/graphics.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/painter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/texture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/framebuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/animatedtexture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/framebuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/texturemanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/borderimage.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/image.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/particlemanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/particlesystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/particleemitter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/particleaffector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/particle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/shader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/shaderprogram.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/paintershaderprogram.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphics/coordsbuffer.cpp

    # framework otml
    ${CMAKE_CURRENT_LIST_DIR}/otml/otmldocument.cpp
    ${CMAKE_CURRENT_LIST_DIR}/otml/otmlemitter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/otml/otmlnode.cpp
    ${CMAKE_CURRENT_LIST_DIR}/otml/otmlparser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/otml/otmlexception.cpp

    # framework luascript
    ${CMAKE_CURRENT_LIST_DIR}/luascript/luainterface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/luascript/luaobject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/luascript/luaexception.cpp
    ${CMAKE_CURRENT_LIST_DIR}/luascript/luavaluecasts.cpp

    # framework ui
    ${CMAKE_CURRENT_LIST_DIR}/ui/uimanager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/uiwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/uilabel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/uibutton.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/uilineedit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/uiwindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/uianchorlayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/uiverticallayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/uilayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/uiprogressbar.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/uicheckbox.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/uiframecounter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ui/uitranslator.cpp

    # framework third party
    ${CMAKE_CURRENT_LIST_DIR}/thirdparty/apngloader.cpp
)
