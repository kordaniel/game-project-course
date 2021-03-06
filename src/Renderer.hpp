#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL.h>

#include "Camera.hpp"
#include "Color.hpp"
#include "Geometry.hpp"
#include "Timetools.hpp"
#include "Window.hpp"


class Renderer
{
public:
    enum class BlendMode {
        NONE  = SDL_BLENDMODE_NONE,  // dstRGBA = srcRGBA
        BLEND = SDL_BLENDMODE_BLEND, // "Alpha blending" (default)
        ADD   = SDL_BLENDMODE_ADD,   // "Additive blending"
        MOD   = SDL_BLENDMODE_MOD    // "Color modulate"
    };

    Renderer(Window& window, bool vsync = true, bool accelerated = true);
    Renderer(const Renderer& other) = delete;
    Renderer(Renderer&& other)      = delete;
    ~Renderer(void);

    uint32_t      GetFlags(void)          const;
    Dimensions2D  GetLogicalSize(void)    const;
    Dimensions2D  GetOutputSize(void)     const;
    Dimensions2D  GetMaxTextureSize(void) const;
    bool          GetIsVsyncced(void)     const;
    SDL_Renderer* GetSdlRenderer(void)    const;
    Rectangle     GetViewport(void)       const;

    void          ToggleFullscreen(void)  const;

    /// Swap framebuffers, "draw".
    /// @param: doRenderClear Has the default value true. If this is true, then
    /// initialize the backbuffer for the next frame after swapping
    void RenderPresent(bool doRenderClear = true) const;
    
    /// Clear the entire screen to our selected color.
    void RenderClear(void)                     const;
    void SetRenderDrawColor(Color c)           const;
    void SetLogicalSize(Dimensions2D size)     const;
    void SetDrawBlendMode(BlendMode blendMode) const;
    void SetVsync(bool enabled)                const;
    void SetViewport(const SDL_Rect* viewPort = nullptr) const;

    /// Set the target to render to.
    /// @param texture the target to render to. Default arg is nullptr which sets the target to the screen.
    void SetRenderTarget(SDL_Texture* texture = nullptr) const;

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

    void DrawCircle(const Point2D posCentre, int radius)       const;
    void DrawCircleFilled(const Point2D posCentre, int radius) const;

    /// Draw a line on the current rendering target.
    /// @param point1 The X,Y coords of the start point.
    /// @param point2 The X,Y coords of the end point.
    void DrawLine(Point2D point1, Point2D point2) const;

    void DrawRectangle(Rectangle rectangle)  const;
    void DrawRectangle(Rectangle* rectangle) const;
    void FillRectangle(Rectangle rectangle)  const;
    void FillRectangle(Rectangle* rectangle) const;

    void DrawFilledRectangle(Point2D posCentre, Dimensions2D size) const;

private:
    Uint32 getFlags(void) const;
    static Uint32 combineRendererFlags(bool renderSoftware, bool renderAccelerated,
                                       bool presentVsync, bool renderTargetTexture);

private:
    Window&       _targetWindow;
    SDL_Renderer* _renderer;

};


// TODO: Delete this interface(?)
class DrawableObject
{
public:
    DrawableObject(void) = default;
    DrawableObject(const DrawableObject& other) = delete;
    DrawableObject(DrawableObject&& other)      = delete;
    virtual ~DrawableObject(void) = default;

    virtual void Draw(const Renderer& renderer, const Camera& camera, Timestep it) const = 0;
};

#endif // RENDERER_HPP
