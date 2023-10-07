#ifdef PLANAR_YUV444
Texture2D<min16float> yuv444Texture : register(t0);
#else
Texture2D<min16float> luminancePlane : register(t0);
Texture2D<min16float2> chrominancePlane : register(t1);
#endif
SamplerState theSampler : register(s0);

struct ShaderInput
{
    float4 pos : SV_POSITION;
#ifdef PLANAR_YUV444
    float2 tex_y : TEXCOORD0;
    float2 tex_u : TEXCOORD1;
    float2 tex_v : TEXCOORD2;
#else
    float2 tex : TEXCOORD0;
#endif
};
