#include "Block.hpp"
#include "CommonConstants.hpp"
#include <iostream>

using namespace MatchThree::Controller;

namespace MatchThree 
{
    namespace Model
    {
        Block::Block(): m_id(-1),
                        m_current_falling_time(0),
                        m_free_fall(false),
                        m_state(Controller::BlockState::DEAD),
                        m_screen_current_position({-1,-1}),
                        m_screen_target_position({ -1,-1 })
        {}

        Block::Block(unsigned int id):  m_id(id),
                                        m_current_falling_time(0),
                                        m_free_fall(false),
                                        m_state(Controller::BlockState::DEAD),
                                        m_screen_current_position({ -1,-1 }),
                                        m_screen_target_position({ -1,-1 })
        {}

        Block::Block(const Block & other):  m_id(other.m_id),
                                            m_current_falling_time(other.m_current_falling_time),
                                            m_free_fall(other.m_free_fall),
                                            m_state(other.m_state),
                                            m_screen_current_position(other.m_screen_current_position),
                                            m_screen_target_position(other.m_screen_target_position)
        {}
       
        Block::Block(Block&& other) noexcept: m_id(other.m_id),
                                              m_current_falling_time(other.m_current_falling_time),
                                              m_free_fall(other.m_free_fall),
                                              m_state(other.m_state),
                                              m_screen_current_position(std::move(other.m_screen_current_position)),
                                              m_screen_target_position(std::move(other.m_screen_target_position))
        {}

        bool Block::operator==(const Block& other) 
        {
            return  m_id == other.m_id &&
                    m_current_falling_time == other.m_current_falling_time &&
                    m_screen_current_position == other.m_screen_current_position &&
                    m_screen_target_position == other.m_screen_target_position;
        }

        bool Block::operator!=(const Block& other)
        {
            return !(*this == other);
        }

        Block& Block::operator=(const Block& other)
        {
            if (*this != other)
            {
                m_id = other.m_id;
                m_current_falling_time = other.m_current_falling_time;
                m_free_fall = other.m_free_fall;
                m_state = other.m_state;
                m_screen_current_position = other.m_screen_current_position;
                m_screen_target_position = other.m_screen_target_position;
            }
            return *this;
        }

        Block& Block::operator=(Block&& other) noexcept
        {
            if (*this != other)
            {
                m_id = other.m_id;
                m_free_fall = other.m_free_fall;
                m_current_falling_time = other.m_current_falling_time;
                m_state = other.m_state;
                m_screen_current_position = std::move(other.m_screen_current_position);
                m_screen_target_position = std::move(other.m_screen_target_position);
            }
            return *this;
        }

        Block::~Block() {}

        bool Block::isPossibleTarget(int target_height) const
        {
            /*
                Between saing the index and screen target position, 
                I decided to save screen target position. Because:
                    - Block should not know anything about board.
                    - Saving both was redundant.
                    - Saving screen reduces calculation in long run.
                    - It was the only solustion that came to my mind.
            */
            const int new_possible_position = BOARD_POS_R + target_height * BLOCK_SIZE_R;
            return m_screen_current_position[1] <= new_possible_position;
        }

        int Block::getBlockID() const
        {
            return m_id;
        }

        Controller::BlockState Block::getBlockState() const
        {
            return m_state;
        }

        vector2D Block::getScreenCurrentPosition() const
        {
            return m_screen_current_position;
        }

        vector2D Block::getScreenTargetPosition() const
        {
            return m_screen_target_position;
        }

        void Block::setBlockFree()
        {
            m_id = -1;
            m_free_fall = false;
            m_state = Controller::BlockState::DEAD;
            m_screen_current_position = m_screen_target_position = {-1,-1};
        }

        void Block::setBlockId(int new_id)
        {
            m_id = new_id;
        }

        void Block::setBlockState(Controller::BlockState new_state)
        {
            m_state = new_state;
        }

