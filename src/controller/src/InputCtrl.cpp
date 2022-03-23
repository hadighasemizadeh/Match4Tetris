#include "InputCtrl.hpp"

#include <SDL.h>
#include <iostream>

namespace MatchThree 
{
    namespace Controller
    {
        void InputCtrl::onLeftKey()
        {
            m_board->slide(-1);
        }

        void InputCtrl::onDownKey()
        {
            m_board->freeFall();
        }

        void InputCtrl::onRightKey()
        {
            m_board->slide(1);
        }

        void InputCtrl::onSpaceKey()
        {
            m_board->filpFallingBlocks();
        }

        void InputCtrl::onMouseUpHandler(int x, int y)
        {
            //if (m_event.button.button == SDL_BUTTON_LEFT) 
            //{
            //}
        }

        void InputCtrl::keyDownHandler()
        {
            switch (m_event.key.keysym.sym)
            {
                case SDLK_s:
                case SDLK_DOWN:
                    onDownKey();
                    break;
                default:
                    break;
            }
        }
        
        void InputCtrl::keyUpHandler()
        {
            switch (m_event.key.keysym.sym)
            {
            case SDLK_a:
            case SDLK_LEFT:
                onLeftKey();
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                onRightKey();
                break;
            case SDLK_SPACE:
                onSpaceKey();
                break;
            default:
                break;
            }
        }

        InputCtrl::InputCtrl(const std::unique_ptr<Board>& board):  m_board(board),
                                                                    m_event()
        {}

        bool InputCtrl::pollEvent(unsigned int current_time)
        {
            while(SDL_PollEvent(&m_event))
            {
                switch(m_event.type)
                {
                    case SDL_QUIT:
                        return true;
                    case SDL_MOUSEBUTTONUP:
                        onMouseUpHandler(m_event.motion.x, m_event.motion.y);
                        break;
                    case SDL_KEYDOWN:
                        keyDownHandler();
                        break;
                    case SDL_KEYUP:
                        keyUpHandler();
                        break;
                    default:
                        break;
                }
            }
            return false;
        }
    }
}