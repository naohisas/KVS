/*****************************************************************************/
/**
 *  @file   resize.frag
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright 2007 Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: resize.frag 260 2009-07-07 04:39:41Z naohisa.sakamoto $
 */
/*****************************************************************************/
uniform sampler2D texture;
uniform vec2 start;
uniform vec2 step;
uniform ivec2 count;
uniform float scale;


/*===========================================================================*/
/**
 *  @brief  Resize texture.
 */
/*===========================================================================*/
void main()
{
#if 0 // don't use linear interpolation
    int x, y;
    vec2 pos1, pos2;
    vec4 c = vec4( 0.0, 0.0, 0.0, 0.0 );
    pos1 = gl_TexCoord[0].st + start;
    for ( y = 0; y < count.t; y++ )
    {
        pos2 = pos1;
        for ( x = 0; x < count.s; x++ )
        {
            c += texture2D( texture, pos2 );
            pos2.s += step.s;
        }
        pos1.t += step.t;
    }
    gl_FragColor = c * scale;
#else
    int x, y;
    vec2 pos1, pos2, pos3;
    vec4 c = vec4( 0.0, 0.0, 0.0, 0.0 );
    pos1 = gl_TexCoord[0].st + start;
    float weightxy, weighty;
    for ( y = 0; y < count.t; y++ )
    {
        pos2 = pos1;
        if ( x+1 < count.s )
        {
            // vertical interpolation
            weighty = 2.0;
            pos2.t += step.t * 0.5;
            y++;
            pos1.t += step.t;
        }
        else
        {
            weighty = 1.0;
        }
        for ( x = 0; x < count.s; x++ )
        {
            weightxy = weighty;
            pos3 = pos2;
            if ( x+1 < count.s )
            {
                // horizontal interpolation
                weightxy = weighty * 2.0;
                pos3.s += step.s * 0.5;
                x++;
                pos2.s += step.s;
            }
            c += texture2D( texture, pos3 ) * weightxy;
            pos2.s += step.s;
        }
        pos1.t += step.t;
    }
    gl_FragColor = c * scale;
    gl_FragColor.a = gl_Color.a;
#endif
}
