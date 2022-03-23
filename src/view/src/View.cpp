#include "View.hpp"
#include <sstream>

namespace MatchThree 
{
    namespace View
    {
        View::View(const GameLogic& game_logic): m_game_logic(game_logic),
                                                 m_renderer(std::make_unique<Renderer>())
        {}

        void View::gameOverView()
        {
            m_renderer->setColor(0, 0, 0, 230);

            int y = static_cast<int>(Controller::WINDOW_HEIGHT / 8);

            m_renderer->drawFilledRectangle( 0, y,
                                             Controller::WINDOW_WIDTH,
                                             static_cast<int>(Controller::WINDOW_HEIGHT / 2));

            m_renderer->drawText("GAME OVER!", 
                                  static_cast<int>(Controller::WINDOW_WIDTH / 2)-100,
                                  static_cast<int>(2*y),
                                  { 255, 155, 0, 225 });
        }

        void View::playView()
        {
            // Draw the board's background
            m_renderer->drawFilledRectangle(Controller::BOARD_POS_C, Controller::BOARD_POS_R,
                                            Controller::COLUMN * Controller::BLOCK_SIZE_C,
                                            Controller::ROW * Controller::BLOCK_SIZE_R);
            // draw all blocks
            for (const auto& c_blocks : m_game_logic.m_board->getGrids())
            {
                for (const auto& block : c_blocks)
                {
                    if (!block.empty())
                    {
                        m_renderer->drawTexture(block.getBlockID(),
                                                block.getScreenCurrentPosition()[0],
                                                block.getScreenCurrentPosition()[1]);
                    }
                }
            }
            // Set rectangles background color
            m_renderer->setColor(1, 55, 200, 160);
            // Draw score background
            m_renderer->drawFilledRectangle( 20, 20 , 120, 100);
            m_renderer->drawText("Score",  40, 40 );
            m_renderer->drawText(std::to_string(m_game_logic.getPlayerInfo().score).c_str(), 70, 80);
        }

        void View::update(const GameState game_state)
        {
            m_renderer->clear();
            m_renderer->drawBackground(0);
            
            switch (game_state)
            {
                case GameState::GameOver:
                    gameOverView();
                    break;
                case GameState::Play:
                    playView();
                    break;
                default:
                    break;
            }
            m_renderer->present();
        }
    }
}