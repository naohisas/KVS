/*****************************************************************************/
/**
 *  @file   FileChecker.h
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
#pragma once
#include <string>
#include <iostream>


namespace kvscheck
{

/*==========================================================================*/
/**
 *  File checker class.
 */
/*==========================================================================*/
class FileChecker
{
public:
    enum FormatType
    {
        UnknownFormat = 0, ///< unknown format (not supported format)
        AVSFieldFormat,    ///< AVS Field format
        AVSUcdFormat,      ///< AVS UCD format
        BitmapFormat,      ///< Bitmap image format
        CSVFormat,         ///< CSV format
        DICOMFormat,       ///< DICOM image format
        GISFormat,         ///< GIS format
        GrADSFormat,       ///< GrADS format
        KVSMLFormat,       ///< KVSML format
        PLYFormat,         ///< PLY format
        PBMFormat,         ///< PBM image format
        PGMFormat,         ///< PGM image format
        PPMFormat,         ///< PPM image format
        STLFormat,         ///< STL format
        TIFFFormat,        ///< TIFF image format
        IPLabFormat        ///< IPLab image format
    };

private:
    std::string m_filename; ///< filename

public:
    FileChecker( const std::string& filename );
    const std::string& filename() const { return m_filename; }
    FormatType fileFormat() const;
    friend std::ostream& operator << ( std::ostream& os, const FileChecker& checker );
};

} // end of namespace kvscheck
