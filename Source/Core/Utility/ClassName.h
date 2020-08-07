/****************************************************************************/
/**
 *  @file   ClassName.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#define kvsClassName( this_class )          \
    public:                                 \
    virtual const char* className() const { \
        return #this_class;                 \
    }

#define kvsClassName_without_virtual( this_class ) \
    public:                                        \
    const char* className() const {                \
        return #this_class;                        \
    }
