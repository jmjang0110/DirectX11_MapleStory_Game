#ifndef _FUNC
#define _FUNC



int IsBind(in Texture2D _tex)
{
    uint width = 0;
    uint height = 0;
    _tex.GetDimensions(width, height); // 가로 세로 해상도를 받는다. 
    
    if (0 != width || 0 != height)
        return 1;
   
    return 0;
}


#endif