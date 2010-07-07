
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2010, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Script.cpp"                                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <sstream>
#include  "hurricane/Error.h"
#include  "hurricane/Cell.h"
#include  "hurricane/isobar/PyCell.h"
#include  "hurricane/isobar/PyCellViewer.h"
#include  "hurricane/isobar/Script.h"


namespace {

  const char* __editorKw = "__editor";

} // End of anonymous namespace.


namespace Isobar {

  using std::string;
  using Hurricane::Cell;
  using Hurricane::CellViewer;


// -------------------------------------------------------------------
// Class  :  "Isobar::Script".


  vector<string>  Script::_pathes;


  void  Script::addPath ( const string& path )
  { _pathes.push_back ( path ); }


  void  Script::removePath ( const string& path )
  {
    vector<string>::iterator ipath = _pathes.begin();
    for ( ; ipath != _pathes.end() ; ++ipath ) {
      if ( (*ipath) == path ) {
        _pathes.erase ( ipath );
        return;
      }
    }
  }


  Script::Script ( const string& name )
    : _moduleName     (name)
    , _sysModule      (NULL)
    , _hurricaneModule(NULL)
    , _userModule     (NULL)
    , _pyFunction     (NULL)
    , _pyArgs         (NULL)
    , _pyResult       (NULL)
    , _cellViewer     (NULL)
  { }


  Script::~Script ()
  {
    _finalize();
    Py_Finalize ();
  }


  Script* Script::create ( const std::string& name )
  {
    Script* script = new Script ( name );
    return script;
  }


  void  Script::destroy ()
  {
    delete this;
  }


  void  Script::setEditor ( CellViewer* viewer )
  { _cellViewer = viewer; }


  bool  Script::runFunction ( const std::string& function, Cell* cell )
  {
    bool returnCode = true;

    _initialize ();

    _userModule = PyImport_ImportModule ( const_cast<char*>(_moduleName.c_str()) );

    if ( _userModule == NULL ) {
      if ( PyErr_Occurred() ) {
        PyErr_Print ();
      }
      _finalize ();
      throw Error("Cannot load python module: <%s>",_moduleName.c_str());
    }

    _setEditor ();

    _pyFunction = PyObject_GetAttrString(_userModule, const_cast<char*>(function.c_str()));
    if ( (_pyFunction == NULL) or not PyCallable_Check(_pyFunction) ) {
      _finalize ();
      throw Error("Python module <%s> doesn't contains any <%s> function."
                 ,_moduleName.c_str(),function.c_str());
    }

    _pyArgs  = PyTuple_New(1);
    if ( cell != NULL ) PyTuple_SetItem ( _pyArgs, 0, (PyObject*)PyCell_Link(cell) );
    else                PyTuple_SetItem ( _pyArgs, 0, Py_None );

    _pyResult = PyEval_CallObject ( _pyFunction, _pyArgs );

    if ( _pyResult == NULL ) {
      cerr << "Something has gone slightly wrong" << endl;
    }

    if ( PyErr_Occurred() ) {
      PyErr_Print ();
      returnCode = false;
    }

    _finalize();

    return true;
  }


  void  Script::_importSys ()
  {
    if ( not Py_IsInitialized() )
      throw Error ( "Script::_importSys(): Called before Py_Initialize()." );

    _sysModule = _importModule ( "sys" );

    PyObject* path = PyObject_GetAttrString ( _sysModule, "path" );
    if ( path == NULL )
      throw Error("Script::_importSys(): No \"sys.path\" attribute.");

    vector<string>::iterator ipath = _pathes.begin();

    for ( ; ipath != _pathes.end() ; ++ipath ) {
      PyObject* element = PyString_FromString ( const_cast<char*>((*ipath).c_str()) );
      PyList_Insert ( path, 0, element );
    }
  }


  void  Script::_importHurricane ()
  {
    if ( not Py_IsInitialized() )
      throw Error ( "Script::_importHurricane(): Called before Py_Initialize()." );

    _hurricaneModule = PyImport_ImportModule ( "Hurricane" );
    if ( _hurricaneModule == NULL )
      throw Error("Script::_importModule(): No Hurricane module.");

    PyModule_AddObject ( _hurricaneModule, const_cast<char*>(__editorKw), Py_None );
  }


  PyObject* Script::_importModule ( const string& moduleName )
  {
    if ( not Py_IsInitialized() )
      throw Error ( "Script::_importModule(): Called before Py_Initialize() while importing <%s>."
                  , moduleName.c_str() );

    PyObject* module = PyImport_ImportModule ( const_cast<char*>(moduleName.c_str()) );
    if ( module == NULL )
      throw Error("Script::_importModule(): No <%s> module.",moduleName.c_str());

    return module;
  }


  void  Script::_initialize ()
  {
    Py_Initialize    ();
    _importSys       ();
    _importHurricane ();
  }


  void  Script::_finalize ()
  {
    if ( not Py_IsInitialized() ) return;

    if ( _pyResult        != NULL ) Py_DECREF ( _pyResult );
    if ( _pyArgs          != NULL ) Py_DECREF ( _pyArgs );
    if ( _pyFunction      != NULL ) Py_DECREF ( _pyFunction );
    if ( _userModule      != NULL ) Py_DECREF ( _userModule );
    if ( _hurricaneModule != NULL ) Py_DECREF ( _hurricaneModule );
    if ( _sysModule       != NULL ) Py_DECREF ( _sysModule );

    Py_Finalize ();

    _userModule       = NULL;
    _sysModule        = NULL;
    _hurricaneModule  = NULL;
    _pyFunction       = NULL;
    _pyArgs           = NULL;
    _pyResult         = NULL;
  }


  void  Script::_setEditor ()
  {
    if ( _userModule == NULL ) return;

    PyObject* dictionary = PyModule_GetDict ( _userModule );

    if ( _cellViewer != NULL ) {
      PyCellViewer* pyCellViewer = PyObject_NEW ( PyCellViewer, &PyTypeCellViewer );
      pyCellViewer->_object = _cellViewer;

      PyDict_SetItemString ( dictionary, __editorKw, (PyObject*)pyCellViewer );
    } else {
      PyDict_SetItemString ( dictionary, __editorKw, Py_None );
    }
  }


} // End of Isobar namespace.