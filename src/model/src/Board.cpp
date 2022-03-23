#include "Board.hpp"
#include "Utils.hpp"
#include "CommonConstants.hpp"
#include <queue>
#include <iostream>
#include <algorithm>

namespace MatchThree {
    namespace Model
    {
        Board::Board() : m_start_Point({-1,-1}),
                          m_board_state(BoardState::SPWANNING),
                          m_generated_index{ {-1,-1},{-1,-1} }
        {                                 
            m_grid.resize(Controller::COLUMN, std::vector<Block>(Controller::ROW));
        }

        Board::~Board() {}

        //===================================================================
        // TUDO: Move into Matchfinder script and call it from game logic
        bool  Board::isSafe(const grid& grid,
                            int c, int r,
                            const std::vector<std::vector<bool>>& processed,
                            const int block_id)
        {
            return ((c >= 0 && c < grid.size()) &&
                    (r >= 0 && r < grid[0].size()) &&
                    grid[c][r].getBlockID()== block_id &&
                    !processed[c][r]);
        }

        void Board::BFS(const grid& grid,
                        std::vector<std::vector<bool>>& processed,
                        std::vector<Index2D>& out,
                        Index2D index,
                        const int block_id)
        {
            // create an empty queue and enqueue source node
            std::queue<Index2D> q;
            q.push(index);

            // mark source node as processed
            processed[index[0]][index[1]] = true;
            out.push_back(index);
            // Neighbours in all four direction
            int col[] = { 1, 0, -1, 0};
            int row[] = { 0, -1, 0, 1};

            // loop till queue is empty
            while (!q.empty())
            {
                // dequeue front node and process it
                int c = q.front()[0];
                int r = q.front()[1];
                q.pop();

                for (int k = 0; k < 4; k++)
                {
                    if (isSafe(grid, c + col[k], r + row[k], processed, block_id))
                    {
                        // mark neighbour as processed and enqueue it
                        processed[c + col[k]][r + row[k]] = true;
                        out.push_back({ c + col[k], r + row[k] });
                        q.push({ c + col[k], r + row[k] });
                    }
                }
            }
        }

        void Board::findMatches(const grid& grid, Index2D start_index, std::vector<Index2D>& out)
        {
            using namespace std;

            if (start_index[0] == -1 || start_index[1] == -1)
                return;

            const int block_id = grid[start_index[0]][start_index[1]].getBlockID();

            if (block_id < 1 )
                return;

            vector<vector<bool>> temp_index_finder(grid.size(), vector<bool>(grid[0].size()));
            
            BFS(grid, temp_index_finder, out, start_index, block_id);
        }
        
        int Board::removeMatches() 
        {
            if (BoardState::MATCH_CHECKING != m_board_state)
                return 0;

            holes dead_list;

            unsigned int total_matches = 0;

            for (auto& index : m_generated_index)
            {
                if (index[0] == -1 || index[1] == -1)
                    continue;
                
                // Find matches and put the blocks indeces in dead_list
                findMatches(m_grid, index, dead_list);

                // Only re-try it after the next spwan
                index = {-1,-1};

                // MINIMUM_NUMBER_OF_MATCHES+ matches or nothing
                if (dead_list.size() >= MINIMUM_NUMBER_OF_MATCHES)
                {
                    // Remove matches
                    for (auto matchIndex : dead_list)
                    {
                        // remove matches from board
                        m_grid[matchIndex[0]][matchIndex[1]].setBlockFree();
                    }

                    // Save the black holes in a list for the future use.
                    black_holes.insert(std::end(black_holes), std::begin(dead_list), std::end(dead_list));

                    total_matches += dead_list.size();
                }                
                // Reset the list for the possible next try
                dead_list.clear();
            }   

            if (total_matches == 0 && black_holes.size()>0)
            {
                auto itr = black_holes.begin();
                while (itr != black_holes.end())
                {
                    if (m_grid[(*itr)[0]][(*itr)[1]].getBlockID() == -1 || (*itr)[0] == -1 || (*itr)[1] == -1)
                    {
                        itr = black_holes.erase(itr);
                    }
                    else
                    {
                        // Find matches and put the blocks indeces in dead_list
                        findMatches(m_grid, *itr, dead_list);

                        // MINIMUM_NUMBER_OF_MATCHES+ matches or nothing
                        if (dead_list.size() >= MINIMUM_NUMBER_OF_MATCHES)
                        {
                            // Remove matches
                            for (auto matchIndex : dead_list)
                            {
                                // remove matches from board
                                m_grid[matchIndex[0]][matchIndex[1]].setBlockFree();
                            }

                            // Copy dead list for next match checking try
                            black_holes = dead_list;
                            total_matches += dead_list.size();
                            break;
                        }
                        else
                        {
                            // Reset the list for the possible next try
                            dead_list.clear();
                            itr = black_holes.erase(itr);
                        }
                    }
                }
            }

            m_board_state = (total_matches > 0) ? BoardState::GRAVITY : BoardState::SPWANNING;
            return total_matches;
        }

