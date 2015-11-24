/****************************************************************************/
/**
 *  @file Material.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Material.h 1103 2012-04-21 05:32:51Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#pragma once

#if KVS_ENABLE_DEPRECATED

#include <kvs/OpenGL>
#include <kvs/ClassName>
#include <kvs/Vector4>


namespace kvs
{

class RGBAColor;

/*==========================================================================*/
/**
 *  Material class.
 */
/*==========================================================================*/
class Material
{
    kvsClassName( kvs::Material );

public:

    enum MaterialFace
    {
        Front        = GL_FRONT,
        Back         = GL_BACK,
        FrontAndBack = GL_FRONT_AND_BACK
    };

protected:

    float        m_ambient[4];  ///< ambient color
    float        m_diffuse[4];  ///< diffuse color
    float        m_specular[4]; ///< specular color
    float        m_shininess;   ///< shininess [0,128]
    MaterialFace m_face;        ///< material face

public:

    Material( void );

    Material(
        const kvs::RGBAColor& ambient,
        const kvs::RGBAColor& diffuse,
        const kvs::RGBAColor& specular,
        const float shininess,
        const MaterialFace face = FrontAndBack );

    Material(
        const float ambient[4],
        const float diffuse[4],
        const float specular[4],
        const float shininess,
        const MaterialFace face = FrontAndBack );

    Material(
        const float ambi_r, const float ambi_g, const float ambi_b, const float ambi_a,
        const float diff_r, const float diff_g, const float diff_b, const float diff_a,
        const float spec_r, const float spec_g, const float spec_b, const float spec_a,
        const float shininess,
        const MaterialFace face = FrontAndBack );

    Material( const Material& material );

    virtual ~Material( void );

public:

    void initialize( void );

    void clear( void );

public:

    Material& operator = ( const Material& material );

public:

    const kvs::Vector4f ambient( void ) const;

    const kvs::Vector4f diffuse( void ) const;

    const kvs::Vector4f specular( void ) const;

    const float shininess( void ) const;

public:

    void setFace( const MaterialFace face );

    void setAmbient( const float r, const float g, const float b, const float a );

    void setAmbient( const float ambient[4] );

    void setAmbient( const kvs::RGBAColor& ambient );

    void setDiffuse( const float r, const float g, const float b, const float a );

    void setDiffuse( const float diffuse[4] );

    void setDiffuse( const kvs::RGBAColor& diffuse );

    void setSpecular( const float r, const float g, const float b, const float a );

    void setSpecular( const float specular[4] );

    void setSpecular( const kvs::RGBAColor& specular );

    void setShininess( const float shininess );

public:

    void apply( void );
};

