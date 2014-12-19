/*****************************************************************************/
/**
 *  @file   multiple_tetrahedra2.geom
 *  @author Jun Nishimura
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#version 120
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

varying in vec4 position_in[4];
varying in float value_in[4];
varying in vec2 random_index_in[4];
varying in vec3 normal_in[4];

varying out vec3 position;
varying out vec3 normal;
varying out vec2 random_index;

varying out float depth_front;
varying out float depth_back;

varying out float scalar_front;
varying out float scalar_back;
varying out float segment_length;

uniform vec2 preintegration_scale_offset;
uniform sampler2D decomposition_texture;

vec3 screen_position[4];

int direction( in int p0, in int p1, in int p2 )
{
    vec2 v1 = screen_position[p1].xy - screen_position[p0].xy;
    vec2 v2 = screen_position[p2].xy - screen_position[p0].xy;
    float cross_product = v1.x * v2.y - v1.y * v2.x;
    return( int( sign( cross_product ) + 1.0 ) );
}

void emitExistPoint( const in int index, const in float dist )
{
    gl_Position = gl_PositionIn[index]; 
    position    = position_in[index].xyz;
    depth_front = gl_PositionIn[index].z / gl_PositionIn[index].w;
    depth_back  = depth_front;

    normal = normal_in[index].xyz;

    scalar_front = value_in[index];
    scalar_back  = value_in[index];
    segment_length     = dist;
    EmitVertex();
}

void emitNewPoint(
    const in vec4 center_position,
    const in vec3 center_position_3D,
    const in float center_another_depth,
    const in vec3 center_normal_3D,
    const in float center_scalar_front,
    const in float center_scalar_back,
    const in float center_distance )
{
    gl_Position = center_position;
    position    = center_position_3D;
    depth_front = center_position.z / center_position.w;
    depth_back  = center_another_depth;

    normal = center_normal_3D;

    scalar_front = center_scalar_front;
    scalar_back  = center_scalar_back;
    segment_length     = center_distance;
    EmitVertex();
}

//vec3 gradientOfCell( void )
//{
//    mat4 a = mat4( position_in[0], position_in[1], position_in[2], position_in[3] );
//    vec4 b = vec4( value_in[0], value_in[1], value_in[2], value_in[3] );
//    vec4 x = inverse( a ) * b;
//
//    return x.xyz;
//}

float distance_to_texture_coord( const in float dist )
{
    return( dist * preintegration_scale_offset.x + preintegration_scale_offset.y );
}

void calculate_across_triangle_to_line_2D( out float r2, out float r3, in int p0, in int p1, in int p2, in int p3 )
{
    vec2 v01 = screen_position[p0].xy - screen_position[p1].xy;
    vec2 v21 = screen_position[p2].xy - screen_position[p1].xy;
    vec2 v31 = screen_position[p3].xy - screen_position[p1].xy;
    float delta = v21.x * v31.y - v31.x * v21.y;
    if ( delta == 0 )
    {
        r2 = r3 = 1.0 / 3.0;
        return;
    }

    float delta_inv = 1.0 / delta;
    r2 = delta_inv * (  v31.y * v01.x - v31.x * v01.y );
    r3 = delta_inv * ( -v21.y * v01.x + v21.x * v01.y );
}

float perspective_correct( const in float r, const in float p0z, const in float p1z )
{
    if ( p0z == p1z ) return r;

    float p0z_inv = 1.0 / p0z;
    float p1z_inv = 1.0 / p1z;
    float pz_inv  = p0z_inv + ( p1z_inv - p0z_inv ) * r;
    float pz = 1.0 / pz_inv;

    float result = ( pz - p0z ) / ( p1z - p0z );
    if ( result < 0.0 ) result = 0.0;
    if ( result > 1.0 ) result = 1.0;
    return( result );
}

void perspective_correct_triangle( inout float r2, inout float r3, in int p0, in int p1, in int p2, in int p3 )
{
    if ( r2 + r3 <= 0 ) return;
    float p1z    = gl_PositionIn[p1].w;
    float p2z    = gl_PositionIn[p2].w;
    float p3z    = gl_PositionIn[p3].w;
    float k_2D   = r2 + r3;
    float r23_2D = r3 / k_2D;

    float r23_3D = perspective_correct( r23_2D, p2z, p3z );

    float p23z = p2z + ( p3z - p2z ) * r23_3D;
    float k_3D = perspective_correct( k_2D, p1z, p23z );

    r3 = r23_3D * k_3D;
    r2 = k_3D - r3;
}

void perspective_correct_line( inout float r, const in int p0, const in int p1 )
{
    float p0z  = gl_PositionIn[p0].w;
    float p1z  = gl_PositionIn[p1].w;
    float r_3D = perspective_correct( r, p0z, p1z );

    r = r_3D;
}

void calculate_across_line_to_line_2D( out float r1, out float r3, in int p0, in int p1, in int p2, in int p3 )
{
    vec2 v02 = screen_position[p0].xy - screen_position[p2].xy;
    vec2 v01 = screen_position[p0].xy - screen_position[p1].xy;
    vec2 v32 = screen_position[p3].xy - screen_position[p2].xy;
    float delta = v01.x * v32.y - v32.x * v01.y;
    if ( delta == 0 )
    {
        r1 = r3 = 1.0 / 2.0;
        return;
    }
    float delta_inv = 1.0 / delta;
    r1 = delta_inv * (  v32.y * v02.x - v32.x * v02.y );
    r3 = delta_inv * ( -v01.y * v02.x + v01.x * v02.y );
}

//    type 1
//
//   p3-------------p2
//     \ ~-_   _-~ /
//      \   ~p0   /
//       \   |   /
//        \  |  /
//         \ | /
//          \|/
//           p1
//
void create_type_1( in int p0, in int p1, in int p2, in int p3 )
{
   float r2, r3;
   calculate_across_triangle_to_line_2D( r2, r3, p0, p1, p2, p3 );
   perspective_correct_triangle( r2, r3, p0, p1, p2, p3 );

   vec4  p123 = position_in[p1] + ( position_in[p2]  - position_in[p1] ) * r2 + ( position_in[p3] - position_in[p1] ) * r3;
   float s123 = value_in[p1] + ( value_in[p2] - value_in[p1] ) * r2 + ( value_in[p3] - value_in[p1] ) * r3;

   vec4 center_position;
   vec3 center_position_3D;
   float center_scalar_front, center_scalar_back; 

   vec3 n123 = normal_in[p1] + ( normal_in[p2] - normal_in[p1] ) * r2 + ( normal_in[p3] - normal_in[p1] ) * p3;
   vec3 center_normal_3D;

    float a_depth;

   if ( length( p123.z ) < length( position_in[p0].z ) )
   {
       center_position = gl_ProjectionMatrix * p123;
       center_position_3D = p123.xyz;
       vec4 tmp = gl_PositionIn[p0];
       a_depth = tmp.z / tmp.w;

       center_normal_3D = n123.xyz;

       center_scalar_front = s123;
       center_scalar_back = value_in[p0];
   }
   else
   {
       center_position = gl_PositionIn[p0];
       center_position_3D = position_in[p0].xyz;
       vec4 tmp = gl_ProjectionMatrix * p123;
       a_depth = tmp.z / tmp.w;

       center_normal_3D = normal_in[p0].xyz;

       center_scalar_front = value_in[p0];
       center_scalar_back = s123;
   }

   float center_distance = distance_to_texture_coord( length( position_in[p0] - p123 ) );

    // p1-p2-C p2-C-p3 C-p3-p1
    emitExistPoint( p1, 0.0 );
    emitExistPoint( p2, 0.0 );
    emitNewPoint( center_position, center_position_3D, a_depth, center_normal_3D, center_scalar_front, center_scalar_back, center_distance );
    emitExistPoint( p3, 0.0 );
    emitExistPoint( p1, 0.0 );
    EndPrimitive();
}

//    type 2
//           p0
//        _-~ | ~-_
//     _-~    |    ~-_ 
//   p3 -  -  | -  -  p2
//     \      |      /
//       \    |    /
//         \  |  /
//           \|/
//            p1
//
void create_type_2( in int p0, in int p1, in int p2, in int p3 )
{

   float r1, r3;
   calculate_across_line_to_line_2D( r1, r3, p0, p1, p2, p3 );
   perspective_correct_line( r1, p0, p1 );
   perspective_correct_line( r3, p2, p3 );

   vec4  p01 = position_in[p0] + ( position_in[p1] - position_in[p0] ) * r1;
   float s01 = value_in[p0] + ( value_in[p1] - value_in[p0] ) * r1;

   vec4  p23 = position_in[p2] + ( position_in[p3]  - position_in[p2] ) * r3;
   float s23 = value_in[p2] + ( value_in[p3] - value_in[p2] ) * r3;

   vec4 center_position;
   vec3 center_position_3D;
   float center_scalar_front, center_scalar_back; 

   vec3 n01 = normal_in[p0] + ( normal_in[p1] - normal_in[p0] ) * r1;
   vec3 n23 = normal_in[p2] + ( normal_in[p3] - normal_in[p2] ) * r3;
   vec3 center_normal_3D;

    float a_depth;

   if ( length( p01.xyz ) < length( p23.xyz ) )
   {
       center_position = gl_ProjectionMatrix * p01;
       center_position_3D = p01.xyz;
       vec4 tmp = gl_ProjectionMatrix * p23;
       a_depth = tmp.z / tmp.w;

       center_normal_3D = n01.xyz;

       center_scalar_front = s01;
       center_scalar_back = s23;
   }
   else
   {
       center_position = gl_ProjectionMatrix * p23;
       center_position_3D = p23.xyz;
       vec4 tmp = gl_ProjectionMatrix * p01;
       a_depth = tmp.z / tmp.w;

       center_normal_3D = n23.xyz;

       center_scalar_front = s23;
       center_scalar_back = s01;
   }

   float center_distance = distance_to_texture_coord( length( p01.z - p23.z ) );

    // right half: p0-p2-C, p2-C-p1
    emitExistPoint( p0, 0.0 );
    emitExistPoint( p2, 0.0 );
    emitNewPoint( center_position, center_position_3D, a_depth, center_normal_3D, center_scalar_front, center_scalar_back, center_distance );
    emitExistPoint( p1, 0.0 );
    EndPrimitive();

    // left half: p0-p3-C, p3-C-p1
    emitExistPoint( p0, 0.0 );
    emitExistPoint( p3, 0.0 );
    emitNewPoint( center_position, center_position_3D, a_depth, center_normal_3D, center_scalar_front, center_scalar_back, center_distance );
    emitExistPoint( p1, 0.0 );
    EndPrimitive();
}

//    type 3
//           p2
//        _-~ |
//     _-~    |
//   p0-------p1
//     \      |
//       \    |
//         \  |
//           \|
//            p3
//
//    in type 2, two arealess triangle are generated.
//    but this pattern is very rare case.
//
void create_type_3( in int p0, in int p1, in int p2, in int p3 )
{
    create_type_2( p0, p1, p2, p3 );
}

//    type 4
//           p0
//            | ~-_
//            |    ~-_ 
//            |       p2,p3
//            |      /
//            |    /
//            |  /
//            |/
//            p1
//
void create_type_4( in int p0, in int p1, in int p2, in int p3 )
{

    int pFront, pBack;
    if ( position_in[p2].z < position_in[p3].z )
    {
        pFront = p2;
        pBack = p3;
    }
    else
    {
        pFront = p3;
        pBack = p2;
    }

    float a_depth = gl_PositionIn[pFront].z / gl_PositionIn[pFront].w;

    float center_distance = distance_to_texture_coord( length( position_in[pBack] - position_in[pFront] ) );
    emitExistPoint( p0, 0.0 );
    emitExistPoint( p1, 0.0 );
    emitNewPoint( gl_PositionIn[pBack], position_in[pBack].xyz, a_depth, normal_in[pBack].xyz, value_in[pBack], value_in[pFront], center_distance );
    //emitExistPoint( pFront, center_distance );
    EndPrimitive();
}

void main( void )
{
    if ( gl_PositionIn[0].w <= 0 ||
         gl_PositionIn[1].w <= 0 ||
         gl_PositionIn[2].w <= 0 ||
         gl_PositionIn[3].w <= 0 ) return;

    screen_position[0] = gl_PositionIn[0].xyz / gl_PositionIn[0].w;
    screen_position[1] = gl_PositionIn[1].xyz / gl_PositionIn[1].w;
    screen_position[2] = gl_PositionIn[2].xyz / gl_PositionIn[2].w;
    screen_position[3] = gl_PositionIn[3].xyz / gl_PositionIn[3].w;

    if ( ( screen_position[0].x < -1.0 && screen_position[1].x < -1.0 && screen_position[2].x < -1.0 && screen_position[3].x < -1.0 ) ||
         ( screen_position[0].x >  1.0 && screen_position[1].x >  1.0 && screen_position[2].x >  1.0 && screen_position[3].x >  1.0 ) ||
         ( screen_position[0].y < -1.0 && screen_position[1].y < -1.0 && screen_position[2].y < -1.0 && screen_position[3].y < -1.0 ) ||
         ( screen_position[0].y >  1.0 && screen_position[1].y >  1.0 && screen_position[2].y >  1.0 && screen_position[3].y >  1.0 ) ) return;

    random_index = random_index_in[0] * 3.0 + random_index_in[1] * 5.0 + random_index_in[2] * 7.0 + random_index_in[3] * 11.0;

    int d321 = direction( 3, 2, 1 );
    int d230 = direction( 2, 3, 0 );
    int d103 = direction( 1, 0, 3 );
    int d012 = direction( 0, 1, 2 );

    int pos = d321 * 27 + d230 * 9 + d103 * 3 + d012;
    vec2 t_pos = vec2( ( float(pos) + 0.5 ) / 81.0, 0.5 );
    vec4 info = texture2D( decomposition_texture, t_pos );
    int type = int( ( info.a * 255.0 + 16.0 ) / 32.0 );
    int p0   = int( ( info.x * 255.0 + 16.0 ) / 32.0 );
    int p1   = int( ( info.y * 255.0 + 16.0 ) / 32.0 );
    int p2   = int( ( info.z * 255.0 + 16.0 ) / 32.0 );
    int p3   = 6 - ( p0 + p1 + p2 );

    if ( type == 1 )
    {
        create_type_1( p0, p1, p2, p3 );
    }
    else if ( type == 2 )
    {
        create_type_2( p0, p1, p2, p3 );
    }
    else if ( type == 3 )
    {
        create_type_3( p0, p1, p2, p3 );
    }
    else if ( type == 4 )
    {
        create_type_4( p0, p1, p2, p3 );
    }

}
