#ifndef _BOARD_HPP_
#define _BOARD_HPP_

#include <memory>
#include <vector>

#include "Entity.hpp"
#include "CommonConstants.hpp"
#include "Block.hpp"
#include "Utils.hpp"

using namespace MatchThree::Controller;

namespace MatchThree {
    namespace Model
    {
        using grid  = std::vector<std::vector<Block>>;
        using holes = std::vector<Index2D>;
        class Board 
        {
        private:
            grid        m_grid;

            BoardState  m_board_state;
            Index2D     m_generated_index[2];
            Index2D     m_start_Point;

            holes       black_holes;
            void        resetInPair();

            Block&          gridAt(Index2D index);
            const Block&    gridAt(Index2D index) const;

            // TUDO: Move into Matchfinder script and call it from game logic
            bool isSafe(const grid& grid,
                        int c, int r,
                        const std::vector<std::vector<bool>>& processed,
                        const int block_id);

            void Board::BFS(const grid& grid,
                            std::vector<std::vector<bool>>& processed,
                            std::vector<Index2D>& out,
                            Index2D index,
                            const int block_id);

            void findMatches(const grid& grid, Index2D start_index, std::vector<Index2D>& out);
            //===================================================================

            // Find a free grid from the middle column
            std::vector<Index2D> findAvialableIndexInDirection(Index2D head, Index2D tail, int direction) const;
            // Generate a new random block
            void    spawnRandomNewBlock(unsigned int current_time);
            // Update the grid's block current location 
            void    applyAnimation(unsigned int current_time);
            void    applyGravity();
        public:
            Board();
            ~Board();

            // TUDO: Move to GameLogic script
            int    removeMatches();

            int      firstAvialableRowInColumn(int column) const;

            BoardState  getBoardState() const;
            grid        getGrids() const;

            bool    isBoardDead() const;
            bool    areNeighbours(Index2D index1, Index2D index2) const;
            bool    isWall(Index2D index)const;
            void    setBoardState(BoardState new_board_state);

            std::vector<Index2D> findAvialableStartIndex() const;
            // Place newly generated falling block instantly
            void    freeFall();
            // Swap two blocks
            void    swap(Index2D v1, Index2D v2);
            // Swap generated and falling blocks
            void    filpFallingBlocks();
            // Slide left/right after spwan while falling
            void    slide(int direction);
            // update board and prepare it for rendering fase
            int    update(unsigned int current_time);
        };
    } 
}
#endif