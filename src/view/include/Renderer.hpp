#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Entity.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <memory>
#include <vector>
#include "IRenderer.hpp"

namespace MatchThree 
{
    namespace View
    {       
        using std::shared_ptr;

        class Renderer : public IRenderer
        {
        private:
            SDL_Window*                 m_window;
            SDL_Renderer*               m_renderer;
            TTF_Font*                   m_font;
            std::vector<SDL_Texture*>	m_textures;
            void validateTexture(unsigned int t_id) const;
         public:
             Renderer();
             ~Renderer();

             virtual void clear() override;
             virtual void drawBackground(unsigned int texture_id);
             virtual void drawFilledRectangle(int x, int y, int w, int h) override;
             virtual void drawText(const char *text, int x, int y, SDL_Color color={ 255, 255, 255, 225 }) override;
             virtual void drawTexture(unsigned int texture_id, int x, int y) override;
             virtual void drawTextureCentered(unsigned int texture_id, int x, int y, int w, int h, double scale = 1.0) override;
             virtual void drawButton(std::string btn_text, int x, int y, int w, int h) override;
             virtual void present() override;
             virtual void resetClipRect() override;
	         virtual void setClipRect(int x, int y, int w, int h)  override;
             virtual void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) override;
        };
    } 
}
#endif