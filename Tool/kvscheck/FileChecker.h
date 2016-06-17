/****************************************************************************/
/**
 *  @file FileChecker.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FileChecker.h 1433 2013-03-17 01:23:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVSCHECK__FILE_CHECKER_H_INCLUDE
#define KVSCHECK__FILE_CHECKER_H_INCLUDE

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
        TIFFFormat         ///< TIFF image format
    };

private:

    std::string m_filename; ///< filename

public:

    FileChecker( const std::string& filename );

    const std::string& filename() const;
    FormatType fileFormat() const;
    friend std::ostream& operator << ( std::ostream& os, const FileChecker& checker );
};

} // end of namespace kvscheck

#endif // KVSCHECK__FILE_CHECKER_H_INCLUDE
