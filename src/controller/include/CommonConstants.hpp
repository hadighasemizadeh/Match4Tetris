#ifndef _COMMON_CONSTANTS_HPP_
#define _COMMON_CONSTANTS_HPP_

#include "Utils.hpp"
#include <map>
#include <string>

namespace MatchThree
{
    namespace Controller
    {
        constexpr float DELTA   = 0.01f;
        constexpr float GRAVITY = 70.0f;       // Gravity!! (in a galexy far far away)

        constexpr unsigned int ROW = 16;
        constexpr unsigned int COLUMN = 8;

        constexpr unsigned int MINIMUM_NUMBER_OF_MATCHES = 4;

        enum class GameState
        {
            GameOver,
            Pause,
            Play
        };

        enum class BoardState
        {
            SPWANNING,
            ANIMATING,
            MATCH_CHECKING,
            GRAVITY,
            DEAD
        };

        enum class BlockState
        {
            IDLE,
            FALLING,
            SWAPPING,
            DYING,
            DEAD
        };

        constexpr int WINDOW_WIDTH  = 1280;
        constexpr int WINDOW_HEIGHT = 720;

        constexpr int BLOCK_SIZE_C = 31;
        constexpr int BLOCK_SIZE_R = 31;

        constexpr int BOARD_POS_C = WINDOW_WIDTH/2 - (COLUMN/2) * BLOCK_SIZE_C;
        constexpr int BOARD_POS_R = WINDOW_HEIGHT/2 - (ROW/2) * BLOCK_SIZE_R;

        constexpr float FRAME_TIME = 1.0f / 60.0f;

        constexpr char* const DEFAULT_FONT = "res/fonts/waltographUI.ttf";
        constexpr int DEFAULT_FONT_SIZE = 24;
        
        constexpr int FIRST_BLOCK_ID = 1;
        const std::map<int, std::string> block_tid_to_path{
            { 0, "res/textures/background.jpg"},
            { 1, "res/textures/blue.png" },
            { 2, "res/textures/green.png"}, 
            { 3, "res/textures/orange.png"}, 
            { 4, "res/textures/red.png"}, 
        };
    }
}
#endif