/* *************************************************************************
   project:      multipurpose library
   subproject:   options library
   module:       Option
   purpose:      Definition of option types
   begin:		 03.03.2004
   copyright:    (C) Daniel Krajzewicz
   email:        daniel@krajzewicz.de
   *************************************************************************
   Version:
   Remarks:
   *************************************************************************
   ToDo:
   *********************************************************************** */

/* =========================================================================
 * global configuration import
 * ======================================================================= */
#if defined _MSC_VER || defined __BORLANDC__
#include <windows_config.h>
#else
#include <config.h>
#endif



/* =========================================================================
 * included modules
 * ======================================================================= */
#include <string>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <limits.h>
#include "Option.h"

/* -------------------------------------------------------------------------
 * (optional) memory checking
 * ----------------------------------------------------------------------- */
#ifdef DEBUG_ALLOC_NVWA
#include <debug_new.h>
#endif



/* =========================================================================
 * used namespaces
 * ======================================================================= */
using namespace std;



/* =========================================================================
 * method definitions
 * ======================================================================= */
/* -------------------------------------------------------------------------
 * Option-methods
 * ----------------------------------------------------------------------- */
Option::Option(bool hasDefault)
    : myAmSetable(true), myAmSet(hasDefault), myHaveDefaultValue(hasDefault) {
}


Option::Option(const Option &s)
    : myAmSetable(s.myAmSetable), myAmSet(s.myAmSet),
      myHaveDefaultValue(s.myHaveDefaultValue) {
}


Option &Option::operator=(const Option &s) {
    if(&s==this) return *this;
    myAmSetable = s.myAmSetable;
    myAmSet = s.myAmSet;
    myHaveDefaultValue = s.myHaveDefaultValue;
    return *this;
}


Option::~Option() {
}


bool
Option::isSet() {
    return myAmSet;
}


bool
Option::isDefault() {
    return myHaveDefaultValue;
}


void
Option::remarkSetable() {
    myAmSetable = true;
}


void
Option::setSet() {
    if(myAmSetable==false) {
        throw std::runtime_error("This option was already set.");
    }
    myAmSetable = false;
    myHaveDefaultValue = false;
    myAmSet = true;
}


void
Option::setDescription(const std::string &desc) {
    myDescription = desc;
}


const std::string &
Option::getDescription() const {
    return myDescription;
}


bool
Option::isFileName() const {
    return false;
}



/* -------------------------------------------------------------------------
 * Option_Integer-methods
 * ----------------------------------------------------------------------- */
Option_Integer::Option_Integer()
    : Option() {
}


Option_Integer::Option_Integer(int value)
    : Option(true), myValue(value) {
}


Option_Integer::Option_Integer(const Option_Integer &s)
    : Option(s) {
    myValue = s.myValue;
}


Option_Integer &
Option_Integer::operator=(const Option_Integer &s) {
    if(&s==this) return *this;
    Option::operator=(s);
    myValue = s.myValue;
    return *this;
}


Option_Integer::~Option_Integer() {
}


std::string
Option_Integer::getTypeName() {
    return "int";
}


void 
Option_Integer::set(const std::string &value) {
    myValue = parseInteger(value.c_str());
    setSet();
}


int 
Option_Integer::getValue() const {
    return myValue;
}


std::string 
Option_Integer::getValueAsString() const {
    std::ostringstream oss;
    oss << myValue;
    return oss.str();
}


int 
Option_Integer::parseInteger(const char *data) {
    // adapted from https://stackoverflow.com/questions/194465/how-to-parse-a-string-to-an-int-in-c/6154614#6154614
    if(data==0||data[0]==0) {
        throw runtime_error("inconvertible");
    }
    char *end;
    errno = 0;
    long l = strtol(data, &end, 0); // base=0->guess
    if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
        throw std::runtime_error("overflow");
    }
    if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
        throw std::runtime_error("underflow");
    }
    if (*data == '\0' || *end != '\0') {
        throw std::runtime_error("inconvertible");
    }
    return l;
}


/* -------------------------------------------------------------------------
 * Option_Bool-methods
 * ----------------------------------------------------------------------- */
Option_Bool::Option_Bool()
    : Option() {
}


Option_Bool::Option_Bool(bool value)
    : Option(true), myValue(value) {
}


Option_Bool::Option_Bool(const Option_Bool &s)
    : Option(s) {
    myValue = s.myValue;
}


Option_Bool &
Option_Bool::operator=(const Option_Bool &s) {
    if(&s==this) return *this;
    Option::operator =(s);
    myValue = s.myValue;
    return *this;
}


Option_Bool::~Option_Bool() {
}


std::string
Option_Bool::getTypeName() {
    return "bool";
}


