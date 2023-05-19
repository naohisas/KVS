/****************************************************************************/
/**
 *  @file   ColorMap.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "ColorMap.h"
#include <kvs/Assert>
#include <kvs/RGBColor>
#include <kvs/HCLColor>
#include <kvs/HSLColor>
#include <kvs/HSVColor>
#include <kvs/LabColor>
#include <kvs/MshColor>
#include <kvs/Math>


namespace
{

// Default values.
const size_t NumberOfChannels = 3;
kvs::ColorMap::ColorMapFunction DefaultColorMap = kvs::ColorMap::BrewerRdBu;

}


namespace kvs
{

void ColorMap::SetDefaultColorMap( ColorMapFunction func )
{
    ::DefaultColorMap = func;
}

/*===========================================================================*/
/**
 *  @brief  Returns rainbow colormap.
 *  @param  resolution [in] table resolution
 *  @return rainbow colormap
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::Rainbow( const size_t resolution )
{
    const auto hue_min = 0.0f;
    const auto hue_mid = 120.0f / 360.0f;
    const auto hue_max = 240.0f / 360.0f;
    std::list<kvs::RGBColor> colors = {
        kvs::HSVColor( hue_max, 1.0f, 1.0f ), // blue
        kvs::HSVColor( hue_mid, 1.0f, 1.0f ), // green
        kvs::HSVColor( hue_min, 1.0f, 1.0f )  // red
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToHSV();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns cool-warm diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return cool-warm colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Kenneth Moreland, "Diverging Color Maps for Scientific Visualization",
 *      In Proceedings of the 5th International Symposium on Visual Computing,
 *      December 2009. DOI: 10.1007/978-3-642-10520-3_9.
 *      https://www.kennethmoreland.com/color-maps/
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::CoolWarm( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        {  59, 76, 192 },
        { 180,  4,  38 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToMsh();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer BrBG (brown, (white), blue-green) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return BrBG diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerBrBG( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        {  84,  48,   5 },
        { 140,  81,  10 },
        { 191, 129,  45 },
        { 223, 194, 125 },
        { 246, 232, 195 },
        { 245, 245, 245 },
        { 199, 234, 229 },
        { 128, 205, 193 },
        {  53, 151, 143 },
        {   1, 102,  94 },
        {   0,  60,  48 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer PiYG (pink, (white), yello-green) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return PiYG diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerPiYG( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 142,   1,  82 },
        { 197,  27, 125 },
        { 222, 119, 174 },
        { 241, 182, 218 },
        { 253, 224, 239 },
        { 247, 247, 247 },
        { 230, 245, 208 },
        { 184, 225, 134 },
        { 127, 188, 134 },
        {  77, 146,  33 },
        {  39, 100,  25 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer PRGn (purple, (white), green) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return PRGn diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerPRGn( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        {  64,   0,  75 },
        { 118,  42, 131 },
        { 153, 112, 171 },
        { 194, 165, 207 },
        { 231, 212, 232 },
        { 247, 247, 247 },
        { 217, 240, 211 },
        { 166, 219, 160 },
        {  90, 174,  97 },
        {  27, 120,  55 },
        {   0,  68,  27 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer PuOr (orange, (white), purple) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return PuOr diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerPuOr( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 127,  59,   8 },
        { 179,  88,   6 },
        { 224, 130,  20 },
        { 253, 184,  99 },
        { 254, 224, 182 },
        { 247, 247, 247 },
        { 216, 218, 235 },
        { 178, 171, 210 },
        { 128, 115, 172 },
        {  84,  39, 136 },
        {  45,   0,  75 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer RdBu (red, (white), blue) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return RdBu diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerRdBu( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 103,   0,  31 },
        { 178,  24,  43 },
        { 214,  96,  77 },
        { 244, 165, 130 },
        { 253, 219, 199 },
        { 247, 247, 247 },
        { 209, 229, 240 },
        { 146, 197, 222 },
        {  67, 147, 195 },
        {  33, 102, 172 },
        {   5,  48,  97 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer RdGy (red, (white), grey) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return RdGy diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerRdGy( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 103,   0,  31 },
        { 178,  24,  43 },
        { 214,  96,  77 },
        { 244, 165, 130 },
        { 253, 219, 199 },
        { 255, 255, 255 },
        { 224, 224, 224 },
        { 186, 186, 186 },
        { 135, 135, 135 },
        {  77,  77,  77 },
        {  26,  26,  26 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer RdYlBu (red, yello, blue) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return RdYlBu diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerRdYlBu( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 165,   0,  38 },
        { 215,  48,  39 },
        { 244, 109,  67 },
        { 253, 174,  97 },
        { 254, 224, 144 },
        { 255, 255, 191 },
        { 224, 243, 248 },
        { 171, 217, 233 },
        { 116, 173, 209 },
        {  69, 117, 180 },
        {  49,  54, 149 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer RdYlGn (red, yello, green) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return RdYlGn diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerRdYlGn( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 165,   0,  38 },
        { 215,  48,  39 },
        { 244, 109,  67 },
        { 253, 174,  97 },
        { 254, 224, 139 },
        { 255, 255, 191 },
        { 217, 239, 139 },
        { 166, 217, 106 },
        { 102, 189,  99 },
        {  26, 152,  80 },
        {   0, 104,  55 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer spectral (red, orange, yello, green, blue) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return spectral diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerSpectral( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 158,   1,  66 },
        { 213,  62,  79 },
        { 244, 109,  67 },
        { 253, 174,  97 },
        { 254, 224, 139 },
        { 255, 255, 191 },
        { 230, 245, 152 },
        { 171, 221, 164 },
        { 102, 194, 165 },
        {  50, 136, 189 },
        {  94,  79, 162 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (single-hue) grey colormap.
 *  @param  resolution [in] table resolution
 *  @return single-hue grey colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerSingleHueGreys( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 255, 255 },
        { 240, 240, 240 },
        { 217, 217, 217 },
        { 189, 189, 189 },
        { 150, 150, 150 },
        { 115, 115, 115 },
        {  82,  82,  82 },
        {  37,  37,  37 },
        {   0,   0,   0 }
    };
    //colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (single-hue) green colormap.
 *  @param  resolution [in] table resolution
 *  @return single-hue green colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerSingleHueGreens( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 247, 252, 245 },
        { 229, 245, 224 },
        { 199, 233, 192 },
        { 161, 217, 155 },
        { 116, 196, 118 },
        {  65, 171,  93 },
        {  35, 139,  69 },
        {   0, 109,  44 },
        {   0,  68,  27 }
    };
    //colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (single-hue) blue colormap.
 *  @param  resolution [in] table resolution
 *  @return single-hue blue colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerSingleHueBlues( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 247, 251, 255 },
        { 222, 235, 247 },
        { 198, 219, 239 },
        { 158, 202, 225 },
        { 107, 174, 214 },
        {  66, 146, 198 },
        {  33, 113, 181 },
        {   8,  81, 156 },
        {   8,  48, 107 }
    };
    //colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (single-hue) red colormap.
 *  @param  resolution [in] table resolution
 *  @return single-hue red colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerSingleHueReds( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 245, 240 },
        { 254, 224, 210 },
        { 252, 187, 161 },
        { 252, 146, 114 },
        { 251, 106,  74 },
        { 239,  59,  44 },
        { 203,  24,  29 },
        { 165,  15,  21 },
        { 103,   0,  13 }
    };
    //colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (single-hue) purple colormap.
 *  @param  resolution [in] table resolution
 *  @return single-hue purple colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerSingleHuePurples( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 252, 251, 253 },
        { 239, 237, 245 },
        { 218, 218, 235 },
        { 188, 189, 220 },
        { 158, 154, 200 },
        { 128, 125, 186 },
        { 106,  81, 163 },
        {  84,  39, 143 },
        {  63,   0, 125 }
    };
    //colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}


/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (single-hue) orange colormap.
 *  @param  resolution [in] table resolution
 *  @return single-hue orange colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerSingleHueOranges( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 245, 235 },
        { 254, 230, 206 },
        { 253, 208, 162 },
        { 253, 174, 107 },
        { 253, 141,  60 },
        { 241, 105,  19 },
        { 217,  72,   1 },
        { 166,  54,   3 },
        { 127,  39,   4 }
    };
    //colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns viridis colormap.
 *  @param  resolution [in] table resolution
 *  @return viridis colormap
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::Viridis( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 253, 231,  37 },
        { 181, 221,  43 },
        { 109, 206,  88 },
        {  53, 183, 120 },
        {  30, 156, 137 },
        {  37, 130, 142 },
        {  49, 102, 141 },
        {  62,  72, 136 },
        {  71,  38, 118 },
        {  68,   1,  84 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns plasma colormap.
 *  @param  resolution [in] table resolution
 *  @return plasma colormap
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::Plasma( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 239, 248,  33 },
        { 252, 201,  38 },
        { 250, 159,  58 },
        { 236, 120,  83 },
        { 215,  86, 108 },
        { 188,  54, 133 },
        { 154,  21, 158 },
        { 112,   0, 168 },
        {  68,   3, 158 },
        {  12,   7, 134 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns inferno colormap.
 *  @param  resolution [in] table resolution
 *  @return inferno colormap
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::Inferno( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 252, 254, 164 },
        { 247, 209,  60 },
        { 251, 155,   6 },
        { 237, 104,  37 },
        { 205,  66,  71 },
        { 164,  44,  96 },
        { 118,  27, 109 },
        {  72,  11, 106 },
        {  25,  11,  62 },
        {   0,   0,   3 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns magma colormap.
 *  @param  resolution [in] table resolution
 *  @return magma colormap
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::Magma( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 251, 252, 191 },
        { 254, 201, 141 },
        { 253, 149, 103 },
        { 240,  96,  93 },
        { 203,  62, 113 },
        { 158,  46, 126 },
        { 111,  30, 129 },
        {  66,  15, 116 },
        {  22,  14,  58 },
        {   0,   0,   3 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns cividis colormap.
 *  @param  resolution [in] table resolution
 *  @return cividis colormap
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::Cividis( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 233,  69 },
        { 228, 206,  91 },
        { 197, 181, 107 },
        { 166, 156, 117 },
        { 137, 133, 120 },
        { 111, 112, 115 },
        {  85,  91, 108 },
        {  54,  70, 107 },
        {   0,  50, 110 },
        {   0,  32,  76 }
    };
    colors.reverse();

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new ColoryMap class.
 *  @param  resolution [in] resolution
 *  @param  min_value [in] min value
 *  @param  max_value [in] max value
 */
