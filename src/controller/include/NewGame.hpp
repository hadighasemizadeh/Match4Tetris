#ifndef _NEW_GAME_HPP_
#define _NEW_GAME_HPP_

#include "View.hpp"
#include "InputCtrl.hpp"
#include "Board.hpp"

#include <memory>
#include <string>

namespace MatchThree 
{
    namespace Controller
    {
        class NewGame
        {
        private:
            std::unique_ptr<Board>  m_board;
            GameLogic               m_game_logic;
            InputCtrl               m_input_handler;
            View::View              m_view;
            GameState               m_game_state;
        public:
            NewGame() = delete;
            NewGame(std::string p_name);
            ~NewGame();

            GameState   getGameState() const;
            bool        update(unsigned int current_time);
        };
    }
}
#endif