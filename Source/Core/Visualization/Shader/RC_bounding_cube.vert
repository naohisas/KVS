/*****************************************************************************/
/**
 *  @file   RC_bounding_cube.vert
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#version 120


// Uniform variables (OpenGL variables).
uniform mat4 ModelViewProjectionMatrix; // model-view projection matrix


/*===========================================================================*/
/**
 *  @brief  Main function of vertex shader.
 */
/*===========================================================================*/
void main()
{
    gl_Position = ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor = gl_Vertex;
}