/*
const Material MATERIAL_EMERALD
(
    0.0215,  0.1745,   0.0215,  1.0, // ambient
    0.07568, 0.61424,  0.07568, 1.0, // diffuse
    0.633,   0.727811, 0.633,   1.0, // specular
    0.6 * 128.0                      // shininess
);

const Material MATERIAL_JADE
(
    0.135,    0.2225,   0.1575,   1.0,
    0.54,     0.89,     0.63,     1.0,
    0.316228, 0.316228, 0.316228, 1.0,
    0.1 * 128.0
);

const Material MATERIAL_OBSIDIAN
(
    0.05375,  0.05,     0.06625,  1.0,
    0.18275,  0.17,     0.22525,  1.0,
    0.332741, 0.328634, 0.346435, 1.0,
    0.3 * 128.0
);

const Material MATERIAL_PEARL
(
    0.25,     0.20725,  0.20725,  1.0,
    1.0,      0.829,    0.829,    1.0,
    0.296648, 0.296648, 0.296648, 1.0,
    0.088 * 128.0
);

const Material MATERIAL_RUBY
(
    0.1745,   0.01175,  0.01175,  1.0,
    0.61424,  0.04136,  0.04136,  1.0,
    0.727811, 0.626959, 0.626959, 1.0,
    0.6 * 128.0
);

const Material MATERIAL_TURQUOISE
(
    0.1,      0.18725, 0.1745,   1.0,
    0.396,    0.74151, 0.69102,  1.0,
    0.297254, 0.30829, 0.306678, 1.0,
    0.1 * 128.0
);

const Material MATERIAL_BRASS
(
    0.329412, 0.223529, 0.027451, 1.0,
    0.780392, 0.568627, 0.113725, 1.0,
    0.992157, 0.941176, 0.807843, 1.0,
    0.21794872 * 128.0
);

const Material MATERIAL_BRONZE
(
    0.2125,   0.1275,   0.054,    1.0,
    0.714,    0.4284,   0.18144,  1.0,
    0.393548, 0.271906, 0.166721, 1.0,
    0.2 * 128.0
);

const Material MATERIAL_CHROME
(
    0.25,     0.25,     0.25,     1.0,
    0.4,      0.4,      0.4,      1.0,
    0.774597, 0.774597, 0.774597, 1.0,
    0.6 * 128.0
);

const Material MATERIAL_COPPER
(
    0.19125,  0.0735,   0.0225,   1.0,
    0.7038,   0.27048,  0.0828,   1.0,
    0.256777, 0.137622, 0.086014, 1.0,
    0.1 * 128.0
);

const Material MATERIAL_GOLD
(
    0.24725,  0.1995,   0.0745,   1.0,
    0.75164,  0.60648,  0.22648,  1.0,
    0.628281, 0.555802, 0.366065, 1.0,
    0.4 * 128.0
);

const Material MATERIAL_SILVER
(
    0.19225,  0.19225,  0.19225,  1.0,
    0.50754,  0.50754,  0.50754,  1.0,
    0.508273, 0.508273, 0.508273, 1.0,
    0.4 * 128.0
);

const Material MATERIAL_BLACK_PLASTIC
(
    0.0,  0.0,  0.0,  1.0,
    0.01, 0.01, 0.01, 1.0,
    0.50, 0.50, 0.50, 1.0,
    0.25 * 128.0
);

const Material MATERIAL_CYAN_PLASTIC
(
    0.0,        0.1,        0.06,       1.0,
    0.0,        0.50980392, 0.50980392, 1.0,
    0.50196078, 0.50196078, 0.50196078, 1.0,
    0.25 * 128.0
);

const Material MATERIAL_GREEN_PLASTIC
(
    0.0,  0.0,  0.0,  1.0,
    0.1,  0.35, 0.1,  1.0,
    0.45, 0.55, 0.45, 1.0,
    0.25 * 128.0
);

const Material MATERIAL_RED_PLASTIC
(
    0.0, 0.0, 0.0, 1.0,
    0.5, 0.0, 0.0, 1.0,
    0.7, 0.6, 0.6, 1.0,
    0.25 * 128.0
);

const Material MATERIAL_WHITE_PLASTIC
(
    0.0,  0.0,  0.0,  1.0,
    0.55, 0.55, 0.55, 1.0,
    0.70, 0.70, 0.70, 1.0,
    0.25 * 128.0
);

const Material MATERIAL_YELLOW_PLASTIC
(
    0.0,  0.0,  0.0,  1.0,
    0.5,  0.5,  0.0,  1.0,
    0.60, 0.60, 0.50, 1.0,
    0.25 * 128.0
);

const Material MATERIAL_BLACK_RUBBER
(
    0.02,    0.02, 0.02, 1.0,
    0.01,    0.01, 0.01, 1.0,
    0.4,     0.4,  0.4,  1.0,
    0.078125 * 128.0
);

const Material MATERIAL_CYAN_RUBBER
(
    0.0,     0.05, 0.05, 1.0,
    0.4,     0.5,  0.5,  1.0,
    0.04,    0.7,  0.7,  1.0,
    0.078125 * 128.0
);

const Material MATERIAL_GREEN_RUBBER
(
    0.0,     0.05, 0.0,  1.0,
    0.4,     0.5,  0.4,  1.0,
    0.04,    0.7,  0.04, 1.0,
    0.078125 * 128.0
);

const Material MATERIAL_RED_RUBBER
(
    0.05,    0.0,  0.0,  1.0,
    0.5,     0.4,  0.4,  1.0,
    0.7,     0.04, 0.04, 1.0,
    0.078125 * 128.0
);

const Material MATERIAL_WHITE_RUBBER
(
    0.05,    0.05, 0.05, 1.0,
    0.5,     0.5,  0.5,  1.0,
    0.7,     0.7,  0.7,  1.0,
    0.078125 * 128.0
);

const Material MATERIAL_YELLOW_RUBBER
(
    0.05,    0.05, 0.0,  1.0,
    0.5,     0.5,  0.4,  1.0,
    0.7,     0.7,  0.04, 1.0,
    0.078125 * 128.0
);
*/

} // end of namespace kvs

#endif