        void Block::setScreenCurrentPositionWithIndex(int column, int row_direction)
        {
            m_screen_current_position[0] = Controller::BOARD_POS_C + column * Controller::BLOCK_SIZE_C;
            m_screen_current_position[1] += (row_direction * Controller::BLOCK_SIZE_R);
        }

        void Block::setScreenCurrentPosition(const vector2D position, Index2D direction)
        {
            m_screen_current_position[0] = position[0] + direction[0] *Controller::BLOCK_SIZE_C;
            m_screen_current_position[1] = position[1] + direction[1] * Controller::BLOCK_SIZE_R;
        }

        void Block::setScreenTargetPosition(const Index2D& target_index)
        {
            m_screen_target_position[0] = Controller::BOARD_POS_C + target_index[0] * Controller::BLOCK_SIZE_C;
            m_screen_target_position[1] = Controller::BOARD_POS_R + target_index[1] * Controller::BLOCK_SIZE_R;
        }

        void Block::setStartFallingTime(unsigned int current_time)
        {
            m_current_falling_time = current_time;
        }
        
        void Block::fallInto(Index2D target_Index)
        {
            m_state = BlockState::FALLING;
            setScreenTargetPosition(target_Index);
        }

        // ======================================================
        bool Block::empty() const
        {
            return m_id <= 0;
        }

        bool Block::isDead()  const
        {
            return  Controller::BlockState::DEAD == m_state ||
                m_id < Controller::FIRST_BLOCK_ID;
        }

        bool Block::isFalling()  const
        {
            return  Controller::BlockState::FALLING == m_state;
        }

        bool Block::isMovable() const
        {
            return Controller::BlockState::IDLE == m_state;
        }

        bool Block::isFreeFall() const
        {
            return m_free_fall;
        }

        void Block::freeFall()
        {
            m_free_fall = true;
        }

        void Block::slideTo(Index2D target_Index, int direction)
        {
            m_screen_current_position[0] += (Controller::BLOCK_SIZE_C * direction);

            m_screen_target_position[0] = BOARD_POS_C + target_Index[0] * BLOCK_SIZE_C;
            m_screen_target_position[1] = BOARD_POS_R + target_Index[1] * BLOCK_SIZE_R;
        }

        void Block::spwan(const Index2D& start_index, const Index2D& target_index, unsigned int current_time)
        {
            m_id = Utils::getRandomNumber(1, Controller::block_tid_to_path.size() - 1);
            m_current_falling_time = current_time;
            m_state = Controller::BlockState::FALLING;

            // Initialize start position
            m_screen_current_position[0] = Controller::BOARD_POS_C + start_index[0] * Controller::BLOCK_SIZE_C;
            m_screen_current_position[1] = Controller::BOARD_POS_R + start_index[1] * Controller::BLOCK_SIZE_R;

            setScreenTargetPosition(target_index);
        }

        void Block::simulate(Index2D target_index, unsigned int current_time)
        {
            if (isFalling())
            {
                double delta_t = (current_time - m_current_falling_time) * Controller::DELTA;

                // Place it 10 times faster in free fall mode
                int booster = m_free_fall ? 10 : 1;

                m_screen_current_position[1] += (0.5f * Controller::GRAVITY * delta_t * delta_t * booster);
                m_current_falling_time = current_time;

                if (m_screen_current_position[1] >= m_screen_target_position[1])
                {
                    m_screen_current_position[1] = m_screen_target_position[1];
                    m_state = Controller::BlockState::IDLE;
                    m_free_fall = false;
                }
            }
        }

        Index2D positionToIndex(vector2D position)
        {
            return  { static_cast<int>((position[0] - Controller::BOARD_POS_C) / static_cast<float>(Controller::BLOCK_SIZE_C)),
                      static_cast<int>((position[1] - Controller::BOARD_POS_R) / static_cast<float>(Controller::BLOCK_SIZE_R)) };
        }

        vector2D indexToPosition(Index2D index)
        {
            return { Controller::BOARD_POS_C + index[0] * Controller::BLOCK_SIZE_C,
                     Controller::BOARD_POS_R + index[1] * Controller::BLOCK_SIZE_R };
        }
    } 
}