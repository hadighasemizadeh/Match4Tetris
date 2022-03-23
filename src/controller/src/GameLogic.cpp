#include "GameLogic.hpp"
#include "Entity.hpp"

namespace MatchThree 
{
    namespace Controller
    {
        GameLogic::GameLogic(const std::unique_ptr<Board>& board, std::string p_name) : m_board{board},
                                                                                        m_player{p_name ,0},
                                                                                        m_combo{0}
        {}

        GameLogic::~GameLogic()
        {}

        void GameLogic::updatePlayerScore(int points)
        {
            if (points <= 0)
            {
                m_player.score += (m_player.score < 0) ? 0 : points;

                if (m_player.score < 0)
                    m_player.score = 0;

                m_combo = 0;
            }
            else
            {
                m_combo++;
                m_player.score += (points * m_combo);
            }
        }

        Player GameLogic::getPlayerInfo() const
        {
            return m_player;
        }

        bool GameLogic::gameIsOver() const
        {
            std::vector<Index2D> indeces = m_board->findAvialableStartIndex();
            return indeces[0] == Index2D{-1,-1} || indeces[1] == Index2D{ -1,-1 };
        }

        bool GameLogic::update()
        {
            if (gameIsOver()) 
            {
                m_board->setBoardState(BoardState::DEAD);
                return false;
            }

            return true;
        }
    }
}