#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "CommonConstants.hpp"

 /*
     TUDO: This Interface should be updatedand used when we have more than one block type
           and change Board type from Block to Vector of Vector of Enteties. For now, I just
           keep this here. I did not use and completed it for now.
 */
#include <array>
namespace MatchThree 
{
    namespace Model
    {
        using vector2D = std::array<int, 2>;
        using Index2D = std::array<int, 2>;
        struct Entity
        {
            virtual ~Entity() {}
            virtual vector2D getPosition() const = 0;
            virtual bool update() = 0;
        };
    }
}
#endif