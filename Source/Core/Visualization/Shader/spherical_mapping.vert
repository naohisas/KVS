/*****************************************************************************/
/**
 *  @file   spherical_mapping.vert
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "qualifire.h"

uniform vec2 image_size;
uniform vec2 screen_size;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
