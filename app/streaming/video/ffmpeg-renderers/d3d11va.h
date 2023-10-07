#pragma once

#include "renderer.h"
#include "pacer/pacer.h"

#include <d3d11_1.h>
#include <dxgi1_5.h>

extern "C" {
#include <libavutil/hwcontext_d3d11va.h>
}

#include "cuda.h"

class D3D11VARenderer : public IFFmpegRenderer
{
public:
    D3D11VARenderer(int decoderSelectionPass, bool useCudaDecoder = false);
    virtual ~D3D11VARenderer() override;
    virtual bool initialize(PDECODER_PARAMETERS params) override;
    virtual bool prepareDecoderContext(AVCodecContext* context, AVDictionary**) override;
    virtual bool prepareDecoderContextInGetFormat(AVCodecContext* context, AVPixelFormat pixelFormat) override;
    virtual void renderFrame(AVFrame* frame) override;
    virtual void notifyOverlayUpdated(Overlay::OverlayType) override;
    virtual void setHdrMode(bool enabled) override;
    virtual int getRendererAttributes() override;
    virtual int getDecoderCapabilities() override;
    virtual bool needsTestFrame() override;

private:
    static void lockContext(void* lock_ctx);
    static void unlockContext(void* lock_ctx);

    bool setupRenderingResources();
    bool setupVideoTexture();
    void renderOverlay(Overlay::OverlayType type);
    void bindColorConversion(AVFrame* frame);
    void renderVideo(AVFrame* frame);
    bool checkDecoderSupport(IDXGIAdapter* adapter);
    bool createDeviceByAdapterIndex(int adapterIndex, bool* adapterNotFound = nullptr);

    int m_DecoderSelectionPass;

    IDXGIFactory5* m_Factory;
    ID3D11Device* m_Device;
    IDXGIAdapter1* m_Adapter;
    DXGI_ADAPTER_DESC1 m_AdapterDesc;
    IDXGISwapChain4* m_SwapChain;
    ID3D11DeviceContext* m_DeviceContext;
    ID3D11RenderTargetView* m_RenderTargetView;
    SDL_mutex* m_ContextLock;

    DECODER_PARAMETERS m_DecoderParams;
    int m_TextureAlignment;
    int m_DisplayWidth;
    int m_DisplayHeight;
    int m_LastColorSpace;
    bool m_LastFullRange;

    bool m_AllowTearing;

    ID3D11VertexShader* m_VideoVertexShader;
    ID3D11VertexShader* m_VideoVertexShaderYUV444;
    ID3D11PixelShader* m_VideoGenericPixelShader;
    ID3D11PixelShader* m_VideoGenericPixelShaderYUV444;
    ID3D11PixelShader* m_VideoBt601LimPixelShader;
    ID3D11PixelShader* m_VideoBt601LimPixelShaderYUV444;
    ID3D11PixelShader* m_VideoBt2020LimPixelShader;
    ID3D11PixelShader* m_VideoBt2020LimPixelShaderYUV444;
    ID3D11Buffer* m_VideoVertexBuffer;

    ID3D11Texture2D* m_VideoTexture;
    ID3D11ShaderResourceView* m_VideoTextureResourceViews[2];

    SDL_SpinLock m_OverlayLock;
    ID3D11Buffer* m_OverlayVertexBuffers[Overlay::OverlayMax];
    ID3D11Texture2D* m_OverlayTextures[Overlay::OverlayMax];
    ID3D11ShaderResourceView* m_OverlayTextureResourceViews[Overlay::OverlayMax];
    ID3D11PixelShader* m_OverlayPixelShader;

    AVBufferRef* m_HwDeviceContext;
    AVBufferRef* m_HwFramesContext;
    
    CUDADecoderD3D11Interop* m_CUDADecoderD3D11Interop;
};

