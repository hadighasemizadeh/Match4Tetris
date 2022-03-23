#ifndef _VIEW_HPP_
#define _VIEW_HPP_

#include "Utils.hpp"
#include "Renderer.hpp"
#include "GameLogic.hpp"
#include <memory>

using namespace MatchThree::Model;

namespace MatchThree 
{
    namespace View
    {   
        class View
        {
         private:
             const GameLogic&               m_game_logic;
             std::unique_ptr<IRenderer>     m_renderer;
        public:
             View(const GameLogic& game_logic);
             void gameOverView();
             void playView();
             void update(const GameState game_state);
        };
    }
}
#endif