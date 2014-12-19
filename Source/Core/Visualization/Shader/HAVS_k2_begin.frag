/*****************************************************************************/
/**
 *  @file   HAVS_k2_begin.frag
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright 2007 Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: k2_begin.frag 1011 2012-01-31 02:57:30Z naohisa.sakamoto $
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
//
// ---------------------------------------------------------------------------
// The program consists of the following steps:
//
// 1. Clear the off-screen framebuffer to 0.0
// 2. Clear the k-buffers entries 1-2 to -1.0
// 
// The following textures are used:
//
//  Tex 0: off-screen framebuffer
//  Tex 1: k-buffer entries 1 and 2 (2D RGBA 16/32 bpp float)
//

void main( void )
{
    gl_FragData[0] = vec4( 0.0 );
    gl_FragData[1] = vec4( -1.0 );
}
