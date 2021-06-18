/*****************************************************************************/
/**
 *  @file   RC_bounding_cube.frag
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#version 120


/*===========================================================================*/
/**
 *  @brief  Main function of fragment shader.
 */
/*===========================================================================*/
void main()
{
    // Store the depth value in window coordinates to the alpha channel
    // of color buffer.
    gl_FragColor.rgb = gl_Color.rgb;
    gl_FragColor.a = gl_FragCoord.z;
}
