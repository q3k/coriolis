// ****************************************************************************************************
// File: Timer.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_TIMER
#define HURRICANE_TIMER

#include "hurricane/Commons.h"

namespace Hurricane {



// ****************************************************************************************************
// Timer declaration
// ****************************************************************************************************

class Timer {
// ********

// Attributes
// **********

    private: time_t _time;

// Constructors
// ************

    public: Timer();

    public: Timer(const Timer& timer);

// Operators
// *********

    public: Timer& operator=(const Timer& timer);

// Others
// ******

    public: string _getTypeName() const { return _TName("Timer"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

};



} // End of Hurricane namespace.


INSPECTOR_PV_SUPPORT(Hurricane::Timer);


#endif // HURRICANE_TIMER

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************