/*==========================================================================*/
ColorMap::ColorMap( const size_t resolution, const float min_value, const float max_value ):
    m_color_space( RGBSpace ),
    m_resolution( resolution ),
    m_min_value( min_value ),
    m_max_value( max_value ),
    m_points(),
    m_table()
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new ColorMap class.
 *  @param  table [in] opacity value table
 *  @param  min_value [in] min value
 *  @param  max_value [in] max value
 */
/*==========================================================================*/
ColorMap::ColorMap( const ColorMap::Table& table, const float min_value, const float max_value ):
    m_color_space( RGBSpace ),
    m_resolution( table.size() / 3 ),
    m_min_value( min_value ),
    m_max_value( max_value ),
    m_points(),
    m_table( table )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns true if the range is specified.
 *  @return true (specified) or false (not specified)
 */
/*===========================================================================*/
bool ColorMap::hasRange() const
{
    return !kvs::Math::Equal( m_min_value, m_max_value );
}

/*===========================================================================*/
/**
 *  @brief  Adds a control point as RGB color.
 *  @param  value [in] scalar value in [min_value, max_value]
 *  @param  color [in] RGB color value
 */
/*===========================================================================*/
void ColorMap::addPoint( const float value, const kvs::RGBColor color )
{
    m_points.push_back( Point( value, color ) );
}

