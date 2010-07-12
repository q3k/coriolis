
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PyUpdateSession.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x





# ifndef  __PY_UPDATE_SESSION__
#   define  __PY_UPDATE_SESSION__


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/UpdateSession.h"


namespace  Isobar {


extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyUpdateSession".

  typedef struct {
      PyObject_HEAD
  } PyUpdateSession;




// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeUpdateSession;
  extern PyMethodDef   PyUpdateSession_Methods[];

  extern void  PyUpdateSession_LinkPyType  ();


#define IsPyUpdateSession(v)   ( (v)->ob_type == &PyTypeUpdateSession )
#define PYUPDATESESSION(v)     ( (PyUpdateSession*)(v) )
#define PYUPDATESESSION_O(v)   ( PY_UPDATE_SESSION(v)->_object )


}  // End of extern "C".



}  // End of Isobar namespace.
 



# endif
