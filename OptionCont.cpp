/* *************************************************************************
   project:      multipurpose library
   subproject:   options library
   module:       OptionCont
   purpose:      Container for options
   begin:        03.03.2004
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
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include "Option.h"
#include "OptionCont.h"

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
OptionCont::OptionCont() {
}


OptionCont::~OptionCont() {
    for(AddressContType::iterator i=myOptions.begin(); i!=myOptions.end(); i++) {
        delete *i;
    }
}


void
OptionCont::add(char abbr, Option *option) {
    add(convert(abbr), option);
}


void
OptionCont::add(const std::string &name, Option *option) {
    ContType::iterator i = myOptionsMap.find(name);
    if(i!=myOptionsMap.end()) {
        throw std::exception();// ("An option with the name '" + name + "' already exists.");
    }
    AddressContType::iterator j = find(myOptions.begin(), myOptions.end(), option);
    if(j==myOptions.end()) {
        myOptions.push_back(option);
    }
    myOptionsMap.insert(ContType::value_type(name, option));
}


void
OptionCont::add(const std::string &name, char abbr, Option *option) {
    add(name, option);
    add(convert(abbr), option);
}


void
OptionCont::synonymes(const std::string &name1, const std::string &name2) {
    Option *o1 = getOptionSecure(name1);
    Option *o2 = getOptionSecure(name2);
    if(o1==0&&o2==0) {
        throw std::exception();// ("Neither an option with the name '" + name1 + "' nor an option with the name '" + name2 + "' is known.");
    }
    if(o1!=0&&o2!=0) {
        throw std::exception();// ("Both options are already set ('" + name1 + "' and '" + name2 + "')!");
    }
    if(o1!=0) {
        add(name2, o1);
    } else {
        add(name1, o2);
    }
}


int
OptionCont::getInteger(const std::string &name) const {
    Option_Integer *o = dynamic_cast<Option_Integer*>(getOptionSecure(name));
    if(o==0) {
        throw std::runtime_error("This is not an integer option!");
    }
    if(!o->isSet()) {
        throw std::runtime_error("The option is not set!");
    }
    return o->getValue();
}


bool
OptionCont::getBool(const std::string &name) const {
    Option_Bool *o = dynamic_cast<Option_Bool*>(getOptionSecure(name));
    if(o==0) {
        throw std::runtime_error("This is not an bool option!");
    }
    if(!o->isSet()) {
        throw std::runtime_error("The option is not set!");
    }
    return o->getValue();
}


double
OptionCont::getDouble(const std::string &name) const {
    Option_Double *o = dynamic_cast<Option_Double*>(getOptionSecure(name));
    if(o==0) {
        throw std::runtime_error("This is not an double option!");
    }
    if(!o->isSet()) {
        throw std::runtime_error("The option is not set!");
    }
    return o->getValue();
}


const std::string &
OptionCont::getString(const std::string &name) const {
    Option_String *o = dynamic_cast<Option_String*>(getOptionSecure(name));
    if(o==0) {
        throw std::runtime_error("This is not an string option!");
    }
    if(!o->isSet()) {
        throw std::runtime_error("The option is not set!");
    }
    return o->getValue();
}


bool
OptionCont::isSet(const std::string &name) const {
    Option *o = getOption(name);
    return o->isSet();
}


bool
OptionCont::isBool(const std::string &name) const {
    Option_Bool *o = dynamic_cast<Option_Bool*>(getOptionSecure(name));
    if(o==0) {
        return false;
    }
    return true;
}


void
OptionCont::set(const std::string &name, const std::string &value) {
    Option *o = getOption(name);
    o->set(value);
}


void
OptionCont::set(const std::string &name, bool value) {
    Option_Bool *o = dynamic_cast<Option_Bool*>(getOptionSecure(name));
    if(o==0) {
        throw std::runtime_error("This is not a boolean option");
    }
    if(value) {
        o->set("true");
    } else {
        o->set("false");
    }
}


Option *
OptionCont::getOption(const string &name) const {
    ContType::const_iterator i = myOptionsMap.find(name);
    if(i==myOptionsMap.end()) {
        throw std::runtime_error("The option '" + name + "' is not known.");
    }
    return (*i).second;
}


Option *
OptionCont::getOptionSecure(const string &name) const {
    ContType::const_iterator i = myOptionsMap.find(name);
    if(i==myOptionsMap.end()) {
        return 0;
    }
    return (*i).second;
}


bool
OptionCont::contains(const string &name) const {
    ContType::const_iterator i = myOptionsMap.find(name);
    return i!=myOptionsMap.end();
}


std::vector<std::string>
OptionCont::getSynonymes(const std::string &name) const {
    Option *o = getOption(name);
    vector<string> ret;
    for(ContType::const_iterator i=myOptionsMap.begin(); i!=myOptionsMap.end(); i++) {
        if((*i).second==o&&(*i).first!=name) {
            ret.push_back((*i).first);
        }
    }
    return ret;
}


void
OptionCont::remarkUnset() {
    for(AddressContType::iterator i=myOptions.begin(); i!=myOptions.end(); i++) {
        (*i)->remarkSetable();
    }
}


std::string
OptionCont::convert(char abbr) {
    char buf[2];
    buf[0] = abbr;
    buf[1] = 0;
    string s = buf;
    return s;
}


std::ostream &
operator<<(std::ostream &os, const OptionCont &oc) {
    vector<string> known;
    known.reserve(oc.myOptionsMap.size());
    for(OptionCont::ContType::const_iterator i=oc.myOptionsMap.begin(); i!=oc.myOptionsMap.end(); i++) {
        vector<string>::iterator j=find(known.begin(), known.end(), (*i).first);
        if(j==known.end()) {
            Option *o = (*i).second;
            if(o->isSet()) {
                vector<string> synonymes = oc.getSynonymes((*i).first);
                os << (*i).first;
                if(synonymes.size()>0) {
                    os << " (";
                    for(j=synonymes.begin(); j!=synonymes.end();) {
                        known.push_back(*j);
                        os << *j++;
                        if(j!=synonymes.end())
                            os << ", ";
                    }
                    os << ")";
                }
                os << ": " << o->getValueAsString();
                if(o->isDefault()) {
                    os << " (default)";
                }
                os << endl;
            }
        }
    }
    return os;
}




// *************************************************************************
