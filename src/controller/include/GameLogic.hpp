#ifndef _GAME_LOGIC_HPP_
#define _GAME_LOGIC_HPP_

#include "Board.hpp"
#include "Player.hpp"
#include <memory>

using namespace MatchThree::Model;

namespace MatchThree 
{
    namespace Controller
    {
        class GameLogic
        {
        private:

            Player      m_player;
            int         m_combo;

        public:
            const std::unique_ptr<Board>& m_board;
            GameLogic() = delete;
            GameLogic(const std::unique_ptr<Board>& board, std::string p_name);
            ~GameLogic();

            void updatePlayerScore(int points);
            Player getPlayerInfo() const;
            bool gameIsOver() const;
            bool update();
        };
    }
}
#endif