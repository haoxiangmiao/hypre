/*
 * File:          sidl_PreViolation_Skel.c
 * Symbol:        sidl.PreViolation-v0.9.15
 * Symbol Type:   class
 * Babel Version: 1.0.4
 * Release:       $Name$
 * Revision:      @(#) $Id$
 * Description:   Server-side glue code for sidl.PreViolation
 * 
 * Copyright (c) 2000-2002, The Regents of the University of California.
 * Produced at the Lawrence Livermore National Laboratory.
 * Written by the Components Team <components@llnl.gov>
 * All rights reserved.
 * 
 * This file is part of Babel. For more information, see
 * http://www.llnl.gov/CASC/components/. Please read the COPYRIGHT file
 * for Our Notice and the LICENSE file for the GNU Lesser General Public
 * License.
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License (as published by
 * the Free Software Foundation) version 2.1 dated February 1999.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY OF
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the terms and
 * conditions of the GNU Lesser General Public License for more details.
 * 
 * You should have recieved a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * 
 * WARNING: Automatically generated; changes will be lost
 * 
 */

#include "sidl_PreViolation_IOR.h"
#include "sidl_PreViolation.h"
#include <stddef.h>

extern
void
impl_sidl_PreViolation__load(
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_sidl_PreViolation__ctor(
  /* in */ sidl_PreViolation self,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_sidl_PreViolation__ctor2(
  /* in */ sidl_PreViolation self,
  /* in */ void* private_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_sidl_PreViolation__dtor(
  /* in */ sidl_PreViolation self,
  /* out */ sidl_BaseInterface *_ex);

extern struct sidl_BaseClass__object* 
  impl_sidl_PreViolation_fconnect_sidl_BaseClass(const char* url, sidl_bool ar, 
  sidl_BaseInterface *_ex);
extern struct sidl_BaseClass__object* 
  impl_sidl_PreViolation_fcast_sidl_BaseClass(void* bi, sidl_BaseInterface* 
  _ex);
extern struct sidl_BaseException__object* 
  impl_sidl_PreViolation_fconnect_sidl_BaseException(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex);
extern struct sidl_BaseException__object* 
  impl_sidl_PreViolation_fcast_sidl_BaseException(void* bi, sidl_BaseInterface* 
  _ex);
extern struct sidl_BaseInterface__object* 
  impl_sidl_PreViolation_fconnect_sidl_BaseInterface(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex);
extern struct sidl_BaseInterface__object* 
  impl_sidl_PreViolation_fcast_sidl_BaseInterface(void* bi, sidl_BaseInterface* 
  _ex);
extern struct sidl_ClassInfo__object* 
  impl_sidl_PreViolation_fconnect_sidl_ClassInfo(const char* url, sidl_bool ar, 
  sidl_BaseInterface *_ex);
extern struct sidl_ClassInfo__object* 
  impl_sidl_PreViolation_fcast_sidl_ClassInfo(void* bi, sidl_BaseInterface* 
  _ex);
extern struct sidl_PreViolation__object* 
  impl_sidl_PreViolation_fconnect_sidl_PreViolation(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex);
extern struct sidl_PreViolation__object* 
  impl_sidl_PreViolation_fcast_sidl_PreViolation(void* bi, sidl_BaseInterface* 
  _ex);
extern struct sidl_RuntimeException__object* 
  impl_sidl_PreViolation_fconnect_sidl_RuntimeException(const char* url, 
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_RuntimeException__object* 
  impl_sidl_PreViolation_fcast_sidl_RuntimeException(void* bi, 
  sidl_BaseInterface* _ex);
extern struct sidl_SIDLException__object* 
  impl_sidl_PreViolation_fconnect_sidl_SIDLException(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex);
extern struct sidl_SIDLException__object* 
  impl_sidl_PreViolation_fcast_sidl_SIDLException(void* bi, sidl_BaseInterface* 
  _ex);
extern struct sidl_io_Deserializer__object* 
  impl_sidl_PreViolation_fconnect_sidl_io_Deserializer(const char* url, 
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_io_Deserializer__object* 
  impl_sidl_PreViolation_fcast_sidl_io_Deserializer(void* bi, 
  sidl_BaseInterface* _ex);
extern struct sidl_io_Serializable__object* 
  impl_sidl_PreViolation_fconnect_sidl_io_Serializable(const char* url, 
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_io_Serializable__object* 
  impl_sidl_PreViolation_fcast_sidl_io_Serializable(void* bi, 
  sidl_BaseInterface* _ex);
extern struct sidl_io_Serializer__object* 
  impl_sidl_PreViolation_fconnect_sidl_io_Serializer(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex);
extern struct sidl_io_Serializer__object* 
  impl_sidl_PreViolation_fcast_sidl_io_Serializer(void* bi, sidl_BaseInterface* 
  _ex);
extern struct sidl_BaseClass__object* 
  impl_sidl_PreViolation_fconnect_sidl_BaseClass(const char* url, sidl_bool ar, 
  sidl_BaseInterface *_ex);
extern struct sidl_BaseClass__object* 
  impl_sidl_PreViolation_fcast_sidl_BaseClass(void* bi, sidl_BaseInterface* 
  _ex);
extern struct sidl_BaseException__object* 
  impl_sidl_PreViolation_fconnect_sidl_BaseException(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex);
extern struct sidl_BaseException__object* 
  impl_sidl_PreViolation_fcast_sidl_BaseException(void* bi, sidl_BaseInterface* 
  _ex);
extern struct sidl_BaseInterface__object* 
  impl_sidl_PreViolation_fconnect_sidl_BaseInterface(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex);
extern struct sidl_BaseInterface__object* 
  impl_sidl_PreViolation_fcast_sidl_BaseInterface(void* bi, sidl_BaseInterface* 
  _ex);
extern struct sidl_ClassInfo__object* 
  impl_sidl_PreViolation_fconnect_sidl_ClassInfo(const char* url, sidl_bool ar, 
  sidl_BaseInterface *_ex);
extern struct sidl_ClassInfo__object* 
  impl_sidl_PreViolation_fcast_sidl_ClassInfo(void* bi, sidl_BaseInterface* 
  _ex);
extern struct sidl_PreViolation__object* 
  impl_sidl_PreViolation_fconnect_sidl_PreViolation(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex);
extern struct sidl_PreViolation__object* 
  impl_sidl_PreViolation_fcast_sidl_PreViolation(void* bi, sidl_BaseInterface* 
  _ex);
extern struct sidl_RuntimeException__object* 
  impl_sidl_PreViolation_fconnect_sidl_RuntimeException(const char* url, 
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_RuntimeException__object* 
  impl_sidl_PreViolation_fcast_sidl_RuntimeException(void* bi, 
  sidl_BaseInterface* _ex);
extern struct sidl_SIDLException__object* 
  impl_sidl_PreViolation_fconnect_sidl_SIDLException(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex);
extern struct sidl_SIDLException__object* 
  impl_sidl_PreViolation_fcast_sidl_SIDLException(void* bi, sidl_BaseInterface* 
  _ex);
extern struct sidl_io_Deserializer__object* 
  impl_sidl_PreViolation_fconnect_sidl_io_Deserializer(const char* url, 
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_io_Deserializer__object* 
  impl_sidl_PreViolation_fcast_sidl_io_Deserializer(void* bi, 
  sidl_BaseInterface* _ex);
extern struct sidl_io_Serializable__object* 
  impl_sidl_PreViolation_fconnect_sidl_io_Serializable(const char* url, 
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_io_Serializable__object* 
  impl_sidl_PreViolation_fcast_sidl_io_Serializable(void* bi, 
  sidl_BaseInterface* _ex);
extern struct sidl_io_Serializer__object* 
  impl_sidl_PreViolation_fconnect_sidl_io_Serializer(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex);
extern struct sidl_io_Serializer__object* 
  impl_sidl_PreViolation_fcast_sidl_io_Serializer(void* bi, sidl_BaseInterface* 
  _ex);
#ifdef __cplusplus
extern "C" {
#endif

void
sidl_PreViolation__set_epv(struct sidl_PreViolation__epv *epv)
{
  epv->f__ctor = impl_sidl_PreViolation__ctor;
  epv->f__ctor2 = impl_sidl_PreViolation__ctor2;
  epv->f__dtor = impl_sidl_PreViolation__dtor;

}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

void sidl_PreViolation__call_load(void) { 
  sidl_BaseInterface _throwaway_exception = NULL;
  impl_sidl_PreViolation__load(&_throwaway_exception);
}
struct sidl_BaseClass__object* skel_sidl_PreViolation_fconnect_sidl_BaseClass(
  const char* url, sidl_bool ar, sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fconnect_sidl_BaseClass(url, ar, _ex);
}

struct sidl_BaseClass__object* skel_sidl_PreViolation_fcast_sidl_BaseClass(
  void* bi, sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fcast_sidl_BaseClass(bi, _ex);
}

struct sidl_BaseException__object* 
  skel_sidl_PreViolation_fconnect_sidl_BaseException(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fconnect_sidl_BaseException(url, ar, _ex);
}

struct sidl_BaseException__object* 
  skel_sidl_PreViolation_fcast_sidl_BaseException(void* bi, sidl_BaseInterface 
  *_ex) { 
  return impl_sidl_PreViolation_fcast_sidl_BaseException(bi, _ex);
}

struct sidl_BaseInterface__object* 
  skel_sidl_PreViolation_fconnect_sidl_BaseInterface(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fconnect_sidl_BaseInterface(url, ar, _ex);
}

struct sidl_BaseInterface__object* 
  skel_sidl_PreViolation_fcast_sidl_BaseInterface(void* bi, sidl_BaseInterface 
  *_ex) { 
  return impl_sidl_PreViolation_fcast_sidl_BaseInterface(bi, _ex);
}

struct sidl_ClassInfo__object* skel_sidl_PreViolation_fconnect_sidl_ClassInfo(
  const char* url, sidl_bool ar, sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fconnect_sidl_ClassInfo(url, ar, _ex);
}

struct sidl_ClassInfo__object* skel_sidl_PreViolation_fcast_sidl_ClassInfo(
  void* bi, sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fcast_sidl_ClassInfo(bi, _ex);
}

struct sidl_PreViolation__object* 
  skel_sidl_PreViolation_fconnect_sidl_PreViolation(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fconnect_sidl_PreViolation(url, ar, _ex);
}

struct sidl_PreViolation__object* 
  skel_sidl_PreViolation_fcast_sidl_PreViolation(void* bi, sidl_BaseInterface 
  *_ex) { 
  return impl_sidl_PreViolation_fcast_sidl_PreViolation(bi, _ex);
}

struct sidl_RuntimeException__object* 
  skel_sidl_PreViolation_fconnect_sidl_RuntimeException(const char* url, 
  sidl_bool ar, sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fconnect_sidl_RuntimeException(url, ar, _ex);
}

struct sidl_RuntimeException__object* 
  skel_sidl_PreViolation_fcast_sidl_RuntimeException(void* bi, 
  sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fcast_sidl_RuntimeException(bi, _ex);
}

struct sidl_SIDLException__object* 
  skel_sidl_PreViolation_fconnect_sidl_SIDLException(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fconnect_sidl_SIDLException(url, ar, _ex);
}

struct sidl_SIDLException__object* 
  skel_sidl_PreViolation_fcast_sidl_SIDLException(void* bi, sidl_BaseInterface 
  *_ex) { 
  return impl_sidl_PreViolation_fcast_sidl_SIDLException(bi, _ex);
}

struct sidl_io_Deserializer__object* 
  skel_sidl_PreViolation_fconnect_sidl_io_Deserializer(const char* url, 
  sidl_bool ar, sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fconnect_sidl_io_Deserializer(url, ar, _ex);
}

struct sidl_io_Deserializer__object* 
  skel_sidl_PreViolation_fcast_sidl_io_Deserializer(void* bi, 
  sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fcast_sidl_io_Deserializer(bi, _ex);
}

struct sidl_io_Serializable__object* 
  skel_sidl_PreViolation_fconnect_sidl_io_Serializable(const char* url, 
  sidl_bool ar, sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fconnect_sidl_io_Serializable(url, ar, _ex);
}

struct sidl_io_Serializable__object* 
  skel_sidl_PreViolation_fcast_sidl_io_Serializable(void* bi, 
  sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fcast_sidl_io_Serializable(bi, _ex);
}

struct sidl_io_Serializer__object* 
  skel_sidl_PreViolation_fconnect_sidl_io_Serializer(const char* url, sidl_bool 
  ar, sidl_BaseInterface *_ex) { 
  return impl_sidl_PreViolation_fconnect_sidl_io_Serializer(url, ar, _ex);
}

struct sidl_io_Serializer__object* 
  skel_sidl_PreViolation_fcast_sidl_io_Serializer(void* bi, sidl_BaseInterface 
  *_ex) { 
  return impl_sidl_PreViolation_fcast_sidl_io_Serializer(bi, _ex);
}

struct sidl_PreViolation__data*
sidl_PreViolation__get_data(sidl_PreViolation self)
{
  return (struct sidl_PreViolation__data*)(self ? self->d_data : NULL);
}

void sidl_PreViolation__set_data(
  sidl_PreViolation self,
  struct sidl_PreViolation__data* data)
{
  if (self) {
    self->d_data = data;
  }
}
#ifdef __cplusplus
}
#endif
