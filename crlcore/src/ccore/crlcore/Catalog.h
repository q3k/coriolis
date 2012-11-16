
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/Catalog.h"                           |
// +-----------------------------------------------------------------+


#ifndef __CRL_CATALOG_H__
#define __CRL_CATALOG_H__

#include  <string>
#include  <map>

#include  "hurricane/Name.h"
#include  "hurricane/Property.h"
#include  "hurricane/Slot.h"

namespace Hurricane {
  class Cell;
  class Library;
}


namespace CRL {


  using std::string;
  using std::map;
  using Hurricane::_TName;
  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::PrivateProperty;
  using Hurricane::DBo;
  using Hurricane::Cell;
  using Hurricane::Library;
  using Hurricane::Property;


  extern const char* MissingStateProperty;


// -------------------------------------------------------------------
// Class  :  "CRL::Catalog".


  class Catalog {

    public:
      class State;
    public:
      inline                   Catalog      ();
                              ~Catalog      ();
             State*            getState     ( const Name& name, bool add=false );
             void              mergeState   ( const Name& name, const State& other );
             bool              deleteState  ( const Name& name );
             void              clear        ();
             bool              loadFromFile ( const string& path, Library* library );
      inline map<Name,State*>* getStates    ();
             string            _getPrint    () const;
      inline string            _getTypeName () const;
             string            _getString   () const;
             Record*           _getRecord   () const;

    public:
    // Sub-Class: State.
      class State {
        public:
        // Flags Constants.
          enum Flags { FlattenLeaf = 1 << 0
                     , Feed        = 1 << 1
                     , Pad         = 1 << 2
                     , GDS         = 1 << 3
                     , Delete      = 1 << 4
                     , Logical     = 1 << 5
                     , Physical    = 1 << 6
                     , InMemory    = 1 << 7
                     , Views       = Physical|Logical
                     };
        // Constructors.
          inline               State          ();
                              ~State          ();
        // Predicates.
          inline bool          isFlattenLeaf  () const;
          inline bool          isFeed         () const;
          inline bool          isPad          () const;
          inline bool          isGds          () const;
          inline bool          isDelete       () const;
          inline bool          isPhysical     () const;
          inline bool          isLogical      () const;
        // Flags management.
          inline unsigned int  getFlags       ( unsigned int mask=(unsigned int)-1 ) const;
          inline bool          setFlags       ( unsigned int mask, bool value );
          inline bool          setFlattenLeaf ( bool value );
          inline bool          setFeed        ( bool value );
          inline bool          setPad         ( bool value );
          inline bool          setGds         ( bool value );
          inline bool          setDelete      ( bool value );
          inline bool          setPhysical    ( bool value );
          inline bool          setLogical     ( bool value );
        // Accessors.
          inline Cell*         getCell        () const;
          inline Library*      getLibrary     () const;
          inline unsigned int  getDepth       () const;
        // Modifiers.
          inline void          merge          ( const State& other );
          inline Cell*         setCell        ( Cell* cell );
          inline Library*      setLibrary     ( Library* library );
          inline void          setDepth       ( unsigned int depth );
        // Hurricane Management.
          inline string        _getTypeName   () const;
                 string        _getString     () const;
                 Record*       _getRecord     () const;

        private:
        // Internal - Attributes.
                 unsigned int  _flags;
                 unsigned int  _depth;
                 Cell*         _cell;
                 Library*      _library;
    };

    private:
    // Attributes.
      map<Name,State*>  _states;

    private:
                   Catalog  ( const Catalog& );
      static bool  readLine ( const string& s, string& name, State* state );

  };


// -------------------------------------------------------------------
// Class  :  "CRL::CatalogProperty".


  class CatalogProperty : public PrivateProperty {

    public:
      static  Name             _name;
    public:
      static  CatalogProperty* create          ( Catalog::State* state );
      static  Name             getPropertyName ();
      virtual Name             getName         () const;
      inline  Catalog::State*  getState        () const;
      inline  void             setState        ( Catalog::State* state );
      virtual void             onReleasedBy    ( DBo* owner );
      virtual string           _getTypeName    () const;
      virtual string           _getString      () const;
      virtual Record*          _getRecord      () const;

