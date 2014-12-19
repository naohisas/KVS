/*****************************************************************************/
/**
 *  @file   HAVS_k6.frag
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright 2007 Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: k6.frag 1011 2012-01-31 02:57:30Z naohisa.sakamoto $
 */
/*****************************************************************************/
//
// The Hardware-Assisted Visibility Ordering Algorithm
// Copyright (c) 2005, 2006
// University of Utah
// All rights reserved.
//
// This software is licensed under the BSD open-source license. See
// http://www.opensource.org/licenses/bsd-license.php for more detail.
//
// *************************************************************
// Redistribution and use in source and binary forms, with or 
// without modification, are permitted provided that the following 
// conditions are met:
//
// Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer. 
//
// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution. 
//
// Neither the name of the University of Utah nor the names of 
// the contributors may be used to endorse or promote products derived 
// from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
// THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
// OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// 
// The program consists of the following steps:
//
// 1. Find the first and second entries in the fixed size k-buffer list sorted 
//    by d (6+1 entries)
// 2. Perform a 3D pre-integrated transfer function lookup using front and back
//    scalar data values + the segment length computed from the distance values
//    of the first and second entries from the k-buffer.
// 3. Composite the color and opacity from the transfer function with the
//    color and opacity from the framebuffer. Discard winning k-buffer entry,
//    write the remaining k-buffer entries.
//
// The following textures are used:
// 
//   Tex framebuffer: off-screen framebuffer (2D RGBA 16/32 bpp float)
//   Tex kbuffer1: k-buffer entries 1 and 2 (same)
//   Tex kbuffer2: k-buffer entries 3 and 4 (same)
//   Tex kbuffer3: k-buffer entries 5 and 6 (same)
//   Tex lut: transfer function lookup table (3D RGBA 8/16 bpp int)
//

uniform vec4 scale;        // scale and bias parameters (1/pw, 1/ph, 
                           // (1-1/z_size)/max_length, 1/(2*z_size)
uniform sampler2D framebuffer;
uniform sampler2D kbuffer1;
uniform sampler2D kbuffer2;
uniform sampler2D kbuffer3;
uniform sampler3D lut;

void main( void )
{
    // Compute texture coords from window position so that it is not
    // interpolated perspective-correct.
    vec2 position = gl_FragCoord.xy * scale.xy;
    vec4 fbcolor = vec4(texture2D(framebuffer, position));

    // Early discard test
    if (fbcolor.a > 0.99) discard;

    // ---------------------------------------------------------------
    // Fragment Ordering via Selection Sort
    // ---------------------------------------------------------------

    // Compute fragment-to-eye distance
    float dist = length(gl_TexCoord[1]);
    // Setup k-buffer entry k0 using the incoming fragment
    vec2 k0 = vec2(gl_TexCoord[0].x, dist);

    // Read the k-buffer entries k1-k6 from texture.
    // Each k-buffer entry contains the scalar data value in x or z
    // and the distance value in y or w.
    vec4 tmp = vec4(texture2D(kbuffer1, position));
    vec2 k1 = tmp.xy;
    vec2 k2 = tmp.zw;
    tmp = vec4(texture2D(kbuffer2, position));
    vec2 k3 = tmp.xy;
    vec2 k4 = tmp.zw;
    tmp = vec4(texture2D(kbuffer3, position));
    vec2 k5 = tmp.xy;
    vec2 k6 = tmp.zw;

    // Find fragment with minimum distance (first) and save the rest (rest[1-6])
    // first = min_distance(k0, k1); rest1 = max_distance(k0, k1);
    vec2 first = (k0.y < k1.y) ? k0 : k1;
    vec2 rest1 = (k0.y <= k1.y) ? k1 : k0;
    // first = min_distance(first, k2); rest2 = max_distance(first, k2);
    vec2 rest2 = (first.y <= k2.y) ? k2 : first;
    first = (first.y < k2.y) ? first : k2;
    // first = min_distance(first, k3); rest3 = max_distance(first, k3);
    vec2 rest3 = (first.y <= k3.y) ? k3 : first;
    first = (first.y < k3.y) ? first : k3;
    // first = min_distance(first, k4); rest4 = max_distance(first, k4);
    vec2 rest4 = (first.y <= k4.y) ? k4 : first;
    first = (first.y < k4.y) ? first : k4;
    // first = min_distance(first, k5); rest5 = max_distance(first, k5);
    vec2 rest5 = (first.y <= k5.y) ? k5 : first;
    first = (first.y < k5.y) ? first : k5;
    // first = min_distance(first, k6); rest6 = max_distance(first, k6);
    vec2 rest6 = (first.y <= k6.y) ? k6 : first;
    first = (first.y < k6.y) ? first : k6;

    // Find fragment with minimum distance (second) from the rest.
    // second = min_distance(rest1, rest2);
    vec2 second = (rest1.y < rest2.y) ? rest1 : rest2;
    second = (second.y < rest3.y) ? second : rest3;
    second = (second.y < rest4.y) ? second : rest4;
    second = (second.y < rest5.y) ? second : rest5;
    second = (second.y < rest6.y) ? second : rest6;

    // ---------------------------------------------------------------
    // Transfer Function
    // ---------------------------------------------------------------

    // Set up texcoords for lookup
    vec3 texcoords = vec3(second.x, first.x, second.y - first.y);
    texcoords.z = texcoords.z * scale.z + scale.w;

    // Transfer function lookup
    vec4 col = vec4(texture3D(lut, texcoords));

    // Invalidate initialization fragments and fragments with a distance
    // greater than unit scale (non-convexity)
    vec4 zero = vec4(0.0);
    if (first.x <= 0.0) col = zero;
    if (texcoords.z > 1.0) col = zero;

    // ---------------------------------------------------------------
    // Compositing
    // ---------------------------------------------------------------

    // Composite color with the framebuffer: FRONT TO BACK
    float transp = 1.0 - fbcolor.a;
    vec4 final = col * transp + fbcolor;

    // Write the k-buffer entries back into textures.
    gl_FragData[1] = vec4(rest1.x, rest1.y, rest2.x, rest2.y);
    gl_FragData[2] = vec4(rest3.x, rest3.y, rest4.x, rest4.y);
    gl_FragData[3] = vec4(rest5.x, rest5.y, rest6.x, rest6.y);
    gl_FragData[0] = final;
}
