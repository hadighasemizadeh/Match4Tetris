SET (HEADERs    include/Block.hpp
                include/Board.hpp
                include/Entity.hpp
                include/Player.hpp)
                
SET (SRCs   src/Block.cpp
            src/Board.cpp)

ADD_LIBRARY(model    ${HEADERs}  ${SRCs})

TARGET_LINK_LIBRARIES (model    utils)