void 
Option_Bool::set(const std::string &v) {
    std::string value = v;
    // Borland does not know how to transform
    for(size_t i=0; i<value.length(); ++i) {
        if(value[i]>='A'&&value[i]<='Z') {
            value[i] = value[i] - ('A'-'a');
        }
    }
    //std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    if(value=="t"||value=="true"||value=="1") {
        myValue = true;
    } else if(value=="f"||value=="false"||value=="0") {
        myValue = false;
    } else {
        throw std::runtime_error("inconvertible");
    }
    setSet();
}


bool 
Option_Bool::getValue() const {
    return myValue;
}


std::string 
Option_Bool::getValueAsString() const {
    if(myValue) {
        return "true";
    } else {
        return "false";
    }
}



/* -------------------------------------------------------------------------
 * Option_Double-methods
 * ----------------------------------------------------------------------- */
Option_Double::Option_Double()
    : Option() {
}


Option_Double::Option_Double(double value)
    : Option(true), myValue(value) {
}


Option_Double::Option_Double(const Option_Double &s)
    : Option(s) {
    myValue = s.myValue;
}


Option_Double &
Option_Double::operator=(const Option_Double &s) {
    if(&s==this) return *this;
    Option::operator =(s);
    myValue = s.myValue;
    return *this;
}


Option_Double::~Option_Double() {
}


std::string
Option_Double::getTypeName() {
    return "double";
}


void 
Option_Double::set(const std::string &value) {
    myValue = parseDouble(value.c_str());
    setSet();
}


double 
Option_Double::getValue() const {
    return myValue;
}


std::string 
Option_Double::getValueAsString() const {
    std::ostringstream oss;
    oss << myValue;
    return oss.str();
}


double 
Option_Double::parseDouble(const char *data) {
    if(data==0||data[0]==0) {
        throw runtime_error("inconvertible");
    }
    double ret = 0;
    int i = 0;
    double sgn = 1;
    if(data[0]=='+') {
        i++;
    } else if(data[0]=='-') {
        i++;
        sgn = -1;
    }
    for(; data[i]!=0&&data[i]!='.'&&data[i]!=','&&data[i]!='e'&&data[i]!='E'; ++i) {
        ret = ret * 10;
        char akt = (char) data[i];
        if(akt<'0'||akt>'9') {
            throw runtime_error("inconvertible");
        }
        ret = ret + akt - 48;
    }
    // check what has happened - end of string, e or decimal point
    if((char) data[i]!='.'&&(char) data[i]!=','&&data[i]!='e'&&data[i]!='E') {
        if(i==0) {
            throw runtime_error("inconvertible");
        }
        return ret * sgn;
    }
    if(data[i]=='e'||data[i]=='E') {
        // no decimal point, just an exponent
        int exp = Option_Integer::parseInteger(data+i+1);
        float exp2 = (float) pow(10.0, exp);
        return ret*sgn*exp2;
    }
    float div = 10;
    // skip the dot
    ++i;
    // parse values behin decimal point
    for(; data[i]!=0&&data[i]!='e'&&data[i]!='E'; ++i) {
        char akt = (char) data[i];
        if(akt<'0'||akt>'9') {
            runtime_error("inconvertible");
        }
        ret = ret + ((float)(akt - 48)) / div;
        div = div * 10;
    }
    if(data[i]!='e'&&data[i]!='E') {
        // no exponent
        return ret * sgn;
    }
    // eponent and decimal dot
    int exp = Option_Integer::parseInteger(data+i+1);
    float exp2 = (float) pow(10.0, exp);
    return ret*sgn*exp2;
}

/* -------------------------------------------------------------------------
 * Option_String-methods
 * ----------------------------------------------------------------------- */
Option_String::Option_String()
    : Option() {
}


Option_String::Option_String(const std::string &value)
    : Option(true), myValue(value) {
}


Option_String::Option_String(const Option_String &s)
    : Option(s) {
    myValue = s.myValue;
}


Option_String &
Option_String::operator=(const Option_String &s) {
    if(&s==this) return *this;
    Option::operator =(s);
    myValue = s.myValue;
    return *this;
}


Option_String::~Option_String() {
}


std::string
Option_String::getTypeName() {
    return "string";
}


void 
Option_String::set(const std::string &value) {
    setSet();
    myValue = value;
}


const std::string &
Option_String::getValue() const {
    return myValue;
}


std::string 
Option_String::getValueAsString() const {
    return myValue;
}


/* -------------------------------------------------------------------------
 * Option_FileName-methods
 * ----------------------------------------------------------------------- */
Option_FileName::Option_FileName()
    : Option_String() {
}


Option_FileName::Option_FileName(const std::string &value)
    : Option_String(value) {
}


Option_FileName::Option_FileName(const Option_FileName &s)
    : Option_String(s) {
}


Option_FileName &
Option_FileName::operator=(const Option_FileName &s) {
    if(&s==this) return *this;
    Option_String::operator =(s);
    return *this;
}


Option_FileName::~Option_FileName() {
}



bool
Option_FileName::isFileName() const {
    return true;
}


std::string
Option_FileName::getTypeName() {
    return "filename";
}




// *************************************************************************
