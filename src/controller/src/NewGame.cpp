#include "NewGame.hpp"

namespace MatchThree 
{
    namespace Controller
    {
        NewGame::NewGame(std::string p_name):   m_board(std::make_unique<Board>()),
                                                m_game_logic(m_board, p_name),
                                                m_input_handler(m_board),
                                                m_view(m_game_logic),
                                                m_game_state(GameState::Play)
        {}

        NewGame::~NewGame() {}

        GameState NewGame::getGameState() const
        {
            return m_game_state;
        }

        bool NewGame::update(unsigned int current_time)
        {
            // Handle user's input
            if (m_input_handler.pollEvent(current_time))
                return false;
           
            // TUDO: Write a better scoring mechanism lik
            // Update board and see how much point did we get
            int points = m_board->update(current_time);

            // update the game logic. The game is over, if the logic cannot update
            if (!m_game_logic.update())
                m_game_state = GameState::GameOver;

            m_game_logic.updatePlayerScore(points);

            // Render changes
            m_view.update(m_game_state);

            return true;
        }
    }
}