        void Board::applyGravity()
        {
            if (BoardState::GRAVITY != m_board_state)
                return;

            bool falling_blocks_exist = false;

            // Sort holes list to have highest index and lowest column first
            std::sort(black_holes.begin(),
                black_holes.end(),
                [](const Index2D& index1, const Index2D& index2)
                {
                    if (index1[1] == index2[1]) 
                        return index1[0] < index2[0];

                    return index1[1] > index2[1];
                });

            // Update all blocks state that are above the hole
            for (const auto& hole : black_holes)
            {
                int height = 1;
                for (int r = hole[1] - 1; r >= 0; r--)
                {
                    if (m_grid[hole[0]][r].getBlockID() == -1)
                    {
                        height++;
                    }
                    else
                    {
                        Index2D new_target_index { hole[0], r + height };

                        m_grid[hole[0]][r].fallInto(new_target_index);

                        swap({ hole[0], r }, new_target_index);

                        falling_blocks_exist = true;
                    }
                }
            }

            black_holes.clear();

            m_board_state = falling_blocks_exist ? BoardState::ANIMATING : BoardState::SPWANNING;
        }

        //===================================================================
                 
        Block& Board::gridAt(Index2D index)
        {
            return m_grid[index[0]][index[1]];
        }

        const Block& Board::gridAt(Index2D index) const
        {
            return m_grid[index[0]][index[1]];
        }

        grid Board::getGrids() const
        {
            return m_grid;
        }

        BoardState Board::getBoardState() const
        {
            return m_board_state;
        }

        bool Board::areNeighbours(Index2D index1, Index2D index2) const
        {
            int diff_x = abs(index1[0] - index2[0]);
            int diff_y = abs(index1[1] - index2[1]);
            return ((0 == diff_x && 1 == diff_y) || (1 == diff_x && 0 == diff_y));
        }
        
        bool Board::isWall(Index2D index) const
        {
            return  index[0] < 0 || index[0] >= m_grid.size() ||
                    index[1] < 0 || index[1] >= m_grid[0].size();
        }

        bool Board::isBoardDead() const
        {
            return BoardState::DEAD == m_board_state;
        }

        void Board::freeFall()
        {
            // The spwaned block is still falling
            if (m_generated_index[0][0] == -1 || m_generated_index[0][1] == -1 ||
                m_generated_index[1][0] == -1 || m_generated_index[1][1] == -1)
            {
                return;
            }

            m_grid[m_generated_index[0][0]][m_generated_index[0][1]].freeFall();

            if (m_generated_index[1][0] != -1 && m_generated_index[1][1] != -1)
                m_grid[m_generated_index[1][0]][m_generated_index[1][1]].freeFall();
        }

        void Board::resetInPair()
        {
            m_generated_index[0] = m_generated_index[1] = {-1,-1};
        }
        
        void Board::setBoardState(BoardState new_board_state)
        {
            m_board_state = new_board_state;
        }

        void Board::swap(Index2D v1, Index2D v2)
        {
            const auto tmp_block1 = m_grid[v1[0]][v1[1]];
            const auto tmp_block2 = m_grid[v2[0]][v2[1]];
            m_grid[v1[0]][v1[1]] = tmp_block2;
            m_grid[v2[0]][v2[1]] = tmp_block1;
        }

        int Board::firstAvialableRowInColumn(int column) const
        {
            // Is there any wall in the defined direction for the lowest index
            if (column < 0 || column >= m_grid.size())
            {
                std::cout << "Ouch!! This is a wall here!\n";
                return -1;
            }

            for (int reversed_row = m_grid[0].size() - 1; reversed_row >= 0; reversed_row--)
            {
                if (m_grid[column][reversed_row].empty())
                    return reversed_row;
            }
            return -1;
        }

