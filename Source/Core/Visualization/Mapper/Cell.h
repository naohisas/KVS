/*****************************************************************************/
/**
 *  @file   Cell.h
 *  @author Naoya Maeda
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Cell.h 1332 2012-10-13 09:36:23Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include "BasicTetrahedralCell.h"
#include "BasicHexahedralCell.h"
#include "BasicQuadraticTetrahedralCell.h"
#include "BasicQuadraticHexahedralCell.h"
#include "BasicPyramidalCell.h"
#include "BasicCubicCell.h"
#include "BasicGenericCell.h"
#include "CreateCell.h"


namespace kvs
{

namespace cell
{

typedef float CellValueType;

typedef BasicTetrahedralCell          <CellValueType> TetrahedralCell;
typedef BasicHexahedralCell           <CellValueType> HexahedralCell;
typedef BasicQuadraticTetrahedralCell <CellValueType> QuadraticTetrahedralCell;
typedef BasicQuadraticHexahedralCell  <CellValueType> QuadraticHexahedralCell;
typedef BasicPyramidalCell            <CellValueType> PyramidalCell;
typedef BasicCubicCell                <CellValueType> CubicCell;
typedef BasicGenericCell              <CellValueType> GenericCell;

} // end of namespace cell

} // end of namespace kvs
