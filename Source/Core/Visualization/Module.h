/****************************************************************************/
/**
 *  @file   Module.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Module.h 1795 2014-08-01 08:38:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__MODULE_H_INCLUDE
#define KVS__MODULE_H_INCLUDE

#include <cstring>
#include <kvs/SharedPointer>

#define KVS_MODULE_OBJECT kvs::ObjectBase::ModuleTag
#define KVS_MODULE_IMPORTER kvs::ImporterBase::ModuleTag
#define KVS_MODULE_EXPORTER kvs::ExporterBase::ModuleTag
#define KVS_MODULE_FILTER kvs::FilterBase::ModuleTag
#define KVS_MODULE_MAPPER kvs::MapperBase::ModuleTag
#define KVS_MODULE_RENDERER kvs::RendererBase::ModuleTag

#define kvsModuleBase( this_class )                                     \
    public:                                                             \
    struct  ModuleTag{};                                                \
    typedef ModuleTag ModuleCategory;                                   \
    typedef kvs::SharedPointer<this_class > Pointer;                    \
    virtual const char* moduleName() const { return #this_class; }

#define kvsModule( this_class, category )                               \
    public:                                                             \
    typedef kvs:: category##Base::ModuleTag ModuleCategory;             \
    typedef kvs::SharedPointer<this_class > Pointer;                    \
    virtual const char* moduleName() const { return #this_class; }      \
    static Pointer DownCast( kvs:: category##Base::Pointer& m )         \
    {                                                                   \
        typedef this_class T_;                                          \
        typedef kvs:: category##Base U_;                                \
        return kvs::dynamic_pointer_cast<T_,U_>( m );                   \
    };                                                                  \
    static const Pointer DownCast( const kvs:: category##Base::Pointer& m ) \
    {                                                                   \
        typedef this_class T_;                                          \
        typedef kvs:: category##Base U_;                                \
        return kvs::dynamic_pointer_cast<T_,U_>( kvs::const_pointer_cast<U_,U_>( m ) ); \
    };                                                                  \
    static this_class* DownCast( kvs:: category##Base* m )              \
    {                                                                   \
        return dynamic_cast<this_class *>( m );                         \
    };                                                                  \
    static const this_class* DownCast( const kvs:: category##Base* m )  \
    {                                                                   \
        return dynamic_cast<this_class *>( const_cast<kvs:: category##Base *>( m ) ); \
    }

// TODO: Remove dynamic_cast from the DownCast method.
// The implementation of the module in KVS need to be changed in order
// to remove dynamic_cast from the DownCast method. Here is a simple
// implementation of the DownCast method without dynamic_cast. In the
// current implementation of the module, the module name of the object
// module will be inherited from the importer, filter or mapper modules.
// Therefore, moduleName() cannot be compared with #this_class in the
// following code.
//
//    static this_class* DownCast( kvs:: category##Base* m )
//    {
//        if ( m && strcmp( m->moduleName(), #this_class ) )
//        {
//            return static_cast<this_class *>( m );
//        }
//        return NULL;
//    };

/* DEPRECATED */
#define kvsModuleCategory( module_category )                            \
    public:                                                             \
    typedef kvs:: module_category##Base::ModuleTag ModuleCategory

/* DEPRECATED */
#define kvsModuleName( module_name )            \
    public:                                     \
    virtual const char* moduleName() const {    \
        return #module_name;                    \
    }

/* DEPRECATED */
#define kvsModuleBaseClass( base_class )        \
    typedef base_class BaseClass

/* DEPRECATED */
#define kvsModuleSuperClass( super_class )      \
    typedef super_class SuperClass

namespace kvs
{

template <typename T>
struct ModuleTraits
{
    typedef typename T::ModuleCategory ModuleCategory;
};

} // end of namespace kvs

#endif // KVS__MODULE_H_INCLUDE
