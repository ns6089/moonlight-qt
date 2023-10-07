struct ShaderInput
{
    float2 pos : POSITION;
    float2 tex : TEXCOORD0;
};

struct ShaderOutput
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

ShaderOutput main(ShaderInput input) 
{
    ShaderOutput output;
    output.pos = float4(input.pos, 0.0, 1.0);
#ifdef PLANAR_YUV444
    output.tex_y = float2(input.tex.x, input.tex.y / 3.);
    output.tex_u = float2(input.tex.x, 1 / 3. + input.tex.y / 3.);
    output.tex_v = float2(input.tex.x, 2 / 3. + input.tex.y / 3.);
#else
    output.tex = input.tex;
#endif
    return output;
}
