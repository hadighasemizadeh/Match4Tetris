#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include "Entity.hpp"

#include <memory>
#include <string>
#include <vector>

using namespace MatchThree::Controller;

namespace MatchThree 
{
    namespace Model
    {       
        struct NotImplementedException : public std::exception{};
        class Block //: public Entity
        {
        private:
            vector2D        m_screen_current_position;
            vector2D        m_screen_target_position;
            
            BlockState		m_state;

            int             m_id;
            unsigned int    m_current_falling_time;
            bool            m_free_fall;

        public:
            Block();
            explicit Block(unsigned int id);
            Block(const Block& other);
            Block(Block&& other) noexcept;
            ~Block();

            bool operator==(const Block& other);
            bool operator!=(const Block& other);

            Block& operator=(const Block& other);
            Block& operator=(Block&& other) noexcept;

            BlockState   getBlockState() const;
            vector2D     getScreenCurrentPosition() const;
            vector2D     getScreenTargetPosition() const;
            int     getBlockID() const;
            
            void    setBlockFree();
            void    setBlockId(int new_id);
            void    setBlockState(Controller::BlockState new_state);
            void    setStartFallingTime(unsigned int current_time);

            void    setScreenCurrentPosition(const vector2D position, Index2D direction);
            void    setScreenTargetPosition(const Index2D& target_index);

            void    setScreenCurrentPositionWithIndex(int column, int row_direction);

            bool    empty() const;
            bool    isDead() const;
            bool    isFalling()  const;
            bool    isMovable() const;
            bool    isPossibleTarget(int target_height) const;
            bool    isFreeFall() const;

            void    freeFall();
            void    spwan(const Index2D& start_index, const Index2D& target_index, unsigned int current_time);

            void    slideTo(Index2D target_Index, int direction);
            void    fallInto(Index2D target_Index);

            void    simulate(Index2D target_index, unsigned int current_time);
        };

        Index2D  positionToIndex(vector2D position);
        vector2D indexToPosition(Index2D index);
    } 
}
#endif