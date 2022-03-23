#include "Renderer.hpp"
#include "CommonConstants.hpp"
#include "RendererExceptionHandler.hpp"
#include <sstream>

namespace MatchThree 
{
    namespace View
    {
        void Renderer::validateTexture(unsigned int t_id) const
        {
            if (t_id >= sizeof(Controller::block_tid_to_path) || t_id < 0)
            {
                std::ostringstream errorStream;
                errorStream << "Invalid texture id: " << t_id;
                throw new RendererExceptionHandler(errorStream.str());
            }
        }
        
        Renderer::Renderer(): m_window(nullptr), m_renderer(nullptr)
        {
            std::ostringstream errorStream;
            if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
            {
                errorStream << "Error: SDL_INIT_EVERYTHING. detail: "  << SDL_GetError();
                throw new RendererExceptionHandler(errorStream.str());
            }

            bool is_windowed = false;
            m_window = SDL_CreateWindow("Match4Tetris",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        Controller::WINDOW_WIDTH,
                                        Controller::WINDOW_HEIGHT,
                                        is_windowed? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN
                                        );
            if(!m_window)
            {
                errorStream << "Error: SDL_CreateWindow. detail: " << SDL_GetError();
                throw new RendererExceptionHandler(errorStream.str());
            }

            m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(!m_renderer)
            {
                errorStream << "Error: SDL_CreateRenderer detail:  " << SDL_GetError();
                throw new RendererExceptionHandler(errorStream.str());
            }

            for(const auto& block : Controller::block_tid_to_path)
            {
                SDL_Texture *tex = IMG_LoadTexture(m_renderer, block.second.c_str());

                if(!tex)
                {
                    errorStream << "ERROR: IMG_LoadTexture detail: " << SDL_GetError();
                    throw new RendererExceptionHandler(errorStream.str());
                }
                m_textures.push_back(tex);
            }

            if(TTF_Init() != 0)
            {
                errorStream << "ERROR: TTF init. detail:";
                throw new RendererExceptionHandler(errorStream.str());
            }

            m_font = TTF_OpenFont(Controller::DEFAULT_FONT, Controller::DEFAULT_FONT_SIZE);
            if (!m_font)
            {
                errorStream << "ERROR: Font loading detail: ";
                throw new RendererExceptionHandler(errorStream.str());
            }
        }

        Renderer::~Renderer()
        {
            TTF_CloseFont(m_font);

            for (auto texture : m_textures)
                SDL_DestroyTexture(texture);

            m_textures.clear();

            if(m_renderer)
                SDL_DestroyRenderer(m_renderer);

            if(m_window)
                SDL_DestroyWindow(m_window);

            SDL_Quit();            
        }

        void Renderer::clear()
        {
            SDL_RenderClear(m_renderer);
        }

        void Renderer::drawBackground(unsigned int texture_id)
        {
            validateTexture(texture_id);
            SDL_RenderCopy(m_renderer, m_textures[texture_id], nullptr, nullptr);
        }

        void Renderer::drawFilledRectangle(int x, int y, int w, int h)
        {
            SDL_Rect rect = { x, y, w, h};
            SDL_RenderFillRect(m_renderer, &rect);
        }
        
        void Renderer::drawText(const char* text, int x, int y, SDL_Color color)
        {
            SDL_Surface* surface = TTF_RenderText_Blended(m_font, text, color);
            if (!surface)
                return;

            SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);

            SDL_FreeSurface(surface);

            if (!texture)
                return;

            SDL_Rect rect = { x, y };
            SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
            SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
            SDL_DestroyTexture(texture);
        }

        void Renderer::drawTexture(unsigned int texture_id, int x, int y)
        {
            validateTexture(texture_id);
            SDL_Rect rect = { x, y};
            // Set the SDL rectangle width  and height with the texture width and height
            SDL_QueryTexture(m_textures[texture_id], nullptr, nullptr, &rect.w, &rect.h);
            SDL_RenderCopy(m_renderer, m_textures[texture_id], nullptr, &rect);
        }
       
        void Renderer::drawTextureCentered(unsigned int texture_id, int x, int y, int w, int h, double scale)
        {
            validateTexture(texture_id);
            int texture_width, texture_height;
            // Set the SDL rectangle width  and height with the texture width and height
            SDL_QueryTexture(m_textures[texture_id], nullptr, nullptr, &texture_width, &texture_height);

            SDL_Rect rect;
            rect.x = x + static_cast<int>((w - texture_width)/2);
            rect.y = y + static_cast<int>((h - texture_height)/2);
            rect.w = static_cast<int>( texture_width * scale);
            rect.h = static_cast<int>( texture_height * scale);

            SDL_RenderCopy(m_renderer, m_textures[texture_id], nullptr, &rect);
        }

        void Renderer::drawButton(std::string btn_text, int x, int y, int w, int h)
        {
            setColor(255, 255, 255, 255);
            drawFilledRectangle(x,y, w, h);
            drawText( btn_text.c_str(),
                      x + static_cast<int>(w / 6),
                      y + static_cast<int>(h / 6),
                      {0, 0, 150, 255});
        }

        void Renderer::present()
        {
            SDL_RenderPresent(m_renderer);
        }

        void Renderer::resetClipRect()
        {
            SDL_RenderSetClipRect(m_renderer, nullptr);
        }
    
        void Renderer::setClipRect(int x, int y, int w, int h)
        {
            SDL_Rect rect = { x, y, w, h};
            SDL_RenderSetClipRect(m_renderer, &rect);
        }
       
        void Renderer::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        {
            SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        }
    } 
}