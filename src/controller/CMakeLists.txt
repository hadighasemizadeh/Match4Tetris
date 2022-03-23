SET (HEADERs    include/CommonConstants.hpp
                include/GameLogic.hpp
                include/NewGame.hpp
                include/InputCtrl.hpp
                include/Run.hpp)
                
SET (SRCs   src/GameLogic.cpp
            src/NewGame.cpp
            src/InputCtrl.cpp
            src/Run.cpp)
            
ADD_LIBRARY(controller    ${HEADERs}  ${SRCs})

TARGET_LINK_LIBRARIES(controller    utils
                                    model
                                    view)