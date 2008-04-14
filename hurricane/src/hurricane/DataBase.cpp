// ****************************************************************************************************
// File: DataBase.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "DataBase.h"
#include "Technology.h"
#include "Library.h"
#include "Cell.h"
#include "Timer.h"
#include "Error.h"
#include "UpdateSession.h"

namespace Hurricane {



// ****************************************************************************************************
// DataBase implementation
// ****************************************************************************************************

static DataBase* DATA_BASE = NULL;

DataBase::DataBase()
// *****************
:    Inherit(),
    _technology(NULL),
    _rootLibrary(NULL)
{
    if (DATA_BASE)
        throw Error("Can't create " + _TName("DataBase") + " : already exists");
}

DataBase* DataBase::create()
// *************************
{
    DataBase* dataBase = new DataBase();

    dataBase->_postCreate();

    return dataBase;
}

void DataBase::_postCreate()
// *************************
{
    Inherit::_postCreate();

    DATA_BASE = this;
}

void DataBase::_preDestroy()
// ************************
{
    UpdateSession::open();
    Inherit::_preDestroy();

    if (_rootLibrary) _rootLibrary->destroy();
    if (_technology) _technology->destroy();
    UpdateSession::close();

    DATA_BASE = NULL;
}

string DataBase::_getString() const
// ********************************
{
    return Inherit::_getString();
}

Record* DataBase::_getRecord() const
// ***************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Technology", _technology));
        record->add(getSlot("RootLibrary", _rootLibrary));
        record->add(getSlot("Precision", getPrecision()));
        record->add(getSlot("Resolution", getValueString(1)));
        record->add(getSlot("GridStep", getValueString(getGridStep())));
    }
    return record;
}

// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

DataBase* getDataBase()
// ********************
{
    return DATA_BASE;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
