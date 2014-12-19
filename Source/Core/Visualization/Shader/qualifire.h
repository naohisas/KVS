/*****************************************************************************/
/**
 *  @file   qualifire.h
 *  @author Naohisa Sakamoto
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

// Type qualifiers.
#if __VERSION__ >= 130
#define VertIn in
#define VertOut out
#define GeomIn in
#define GeomOut out
#define FragIn in
#else
#define VertIn attribute
#define VertOut varying
#define GeomIn varying in
#define GeomOut varying out
#define FragIn varying
#endif

// Interpolation qualifiers.
#if __VERSION__ >= 130
#define VertOutWithFlat flat VertOut
#define VertOutWithSmooth smooth VertOut
#define VertOutWithNoperspective noperspective VertOut
#define GeomInWithFlat flat GeomIn
#define GeomInWithSmooth smooth GeomIn
#define GeomInWithNoperspective noperspective GeomIn
#define GeomOutWithFlat flat GeomOut
#define GeomOutWithSmooth smooth GeomOut
#define GeomOutWithNoperspective noperspective GeomOut
#define FragInWithFlat flat FragIn
#define FragInWithSmooth smooth FragIn
#define FragInWithNoperspective noperspective FragIn
#else
// not supported under GLSL 1.20
#define VertOutWithFlat VertOut
#define VertOutWithSmooth VertOut
#define VertOutWithNoperspective VertOut
#define GeomInWithFlat GeomIn
#define GeomInWithSmooth GeomIn
#define GeomInWithNoperspective GeomIn
#define GeomOutWithFlat GeomOut
#define GeomOutWithSmooth GeomOut
#define GeomOutWithNoperspective GeomOut
#define FragInWithFlat FragIn
#define FragInWithSmooth FragIn
#define FragInWithNoperspective FragIn
#endif
