#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

namespace MatchThree 
{
    namespace Model
    {
        struct Player
        {
            std::string name;
            unsigned int score;
        };
    }
}
#endif