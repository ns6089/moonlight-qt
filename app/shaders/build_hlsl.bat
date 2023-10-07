fxc /T vs_4_0_level_9_3 /Fo d3d11_vertex.fxc d3d11_vertex.hlsl

fxc /T ps_4_0_level_9_3 /Fo d3d11_overlay_pixel.fxc d3d11_overlay_pixel.hlsl
fxc /T ps_4_0_level_9_3 /Fo d3d11_genyuv_pixel.fxc d3d11_genyuv_pixel.hlsl
fxc /T ps_4_0_level_9_3 /Fo d3d11_bt601lim_pixel.fxc d3d11_bt601lim_pixel.hlsl
fxc /T ps_4_0_level_9_3 /Fo d3d11_bt2020lim_pixel.fxc d3d11_bt2020lim_pixel.hlsl

fxc /D PLANAR_YUV444 /T vs_4_0_level_9_3 /Fo d3d11_vertex_yuv444.fxc d3d11_vertex.hlsl
fxc /D PLANAR_YUV444 /T ps_4_0_level_9_3 /Fo d3d11_genyuv_pixel_yuv444.fxc d3d11_genyuv_pixel.hlsl
fxc /D PLANAR_YUV444 /T ps_4_0_level_9_3 /Fo d3d11_bt601lim_pixel_yuv444.fxc d3d11_bt601lim_pixel.hlsl
fxc /D PLANAR_YUV444 /T ps_4_0_level_9_3 /Fo d3d11_bt2020lim_pixel_yuv444.fxc d3d11_bt2020lim_pixel.hlsl
