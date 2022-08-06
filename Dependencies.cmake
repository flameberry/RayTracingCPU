# OpenGL Helper Libs
set(GRAPHICS_LIBS glfw Glad imgui)

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
