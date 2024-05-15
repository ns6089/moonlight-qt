#pragma once

#include "renderer.h"
#include "swframemapper.h"

#if defined HAVE_CUDA && !defined _WIN32
#define USE_SDL_CUDA_INTEROP;
#include "cuda.h"
#endif

class SdlRenderer : public IFFmpegRenderer {
public:
    SdlRenderer();
    virtual ~SdlRenderer() override;
    virtual bool initialize(PDECODER_PARAMETERS params) override;
    virtual bool prepareDecoderContext(AVCodecContext* context, AVDictionary** options) override;
    virtual void renderFrame(AVFrame* frame) override;
    virtual bool isRenderThreadSupported() override;
    virtual bool isPixelFormatSupported(int videoFormat, enum AVPixelFormat pixelFormat) override;
    virtual bool testRenderFrame(AVFrame* frame) override;

private:
    void renderOverlay(Overlay::OverlayType type);

    int m_VideoFormat;
    SDL_Renderer* m_Renderer;
    SDL_Texture* m_Texture;
    int m_ColorSpace;
    SDL_Texture* m_OverlayTextures[Overlay::OverlayMax];
    SDL_Rect m_OverlayRects[Overlay::OverlayMax];

    SwFrameMapper m_SwFrameMapper;

#ifdef USE_SDL_CUDA_INTEROP
    CUDAGLInteropHelper* m_CudaGLHelper;
#endif
};

