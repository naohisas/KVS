/****************************************************************************/
/**
 *  @file   TagTable.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TagTable.h 631 2010-10-10 02:15:35Z naohisa.sakamoto $
 */
/****************************************************************************/
#pragma once

#include "Tag.h"


namespace kvs
{

namespace tiff
{

const kvs::tiff::Tag TagTable[] =
{
    kvs::tiff::Tag( 254, "subfile data descriptor"),
    kvs::tiff::Tag( 255, "kind of data in subfile"),
    kvs::tiff::Tag( 256, "image width in pixels"),
    kvs::tiff::Tag( 257, "image height in pixels"),
    kvs::tiff::Tag( 258, "bits per channel (sample)"),
    kvs::tiff::Tag( 259, "data compression technique"),
    kvs::tiff::Tag( 262, "photometric interpretation"),
    kvs::tiff::Tag( 263, "thresholding used on data"),
    kvs::tiff::Tag( 264, "dithering matrix width"),
    kvs::tiff::Tag( 265, "dithering matrix height"),
    kvs::tiff::Tag( 266, "data order within a byte"),
    kvs::tiff::Tag( 269, "name of doc. image is from"),
    kvs::tiff::Tag( 270, "info about image"),
    kvs::tiff::Tag( 271, "scanner manufacturer name"),
    kvs::tiff::Tag( 272, "scanner model name/number"),
    kvs::tiff::Tag( 273, "offsets to data strips"),
    kvs::tiff::Tag( 274, "image orientation"),
    kvs::tiff::Tag( 277, "samples per pixel"),
    kvs::tiff::Tag( 278, "rows per strip of data"),
    kvs::tiff::Tag( 279, "bytes counts for strips"),
    kvs::tiff::Tag( 280, "minimum sample value"),
    kvs::tiff::Tag( 281, "maximum sample value"),
    kvs::tiff::Tag( 282, "pixels/resolution in x"),
    kvs::tiff::Tag( 283, "pixels/resolution in y"),
    kvs::tiff::Tag( 284, "storage organization"),
    kvs::tiff::Tag( 285, "page name image is from"),
    kvs::tiff::Tag( 286, "x page offset of image lhs"),
    kvs::tiff::Tag( 287, "y page offset of image lhs"),
    kvs::tiff::Tag( 288, "byte offset to free block"),
    kvs::tiff::Tag( 289, "sizes of free blocks"),
    kvs::tiff::Tag( 290, "gray scale curve accuracy"),
    kvs::tiff::Tag( 291, "gray scale response curve"),
    kvs::tiff::Tag( 292, "32 flag bits"),
    kvs::tiff::Tag( 293, "32 flag bits"),
    kvs::tiff::Tag( 296, "units of resolutions"),
    kvs::tiff::Tag( 297, "page numbers of multi-page"),
    kvs::tiff::Tag( 300, "color curve accuracy"),
    kvs::tiff::Tag( 301, "colorimetry info"),
    kvs::tiff::Tag( 305, "name & release"),
    kvs::tiff::Tag( 306, "creation date and time"),
    kvs::tiff::Tag( 315, "creator of image"),
    kvs::tiff::Tag( 316, "machine where created"),
    kvs::tiff::Tag( 317, "prediction scheme w/ LZW"),
    kvs::tiff::Tag( 318, "image white point"),
    kvs::tiff::Tag( 319, "primary chromaticities"),
    kvs::tiff::Tag( 320, "RGB map for pallette image"),
    kvs::tiff::Tag( 321, "highlight+shadow info"),
    kvs::tiff::Tag( 322, "rows/data tile"),
    kvs::tiff::Tag( 323, "cols/data tile"),
    kvs::tiff::Tag( 324, "offsets to data tiles"),
    kvs::tiff::Tag( 325, "byte counts for tiles"),
    kvs::tiff::Tag( 326, "lines w/ wrong pixel count"),
    kvs::tiff::Tag( 327, "regenerated line info"),
    kvs::tiff::Tag( 328, "max consecutive bad lines"),
    kvs::tiff::Tag( 330, "subimage descriptors"),
    kvs::tiff::Tag( 332, "inks in separated image"),
    kvs::tiff::Tag( 333, "ascii names of inks"),
    kvs::tiff::Tag( 334, "number of inks"),
    kvs::tiff::Tag( 336, "dot range"),
    kvs::tiff::Tag( 337, "separation target"),
    kvs::tiff::Tag( 338, "info about extra samples"),
    kvs::tiff::Tag( 339, "data sample format"),
    kvs::tiff::Tag( 340, "variable MinSampleValue"),
    kvs::tiff::Tag( 341, "variable MaxSampleValue"),
    kvs::tiff::Tag( 347, "JPEG table stream")
};

const size_t TagTableSize = sizeof( TagTable ) / sizeof( kvs::tiff::Tag );

} // end of namespace tiff

} // end of namespace kvs
