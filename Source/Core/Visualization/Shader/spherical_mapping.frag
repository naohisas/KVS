#include "texture.h"
#include "qualifire.h"

const float PI = 3.14159265;
const float DEG2RAD = 0.01745329251994329576923690768489;

const float hfov_deg = 90.0;
const float vfov_deg = 90.0;
const float hfov_rad = hfov_deg * DEG2RAD;
const float vfov_rad = vfov_deg * DEG2RAD;

uniform mat3 R;
uniform vec2 image_size;
uniform vec2 screen_size;
uniform sampler2D spherical_map;

vec3 RotateXY( vec3 p, vec2 angle )
{
    vec2 c = cos( angle );
    vec2 s = sin( angle );
    p = vec3( p.x, c.x * p.y + s.x * p.z, - s.x * p.y + c.x * p.z );
    return vec3( c.y * p.x + s.y * p.z, p.y, - s.y * p.x + c.y * p.z );
}

void main()
{
    vec2 uv = gl_FragCoord.xy * 2.0 / screen_size.xy - 1.0;
    vec3 dir = normalize(vec3( uv.xy * vec2( tan( 0.5 * hfov_rad ), tan( 0.5 * vfov_rad ) ), 1.0 ) );
    vec3 rot = vec3( ( ( vec2( 1024, 768 ) / image_size.xy ) - 0.5 ) * vec2( 2.0 * PI,  PI ), 0.0 );
    vec3 rd = normalize( RotateXY( dir * R, rot.yx ) );

    vec2 tex_coord = vec2( -atan( rd.z, rd.x ) - PI * 0.5, -asin( rd.y ) + PI * 0.5 ) / vec2( 2.0 * PI, PI );
    gl_FragColor = LookupTexture2D( spherical_map, tex_coord );
}
