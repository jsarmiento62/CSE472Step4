//Cubemap wrapper based on CB Owen's texture code.
//Version 1.0 by Y Tong
#pragma once
#include "GrTexture.h"


class LibGrafx CGrCubeTexture  
{
public:
    //! Default Constructor
    CGrCubeTexture();
    virtual ~CGrCubeTexture();

	bool LoadFiles(LPCTSTR filename0, LPCTSTR filename1, LPCTSTR filename2,
		LPCTSTR filename3, LPCTSTR filename4, LPCTSTR filename5);

    GLuint TexName();
    void Clear();


    //! Direct access to the texture image bits. 
    /*! This function can be used to directly access the 
        data for the texture image. It consists of a sequence of 
        bytes in the order B, G, and R. Rows start on the texture
        word (32 bit) boundary. */
    BYTE *ImageBits() const;

private:
    // Set true if the texture map has been initialized as a normal texture
    bool m_initialized;

    // OpenGL name for the normal texture
    GLuint m_texname;

	CGrTexture m_textures[6];
};