        std::vector<Index2D> Board::findAvialableStartIndex() const
        {
            // Try to find an avialable column from center
            for (int c = 0; c < m_grid.size(); ++c)
            {
                int idx = (c + static_cast<int>((m_grid.size() - 1) / 2.0f)) % (m_grid.size());
                // horizantal or vertical
                if (m_grid[idx][1].empty())
                {
                    return { {idx, 0}, {idx, 1} };
                }
            }

            // Try to find an avialable column from center
            for (int c = 0; c < m_grid.size() - 1; ++c)
            {
                // horizantal 
                if (m_grid[c][0].empty() && m_grid[c + 1][0].empty())
                    return { {c, 0}, {c + 1, 1} };
            }

            return { {-1,-1}, {-1,-1} };
        }

        std::vector<Index2D> Board::findAvialableIndexInDirection(Index2D head, Index2D tail, int direction) const
        {
            if (tail[0] == -1 || tail[1] == -1 ||
                head[0] == -1 || head[1] == -1)
            {
                std::cout << "Invalid generated index!\n";
                return { { -1,-1 }, { -1,-1 } };
            }

            // Validate direction
            if (direction == 0)
            {
                std::cout << "Direction must be either 1 or -1!!\n";
                return { { -1,-1 }, { -1,-1 } };
            }

            // Clear the direction just in case of
            direction = (direction > 0 ? 1 : -1);

            // Falling pair current indexces
            Index2D pair_idx[2] = { positionToIndex(gridAt(tail).getScreenCurrentPosition()),
                                    positionToIndex(gridAt(head).getScreenCurrentPosition()) };

            // Update with our new expected direction
            pair_idx[0][0] += direction;
            pair_idx[1][0] += direction;

            if (pair_idx[0][0] < 0 || pair_idx[0][0] >= m_grid.size() || 
                pair_idx[1][0] < 0 || pair_idx[1][0] >= m_grid.size())
            {
                std::cout << "There is a wall here!!\n";
                return { { -1,-1 }, { -1,-1 } };
            }

            // If they were in the same row
            if (pair_idx[0][1] == pair_idx[1][1])
            {
                if (direction > 0 && pair_idx[0][0] < pair_idx[1][0] ||
                    direction < 0 && pair_idx[0][0] > pair_idx[1][0])
                {
                    int new_head_row = firstAvialableRowInColumn(pair_idx[1][0]);
                    if (pair_idx[1][1] > new_head_row)
                    {
                        std::cout << "Cannot jump!\n";
                        return { { -1,-1 }, { -1,-1 } };
                    }

                    pair_idx[1][1] = new_head_row;
                    pair_idx[0][1] = head[1];
                } 
                else
                {
                    int tail_new_row = firstAvialableRowInColumn(pair_idx[0][0]);
                    if (pair_idx[0][1] > tail_new_row)
                    {
                        std::cout << "Cannot jump!\n";
                        return { { -1,-1 }, { -1,-1 } };
                    }

                    pair_idx[0][1] = tail_new_row;
                    pair_idx[1][1] = tail[1];
                }               
            }
            else // They were in the same column
            {
                // Head is in front
                if (pair_idx[0][1] < pair_idx[1][1])
                {
                    int new_head_row = firstAvialableRowInColumn(pair_idx[1][0]);
                    if (pair_idx[0][1] > head[1] || pair_idx[1][1] > new_head_row)
                    {
                        std::cout << "Cannot jump!\n";
                        return { { -1,-1 }, { -1,-1 } };
                    }

                    pair_idx[1][1] = new_head_row;
                    pair_idx[0][1] = new_head_row - 1;
                }
                else // Tail is in front
                {
                    int tail_new_row = firstAvialableRowInColumn(pair_idx[0][0]);
                    if (pair_idx[1][1] > tail[1] || pair_idx[0][1] > tail_new_row)
                    {
                        std::cout << "Cannot jump!\n";
                        return { { -1,-1 }, { -1,-1 } };
                    }
                    pair_idx[0][1] = tail_new_row;
                    pair_idx[1][1] = tail_new_row - 1;
                }
            }

            // Is there any wall in the defined direction for the lowest index
            if (isWall(pair_idx[0]) || isWall(pair_idx[1]))
            {
                std::cout << "Ouch!! This is a wall here!\n";
                return { { -1,-1 }, { -1,-1 } };
            }

            return { pair_idx[0], pair_idx[1] };
        }

