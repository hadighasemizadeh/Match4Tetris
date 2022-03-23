#ifndef _INPUT_CTRL_HPP_
#define _INPUT_CTRL_HPP_

#include "GameLogic.hpp"
#include "Board.hpp"

#include <SDL.h>

using namespace MatchThree::Model;

namespace MatchThree 
{
    namespace Controller
    {
        class InputCtrl
        {
        private:
            SDL_Event m_event;
            const std::unique_ptr<Board>& m_board;

            // Mouse Events
            void onMouseUpHandler(int x, int y);
            // Key Events
            void onLeftKey();
            void onDownKey();
            void onRightKey();
            void onSpaceKey();

            void keyDownHandler();
            void keyUpHandler();
        public:
            InputCtrl(const std::unique_ptr<Board>& board);
            bool pollEvent(const unsigned int current_time);
        };
    } 
}
#endif