    protected:
    // Attributes.
      Catalog::State*  _state;
      
    protected:
    // Constructor.
      inline  CatalogProperty ( Catalog::State* state );
  };


// -------------------------------------------------------------------
// Inline Functions. 

  inline                   Catalog::State::State            () : _flags(0), _depth(1), _cell(NULL), _library(NULL) { }
  inline bool              Catalog::State::isFlattenLeaf    () const { return (_flags&FlattenLeaf)?1:0; }
  inline bool              Catalog::State::isFeed           () const { return (_flags&Feed       )?1:0; }
  inline bool              Catalog::State::isPad            () const { return (_flags&Pad        )?1:0; }
  inline bool              Catalog::State::isGds            () const { return (_flags&GDS        )?1:0; }
  inline bool              Catalog::State::isDelete         () const { return (_flags&Delete     )?1:0; }
  inline bool              Catalog::State::isPhysical       () const { return (_flags&Physical   )?1:0; }
  inline bool              Catalog::State::isLogical        () const { return (_flags&Logical    )?1:0; }
  inline unsigned int      Catalog::State::getFlags         ( unsigned int mask ) const { return ( _flags & mask ); }
  inline bool              Catalog::State::setFlags         ( unsigned int mask, bool value ) {
                                                              switch ( value ) {
                                                                case true:  _flags |=  mask; break;
                                                                case false: _flags &= ~mask; break;
                                                              }
                                                              return ( (_flags&mask) ? true : false );
                                                            }
  inline bool              Catalog::State::setFlattenLeaf   ( bool value ) { return setFlags(FlattenLeaf,value); }
  inline bool              Catalog::State::setFeed          ( bool value ) { return setFlags(Feed       ,value); }
  inline bool              Catalog::State::setPad           ( bool value ) { return setFlags(Pad        ,value); }
  inline bool              Catalog::State::setGds           ( bool value ) { return setFlags(GDS        ,value); }
  inline bool              Catalog::State::setDelete        ( bool value ) { return setFlags(Delete     ,value); }
  inline bool              Catalog::State::setPhysical      ( bool value ) { return setFlags(Physical   ,value); }
  inline bool              Catalog::State::setLogical       ( bool value ) { return setFlags(Logical    ,value); }
  inline Cell*             Catalog::State::setCell          ( Cell* cell ) { return _cell = cell; }
  inline Library*          Catalog::State::setLibrary       ( Library* library ) { return _library = library; }
  inline void              Catalog::State::setDepth         ( unsigned int depth ) { _depth = depth; }
  inline Cell*             Catalog::State::getCell          () const { return _cell; }
  inline Library*          Catalog::State::getLibrary       () const { return _library; }
  inline unsigned int      Catalog::State::getDepth         () const { return _depth; }
  inline string            Catalog::State::_getTypeName     () const { return _TName("Catalog::State"); }

  inline                   Catalog::Catalog                 () : _states() {}
  inline map<Name,Catalog::State*>*
                           Catalog::getStates               () { return &_states; }
  inline string            Catalog::_getTypeName            () const { return _TName("Catalog"); }

  inline                   CatalogProperty::CatalogProperty ( Catalog::State* state ) : PrivateProperty(), _state(state) {}
  inline Catalog::State*   CatalogProperty::getState        () const { return _state; }
  inline void              CatalogProperty::setState        ( Catalog::State* state ) { _state = state; }


// -------------------------------------------------------------------
// Class  :  "CRL::CatalogExtension".


