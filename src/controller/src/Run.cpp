
#include "Run.hpp"
#include "NewGame.hpp"

#include <iostream>
#include <SDL.h>
#include <memory>

namespace MatchThree 
{
    namespace Controller
    {
        void Run()
        {
            // Create a new game for hard coded player name
            // I do not know for now how big this going to be, so I will make unique ptr
            std::unique_ptr<NewGame> new_game(std::make_unique<NewGame> ("Player1"));

            unsigned int last_update = SDL_GetTicks();
            // Game main loop
            while (true)
            {
                unsigned int current_time = SDL_GetTicks();

                try
                {
                    // TUDO: Handle possible excptions
                    // Update game
                    if (!new_game->update(last_update))
                        return;
                }
                catch (const std::exception&)
                {
                    std::cout << "Exception!!\n";
                }

                float dT = (current_time - last_update) / 1000.0f;
                last_update = current_time;
                // keep it 60 FPS
                SDL_Delay(floor(16.666f - dT));

            }
        }
    } 
}