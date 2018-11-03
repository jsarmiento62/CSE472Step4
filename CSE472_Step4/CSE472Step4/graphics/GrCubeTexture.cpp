//
//  Name :         GrTexture.cpp
//  Description :  Implementation of the CGrTexture class.
//                 This class allows for the simple manipulation of BITMAP images.
//  Version :      See Texture.h
//

#include <stdafx.h>
#include "..\ShaderWnd\ShaderWnd.h"
#include "GrCubeTexture.h"
#include <cassert>
#include <vector>

using namespace std;

#include <GL/glu.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrCubeTexture::CGrCubeTexture()
{
   m_texname = 0;

   m_initialized = false;
}

CGrCubeTexture::~CGrCubeTexture()
{
    Clear();
}


void CGrCubeTexture::Clear()
{
    if(m_initialized)
    {
        glDeleteTextures(1, &m_texname);
        m_initialized = false;
    }
}


GLuint CGrCubeTexture::TexName()
{
    if(m_initialized)
        return m_texname;

	glGenTextures(1, &m_texname);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texname);
	for (GLuint i = 0; i < 6; i++)
	{
		image = m_textures[i].ImageBits();
		width = m_textures[i].Width();
		height = m_textures[i].Height();
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGBA, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image
			);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    m_initialized = true;

    return m_texname;
}


bool CGrCubeTexture::LoadFiles(LPCTSTR filename0, LPCTSTR filename1, LPCTSTR filename2, LPCTSTR filename3, LPCTSTR filename4, LPCTSTR filename5)
{
	vector<LPCTSTR> files;
	files.push_back(filename0);
	files.push_back(filename1);
	files.push_back(filename2);
	files.push_back(filename3);
	files.push_back(filename4);
	files.push_back(filename5);
	Clear();

	for (int i = 0; i < 6; i++) {
		if (m_textures[i].LoadFile(files[i]) == false)
			return false;
	}

	return true;
}
