#pragma once

//! This class allows you to draw a torus

class CTorus
{
public:
    CTorus(void);
    virtual ~CTorus(void);

    //! Set the large radius of the torus
    /*! This is the radius of the entire torus. The
        default is 5cm.
        \param r Radius in cm */
    void SetR1(double r) {m_r1 = r;}

    //! Gets the large radus of the torus
    double GetR1() const {return m_r1;}

    //! Set the small radius of the torus
    /*! This is the radius of a cross section of the torus.
        The default is 1cm.
        \param r Radius in cm */
    void SetR2(double r) {m_r2 = r;}
    double GetR2() const {return m_r2;}

    //! Set the number of steps around the large radius.
    /*! This is the number of tessellation steps around the 
        large radius of the torus. The default is 50.
        \param s Number of steps */
    void SetSteps1(double s) {m_steps1 = s;}

    //! Get the number of steps around the large radius.
    double GetSteps1() const {return m_steps1;}

    //! Set the number of steps around the small radius.
    /*! This is the number of tessellation steps around the 
        small radius (cross section) of the torus. The default is 20.
        \param s Number of steps */
    void SetSteps2(double s) {m_steps2 = s;}

    //! Get the number of steps around the small radius.
    double GetSteps2() const {return m_steps2;}

    //! Draw the torus
    void Draw();

private:
    void TorusVertex(double a1, double r1, double a2, double r2, 
                         double *v, double *n);
                         
    //! Large radius of the torus
    //! This is the radius of the entire torus
    double  m_r1;

    //! Small radius of the torus
    //! This is the radius of a cross section of the torus
    double  m_r2;

    //! Number of steps in the large radius
    double  m_steps1;

    //! Number of steps in the small radius
    double  m_steps2;
};

