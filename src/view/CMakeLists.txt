SET (HEADERs    Include/IRenderer.hpp
                include/Renderer.hpp
                include/RendererExceptionHandler.hpp
                include/View.hpp)

SET (SRCs   src/Renderer.cpp
            src/View.cpp)

ADD_LIBRARY(view    ${HEADERs}  ${SRCs})

TARGET_LINK_LIBRARIES(view  utils
                            ${SDL2_LIBRARIES}
                            ${SDL2_IMAGE_LIBRARIES})