/*===========================================================================*/
/**
 *  @brief  Adds a control point as HSV color
 *  @param  value [in] scalar value in [min_value, max_value]
 *  @param  color [in] HSV color value
 */
/*===========================================================================*/
void ColorMap::addPoint( const float value, const kvs::HSVColor color )
{
    this->addPoint( value, kvs::RGBColor( color ) );
}

void ColorMap::setPoints( const std::list<kvs::RGBColor>& colors )
{
    const size_t ncolors = colors.size();
    const float stride = 1.0f / ( ncolors - 1 );

    this->addPoint( 0.0f, colors.front() ); // start point
    auto color = colors.begin(); color++;
    auto end = colors.end(); end--;
    size_t index = 1;
    while ( color != end )
    {
        const float value = kvs::Math::Round( m_resolution * stride * index );
        this->addPoint( value, *color );
        color++; index++;
    }
    this->addPoint( float( m_resolution - 1 ), colors.back() ); // end point
}

/*===========================================================================*/
/**
 *  @brief  Removes the constrol point associated with the given scalar value.
 *  @param  value [in] scalar value in [min_value, max_value]
 */
/*===========================================================================*/
void ColorMap::removePoint( const float value )
{
    m_points.remove_if( [ value ]( Point& p ) { return kvs::Math::Equal( p.first, value ); } );
}

/*==========================================================================*/
/**
 *  @brief  Creates the color map.
 */
