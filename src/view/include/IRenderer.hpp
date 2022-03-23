#ifndef I_RENDERER_HPP_
#define I_RENDERER_HPP_

namespace MatchThree
{
	namespace View
	{
		class IRenderer
		{
		public:
			virtual ~IRenderer() {}
            virtual void clear() = 0;
            virtual void drawBackground(unsigned int texture_id) = 0;
            virtual void drawFilledRectangle(int x, int y, int w, int h) = 0;
            virtual void drawText(const char* text, int x, int y, SDL_Color color = { 255, 255, 255, 225 }) = 0;
            virtual void drawTexture(unsigned int texture_id, int x, int y) = 0;
            virtual void drawTextureCentered(unsigned int texture_id, int x, int y, int w, int h, double scale = 1.0) = 0;
            virtual void drawButton(std::string btn_text, int x, int y, int w, int h) = 0;
            virtual void present() = 0;
            virtual void resetClipRect() = 0;
            virtual void setClipRect(int x, int y, int w, int h) = 0;
            virtual void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;
		};
	}
}
#endif // !I_RENDERER_HPP_