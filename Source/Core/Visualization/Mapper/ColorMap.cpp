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
//kvs::ColorMap::ColorMapFunction DefaultColorMap = kvs::ColorMap::BrewerRdBu;
kvs::ColorMap::ColorMapFunction DefaultColorMap = kvs::ColorMap::Fast;

}


namespace kvs
{

void ColorMap::SetDefaultColorMap( ColorMapFunction func )
{
    ::DefaultColorMap = func;
}

kvs::ColorMap ColorMap::DefaultColorMap( const size_t resolution )
{
    return ::DefaultColorMap( resolution );
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
 *  @brief  Returns fast diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return fast colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] F. Samsel, W. A. Scott and K. Moreland, A New Default Colormap for ParaView,
 *      In IEEE Computer Graphics and Applications, vol. 44, no. 04, pp. 150-160,
 *      July-Aug. 2024, doi: 10.1109/MCG.2024.3383137.
 *      https://www.computer.org/csdl/magazine/cg/2024/04/10640196/1ZySI5cfpNm
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::Fast( const size_t resolution )
{
    Points colors = {
        { 1.00f * 255, { 150,  20,  30 } },
        { 0.85f * 255, { 204,  90,  41 } },
        { 0.71f * 255, { 237, 158,  80 } },
        { 0.59f * 255, { 244, 213, 130 } },
        { 0.50f * 255, { 229, 241, 196 } },
        { 0.43f * 255, { 175, 237, 234 } },
        { 0.30f * 255, {  91, 190, 243 } },
        { 0.17f * 255, {  62, 117, 207 } },
        { 0.00f * 255, {  14,  14, 120 } }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
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

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (multi-hue) blue-green colormap.
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
kvs::ColorMap kvs::ColorMap::BrewerMultiHueBuGn( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 247, 252, 253 },
        { 229, 245, 249 },
        { 204, 236, 230 },
        { 153, 216, 201 },
        { 102, 194, 164 },
        {  65, 174, 118 },
        {  35, 139,  69 },
        {   0, 109,  44 },
        {   0,  68,  27 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (multi-hue) blue-purple colormap.
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
kvs::ColorMap kvs::ColorMap::BrewerMultiHueBuPu( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 247, 252, 253 },
        { 224, 236, 244 },
        { 191, 211, 230 },
        { 158, 188, 218 },
        { 140, 150, 198 },
        { 140, 107, 177 },
        { 136,  65, 157 },
        { 129,  15, 124 },
        {  77,   0,  75 },
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (multi-hue) green-blue colormap.
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
kvs::ColorMap kvs::ColorMap::BrewerMultiHueGnBu( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 247, 252, 240 },
        { 224, 243, 219 },
        { 204, 235, 197 },
        { 168, 221, 181 },
        { 123, 204, 196 },
        {  78, 179, 211 },
        {  43, 140, 190 },
        {   8, 104, 172 },
        {   8,  64, 129 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (multi-hue) orange-red colormap.
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
kvs::ColorMap kvs::ColorMap::BrewerMultiHueOrRd( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 247, 236 },
        { 254, 232, 200 },
        { 253, 212, 158 },
        { 253, 187, 132 },
        { 252, 141,  89 },
        { 239, 101,  72 },
        { 215,  48,  31 },
        { 179,   0,   0 },
        { 127,   0,   0 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (multi-hue) purple-blue colormap.
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
kvs::ColorMap kvs::ColorMap::BrewerMultiHuePuBu( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 247, 251 },
        { 236, 231, 242 },
        { 208, 209, 230 },
        { 166, 189, 219 },
        { 116, 169, 207 },
        {  54, 144, 192 },
        {   5, 112, 176 },
        {   4,  90, 141 },
        {   2,  56,  88 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (multi-hue) purple-blue-green colormap.
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
kvs::ColorMap kvs::ColorMap::BrewerMultiHuePuBuGn( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 247, 251 },
        { 236, 226, 240 },
        { 208, 209, 230 },
        { 166, 189, 219 },
        { 103, 169, 207 },
        {  54, 144, 192 },
        {   2, 129, 138 },
        {   1, 108,  89 },
        {   1,  70,  54 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (multi-hue) purple-red colormap.
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
kvs::ColorMap kvs::ColorMap::BrewerMultiHuePuRd( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 247, 244, 249 },
        { 231, 225, 239 },
        { 212, 185, 218 },
        { 201, 148, 199 },
        { 223, 101, 176 },
        { 231,  41, 138 },
        { 206,  18,  86 },
        { 152,   0,  67 },
        { 103,   0,  31 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (multi-hue) red-purple colormap.
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
kvs::ColorMap kvs::ColorMap::BrewerMultiHueRdPu( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 247, 243 },
        { 253, 224, 221 },
        { 252, 197, 192 },
        { 250, 159, 181 },
        { 247, 104, 161 },
        { 221,  52, 151 },
        { 174,   1, 126 },
        { 122,   1, 119 },
        {  73,   0, 106 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (multi-hue) yellow-green colormap.
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
kvs::ColorMap kvs::ColorMap::BrewerMultiHueYlGn( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 255, 229 },
        { 247, 252, 185 },
        { 217, 240, 163 },
        { 173, 221, 142 },
        { 120, 198, 121 },
        {  65, 171,  93 },
        {  35, 132,  67 },
        {   0, 104,  55 },
        {   0,  69,  41 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (multi-hue) yellow-green-blue colormap.
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
kvs::ColorMap kvs::ColorMap::BrewerMultiHueYlGnBu( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 255, 217 },
        { 237, 248, 177 },
        { 199, 233, 180 },
        { 127, 205, 187 },
        {  65, 182, 196 },
        {  29, 145, 192 },
        {  34,  94, 168 },
        {  37,  52, 148 },
        {   8,  29,  88 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (multi-hue) yellow-orange-brown colormap.
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
kvs::ColorMap kvs::ColorMap::BrewerMultiHueYlOrBr( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 255, 229 },
        { 255, 247, 188 },
        { 254, 227, 145 },
        { 254, 196,  79 },
        { 254, 153,  41 },
        { 236, 112,  20 },
        { 204,  76,   2 },
        { 153,  52,   4 },
        { 102,  37,   6 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer sequential (multi-hue) yellow-orange-red colormap.
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
kvs::ColorMap kvs::ColorMap::BrewerMultiHueYlOrRd( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 255, 204 },
        { 255, 237, 160 },
        { 254, 217, 118 },
        { 254, 178,  76 },
        { 253, 141,  60 },
        { 252,  78,  42 },
        { 227,  26,  28 },
        { 189,   0,  38 },
        { 128,   0,  38 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns viridis colormap. [perceptually sequential colormap]
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
 *  @brief  Returns plasma colormap. [perceptually sequential colormap]
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
 *  @brief  Returns inferno colormap. [perceptually sequential colormap]
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
 *  @brief  Returns magma colormap. [perceptually sequential colormap]
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
 *  @brief  Returns cividis colormap. [perceptually sequential colormap]
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

/*===========================================================================*/
/**
 *  @brief  Returns cork colormap. [diverging colormap]
 *  @param  resolution [in] table resolution
 *  @return cork colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Crameri, F. (2018), Scientific colour maps, Zenodo,
 *      doi:10.5281/zenodo.1243862
 *      https://zenodo.org/records/8409685
 */
/*===========================================================================*/
kvs::ColorMap kvs::ColorMap::Cork( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        {  44,  25,  76 },
        {  40,  69, 120 },
        {  73, 116, 159 },
        { 131, 161, 190 },
        { 202, 215, 227 },
        { 210, 225, 210 },
        { 142, 179, 141 },
        {  79, 137,  78 },
        {  28,  91,  25 },
        {  15,  41,   3 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns broc colormap. [diverging colormap]
 *  @param  resolution [in] table resolution
 *  @return broc colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Crameri, F. (2018), Scientific colour maps, Zenodo,
 *      doi:10.5281/zenodo.1243862
 *      https://zenodo.org/records/8409685
 */
/*===========================================================================*/
kvs::ColorMap kvs::ColorMap::Broc( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        {  44,  26,  76 },
        {  40,  68, 119 },
        {  75, 118, 160 },
        { 139, 167, 194 },
        { 206, 217, 229 },
        { 232, 232, 210 },
        { 197, 197, 143 },
        { 141, 141,  86 },
        {  85,  85,  39 },
        {  38,  38,   0 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns vik colormap. [diverging colormap]
 *  @param  resolution [in] table resolution
 *  @return vik colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Crameri, F. (2018), Scientific colour maps, Zenodo,
 *      doi:10.5281/zenodo.1243862
 *      https://zenodo.org/records/8409685
 */
/*===========================================================================*/
kvs::ColorMap kvs::ColorMap::Vik( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        {   0,  18,  97 },
        {   3,  62, 125 },
        {  30, 111, 157 },
        { 113, 168, 196 },
        { 201, 221, 231 },
        { 234, 206, 189 },
        { 211, 151, 116 },
        { 190, 101,  51 },
        { 139,  39,   6 },
        {  89,   0,   8 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns lisbon colormap. [diverging colormap]
 *  @param  resolution [in] table resolution
 *  @return lisbon colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Crameri, F. (2018), Scientific colour maps, Zenodo,
 *      doi:10.5281/zenodo.1243862
 *      https://zenodo.org/records/8409685
 */
/*===========================================================================*/
kvs::ColorMap kvs::ColorMap::Lisbon( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 230, 229, 255 },
        { 155, 175, 211 },
        {  81, 119, 164 },
        {  30,  67, 104 },
        {  17,  30,  44 },
        {  39,  37,  26 },
        {  87,  81,  52 },
        { 141, 133,  86 },
        { 201, 195, 144 },
        { 255, 255, 217 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns tofino colormap. [diverging colormap]
 *  @param  resolution [in] table resolution
 *  @return tofino colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Crameri, F. (2018), Scientific colour maps, Zenodo,
 *      doi:10.5281/zenodo.1243862
 *      https://zenodo.org/records/8409685
 */
/*===========================================================================*/
kvs::ColorMap kvs::ColorMap::Tofino( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 222, 217, 255 },
        { 147, 164, 222 },
        {  74, 107, 172 },
        {  39,  60, 101 },
        {  18,  25,  38 },
        {  18,  34,  20 },
        {  36,  77,  40 },
        {  63, 129,  68 },
        { 136, 185, 112 },
        { 219, 230, 155 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns berlin colormap. [diverging colormap]
 *  @param  resolution [in] table resolution
 *  @return berlin colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Crameri, F. (2018), Scientific colour maps, Zenodo,
 *      doi:10.5281/zenodo.1243862
 *      https://zenodo.org/records/8409685
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::Berlin( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 158, 176, 255 },
        {  91, 164, 219 },
        {  45, 117, 151 },
        {  26,  66,  86 },
        {  17,  25,  30 },
        {  40,  13,   1 },
        {  80,  24,   3 },
        { 138,  63,  42 },
        { 196, 117, 106 },
        { 255, 173, 173 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns bam colormap. [diverging colormap]
 *  @param  resolution [in] table resolution
 *  @return bam colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Crameri, F. (2018), Scientific colour maps, Zenodo,
 *      doi:10.5281/zenodo.1243862
 *      https://zenodo.org/records/8409685
 */
/*===========================================================================*/
kvs::ColorMap kvs::ColorMap::Bam( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 101,   2,  75 },
        { 158,  60, 133 },
        { 201, 112, 178 },
        { 228, 174, 214 },
        { 245, 227, 239 },
        { 239, 243, 229 },
        { 193, 218, 162 },
        { 124, 168,  86 },
        {  69, 123,  42 },
        {  13,  76,   0 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns roma colormap. [diverging colormap]
 *  @param  resolution [in] table resolution
 *  @return roma colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Crameri, F. (2018), Scientific colour maps, Zenodo,
 *      doi:10.5281/zenodo.1243862
 *      https://zenodo.org/records/8409685
 */
/*===========================================================================*/
kvs::ColorMap kvs::ColorMap::Roma( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 126,  23,   0 },
        { 153,  82,  21 },
        { 176, 128,  43 },
        { 200, 180,  85 },
        { 208, 227, 163 },
        { 164, 229, 211 },
        {  93, 193, 211 },
        {  50, 146, 194 },
        {  32, 100, 174 },
        {   3,  49, 152 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns vanimo colormap. [diverging colormap]
 *  @param  resolution [in] table resolution
 *  @return vanimo colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Crameri, F. (2018), Scientific colour maps, Zenodo,
 *      doi:10.5281/zenodo.1243862
 *      https://zenodo.org/records/8409685
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::Vanimo( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 205, 253 },
        { 211, 129, 196 },
        { 161,  73, 142 },
        {  92,  36,  79 },
        {  35,  20,  29 },
        {  27,  30,  17 },
        {  54,  73,  26 },
        {  90, 124,  42 },
        { 133, 181,  77 },
        { 190, 253, 165 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns managua colormap. [diverging colormap]
 *  @param  resolution [in] table resolution
 *  @return managua colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Crameri, F. (2018), Scientific colour maps, Zenodo,
 *      doi:10.5281/zenodo.1243862
 *      https://zenodo.org/records/8409685
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::Managua( const size_t resolution )
{
    std::list<kvs::RGBColor> colors = {
        { 255, 207, 103 },
        { 221, 154,  85 },
        { 185, 108,  70 },
        { 146,  70,  59 },
        { 103,  43,  60 },
        {  78,  49,  94 },
        {  78,  85, 147 },
        {  91, 128, 188 },
        { 109, 177, 222 },
        { 129, 231, 255 }
    };

    kvs::ColorMap cmap( resolution );
    cmap.setPoints( colors );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns sequential colormap (single hue).
 *  @param  c [in] color
 *  @param  resolution [in] table resolution
 *  @return sequence colormap
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::Sequential(
    const kvs::RGBColor& c,
    const size_t resolution )
{
    const kvs::LabColor lab( c );
    const auto l = lab.l();
    const auto a = lab.a();
    const auto b = lab.b();
    const auto lmax = 100.0f;

    Table table( resolution * 3 );
    for ( size_t i = 0; i < resolution; i++ )
    {
        const auto ratio = float(i) / ( resolution - 1 );
        const auto li = kvs::Math::Mix( lmax, l, ratio );
        const auto rgb = kvs::LabColor( li, a, b ).toRGBColor();
        table[ 3 * i + 0 ] = rgb.r();
        table[ 3 * i + 1 ] = rgb.g();
        table[ 3 * i + 2 ] = rgb.b();
    }

    return kvs::ColorMap( table );
}

/*===========================================================================*/
/**
 *  @brief  Returns sequential colormap (multiple hue).
 *  @param  cs [in] color list
 *  @param  resolution [in] table resolution
 *  @return sequence colormap
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::Sequential(
    const std::list<kvs::RGBColor>& cs,
    const size_t resolution )
{
    kvs::ColorMap cmap( resolution );
    cmap.setPoints( cs );
    cmap.setColorSpaceToLab();
    cmap.create();
    return cmap;
}

/*===========================================================================*/
/**
 *  @brief  Returns diverging colormap.
 *  @param  c1 [i] left side color
 *  @param  c2 [i] right side color
 *  @param  resolution [i] table resolution
 *  @return diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Kenneth Moreland, "Diverging Color Maps for Scientific Visualization",
 *      In Proceedings of the 5th International Symposium on Visual Computing,
 *      December 2009.
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::Diverging(
    const kvs::RGBColor& c1,
    const kvs::RGBColor& c2,
    const size_t resolution )
{
    kvs::ColorMap cmap( resolution );
    cmap.setPoints( { c1, c2 } );
    cmap.setColorSpaceToMsh();
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

ColorMap::ColorMap( const size_t resolution, const Points& points ):
    m_resolution( resolution )
{
    this->setPoints( points );
    this->create();
}

ColorMap::ColorMap( const size_t resolution, const Points& points, const float min_value, const float max_value )
{
    this->setPoints( points );
    this->setRange( min_value, max_value );
    this->create();
}

ColorMap::ColorMap( const size_t resolution, const std::list<kvs::RGBColor>& colors )
{
    this->setPoints( colors );
    this->create();
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

void ColorMap::reversePoints()
{
    auto itr_f = m_points.begin();
    auto itr_b = m_points.rbegin();
    for ( size_t i = 0; i < m_points.size() / 2; i++ )
    {
        std::swap( (itr_f++)->second, (itr_b++)->second );
    }
}

void ColorMap::reverse()
{
    if ( !m_points.empty() ) { this->reversePoints(); }
    if ( m_table.size() > 0 )
    {
        for ( size_t i = 0; i < m_resolution / 2; i++ )
        {
            const auto j = m_resolution - 1 - i;
            std::swap( m_table[ 3 * i + 0 ], m_table[ 3 * j + 0 ] );
            std::swap( m_table[ 3 * i + 1 ], m_table[ 3 * j + 1 ] );
            std::swap( m_table[ 3 * i + 2 ], m_table[ 3 * j + 2 ] );
        }
    }
}

void ColorMap::brighten( const float beta )
{
    KVS_ASSERT( ( -1.0f < beta ) && ( beta <= 1.0f ) );

    const auto r = beta > 0 ? 1.0f - beta : 1.0f / ( 1.0f + beta );
    for ( size_t i = 0; i < m_resolution; i++ )
    {
        const auto c = kvs::RGBColor( m_table.data() + 3 * i );
        const auto lab = kvs::LabColor( c );
        const auto l = std::pow( lab.l() / 100.0f, r );
        const auto a = lab.a();
        const auto b = lab.b();
        const auto rgb = kvs::LabColor( l * 100.f, a, b ).toRGBColor();
        m_table[ 3 * i + 0 ] = rgb.r();
        m_table[ 3 * i + 1 ] = rgb.g();
        m_table[ 3 * i + 2 ] = rgb.b();
    }
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

kvs::ColorMap ColorMap::reversed() const
{
    kvs::ColorMap cmap( this->table().clone(), this->minValue(), this->maxValue() );
    cmap.reverse();
    return cmap;
}

kvs::ColorMap ColorMap::brightened( const float beta ) const
{
    kvs::ColorMap cmap( this->table().clone(), this->minValue(), this->maxValue() );
    cmap.brighten( beta );
    return cmap;
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
