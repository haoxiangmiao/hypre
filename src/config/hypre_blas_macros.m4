dnl #*BHEADER********************************************************************
dnl # Copyright (c) 2008,  Lawrence Livermore National Security, LLC.
dnl # Produced at the Lawrence Livermore National Laboratory.
dnl # This file is part of HYPRE.  See file COPYRIGHT for details.
dnl #
dnl # HYPRE is free software; you can redistribute it and/or modify it under the
dnl # terms of the GNU Lesser General Public License (as published by the Free
dnl # Software Foundation) version 2.1 dated February 1999.
dnl #
dnl # $Revision$
dnl #EHEADER*********************************************************************




dnl @synopsis AC_HYPRE_FIND_BLAS([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
dnl
dnl This macro looks for a library that implements the BLAS
dnl linear-algebra interface (see http://www.netlib.org/blas/).
dnl On success, it sets the BLASLIBS output variable to
dnl hold the requisite library linkages.
dnl
dnl To link with BLAS, you should link with:
dnl
dnl 	$BLASLIBS $LIBS $FCLIBS
dnl
dnl in that order.  FCLIBS is the output variable of the
dnl AC_FC_LIBRARY_LDFLAGS macro, and is sometimes necessary in order to link
dnl with fortran libraries.
dnl
dnl Many libraries are searched for, from ATLAS to CXML to ESSL.
dnl The user may specify a BLAS library by using the --with-blas-libs=<lib>
dnl and --with-blas-lib-dirs=<dir> options.  In order to link successfully,
dnl however, be aware that you will probably need to use the same Fortran
dnl compiler (which can be set via the FC env. var.) as was used to compile
dnl the BLAS library.
dnl
dnl ACTION-IF-FOUND is a list of shell commands to run if a BLAS
dnl library is found, and ACTION-IF-NOT-FOUND is a list of commands
dnl to run it if it is not found. 
dnl
dnl This macro requires autoconf 2.50 or later.
dnl
dnl @version $Id$
dnl @author Steven G. Johnson <stevenj@alum.mit.edu>

AC_DEFUN([AC_HYPRE_FIND_BLAS],
[
  AC_REQUIRE([AC_FC_LIBRARY_LDFLAGS])

#***************************************************************
#   Initialize return variables
#***************************************************************
  BLASLIBS="null"
  BLASLIBDIRS="null"

  AC_ARG_WITH(blas,
	[AS_HELP_STRING([--with-blas], [Find a system-provided BLAS library])])

  case $with_blas in
      yes) ;;
        *) BLASLIBS="internal" ;;
  esac

#***************************************************************
#   Save incoming LIBS and LDFLAGS values to be restored 
#***************************************************************
  hypre_save_LIBS="$LIBS"
  hypre_save_LDFLGS="$LDFLAGS"
  LIBS="$LIBS  -nodefaultlibs -L/usr/local/Cellar/gcc/5.3.0/lib/gcc/5/liblgfortran.a -lm"

#***************************************************************
#   Set possible BLAS library names
#***************************************************************
  BLAS_LIB_NAMES="blas essl dxml cxml mkl scs atlas complib.sgimath sunmath"

#***************************************************************
#   Set search paths for BLAS library
#***************************************************************
  temp_FLAGS="-L/usr/local2/lib -L/usr/lib2 -L/lib -L/opt/intel/mkl70/lib/32"
  LDFLAGS="-nodefaultlibs $temp_FLAGS $LDFLAGS"

#***************************************************************
#   Check for function dgemm in BLAS_LIB_NAMES
#***************************************************************
  if test "$BLASLIBS" = "null"; then
     AC_FC_FUNC(dgemm)
     for lib in $BLAS_LIB_NAMES; do
        AC_CHECK_LIB($lib, $dgemm, [BLASLIBS=$lib])
    done
  fi

#***************************************************************
#   Set path to selected BLAS library 
#***************************************************************
  BLAS_SEARCH_DIRS="/usr/lib /usr/local/lib /lib"

  if test "$BLASLIBS" != "null"; then
     for dir in $BLAS_SEARCH_DIRS; do
         if test "$BLASLIBDIRS" = "null" -a -f $dir/lib$BLASLIBS.a; then
            BLASLIBDIRS=$dir
         fi

         if test "$BLASLIBDIRS" = "null" -a -f $dir/lib$BLASLIBS.so; then
            BLASLIBDIRS=$dir
         fi
     done
  fi

#***************************************************************
#   Set variables if ATLAS or DXML libraries are used 
#***************************************************************
  if test "$BLASLIBS" = "dxml"; then
     AC_DEFINE(HYPRE_USING_DXML, 1, [Using dxml for Blas])
  fi

  if test "$BLASLIBS" = "essl"; then
     AC_DEFINE(HYPRE_USING_ESSL, 1, [Using essl for Blas])
  fi

#***************************************************************
#   Add -L and -l prefixes if values found
#***************************************************************
  if test "$BLASLIBS" != "null" -a "$BLASLIBS" != "internal"; then
     BLASLIBS="-l$BLASLIBS"
  fi

  if test "$BLASLIBDIRS" != "null"; then
     BLASLIBDIRS="-L$BLASLIBDIRS"
  fi

#***************************************************************
#   Restore incoming LIBS and LDFLAGS values
#***************************************************************
  LIBS="$hypre_save_LIBS"
  LDFLAGS="$hypre_save_LDFLGS"

])dnl AC_HYPRE_FIND_BLAS

dnl @synopsis AC_HYPRE_CHECK_USER_BLASLIBS
dnl
dnl This macro checks that the user-provided blas library is 
dnl linkable. Configure fails with an error message if this 
dnl check fails.
dnl
dnl To link with BLAS, you should link with:
dnl
dnl 	$BLASLIBS $LIBS $FCLIBS
dnl
dnl in that order.  FCLIBS is the output variable of the
dnl AC_FC_LIBRARY_LDFLAGS macro, and is sometimes necessary in order to link
dnl with Fortran libraries.
dnl
dnl The user may specify a BLAS library by using the --with-blas-lib=<lib>, or 
dnl --with-blas-libs=<lib> and --with-blas-lib-dirs=<dir> options.  In order to link successfully,
dnl however, be aware that you will probably need to use the same Fortran
dnl compiler (which can be set via the FC env. var.) as was used to compile
dnl the BLAS library.
dnl
dnl @author Daniel Osei-Kuffuor  <oseikuffuor1@llnl.gov>

AC_DEFUN([AC_HYPRE_CHECK_USER_BLASLIBS],
[
  AC_REQUIRE([AC_FC_LIBRARY_LDFLAGS])
dnl **************************************************************
dnl Define some variables
dnl **************************************************************
  hypre_blas_link_ok=""
dnl **************************************************************
dnl Get fortran linker name for test function (dgemm in this case)
dnl **************************************************************
  AC_FC_FUNC(dgemm)
dnl **************************************************************
dnl Get user provided path-to-blas library
dnl **************************************************************
dnl  LDBLASLIBDIRS="$BLASLIBDIRS"
  USERBLASLIBS="$BLASLIBS"
  USERBLASLIBDIRS="$BLASLIBDIRS"  
  BLASLIBPATHS="$BLASLIBDIRS"
  BLASLIBNAMES=""
  SUFFIXES=""

dnl Case where explicit path could be given by the user
  for blas_lib in $BLASLIBS; do
    [blas_lib_name=${blas_lib##*-l}]
    if test $blas_lib = $blas_lib_name;
    then
dnl      if test -f $blas_lib; 
dnl      then
dnl         [libsuffix=${blas_lib##*.}]
dnl         SUFFIXES="$SUFFIXES $libsuffix"
dnl         [dir_path=${blas_lib%/*}]
dnl         BLASLIBPATHS="-L$dir_path $BLASLIBPATHS"
dnl         [blas_lib_name=${blas_lib_name%%.*}]                  
dnl         [blas_lib_name=${blas_lib_name##*/}]  
dnl         [blas_lib_name=${blas_lib_name#*lib}]
dnl         BLASLIBNAMES="$BLASLIBNAMES $blas_lib_name"
dnl      else
dnl         AC_MSG_ERROR([**************** Invalid path to blas library error: ***************************
dnl         User set BLAS library path using either --with-blas-lib=<lib>, or 
dnl        --with-blas-libs=<blas_lib_base_name> and --with-blas_dirs=<path-to-blas-lib>, 
dnl         but the path " $blas_lib " 
dnl         in the user-provided path for --with-blas-libs does not exist. Please
dnl         check that the provided path is correct.
dnl         *****************************************************************************************],[9])         
dnl      fi

dnl         if [[ $blas_lib = /* ]] ;
dnl         then
dnl            if test -f $blas_lib;
dnl            then
dnl               [dir_path=${blas_lib%/*}]
dnl               BLASLIBPATHS="$BLASLIBPATHS -L$dir_path"
dnl               [blas_lib_name=${blas_lib_name%.*}]
dnl               [blas_lib_name=${blas_lib_name##*/}]
dnl               [blas_lib_name=${blas_lib_name#*lib}]
dnl               BLASLIBNAMES="$BLASLIBNAMES $blas_lib_name"
dnl            else
dnl               AC_MSG_ERROR([**************** Invalid path to blas library error: ***************************
dnl               User set BLAS library path using either --with-blas-lib=<lib>, or 
dnl              --with-blas-libs=<blas_lib_base_name> and --with-blas_dirs=<path-to-blas-lib>, 
dnl               but the path " $blas_lib " 
dnl               in the user-provided path for --with-blas-libs does not exist. Please
dnl               check that the provided path is correct.
dnl               *****************************************************************************************],[9])              
dnl            fi
dnl         else
            BLASLIBPATHS="$dir_path $BLASLIBPATHS"
dnl         fi
    else
      BLASLIBNAMES="$BLASLIBNAMES $blas_lib_name"
    fi
  done
    
dnl **************************************************************
dnl Save current LIBS and LDFLAGS to be restored later 
dnl **************************************************************
    hypre_saved_LIBS="$LIBS"
    hypre_saved_LDFLAGS="$LDFLAGS"
    LIBS="$LIBS $FCLIBS"
    LDFLAGS="$BLASLIBPATHS $LDFLAGS"

dnl    echo LDFLAGS=$LDFLAGS
dnl    echo LIBS=$LIBS
dnl    echo BLASLIBPATHS=$BLASLIBPATHS
dnl **************************************************************
dnl Check for dgemm in linkable list of libraries
dnl **************************************************************
    if test "x$BLASLIBNAMES" != "x"; then
       hypre_blas_link_ok=no
    fi
    for blas_lib in $BLASLIBNAMES; do
dnl **************************************************************
dnl Check if library works and print result
dnl **************************************************************                   
dnl        AC_CHECK_LIB($blas_lib, $dgemm, [BLASLIBS=$blas_lib])
        AC_CHECK_LIB($blas_lib, $dgemm, [hypre_blas_link_ok=yes])

dnl      fi
    done

    if test "$hypre_blas_link_ok" = "no"; then
      AC_MSG_ERROR([**************** Non-linkable blas library error: ***************************
      User set BLAS library path using either --with-blas-lib=<lib>, or 
      --with-blas-libs=<blas_lib_base_name> and --with-blas_dirs=<path-to-blas-lib>, 
      but $USERBLASLIBDIRS $USERBLASLIBS provided cannot be used. See "configure --help" for usage details.
      *****************************************************************************************],[9])
    fi

dnl **************************************************************
dnl Restore LIBS and LDFLAGS
dnl **************************************************************
    LIBS="$hypre_saved_LIBS"
    LDFLAGS="$hypre_saved_LDFLAGS" 
dnl  fi
])
dnl Done with macro AC_HYPRE_CHECK_USER_BLASLIBS
  