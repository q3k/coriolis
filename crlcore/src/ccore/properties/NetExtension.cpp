
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./NetExtension.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/Error.h"
#include  "hurricane/Net.h"
#include  "hurricane/Cell.h"

#include  "crlcore/NetExtension.h"


namespace CRL {


  using Hurricane::Error;
  using Hurricane::ForEachIterator;


  const char* MissingNetExtension = "NetExtension::%s(): %s missing the Net extension.";


  template<>
  Name  StandardPrivateProperty<NetExtensionDatas>::_name = "CRL::NetExtension";

  
// -------------------------------------------------------------------
// Class  :  "CRL::NetExtensionDatas".


  NetExtensionDatas::NetExtensionDatas ()
    : _ports()
  { }

  
// -------------------------------------------------------------------
// Class  :  "CRL::NetExtension".


  const Name& NetExtension::getPort ( const Net* net )
  {
    Extension* extension = Extension::get ( net );
    if ( !extension )
      return Name::emptyName();

    return *(extension->getValue()._ports.begin());
  }


  const PortSet* NetExtension::getPorts ( const Net* net )
  {
    Extension* extension = Extension::get ( net );
    if ( !extension )
      return NULL;

    return & ( extension->getValue()._ports );
  }


  const Net* NetExtension::getNetByPort ( const Cell* cell, const Name& port )
  {
    Extension* extension = Extension::get ( cell );
    if ( !extension )
      return NULL;

    forEach ( Net*, inet, cell->getExternalNets() ) {
      const Name& netPort = getPort ( *inet );
      if ( !netPort.isEmpty() && ( netPort == port ) )
        return *inet;
    }

    return NULL;
  }


  void  NetExtension::addPort ( Net* net, const Name& name )
  {
    Extension* extension = _getOrCreate ( net );

    extension->getValue()._ports.insert ( name );
  }


  NetExtension::Extension* NetExtension::_getOrCreate ( Net* net )
  {
    Extension* extension = Extension::get ( net );
    if ( !extension ) {
      extension = Extension::create ();
      net->put ( extension );
    }
    return extension;
  }


} // End of CRL namespace.