        void Board::spawnRandomNewBlock(unsigned int current_time)
        {
            if (BoardState::SPWANNING != m_board_state)
                return;

            // Find avialable spwan start location
            m_generated_index[0] = findAvialableStartIndex()[0];
            m_generated_index[1] = findAvialableStartIndex()[1];

            if (m_generated_index[0][0] == -1 ||m_generated_index[0][1] == -1 ||
                m_generated_index[1][0] == -1 || m_generated_index[1][1] == -1)
            {
                std::cout << "Cannot find a target for head. The board died!!\n";
                m_board_state = BoardState::DEAD;
                return;
            }

            // find avialable target row for head and spwan it
            int head_row = firstAvialableRowInColumn(m_generated_index[1][0]);
            m_grid[m_generated_index[1][0]][head_row].spwan( m_generated_index[1],
                                                            {m_generated_index[1][0], head_row},
                                                            current_time);
            m_generated_index[1][1] = head_row;

            // find avialable target row for tail and spwan it
            int tail_row = firstAvialableRowInColumn(m_generated_index[0][0]);
            m_grid[m_generated_index[0][0]][tail_row].spwan( m_generated_index[0],
                                                            {m_generated_index[0][0], tail_row},
                                                            current_time);
            m_generated_index[0][1] = tail_row;


            m_board_state = BoardState::ANIMATING;
        }

        void Board::filpFallingBlocks()
        {
            if (m_generated_index[0][0] == -1 || m_generated_index[0][1] == -1 ||
                m_generated_index[1][0] == -1 || m_generated_index[1][1] == -1)
            {
                std::cout << "Not flippable!\n";
                return;
            }

            if (gridAt(m_generated_index[0]).isFreeFall() ||
                gridAt(m_generated_index[1]).isFreeFall())
            {
                std::cout << "Cannot be flipped during free fall!\n";
                return;
            }

            vector2D target_pos = gridAt(m_generated_index[1]).getScreenCurrentPosition();

            // Get Head and tail current index from their current screen possition
            Index2D pair_idx[2] = { positionToIndex(gridAt(m_generated_index[0]).getScreenCurrentPosition()),
                                    positionToIndex(target_pos) };

            if (!areNeighbours(pair_idx[0], pair_idx[1]))
            {
                std::cout << "Invalid pair index!\n";
                return;
            }

            // Find tail current index
            Index2D current_state{ pair_idx[0][0] - pair_idx[1][0] ,
                                   pair_idx[0][1] - pair_idx[1][1] };

            // All tail's possible next place
            std::vector<Index2D> neighbors = { {0,-1},{1,0},{0,1},{-1,0} };

            // Find current state index
            int current_state_idx = -1;
            for (size_t i = 0; i < neighbors.size(); i++)
            {
                if (current_state == neighbors[i])
                {
                    current_state_idx = i;
                    break;
                }
            }

            // Validate the tail's current index status
            if (current_state_idx == -1)
            {
                std::cout << "Invalid board index!\n";
                return;
            }

            // Find the tail's next possible index
            int next_state_idx = (current_state_idx + 1) % neighbors.size();

            // Next index will be will be always 90 degree clockwise away
            Index2D next_index{ pair_idx[1][0] + neighbors[next_state_idx][0],
                                pair_idx[1][1] + neighbors[next_state_idx][1] };

            if (isWall(next_index))
            {
                std::cout << "Ouch! There is a wall\n";
                return;
            }

            // Check if target is physically possible to move to
            if (!gridAt(next_index).empty())
            {
                std::cout << "Neighbor is not free!\n";
                return;
            }

            // By default
            int tail_row = m_generated_index[0][1];
            int head_row = m_generated_index[1][1];

            // In case tail goes lower than head
            if (next_state_idx == 2)
            {
                tail_row = head_row;
                head_row--;
                swap(m_generated_index[1], { m_generated_index[1][1], head_row });
            }
            else
            {
                if (next_state_idx == 3)
                    head_row = tail_row;

                tail_row = firstAvialableRowInColumn(next_index[0]);
            }

            // Validate head's and tail's new row
            if (tail_row < pair_idx[0][1] || tail_row < 0 ||
                head_row < pair_idx[1][1] || head_row < 0)
            {
                std::cout << "Invalid row index!\n";
                return;
            }

            // Update Tail
            Index2D target_index_tail{ next_index[0], tail_row };
            if (isWall(target_index_tail))
            {
                std::cout << "Ouch! There is a wall\n";
                return;
            }

            swap(m_generated_index[0], target_index_tail);
            m_generated_index[0] = target_index_tail;
            // Update tail's screen positions
            m_grid[m_generated_index[0][0]][m_generated_index[0][1]].setScreenCurrentPosition(target_pos, neighbors[next_state_idx]);
            m_grid[m_generated_index[0][0]][m_generated_index[0][1]].setScreenTargetPosition(target_index_tail);

            // Update Head | Head column never changes
            if (next_state_idx == 3)
                swap({ m_generated_index[1][0], head_row }, m_generated_index[1]);

            m_generated_index[1][1] = head_row;
            // Update head's target screen positions
            m_grid[m_generated_index[1][0]][m_generated_index[1][1]].setScreenTargetPosition(m_generated_index[1]);
        }

