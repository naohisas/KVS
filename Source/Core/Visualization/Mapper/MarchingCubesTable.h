/****************************************************************************/
/**
 *  @file   MarchingCubesTable.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MarchingCubesTable.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__MARCHING_CUBES_TABLE_H_INCLUDE
#define KVS__MARCHING_CUBES_TABLE_H_INCLUDE

namespace kvs
{

namespace MarchingCubesTable
{

extern const int TriangleID[256][16];
extern const int VertexID[12][2][3];

} // end of namespace MarchingCubesTable

} // end of namespace kvs

#endif // KVS__MARCHING_CUBES_TABLE_H_INCLUDE
