// ****************************************************************************************************
// File: ./hurricane/IntrusiveMap.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#ifndef HURRICANE_INTRUSIVE_MAP
#define HURRICANE_INTRUSIVE_MAP

#include <cstring>
#include <algorithm>

#include "hurricane/Collection.h"
#include "hurricane/Error.h" // AD

namespace Hurricane {



// ****************************************************************************************************
// IntrusiveMap declaration
// ****************************************************************************************************

template<class Key, class Element> class IntrusiveMap {
// **************************************************

// Types
// *****

    class Elements : public Collection<Element*> {
    // *****************************************
        
    // Types
    // *****
    
        public: typedef Collection<Element*> Inherit;
    
        public: class Locator : public Hurricane::Locator<Element*> {
        // ********************************************************
    
        // Types
        // *****
    
            public: typedef Hurricane::Locator<Element*> Inherit;
    
        // Attributes
        // **********
    
            private: const IntrusiveMap* _map;
            private: unsigned _index;
            private: Element* _element;
    
        // Constructors
        // ************
    
            public: Locator(const IntrusiveMap* map = NULL)
            // ********************************************
            :    Inherit(),
                _map(map),
                _index(0),
                _element(NULL)
            {
                if (_map) {
                    unsigned length = _map->_getLength();
                    do {
                        _element = _map->_getArray()[_index++];
                    } while (!_element && (_index < length));
                }
            };

            public: Locator(const Locator& locator)
            // ************************************
            :    Inherit(),
                _map(locator._map),
                _index(locator._index),
                _element(locator._element)
            {
            };
    
        // Operators
        // *********
    
            public: Locator& operator=(const Locator& locator)
            // ***********************************************
            {
                _map = locator._map;
                _index = locator._index;
                _element = locator._element;
                return *this;
            };
    
        // Accessors
        // *********
    
            public: virtual Element* getElement() const
            // ****************************************
            {
                return _element;
            };

            public: virtual Hurricane::Locator<Element*>* getClone() const
            // ***********************************************************
            {
                return new Locator(*this);
            };
    
        // Predicates
        // **********
    
            public: virtual bool isValid() const
            // *********************************
            {
                return (_element != NULL);
            };
    
        // Updators
        // ********
    
            public: virtual void progress()
            // ****************************
            {
                if (_element) {
                    _element = _map->_getNextElement(_element);
                    if (!_element) {
                        unsigned length = _map->_getLength();
                        if (_index < length) {
                            do {
                                _element = _map->_getArray()[_index++];
                            } while (!_element && (_index < length));
                        }
                    }
                }
            };
    
        // Others
        // ******
    
            public: virtual string _getString() const
            // **************************************
            {
                string s = "<" + _TName("IntrusiveMap::Elements::Locator");
                if (_map) s += " " + getString(_map);
                s += ">";
                return s;
            };
    
        };
    
    // Attributes
    // **********
    
        private: const IntrusiveMap* _map;
    
    // Constructors
    // ************
    
        public: Elements(const IntrusiveMap* map = NULL)
        // *********************************************
        :     Inherit(),
            _map(map)
        {
        };

        public: Elements(const Elements& elements)
        // ***************************************
        :     Inherit(),
            _map(elements._map)
        {
        };
    
    // Operators
    // *********
    
        public: Elements& operator=(const Elements& elements)
        // **************************************************
        {
            _map = elements._map;
            return *this;
        };
    
    // Accessors
    // *********
    
        public: virtual Collection<Element*>* getClone() const
        // ***************************************************
        {
            return new Elements(*this);
        };

        public: virtual Hurricane::Locator<Element*>* getLocator() const
        // *************************************************************
        {
            return new Locator(_map);
        };
    
    // Others
    // ******
    
        public: virtual string _getString() const
        // **************************************
        {
            string s = "<" + _TName("IntrusiveMap::Elements");
            if (_map) s += " " + getString(_map);
            s += ">";
            return s;
        };
    
    };

// Attributes
// **********

    private: unsigned _size;
    private: unsigned _length;
    private: Element** _array;

// Constructors
// ************

    public: IntrusiveMap()
    // *******************
    :    _size(0),
        _length(1),
        _array(new Element*[1])
    {
        _array[0] = NULL;
    };

    private: IntrusiveMap(const IntrusiveMap& map); // not implemented to forbid copy

// Destructor
// **********

    public: virtual ~IntrusiveMap()
    // ****************************
    {
        for (unsigned index = 0; index < _length; index++) {
            Element* element = _array[index];
            while (element) {
                _array[index] = _getNextElement(element);
                _setNextElement(element, NULL);
                element = _array[index];
            }
            _array[index] = NULL;
        }
        delete[] _array;
    };

// Operators
// *********

    private: IntrusiveMap& operator=(const IntrusiveMap& map); // not implemented to forbid assignment

// Accessors
// *********

    public: Element* getElement(Key key) const
    // ***************************************
    {
        unsigned index = (_getHashValue(key) / 8) % _length;
        Element* element = _array[index];
        while (element && (_getKey(element) != key)) element = _getNextElement(element);
        return element;
    };

    public: Elements getElements() const
    // *********************************
    {
        return Elements(this);
    };

// Predicates
// **********

    public: bool isEmpty() const
    // *************************
    {
        return (_size == 0);
    };

// Overridables
// ************

    public: virtual Key _getKey(Element* element) const = 0;

    public: virtual unsigned _getHashValue(Key key) const = 0;

    // public: virtual Element* _getNextElement(Element* element) const = 0; // AD
    public: virtual Element* _getNextElement(Element* element) const
    // *************************************************************
    {
        throw Error(_TName("IntrusiveMap") + "::_getNextElement(...) : should be overrided");
        return NULL;
    };

    // public: virtual void _setNextElement(Element* element, Element* nextElement) const = 0; // AD
    public: virtual void _setNextElement(Element* element, Element* nextElement) const
    // *******************************************************************************
    {
        throw Error(_TName("IntrusiveMap") + "::_setNextElement(...) : should be overrided");
    };

// Others
// ******

    public: string _getTypeName() const
    // ********************************
    {
      return _TName("IntrusiveMap");
    }

    public: string _getString() const
    // ******************************
    {
        if (isEmpty())
            return "<" + _getTypeName() + " empty>";
        else
            return "<" + _getTypeName() + " " + getString(_size) + ">";
    };

    public: Record* _getRecord() const
    // *************************
    {
        Record* record = NULL;
        if (!isEmpty()) {
            record = new Record(getString(this));
            unsigned n = 1;
            for (unsigned index = 0; index < _length; index++) {
                n = 1;
                Element* element = _array[index];
                while (element) {
                  record->add(getSlot<Element*>(getString(index) + ":" + getString(n++), element));
                    element = _getNextElement(element);
                }
            }
        }
        return record;
    };

    public: unsigned _getSize() const
    // ******************************
    {
        return _size;
    };

    public: unsigned _getLength() const
    // ********************************
    {
        return _length;
    };

    public: Element** _getArray() const
    // ********************************
    {
        return _array;
    };

    public: bool _contains(Element* element) const
    // *******************************************
    {
        unsigned index = (_getHashValue(_getKey(element)) / 8) % _length;
        Element* currentElement = _array[index];
        while (currentElement && (currentElement != element))
            currentElement = _getNextElement(currentElement);
        return (currentElement != NULL);
    };

    public: void _insert(Element* element)
    // ***********************************
    {
        if (!_contains(element)) {
            unsigned index = (_getHashValue(_getKey(element)) / 8) % _length;
            _setNextElement(element, _array[index]);
            _array[index] = element;
              _size++;
            _resize();
        }
    };

    public: void _remove(Element* element)
    // ***********************************
    {
        if (_contains(element)) {
            unsigned index = (_getHashValue(_getKey(element)) / 8) % _length;
            Element* currentElement = _array[index];
            if (currentElement) {
                if (currentElement == element) {
                    _array[index] = _getNextElement(element);
                    _setNextElement(element, NULL);
                    _size--;
                }
                else {
                    while (_getNextElement(currentElement) && (_getNextElement(currentElement) != element))
                        currentElement = _getNextElement(currentElement);
                    if (currentElement && (_getNextElement(currentElement) == element)) {
                        _setNextElement(currentElement, _getNextElement(element));
                        _setNextElement(element, NULL);
                        _size--;
                    }
                }
            }
        }
    };

    public: void _resize()
    // *******************
    {
        unsigned newLength = _length;
        double ratio = (double)_size / (double)_length;
        if (ratio < 3)
            newLength = max(_size / 8, (unsigned)1);
        else if (10 < ratio)
            newLength = min(_size / 5, (unsigned)512);

        if (newLength != _length) {
            // cerr << "Resizing: " << this << " " << _length << " " << newLength << endl;
            unsigned oldLength = _length;
            Element** oldArray = _array;
            _length = newLength;
            _array = new Element*[_length];
            memset(_array, 0, _length * sizeof(Element*));
            for (unsigned index = 0; index < oldLength; index++) {
                Element* element = oldArray[index];
                while (element) {
                    Element* nextElement = _getNextElement(element);
                    unsigned newIndex = (_getHashValue(_getKey(element)) / 8) % _length;
                    _setNextElement(element, _array[newIndex]);
                    _array[newIndex] = element;
                    element = nextElement;
                }
            }
            delete[] oldArray;
        }
    };

};



} // End of Hurricane namespace.


template<typename Key, typename Element>
inline std::string  getString ( Hurricane::IntrusiveMap<Key,Element>* intrusiveMap )
                              { return intrusiveMap->_getString(); }

template<typename Key, typename Element>
inline std::string  getString ( const Hurricane::IntrusiveMap<Key,Element>* intrusiveMap )
                              { return intrusiveMap->_getString(); }

template<typename Key, typename Element>
inline Hurricane::Record* getRecord ( Hurricane::IntrusiveMap<Key,Element>* intrusiveMap )
                                    { return intrusiveMap->_getRecord(); }

template<typename Key, typename Element>
inline Hurricane::Record* getRecord ( const Hurricane::IntrusiveMap<Key,Element>* intrusiveMap )
                                    { return intrusiveMap->_getRecord(); }


#endif // HURRICANE_INTRUSIVE_MAP


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
