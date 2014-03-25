// 
// File:          sidl_BaseInterface.hxx
// Symbol:        sidl.BaseInterface-v0.9.15
// Symbol Type:   interface
// Babel Version: 1.0.4
// Release:       $Name$
// Revision:      @(#) $Id$
// Description:   Client-side glue code for sidl.BaseInterface
// 
// Copyright (c) 2000-2002, The Regents of the University of California.
// Produced at the Lawrence Livermore National Laboratory.
// Written by the Components Team <components@llnl.gov>
// All rights reserved.
// 
// This file is part of Babel. For more information, see
// http://www.llnl.gov/CASC/components/. Please read the COPYRIGHT file
// for Our Notice and the LICENSE file for the GNU Lesser General Public
// License.
// 
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License (as published by
// the Free Software Foundation) version 2.1 dated February 1999.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY OF
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the terms and
// conditions of the GNU Lesser General Public License for more details.
// 
// You should have recieved a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
// 
// WARNING: Automatically generated; changes will be lost
// 
// 

#ifndef included_sidl_BaseInterface_hxx
#define included_sidl_BaseInterface_hxx

#ifndef included_sidl_cxx_hxx
#include "sidl_cxx.hxx"
#endif
// declare class before main #includes
// (this alleviates circular #include guard problems)[BUG#393]
namespace sidl { 

  class BaseInterface;
} // end namespace sidl

// Some compilers need to define array template before the specializations
namespace sidl {
  template<>
  class array< ::sidl::BaseInterface >;
}
// 
// Forward declarations for method dependencies.
// 
namespace sidl { 

  class BaseInterface;
} // end namespace sidl

namespace sidl { 

  class ClassInfo;
} // end namespace sidl

namespace sidl { 

  class RuntimeException;
} // end namespace sidl

#ifndef included_sidl_cxx_hxx
#include "sidl_cxx.hxx"
#endif
#ifndef included_sidl_BaseInterface_IOR_h
#include "sidl_BaseInterface_IOR.h"
#endif
namespace sidl {
  namespace rmi {
    class Call;
    class Return;
    class Ticket;
  }
  namespace rmi {
    class InstanceHandle;
  }
}
namespace sidl { 

  /**
   * Symbol "sidl.BaseInterface" (version 0.9.15)
   * 
   * Every interface in <code>sidl</code> implicitly inherits
   * from <code>BaseInterface</code>, and it is implemented
   * by <code>BaseClass</code> below.
   */
  class BaseInterface: public virtual ::sidl::StubBase {

    //////////////////////////////////////////////////
    // 
    // Special methods for throwing exceptions
    // 

  private:
    static 
    void
    throwException0(
      struct sidl_BaseInterface__object *_exception
    )
      // throws:
    ;

    //////////////////////////////////////////////////
    // 
    // User Defined Methods
    // 

  public:

    /**
     * <p>
     * Add one to the intrinsic reference count in the underlying object.
     * Object in <code>sidl</code> have an intrinsic reference count.
     * Objects continue to exist as long as the reference count is
     * positive. Clients should call this method whenever they
     * create another ongoing reference to an object or interface.
     * </p>
     * <p>
     * This does not have a return value because there is no language
     * independent type that can refer to an interface or a
     * class.
     * </p>
     */
    void
    addRef() ;


    /**
     * Decrease by one the intrinsic reference count in the underlying
     * object, and delete the object if the reference is non-positive.
     * Objects in <code>sidl</code> have an intrinsic reference count.
     * Clients should call this method whenever they remove a
     * reference to an object or interface.
     */
    void
    deleteRef() ;


    /**
     * Return true if and only if <code>obj</code> refers to the same
     * object as this object.
     */
    bool
    isSame (
      /* in */::sidl::BaseInterface iobj
    )
    ;



    /**
     * Return whether this object is an instance of the specified type.
     * The string name must be the <code>sidl</code> type name.  This
     * routine will return <code>true</code> if and only if a cast to
     * the string type name would succeed.
     */
    bool
    isType (
      /* in */const ::std::string& name
    )
    ;



    /**
     * Return the meta-data about the class implementing this interface.
     */
    ::sidl::ClassInfo
    getClassInfo() ;


    //////////////////////////////////////////////////
    // 
    // End User Defined Methods
    // (everything else in this file is specific to
    //  Babel's C++ bindings)
    // 

  public:
    typedef struct sidl_BaseInterface__object ior_t;
    typedef struct sidl_BaseInterface__external ext_t;
    typedef struct sidl_BaseInterface__sepv sepv_t;

    // default constructor
    BaseInterface() { 
    }

    // RMI connect
    static inline ::sidl::BaseInterface _connect( /*in*/ const std::string& url 
      ) { 
      return _connect(url, true);
    }

    // RMI connect 2
    static ::sidl::BaseInterface _connect( /*in*/ const std::string& url, 
      /*in*/ const bool ar  );

    // default destructor
    virtual ~BaseInterface () { }

    // copy constructor
    BaseInterface ( const BaseInterface& original );

    // assignment operator
    BaseInterface& operator= ( const BaseInterface& rhs );

    // conversion from ior to C++ class
    BaseInterface ( BaseInterface::ior_t* ior );

    // Alternate constructor: does not call addRef()
    // (sets d_weak_reference=isWeak)
    // For internal use by Impls (fixes bug#275)
    BaseInterface ( BaseInterface::ior_t* ior, bool isWeak );

    inline ior_t* _get_ior() const throw() {
      return reinterpret_cast< ior_t*>(d_self);
    }

    void _set_ior( ior_t* ptr ) throw () { 
      d_self = reinterpret_cast< void*>(ptr);
    }

    bool _is_nil() const throw () { return (d_self==0); }

    bool _not_nil() const throw () { return (d_self!=0); }

    bool operator !() const throw () { return (d_self==0); }

    static inline const char * type_name() throw () { return 
      "sidl.BaseInterface";}

    static struct sidl_BaseInterface__object* _cast(const void* src);

    // execute member function by name
    void _exec(const std::string& methodName,
               ::sidl::rmi::Call& inArgs,
               ::sidl::rmi::Return& outArgs);

    /**
     * Get the URL of the Implementation of this object (for RMI)
     */
    ::std::string
    _getURL() // throws:
    //     ::sidl::RuntimeException
    ;


    /**
     * Method to set whether or not method hooks should be invoked.
     */
    void
    _set_hooks (
      /* in */bool on
    )
    // throws:
    //     ::sidl::RuntimeException
    ;

    // return true iff object is remote
    bool _isRemote() const { 
      ior_t* self = const_cast<ior_t*>(_get_ior() );
      struct sidl_BaseInterface__object *throwaway_exception;
      return (*self->d_epv->f__isRemote)(self, &throwaway_exception) == TRUE;
    }

    // return true iff object is local
    bool _isLocal() const {
      return !_isRemote();
    }

  protected:
    // Pointer to external (DLL loadable) symbols (shared among instances)
    static const ext_t * s_ext;

  public:
    static const ext_t * _get_ext() throw ( ::sidl::NullIORException );

  }; // end class BaseInterface
} // end namespace sidl

extern "C" {


#pragma weak sidl_BaseInterface__connectI

#pragma weak sidl_BaseInterface__rmicast

  /**
   * Cast method for interface and class type conversions.
   */
  struct sidl_BaseInterface__object*
  sidl_BaseInterface__rmicast(
    void* obj, struct sidl_BaseInterface__object **_ex);

  /**
   * RMI connector function for the class. (no addref)
   */
  struct sidl_BaseInterface__object*
  sidl_BaseInterface__connectI(const char * url, sidl_bool ar, struct 
    sidl_BaseInterface__object **_ex);


} // end extern "C"
namespace sidl {
  // traits specialization
  template<>
  struct array_traits< ::sidl::BaseInterface > {
    typedef array< ::sidl::BaseInterface > cxx_array_t;
    typedef ::sidl::BaseInterface cxx_item_t;
    typedef struct sidl_BaseInterface__array ior_array_t;
    typedef sidl_interface__array ior_array_internal_t;
    typedef struct sidl_BaseInterface__object ior_item_t;
    typedef cxx_item_t value_type;
    typedef value_type reference;
    typedef value_type* pointer;
    typedef const value_type const_reference;
    typedef const value_type* const_pointer;
    typedef array_iter< array_traits< ::sidl::BaseInterface > > iterator;
    typedef const_array_iter< array_traits< ::sidl::BaseInterface > > 
      const_iterator;
  };

  // array specialization
  template<>
  class array< ::sidl::BaseInterface >: public interface_array< array_traits< 
    ::sidl::BaseInterface > > {
  public:
    typedef interface_array< array_traits< ::sidl::BaseInterface > > Base;
    typedef array_traits< ::sidl::BaseInterface >::cxx_array_t          
      cxx_array_t;
    typedef array_traits< ::sidl::BaseInterface >::cxx_item_t           
      cxx_item_t;
    typedef array_traits< ::sidl::BaseInterface >::ior_array_t          
      ior_array_t;
    typedef array_traits< ::sidl::BaseInterface >::ior_array_internal_t 
      ior_array_internal_t;
    typedef array_traits< ::sidl::BaseInterface >::ior_item_t           
      ior_item_t;

    /**
     * conversion from ior to C++ class
     * (constructor/casting operator)
     */
    array( struct sidl_BaseInterface__array* src = 0) : Base(src) {}

    /**
     * copy constructor
     */
    array( const array< ::sidl::BaseInterface >&src) : Base(src) {}

    /**
     * Assignment to promote a generic array to an
     * array of sidl.BaseInterface references. This
     * will produce a nil array if the generic array
     * isn't an array of objects/interfaces.
     */
    array< ::sidl::BaseInterface >&
    operator =(const basearray &rhs) throw() {
      if (this->d_array != rhs._get_baseior()) {
        deleteRef();
        this->d_array =
          (rhs._get_baseior() &&
           (sidl_interface_array == rhs.arrayType()))
          ? const_cast<sidl__array *>(rhs._get_baseior())
          : NULL;
        addRef();
      }
      return *this;
    }
    /**
     * assignment
     */
    array< ::sidl::BaseInterface >&
    operator =( const array< ::sidl::BaseInterface >&rhs ) { 
      if (d_array != rhs._get_baseior()) {
        if (d_array) deleteRef();
        d_array = const_cast<sidl__array *>(rhs._get_baseior());
        if (d_array) addRef();
      }
      return *this;
    }

  };
}

#ifndef included_sidl_ClassInfo_hxx
#include "sidl_ClassInfo.hxx"
#endif
#endif
