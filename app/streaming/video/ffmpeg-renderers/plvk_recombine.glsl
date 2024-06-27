//!DESC extract U from Y
//!HOOK LUMA
//!BIND LUMA
//!SAVE EXTRACTED_U
//!HEIGHT LUMA.h 2 /
vec4 hook()
{
    return LUMA_tex(vec2(LUMA_pos.x, 0.5 + LUMA_pos.y / 2));
}

//!DESC crop Y
//!HOOK LUMA
//!BIND LUMA
//!HEIGHT LUMA.h 2 /
vec4 hook()
{
    return LUMA_tex(vec2(LUMA_pos.x, LUMA_pos.y / 2));
}

//!DESC recombine UV
//!HOOK CHROMA
//!BIND CHROMA
//!BIND EXTRACTED_U
//!WIDTH CHROMA.w 2 *
//!COMPUTE 32 32
//!OFFSET ALIGN
void hook()
{
    //     Y       U     V
    // +-------+ +---+ +---+
    // |       | |V0 | |V1 |
    // |   Y   | +---+ +---+
    // |       | |V2 | |V3 |
    // +-------+ +---+ +---+
    // |       |
    // |   U   |
    // |       |
    // +-------+

    ivec2 id = ivec2(gl_GlobalInvocationID);
    ivec2 pos = id / 2;
    ivec2 rem = id - 2 * pos;

    float v = 0;

    uint branch = rem.x + 2 * rem.y;
    if (branch == 0) {
        v = texelFetch(CHROMA_raw, pos, 0).x;
    } else if (branch == 1) {
        v = texelFetch(CHROMA_raw, pos, 0).y;
    } else if (branch == 2) {
        v = texelFetch(CHROMA_raw, ivec2(pos.x, pos.y + input_size.y / 4), 0).x;
    } else {
        v = texelFetch(CHROMA_raw, ivec2(pos.x, pos.y + input_size.y / 4), 0).y;
    }

    float u = texelFetch(EXTRACTED_U_raw, ivec2(gl_GlobalInvocationID), 0).x;

    imageStore(out_image, id, vec4(u, v, 0, 1));
}
