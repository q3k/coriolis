// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/AllianceLibrary.h"                   |
// +-----------------------------------------------------------------+


#ifndef  CRL_ALLIANCE_LIBRARY_H
#define  CRL_ALLIANCE_LIBRARY_H

#include <string>
#include <vector>
#include "hurricane/Name.h"
#include "hurricane/Slot.h"

namespace Hurricane {
  class Library;
}


namespace CRL {

  using std::vector;

  using Hurricane::Name;
  using Hurricane::Library;
  using Hurricane::Record;
  using Hurricane::_TName;


// -------------------------------------------------------------------
// Class  :  "CRL::AllianceLibrary".

  class AllianceLibrary {

    public:
    // Constructors.
                              AllianceLibrary ();
                              AllianceLibrary ( const Name& path, Library* library=NULL );
    // Operators
             AllianceLibrary& operator=       ( const AllianceLibrary& directory );
             AllianceLibrary& operator=       ( const std::string& path );
    // Accessors
      inline const Name&      getPath         () const;
      inline Library*         getLibrary      () const;
    // Hurricane management.
      inline std::string      _getTypeName    () const;
             std::string      _getString      () const;
             Record*          _getRecord      () const;

    protected:
    // Internal - Attributes.
             Name             _path;
             Library*         _library;

  };


  typedef  vector<AllianceLibrary*>  AllianceLibraries;


// Inline Functions.
  inline const Name&  AllianceLibrary::getPath      () const { return _path; }
  inline Library*     AllianceLibrary::getLibrary   () const { return _library; }
  inline std::string  AllianceLibrary::_getTypeName () const { return _TName("AllianceLibrary"); }


} // CRL namespace.


INSPECTOR_P_SUPPORT(CRL::AllianceLibrary);

#endif  // CRL_ALLIANCE_LIBRARY_H
