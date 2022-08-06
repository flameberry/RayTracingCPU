# OpenGL Helper Libs
set(GRAPHICS_LIBS glfw Glad)

if(APPLE)
    # Inbuilt mac frameworks required for GLFW
    list(APPEND GRAPHICS_LIBS 
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreFoundation"
    )
elseif(WIN32)
    list(APPEND GRAPHICS_LIBS opengl32.lib)
endif()

# All Include Dirs needed for the project
set(GRAPHICS_INCLUDE_DIRS
    ${SOURCE_DIR}/vendor
    ${SOURCE_DIR}/vendor/imgui
    ${SOURCE_DIR}/vendor/GLFW/include
    ${SOURCE_DIR}/vendor/Glad/include
    ${SOURCE_DIR}/vendor/glm
)

# ImGui files
file(GLOB IMGUI_SRC ${SOURCE_DIR}/vendor/imgui/*.cpp ${SOURCE_DIR}/vendor/imgui/*.h)
list(APPEND IMGUI_SRC
    ${SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp
    ${SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.h
    ${SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
    ${SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.h
    ${SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3_loader.h
)
