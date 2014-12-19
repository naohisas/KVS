/*****************************************************************************/
/**
 *  @file   PlyFile.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PlyFile.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
/*

Header for PLY polygon files.

- Greg Turk

A PLY file contains a single polygonal _object_.

An object is composed of lists of _elements_.  Typical elements are
vertices, faces, edges and materials.

Each type of element for a given object has one or more _properties_
associated with the element type.  For instance, a vertex element may
have as properties three floating-point values x,y,z and three unsigned
chars for red, green and blue.

-----------------------------------------------------------------------

Copyright (c) 1998 Georgia Institute of Technology.  All rights reserved.

Permission to use, copy, modify and distribute this software and its
documentation for any purpose is hereby granted without fee, provided
that the above copyright notice and this permission notice appear in
all copies of this software and that you do not sell the software.

THE SOFTWARE IS PROVIDED "AS IS" AND WITHOUT WARRANTY OF ANY KIND,
EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.

*/
#ifndef KVS__PLY_FILE_H_INCLUDE
#define KVS__PLY_FILE_H_INCLUDE

#include <cstdio>
#include <cstddef>


namespace kvs
{

namespace ply
{

#define PLY_ASCII      1        /* ascii PLY file */
#define PLY_BINARY_BE  2        /* binary PLY file, big endian */
#define PLY_BINARY_LE  3        /* binary PLY file, little endian */

#define PLY_OKAY    0           /* ply routine worked okay */
#define PLY_ERROR  -1           /* error in ply routine */

/* scalar data types supported by PLY format */

#define PLY_START_TYPE 0
#define PLY_CHAR       1
#define PLY_SHORT      2
#define PLY_INT        3
#define PLY_UCHAR      4
#define PLY_USHORT     5
#define PLY_UINT       6
#define PLY_FLOAT      7
#define PLY_DOUBLE     8
#define PLY_END_TYPE   9

#define PLY_SCALAR  0
#define PLY_LIST    1

typedef struct PlyProperty {    /* description of a property */
    const char *name;                           /* property name */
    int external_type;                    /* file's data type */
    int internal_type;                    /* program's data type */
    int offset;                           /* offset bytes of prop in a struct */

    int is_list;                          /* 1 = list, 0 = scalar */
    int count_external;                   /* file's count type */
    int count_internal;                   /* program's count type */
    int count_offset;                     /* offset byte for list count */

} PlyProperty;

typedef struct PlyElement {     /* description of an element */
    char *name;                   /* element name */
    int num;                      /* number of elements in this object */
    int size;                     /* size of element (bytes) or -1 if variable */
    int nprops;                   /* number of properties for this element */
    PlyProperty **props;          /* list of properties in the file */
    char *store_prop;             /* flags: property wanted by user? */
    int other_offset;             /* offset to un-asked-for props, or -1 if none*/
    int other_size;               /* size of other_props structure */
} PlyElement;

typedef struct PlyOtherProp {   /* describes other properties in an element */
    char *name;                   /* element name */
    int size;                     /* size of other_props */
    int nprops;                   /* number of properties in other_props */
    PlyProperty **props;          /* list of properties in other_props */
} PlyOtherProp;

typedef struct OtherData { /* for storing other_props for an other element */
    void *other_props;
} OtherData;

typedef struct OtherElem {     /* data for one "other" element */
    char *elem_name;             /* names of other elements */
    int elem_count;              /* count of instances of each element */
    OtherData **other_data;      /* actual property data for the elements */
    PlyOtherProp *other_props;   /* description of the property data */
} OtherElem;

typedef struct PlyOtherElems {  /* "other" elements, not interpreted by user */
    int num_elems;                /* number of other elements */
    OtherElem *other_list;        /* list of data for other elements */
} PlyOtherElems;

typedef struct PlyFile {        /* description of PLY file */
    FILE *fp;                     /* file pointer */
    int file_type;                /* ascii or binary */
    float version;                /* version number of file */
    int nelems;                   /* number of elements of object */
    PlyElement **elems;           /* list of elements */
    int num_comments;             /* number of comments */
    char **comments;              /* list of comments */
    int num_obj_info;             /* number of items of object information */
    char **obj_info;              /* list of object info items */
    PlyElement *which_elem;       /* which element we're currently writing */
    PlyOtherElems *other_elems;   /* "other" elements from a PLY file */
} PlyFile;

//standard PLY library interface
PlyFile *ply_write(FILE *, int, const char **, int);
PlyFile *ply_open_for_writing(const char *, int, const char **, int, float *);
void ply_describe_element(PlyFile *, const char *, int, int, PlyProperty *);
void ply_describe_property(PlyFile *, const char *, PlyProperty *);
void ply_element_count(PlyFile *, const char *, int);
void ply_header_complete(PlyFile *);
void ply_put_element_setup(PlyFile *, const char *);
void ply_put_element(PlyFile *, void *);
void ply_put_comment(PlyFile *, const char *);
void ply_put_obj_info(PlyFile *, const char *);
PlyFile *ply_read(FILE *, int *, char ***);
PlyFile *ply_open_for_reading( const char *, int *, char ***, int *, float *);
PlyElement *ply_get_element_description(PlyFile *, char *, int*, int*);
void ply_get_element_setup( PlyFile *, char *, int, PlyProperty *);
void ply_get_property(PlyFile *, char *, PlyProperty *);
PlyOtherProp *ply_get_other_properties(PlyFile *, char *, int);
void ply_get_element(PlyFile *, void *);
char **ply_get_comments(PlyFile *, int *);
char **ply_get_obj_info(PlyFile *, int *);
void ply_close(PlyFile *);
void ply_get_info(PlyFile *, float *, int *);
PlyOtherElems *ply_get_other_element (PlyFile *, char *, int);
void ply_describe_other_elements ( PlyFile *, PlyOtherElems *);
void ply_put_other_elements (PlyFile *);
void ply_free_other_elements (PlyOtherElems *);
void ply_describe_other_properties(PlyFile *, PlyOtherProp *, int);

// These methods are internal to the PLY library in the normal distribution
// They should be used carefully
int equal_strings(const char *, const char *);
PlyElement *find_element(PlyFile *, const char *);
PlyProperty *find_property(PlyElement *, const char *, int *);
void write_scalar_type (FILE *, int);
char **get_words(FILE *, int *, char **);
char **old_get_words(FILE *, int *);
void write_binary_item(PlyFile *, int, unsigned int, double, int);
void write_ascii_item(FILE *, int, unsigned int, double, int);
double old_write_ascii_item(FILE *, char *, int);
void add_element(PlyFile *, char **, int);
void add_property(PlyFile *, char **, int);
void add_comment(PlyFile *, char *);
void add_obj_info(PlyFile *, char *);
void copy_property(PlyProperty *, PlyProperty *);
void store_item(char *, int, int, unsigned int, double);
void get_stored_item( void *, int, int *, unsigned int *, double *);
double get_item_value(char *, int);
void get_ascii_item(char *, int, int *, unsigned int *, double *);
void get_binary_item(PlyFile *, int, int *, unsigned int *, double *);
void ascii_get_element(PlyFile *, char *);
void binary_get_element(PlyFile *, char *);
char *my_alloc(int, int, const char *);
int get_prop_type(char *);

} // end of namespace ply

} // end of namespace kvs

#endif // KVS__PLY_FILE_H_INCLUDE