        void Board::slide(int direction)
        {
            if (m_generated_index[0][0] == -1 || m_generated_index[0][1] == -1 ||
                m_generated_index[1][0] == -1 || m_generated_index[1][1] == -1)
            {
                std::cout << "There is nothing to slide!\n";
                return;
            }

            if (gridAt(m_generated_index[0]).isFreeFall() ||
                gridAt(m_generated_index[1]).isFreeFall())
            {
                std::cout << "Cannot slid during free fall!\n";
                return;
            }

            // Find available target indeces for the falling pair
            const auto new_target_indeces = findAvialableIndexInDirection(m_generated_index[1], m_generated_index[0], direction);

            // Validate availibility of the found target indeces
            if (new_target_indeces[0][0] == -1 || new_target_indeces[0][1] == -1 ||
                new_target_indeces[1][0] == -1 || new_target_indeces[1][1] == -1)
            {
                std::cout << "There is no place left in this direction!\n";
                return;
            }

            if (isWall(new_target_indeces[0]) || isWall(new_target_indeces[1]))
            {
                std::cout << "Ouch! There is a wall\n";
                return;
            }

            // TUDO: Change this strange strategy
            // By default head updates first
            int order = 1;
            if (new_target_indeces[0][0] != new_target_indeces[1][0])
            {
                if (direction > 0)
                {
                    order = (new_target_indeces[0][0] > new_target_indeces[1][0] ? 0 : 1);
                }
                else
                {
                    order = (new_target_indeces[0][0] > new_target_indeces[1][0] ? 1 : 0);
                }
            }

            // TUDO: Find a more performant aproach
            const auto tmp_tail = gridAt(m_generated_index[0]);
            const auto tmp_head = gridAt(m_generated_index[1]);

            // Update blocks and board with the new target indeces 
            m_grid[new_target_indeces[0][0]][new_target_indeces[0][1]] = tmp_tail;
            gridAt(m_generated_index[0]).setBlockFree();
            m_generated_index[0] = new_target_indeces[0];
            
            m_grid[new_target_indeces[1][0]][new_target_indeces[1][1]] = tmp_head;
            if (m_generated_index[1] != m_generated_index[0])
                gridAt(m_generated_index[1]).setBlockFree();           
            m_generated_index[1] = new_target_indeces[1];

            gridAt(m_generated_index[0]).slideTo(m_generated_index[0], direction);
            gridAt(m_generated_index[1]).slideTo(m_generated_index[1], direction);
        }

        void Board::applyAnimation(unsigned int current_time)
        {
            if (BoardState::ANIMATING != m_board_state)
                return;

            bool is_still_animating = false;
            // From down left 
            for (int c = 0; c < m_grid.size(); c++)
            {
                // To right up
                for (int reversed_row = m_grid[0].size() - 1; reversed_row >= 0; reversed_row--)
                {
                    m_grid[c][reversed_row].simulate({ c, reversed_row }, current_time);

                    if (Controller::BlockState::FALLING == m_grid[c][reversed_row].getBlockState())
                        is_still_animating = true;
                }
            }

            // Update animating state
            if (!is_still_animating) 
                m_board_state =  BoardState::MATCH_CHECKING;
        }

        int Board::update(unsigned int current_time)
        {
            // Spwan a new pair
            spawnRandomNewBlock(current_time);
            // Apply animation for spwaned pair
            applyAnimation(current_time);
            // Check if there are matches to remove
            int total_matches = removeMatches();
            // Perepare blocks for next matching wave or Spwan
            applyGravity();

            return total_matches;
        }
    }
}