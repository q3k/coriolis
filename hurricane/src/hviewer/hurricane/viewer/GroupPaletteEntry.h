
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Cl�ment                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GroupPaletteEntry.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>


# ifndef  __GROUP_HPALETTE_ENTRY_H__
#   define  __GROUP_HPALETTE_ENTRY_H__


class QPushButton;

# include  "hurricane/Name.h"

# include  "hurricane/viewer/HPaletteEntry.h"


namespace Hurricane {


  class GroupPaletteEntry : public HPaletteEntry {
      Q_OBJECT;

    // Constructor.
    public:
      static  GroupPaletteEntry* create             ( HPalette* palette, const Name& name );

    // Methods.
    public:
      virtual bool               isGroup            () const;
      virtual bool               isBasicLayer       () const;
      virtual const Name&        getName            () const;
      virtual BasicLayer*        getBasicLayer      ();
      virtual bool               isChecked          () const;
      virtual void               setChecked         ( bool state );

    // Slots.
    public slots:
      virtual void               toggle             ();

    // Internal - Attributes.
    protected:
              const Name         _name;
              QPushButton*       _button;
              size_t             _index;
              bool               _expanded;

    // Internal - Constructor.
                                 GroupPaletteEntry  ( HPalette* palette, const Name& name );
                                 GroupPaletteEntry  ( const GroupPaletteEntry& );
              GroupPaletteEntry& operator=          ( const GroupPaletteEntry& );
      virtual void               _postCreate        ();

    // Internal - Methods.
              void               hideShow           ();
              string             getLabel           ();
  };


} // End of Hurricane namespace.


# endif