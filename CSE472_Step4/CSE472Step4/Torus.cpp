#include "stdafx.h"
#include "Torus.h"

#include <cmath>
#include "gl/gl.h"

const double GR_PI = 3.1415926535897932384626433832795;


CTorus::CTorus(void)
{
    //
    // Default values
    //

    // Large radius of the torus
    // This is the radius of the entire torus
    m_r1 = 5;

    // Small radius of the torus
    // This is the radius of a cross section of the torus
    m_r2 = 1;

    // Number of steps in the large radius
    m_steps1 = 50;

    // Number of steps in the small radius
    m_steps2 = 20;
}


CTorus::~CTorus(void)
{
}


//
// Name :         CChildView::Draw()
// Description :  Render a Torus of large radius m_r1 and small radius m_r2
//                m_steps1 is the number of steps we do for the large
//                radius and m_steps2 is the number of steps for the
//                small radius.  There will be m_steps1 * m_steps2 
//                quadrilaterals in the torus.
//
void CTorus::Draw()
{
    // How large are the angular steps in radians
    const double step1r = 2. * GR_PI / m_steps1;
    const double step2r = 2. * GR_PI / m_steps2;

    // We build the torus in slices that go from a1a to a1b
    double a1a = 0;
    double a1b = step1r;

    for(int s=0;  s<m_steps1;  s++, a1a = a1b, a1b += step1r)
    {
        // We build a slice out of quadrilaterals that range from 
        // angles a2a to a2b.

        double a2a = 0;
        double a2b = step2r;

        for(int s2=0;  s2<m_steps2;  s2++, a2a = a2b,  a2b += step2r)
        {
            // We need to know the corners
            double n[3], v[3];

            glBegin(GL_QUADS);

			TorusVertex(a1a, m_r1, a2a, m_r2, v, n);
			glNormal3dv(n);
			glTexCoord2d(a1a / (2 * GR_PI) * 10, a2a / (2 * GR_PI) * 2);
			glVertex3dv(v);

			TorusVertex(a1b, m_r1, a2a, m_r2, v, n);
			glNormal3dv(n);
			glTexCoord2d(a1b / (2 * GR_PI) * 10, a2a / (2 * GR_PI) * 2);
			glVertex3dv(v);

			TorusVertex(a1b, m_r1, a2b, m_r2, v, n);
			glNormal3dv(n);
			glTexCoord2d(a1b / (2 * GR_PI) * 10, a2b / (2 * GR_PI) * 2);
			glVertex3dv(v);

			TorusVertex(a1a, m_r1, a2b, m_r2, v, n);
			glNormal3dv(n);
			glTexCoord2d(a1a / (2 * GR_PI) * 10, a2b / (2 * GR_PI) * 2);
			glVertex3dv(v);

			glEnd();
		}
    }
}



//
// Name :         CTorus::TorusVertex()
// Description :  Compute the x,y,z coordinates and surface normal for a 
//                torus vertex.  
// Parameters :   a1 - The angle relative to the center of the torus
//                r1 - Radius of the entire torus
//                a2 - The angle relative to the center of the torus slice
//                r2 - Radius of the torus ring cross-section
//                v - Returns vertex
//                n - Returns surface normal
//

void CTorus::TorusVertex(double a1, double r1, double a2, double r2, 
                         double *v, double *n)
{
    // Some sines and cosines we'll need.
    double ca1 = cos(a1);
    double sa1 = sin(a1);
    double ca2 = cos(a2);
    double sa2 = sin(a2);

    // What is the center of the slice we are on.
    double centerx = r1 * ca1;
    double centerz = -r1 * sa1;    // Note, y is zero

    // Compute the surface normal
    n[0] = ca2 * ca1;          // x
    n[1] = sa2;                // y
    n[2] = -ca2 * sa1;         // z

    // And the vertex
    v[0] = centerx + r2 * n[0];
    v[1] = r2 * n[1];
    v[2] = centerz + r2 * n[2];
}
