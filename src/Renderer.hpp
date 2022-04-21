#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL.h>

#include "Color.hpp"
#include "Geometry.hpp"


class Renderer
{
public:
    enum class BlendMode {
        NONE  = SDL_BLENDMODE_NONE,  // dstRGBA = srcRGBA
        BLEND = SDL_BLENDMODE_BLEND, // "Alpha blending" (default)
        ADD   = SDL_BLENDMODE_ADD,   // "Additive blending"
        MOD   = SDL_BLENDMODE_MOD    // "Color modulate"
    };

    Renderer(SDL_Window* window, bool vsync = true, bool accelerated = true);
    Renderer(const Renderer& other) = delete;
    Renderer(Renderer&& other)      = delete;
    ~Renderer(void);

    uint32_t      GetFlags(void)       const;
    Dimensions2D  GetLogicalSize(void) const;
    Dimensions2D  GetOutputSize(void)  const;
    bool          GetIsVsyncced(void)  const;
    SDL_Renderer* GetSdlRenderer(void) const;

    /// Swap framebuffers, "draw".
    /// @param: doRenderClear Has the default value true. If this is true, then
    /// initialize the backbuffer for the next frame after swapping
    void RenderPresent(bool doRenderClear = true) const;
    
    /// Clear the entire screen to our selected color.
    void RenderClear(void)                        const;

    void SetRenderDrawColor(Color c)              const;
    void SetLogicalSize(Dimensions2D size)        const;
    void SetDrawBlendMode(BlendMode blendMode)    const;

    /// Copy a portion of the texture to the current rendering target.
    /// @param texture The source texture to copy to the rendering target.
    /// @param srcrect The rectangle defining the portion to copy. nullptr = copy whole texture.
    /// @param dstrect The rectangle defining the destination area to copy to. nullptr = stretch texture to fill the whole target.
    void RenderCopy(SDL_Texture* texture, const SDL_Rect* srcrect = nullptr,
                    const SDL_Rect* dstrect = nullptr) const;

    /// Copy a portion of the texture to the current rendering target, with optional rotation and flipping.
    /// NOTE: This method is untested.
    /// @param texture The source texture to copy to the rendering target.
    /// @param srcrect The rectangle defining the portion to copy. nullptr = copy whole texture.
    /// @param dstrect The rectangle defining the destination area to copy to. nullptr = stretch texture to fill the whole target.
    /// @param angle The angle in _degrees_ that indicates the rotation in clockwise direction to apply to the dstrect.
    /// @param center The point around which dstrect will be rotated. nullptr = rotation will be done around the center point of dstrect.
    /// @param flip Flipping actions to perform on the texture, ORed together.
    void RenderCopyEx(SDL_Texture* texture, const SDL_Rect* srcrect,
                      const SDL_Rect* dstrect, const double angle,
                      const SDL_Point* center, const SDL_RendererFlip flip) const;

    /// Draw a point on the current rendering target.
    void DrawPoint(Point2D position) const;

    /// Draw a line on the current rendering target.
    /// @param point1 The X,Y coords of the start point.
    /// @param point2 The X,Y coords of the end point.
    void DrawLine(Point2D point1, Point2D point2) const;

    void DrawRectangle(Rectangle rectangle)  const;
    void DrawRectangle(Rectangle* rectangle) const;
    void FillRectangle(Rectangle rectangle)  const;
    void FillRectangle(Rectangle* rectangle) const;

private:
    Uint32 getFlags(void) const;
    static Uint32 combineRendererFlags(bool renderSoftware, bool renderAccelerated,
                                       bool presentVsync, bool renderTargetTexture);

private:
    SDL_Renderer* _renderer;

};

#endif // RENDERER_HPP