/*==========================================================================*/
void ColorMap::create()
{
    kvs::Real32 min_value = 0.0f;
    kvs::Real32 max_value = static_cast<kvs::Real32>( m_resolution - 1 );
    if ( this->hasRange() )
    {
        min_value = this->minValue();
        max_value = this->maxValue();
    }

    if ( m_points.size() == 0 )
    {
        *this = ::DefaultColorMap( m_resolution );
    }
    else
    {
        m_table.allocate( ::NumberOfChannels * m_resolution );
        m_points.sort( [] ( const Point& p1, const Point& p2 ) { return p1.first < p2.first; } );

        const kvs::RGBColor black( 0, 0, 0 );
        const kvs::RGBColor white( 255, 255, 255 );
        if ( m_points.front().first > min_value ) this->addPoint( min_value, black );
        if ( m_points.back().first < max_value ) this->addPoint( max_value, white );

        const float stride = ( max_value - min_value ) / static_cast<float>( m_resolution - 1 );
        float f = min_value;
        for ( size_t i = 0; i < m_resolution; ++i, f += stride )
        {
            Points::iterator p = m_points.begin();
            Points::iterator last = m_points.end();

            kvs::RGBColor color( 0, 0, 0 );
            Point p0( min_value, kvs::RGBColor(   0,   0,   0 ) );
            Point p1( max_value, kvs::RGBColor( 255, 255, 255 ) );
            while ( p != last )
            {
                const float s = p->first;
                if ( kvs::Math::Equal( f, s ) )
                {
                    color = p->second;
                    break;
                }
                else if ( f < s )
                {
                    p1 = *p;
                    // Interpolate.
                    const float s0 = p0.first;
                    const float s1 = p1.first;
                    const float ratio = ( f - s0 ) / ( s1 - s0 );
                    if ( m_color_space == RGBSpace )
                    {
                        const kvs::RGBColor c0 = p0.second;
                        const kvs::RGBColor c1 = p1.second;
                        color = kvs::RGBColor::Mix( c0, c1, ratio );
                    }
                    else if ( m_color_space == HCLSpace )
                    {
                        const kvs::HCLColor c0 = p0.second;
                        const kvs::HCLColor c1 = p1.second;
                        color = kvs::HCLColor::Mix( c0, c1, ratio );
                    }
                    else if ( m_color_space == HSLSpace )
                    {
                        const kvs::HSLColor c0 = p0.second;
                        const kvs::HSLColor c1 = p1.second;
                        color = kvs::HSLColor::Mix( c0, c1, ratio );
                    }
                    else if ( m_color_space == HSVSpace )
                    {
                        const kvs::HSVColor c0 = p0.second;
                        const kvs::HSVColor c1 = p1.second;
                        color = kvs::HSVColor::Mix( c0, c1, ratio );
                    }
                    else if ( m_color_space == LabSpace )
                    {
                        const kvs::LabColor c0 = p0.second;
                        const kvs::LabColor c1 = p1.second;
                        color = kvs::LabColor::Mix( c0, c1, ratio );
                    }
                    else if ( m_color_space == MshSpace )
                    {
                        const kvs::MshColor c0 = p0.second;
                        const kvs::MshColor c1 = p1.second;
                        color = kvs::MshColor::Mix( c0, c1, ratio );
                    }
                    break;
                }
                else
                {
                    p0 = *p;
                    ++p;
                    if ( p == last )
                    {
                        if ( kvs::Math::Equal( p0.first, max_value ) )
                        {
                            color = p0.second;
                        }
                    }
                }
            }

            m_table[ i * ::NumberOfChannels + 0 ] = color.r();
            m_table[ i * ::NumberOfChannels + 1 ] = color.g();
            m_table[ i * ::NumberOfChannels + 2 ] = color.b();
        }
    }
}

/*==========================================================================*/
/**
 *  @brief  Access operator [].
 *  @param  index [in] index of the color map.
 *  @retval RGB color value
 */
/*==========================================================================*/
const kvs::RGBColor ColorMap::operator []( const size_t index ) const
{
    KVS_ASSERT( index < this->resolution() );

    const size_t offset = ::NumberOfChannels * index;
    return kvs::RGBColor( m_table.data() + offset );
}

/*===========================================================================*/
/**
 *  @brief  Returns interpolated RGB color value by assuming piecewise linear map.
 *  @param  value [in] value
 *  @return interpolated RGB color value
 */
/*===========================================================================*/
const kvs::RGBColor ColorMap::at( const float value ) const
{
    const float v0 = kvs::Math::Clamp( value, m_min_value, m_max_value );
    const float r = static_cast<float>( m_resolution - 1 );
    const float v = ( v0 - m_min_value ) / ( m_max_value - m_min_value ) * r;
    const size_t s0 = static_cast<size_t>( v );
    const size_t s1 = kvs::Math::Min( s0 + 1, m_resolution - 1 );

    const kvs::RGBColor c0( m_table.data() + ::NumberOfChannels * s0 );
    const kvs::RGBColor c1( m_table.data() + ::NumberOfChannels * s1 );
    return kvs::RGBColor::Mix( c0, c1, v - s0 );
}

/*==========================================================================*/
/**
 *  @brief  Substitution operator =.
 *  @param  opacity_map [in] color map
 *  @retval opacity map
 */
/*==========================================================================*/
ColorMap& ColorMap::operator =( const ColorMap& rhs )
{
    m_resolution = rhs.m_resolution;
    m_min_value = rhs.m_min_value;
    m_max_value = rhs.m_max_value;
    m_points = rhs.m_points;
    m_table = rhs.m_table;
    return *this;
}

} // end of namespace kvs