  class CatalogExtension {
    public:
      static inline bool             isFlattenLeaf  ( const Cell* );
      static inline bool             isFeed         ( const Cell* );
      static inline bool             isPad          ( const Cell* );
      static inline bool             isGds          ( const Cell* );
      static inline bool             isDelete       ( const Cell* );
      static inline bool             isPhysical     ( const Cell* );
      static inline bool             isLogical      ( const Cell* );
    // Flags management.             
      static inline unsigned int     getFlags       ( const Cell*, unsigned int mask=(unsigned int)-1 );
      static inline bool             setFlags       ( const Cell*, unsigned int mask, bool value );
      static inline bool             setFlattenLeaf ( const Cell*, bool value );
      static inline bool             setFeed        ( const Cell*, bool value );
      static inline bool             setPad         ( const Cell*, bool value );
      static inline bool             setGds         ( const Cell*, bool value );
      static inline bool             setDelete      ( const Cell*, bool value );
      static inline bool             setPhysical    ( const Cell*, bool value );
      static inline bool             setLogical     ( const Cell*, bool value );
    // Accessors.
      static inline Library*         getLibrary     ( const Cell* );
      static inline unsigned int     getDepth       ( const Cell* );
    // Modifiers.                    
      static inline Library*         setLibrary     ( const Cell*, Library* library );
      static inline void             setDepth       ( const Cell*, unsigned int depth );
    private:
      static        Catalog::State*  _get           ( const Cell* );
    private:
      static const Cell*     _owner;
      static Catalog::State* _cache;
  };


  inline bool  CatalogExtension::isFlattenLeaf ( const Cell* cell )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->isFlattenLeaf();
  }


  inline bool  CatalogExtension::isFeed ( const Cell* cell )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->isFeed();
  }


  inline bool  CatalogExtension::isGds ( const Cell* cell )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->isGds();
  }


  inline bool  CatalogExtension::isPad ( const Cell* cell )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->isPad();
  }


  inline bool  CatalogExtension::isDelete ( const Cell* cell )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->isDelete();
  }


  inline bool  CatalogExtension::isPhysical ( const Cell* cell )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->isPhysical();
  }


  inline bool  CatalogExtension::isLogical ( const Cell* cell )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->isLogical();
  }


  inline unsigned int  CatalogExtension::getFlags ( const Cell* cell, unsigned int mask )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? 0 : state->getFlags();
  }


  inline bool  CatalogExtension::setFlags ( const Cell* cell, unsigned int mask, bool value )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->setFlags(mask,value);
  }


  inline bool  CatalogExtension::setFlattenLeaf ( const Cell* cell, bool value )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->setFlattenLeaf(value);
  }


  inline bool  CatalogExtension::setFeed ( const Cell* cell, bool value )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->setFeed(value);
  }


  inline bool  CatalogExtension::setPad ( const Cell* cell, bool value )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->setPad(value);
  }


  inline bool  CatalogExtension::setGds ( const Cell* cell, bool value )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->setGds(value);
  }


  inline bool  CatalogExtension::setDelete ( const Cell* cell, bool value )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->setDelete(value);
  }


  inline bool  CatalogExtension::setPhysical ( const Cell* cell, bool value )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->setPhysical(value);
  }


  inline bool  CatalogExtension::setLogical ( const Cell* cell, bool value )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? false : state->setLogical(value);
  }


  inline Library* CatalogExtension::getLibrary ( const Cell* cell )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? NULL : state->getLibrary();
  }


  inline unsigned int  CatalogExtension::getDepth ( const Cell* cell )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? 0 : state->getDepth();
  }


  inline Library* CatalogExtension::setLibrary ( const Cell* cell, Library* library )
  {
    Catalog::State* state = _get(cell);
    return (state == NULL) ? NULL : state->setLibrary(library);
  }


  inline void  CatalogExtension::setDepth ( const Cell* cell, unsigned int depth )
  {
    Catalog::State* state = _get(cell);
    if ( state == NULL ) state->setDepth(depth);
  }


} // End of CRL namespace.




// x-----------------------------------------------------------------x
// |           Functions Overload for Hurricane Management           |
// x-----------------------------------------------------------------x

  
inline std::string  getPrint ( const CRL::Catalog &CATAL ) { return CATAL._getPrint(); }

INSPECTOR_P_SUPPORT(CRL::Catalog);
INSPECTOR_P_SUPPORT(CRL::Catalog::State);


#endif
