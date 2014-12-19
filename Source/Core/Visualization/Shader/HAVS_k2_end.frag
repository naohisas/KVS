/*****************************************************************************/
/**
 *  @file   HAVS_k2_end.frag
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright 2007 Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: k2_end.frag 1011 2012-01-31 02:57:30Z naohisa.sakamoto $
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
//    by d (2 entries)
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
//   Tex lut: transfer function lookup table (3D RGBA 8/16 bpp int)
//
uniform vec4 scale;        // scale and bias parameters (1/pw, 1/ph, 
                           // (1-1/z_size)/max_length, 1/(2*z_size)
uniform sampler2D framebuffer;
uniform sampler2D kbuffer1;
uniform sampler3D lut;

void main( void )
{
    // Computer Texture coords from window position so that it is not
    // interpolated perspective-correct.
    vec2 position = gl_FragCoord.xy * scale.xy;
    vec4 fbcolor = vec4 (texture2D(framebuffer, position));

    // Early discard test
    if(fbcolor.a >= 0.99) discard;

    // ---------------------------------------------------------------
    // Fragment Ordering via Selection Sort
    // ---------------------------------------------------------------

    // Read the k-buffer entries k1 and k2 from texture.
    // Each k-buffer entry contains the scalar data value in x or z
    // And the distance value in y or w.
    vec4 tmp = vec4(texture2D(kbuffer1, position));
    vec2 k1 = tmp.xy;
    vec2 k2 = tmp.zw;

    // first = min_distance(k1,k2); second = max_distance(k1,k2);
    vec2 first = (k1.y < k2.y) ? k1 : k2;
    vec2 second = (k1.y <= k2.y) ? k2 : k1;

    // ---------------------------------------------------------------
    // Transfer Function
    // ---------------------------------------------------------------

    // Set up the texcoords for lookup
    vec3 texcoords = vec3 (first.x, second.x, second.y - first.y);
    texcoords.z = texcoords.z * scale.z + scale.w;

    // Transfer Function lookup
    vec4 col = vec4 (texture3D(lut, texcoords));

    // Invalidate initialization fragments
    vec4 zero = vec4(0.0);
    if (first.x <= 0.0) col = zero;
    if (second.x <= 0.0) col = zero;

    // ---------------------------------------------------------------
    // Compositing
    // ---------------------------------------------------------------

    // Composite color with the framebuffer: FRONT TO BACK
    float transp = 1.0 - fbcolor.a;
    vec4 final = col * transp + fbcolor;

    // Write the k-buffer entries back into textures
    gl_FragData[1] = vec4(second.x, second.y, -1.0, 10000.0);
    gl_FragData[0] = final;
}
