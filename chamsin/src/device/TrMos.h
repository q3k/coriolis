// ****************************************************************************************************
// File: TrMos.h
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#ifndef DEVICE_TRMOS
#define DEVICE_TRMOS


#include "Device.h"
#include "Transistors.h"
#include "MetaTransistor.h"


namespace Hurricane {

  class Library;
  class Name;
  class Record;
  class Transistor;

  class Net;
  class Pin;
}

namespace DEVICE {

using Hurricane::Library;
using Hurricane::Name;
using Hurricane::Record;
using Hurricane::MetaTransistor;
using Hurricane::Transistor;
using Hurricane::Transistors;
using Hurricane::Net;
using Hurricane::Pin;


class TrMos : public Device {
// **************************

#   if !defined(__DOXYGEN_PROCESSOR__)
// Types
// *****
    public : typedef Device Inherit; 

    public : enum PinName { D, G, S, B };

// Attributes
// *******************

    // Structural parameter.
    // ********************
    private  : char _type;
    private  : bool _isBsConnected;
    private  : unsigned _m;

    // Parameter of the electric synthesis. 
    // ***********************************
    /* to do */
	       
    // Physical parameter of realization. 
    // **********************************
    /* Placement. */
    private  : bool _sourceIsFirst;
    private  : bool _hasDummy;
    private  : bool _hasRing;

    /* Routing. */
    private  : vector<PinName> _lowPinOrder;     // relative position of the connectors on the basis of the top.
    private  : vector<PinName> _highPinOrder;

    private  : map<Net*, Pin*> _mapNetToPinBoxInLeftSide;
    private  : map<Net*, Pin*> _mapNetToPinBoxInRightSide;

    private  : double _widthOfSourceWire;  // by defect,  minWidth, unit of valeur is Micro
    private  : double _widthOfDrainWire;   // by defect,  minWidth, unit of valeur is Micro

    /* Others */
    private  : MetaTransistor * _tr1;
    private  : double _capaRouting;
    private  : list<Transistor*> _transistorList;


// Constructors
// ************
    protected : TrMos(Library* library, const Name& name);
    protected : virtual void _PostCreate();
#endif

    public    : static TrMos* Create(Library* library, const Name & name); 


#   if !defined(__DOXYGEN_PROCESSOR__)
// Destructors
// ***********
    protected : ~TrMos() {};
    public    : virtual void Delete();
    protected : virtual void _PreDelete();


// Operations
// **********
    public : virtual void Dses()   { /* to do */}; 
    public : virtual void Shape()  { /* to do */};
#endif


    public : void Create(const char type, const bool isbsconnected);
    public : void Generate(const unsigned m, const bool sourceisfirst, const bool hasring,
		 const unsigned nbsourcecolumn, const unsigned nbdraincolumn);

// Accessors
// *********	    
    public : char GetType() const { return _type; };  
    public : unsigned GetM() const { return _m; };	     
    public : const double GetWidthOfSourceWire() const { return _widthOfSourceWire; };	     
    public : const double GetWidthOfDrainWire() const { return _widthOfDrainWire; };	     
    public : MetaTransistor* GetTr1() const { return _tr1; };	     
    public : Transistors GetTransistors() const ;

// Updators
// ********
    public : void SetMosLength(const double length) { if(_tr1) _tr1->SetLe(length); }
    public : void SetMosWidth(const double width) { if(_tr1) _tr1->SetWe(width); }
    public : void SetWidthOfSourceWire(const double width)  { _widthOfSourceWire = width; };	     
    public : void SetWidthOfDrainWire(const double width) { _widthOfDrainWire=width; };	     
    public : void SetLowPinOrder(const PinName, const PinName) ;	     
    public : void SetHighPinOrder(const PinName, const PinName) ;	     

// Predicats
// *********
    public : bool IsBsConnected() const { return _isBsConnected; };
    public : bool SourceIsFirst() const { return _sourceIsFirst; };
    public : bool HasRing() const { return _hasRing; };
 

#   if !defined(__DOXYGEN_PROCESSOR__)

// Others
// ******
    public: virtual string _GetTypeName() const {return _TName("TrMos"); };
    public: virtual string _GetString() const;
    public: virtual Record* _GetRecord() const;

    public: vector<PinName>& GetLowPinOrder() { return _lowPinOrder; };     
    public: vector<PinName>& GetHighPinOrder() { return _highPinOrder; };

    public: map<Net*, Pin*>& GetMapNetToPinBoxInLeftSide() { return _mapNetToPinBoxInLeftSide; };
    public: map<Net*, Pin*>& GetMapNetToPinBoxInRightSide() { return _mapNetToPinBoxInRightSide; };
     
    public : virtual void _Flush();
    protected : void _PlaceAndRoute();	    
# endif

} ;

} // end of namespace Device 


#endif