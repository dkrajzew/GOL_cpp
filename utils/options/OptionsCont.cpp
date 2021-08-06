/** ************************************************************************
   @project      options library
   @since		 03.03.2004
   @copyright    (c) Daniel Krajzewicz 2004-2021
   @author       Daniel Krajzewicz  
   @email        daniel@krajzewicz.de
   @license      Eclipse Public License v2.0 (EPL v2.0) 
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
#include "Option.h"
#include "OptionsCont.h"

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
OptionsCont::OptionsCont() {
}


OptionsCont::~OptionsCont() {
    for(std::vector<Option*>::iterator i=myOptions.begin(); i!=myOptions.end(); i++) {
        delete *i;
    }
}



/* -------------------------------------------------------------------------
 * Filling Options
 * ----------------------------------------------------------------------- */
void
OptionsCont::add(char abbr, Option *option) {
    add(convert(abbr), option);
}


void
OptionsCont::add(const std::string &name, Option *option) {
    // check whether the name is already used
    std::map<std::string, Option*>::const_iterator i = myOptionsMap.find(name);
    if(i!=myOptionsMap.end()) {
        throw std::exception();// ("An option with the name '" + name + "' already exists.");
    }
    // check whether a synonym already exists, if not, add the option to option's array
    std::vector<Option*>::const_iterator j = find(myOptions.begin(), myOptions.end(), option);
    if(j==myOptions.end()) {
        myOptions.push_back(option);
        // add the option to the sections, if given
        myOption2Section[option] = myCurrentSection;
    }
    // add the option to the name-to-option map
    myOptionsMap.insert(std::map<std::string, Option*>::value_type(name, option));
}


void
OptionsCont::add(const std::string &name, char abbr, Option *option) {
    add(name, option);
    add(convert(abbr), option);
}


void
OptionsCont::addSynonym(const std::string &name1, const std::string &name2) {
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


void
OptionsCont::setDescription(const std::string &name, const std::string &desc, const std::string &semType) {
    Option *o = getOption(name);
    o->setDescription(desc, semType);
}


void 
OptionsCont::beginSection(const std::string &name) {
    myCurrentSection = name;
}


void 
OptionsCont::setHelpHeadAndTail(const std::string &head, const std::string &tail) {
    myHelpHead = head;
    myHelpTail = tail;
}


/* -------------------------------------------------------------------------
 * Filling Options
 * ----------------------------------------------------------------------- */
int
OptionsCont::getInteger(const std::string &name) const {
    Option_Integer *o = dynamic_cast<Option_Integer*>(getOption(name));
    if(o==0) {
        throw std::runtime_error("Option '" + name + "' is not an integer option!");
    }
    if(!o->isSet()) {
        throw std::runtime_error("The option '" + name + "' is not set!");
    }
    return o->getValue();
}


bool
OptionsCont::getBool(const std::string &name) const {
    Option_Bool *o = dynamic_cast<Option_Bool*>(getOption(name));
    if(o==0) {
        throw std::runtime_error("Option '" + name + "' is not a bool option!");
    }
    if(!o->isSet()) {
        throw std::runtime_error("The option '" + name + "' is not set!");
    }
    return o->getValue();
}


double
OptionsCont::getDouble(const std::string &name) const {
    Option_Double *o = dynamic_cast<Option_Double*>(getOption(name));
    if(o==0) {
        throw std::runtime_error("Option '" + name + "' is not a double option!");
    }
    if(!o->isSet()) {
        throw std::runtime_error("The option '" + name + "' is not set!");
    }
    return o->getValue();
}


const std::string &
OptionsCont::getString(const std::string &name) const {
    Option_String *o = dynamic_cast<Option_String*>(getOption(name));
    if(o==0) {
        throw std::runtime_error("Option '" + name + "' is not a string option!");
    }
    if(!o->isSet()) {
        throw std::runtime_error("The option '" + name + "' is not set!");
    }
    return o->getValue();
}


std::string 
OptionsCont::getValueAsString(const std::string &name) const {
    Option *o = getOption(name);
    if(!o->isSet()) {
        throw std::runtime_error("The option '" + name + "' is not set!");
    }
    return o->getValueAsString();
}


std::string 
OptionsCont::getTypeName(const std::string &name) const {
    Option *o = getOption(name);
    return o->getTypeName();
}


bool
OptionsCont::isSet(const std::string &name) const {
    Option *o = getOption(name);
    return o->isSet();
}


bool
OptionsCont::isDefault(const std::string &name) const {
    Option *o = getOption(name);
    return o->isDefault();
}


bool
OptionsCont::isBool(const std::string &name) const {
    Option_Bool *o = dynamic_cast<Option_Bool*>(getOptionSecure(name));
    if(o==0) {
        return false;
    }
    return true;
}


void
OptionsCont::set(const std::string &name, const std::string &value) {
    Option *o = getOption(name);
    o->set(value);
}


void
OptionsCont::set(const std::string &name, bool value) {
    Option_Bool *o = dynamic_cast<Option_Bool*>(getOption(name));
    if(o==0) {
        throw std::runtime_error("Option '" + name + "' is not a boolean option");
    }
    if(value) {
        o->set("true");
    } else {
        o->set("false");
    }
}


Option *
OptionsCont::getOption(const string &name) const {
    std::map<std::string, Option*>::const_iterator i = myOptionsMap.find(name);
    if(i==myOptionsMap.end()) {
        throw std::runtime_error("Option '" + name + "' is not known.");
    }
    return (*i).second;
}


Option *
OptionsCont::getOptionSecure(const string &name) const {
    std::map<std::string, Option*>::const_iterator i = myOptionsMap.find(name);
    if(i==myOptionsMap.end()) {
        return 0;
    }
    return (*i).second;
}


bool
OptionsCont::contains(const string &name) const {
    std::map<std::string, Option*>::const_iterator i = myOptionsMap.find(name);
    return i!=myOptionsMap.end();
}


std::vector<std::string> 
OptionsCont::getSortedOptionNames() const {
    std::vector<std::string> ret;
    for(std::vector<Option*>::const_iterator i=myOptions.begin(); i!=myOptions.end(); i++) {
        ret.push_back(getSynonyms(*i).back());
    }
    return ret;
}

    
std::vector<std::string>
OptionsCont::getSynonyms(const std::string &name) const {
    Option *option = getOption(name);
    return getSynonyms(option);
}


std::vector<std::string>
OptionsCont::getSynonyms(const Option* const option) const {
    vector<string> ret;
    for(std::map<std::string, Option*>::const_iterator i=myOptionsMap.begin(); i!=myOptionsMap.end(); i++) {
        if((*i).second==option) {
            ret.push_back((*i).first);
        }
    }
    return ret;
}





const std::string &
OptionsCont::getSection(const std::string &optionName) const {
    Option *option = getOption(optionName);
    return myOption2Section.find(option)->second; 
}


const std::string &
OptionsCont::getDescription(const std::string &optionName) const {
    Option *option = getOption(optionName);
    return option->getDescription(); 
}


const std::string &
OptionsCont::getHelpHead() const {
    return myHelpHead; 
}


const std::string &
OptionsCont::getHelpTail() const {
    return myHelpTail; 
}







void
OptionsCont::remarkUnset() {
    for(std::vector<Option*>::iterator i=myOptions.begin(); i!=myOptions.end(); i++) {
        (*i)->remarkSetable();
    }
}


std::string
OptionsCont::convert(char abbr) {
    char buf[2];
    buf[0] = abbr;
    buf[1] = 0;
    string s = buf;
    return s;
}


std::ostream &
operator<<(std::ostream &os, const OptionsCont &oc) {
    vector<string> known;
    known.reserve(oc.myOptionsMap.size());
    for(std::map<std::string, Option*>::const_iterator i=oc.myOptionsMap.begin(); i!=oc.myOptionsMap.end(); i++) {
        vector<string>::iterator j=find(known.begin(), known.end(), (*i).first);
        if(j!=known.end()) {
            continue;
        }
        Option *o = (*i).second;
        if(!o->isSet()) {
            continue;
        }
        vector<string> synonyms = oc.getSynonyms((*i).first);
        vector<string>::iterator k = synonyms.begin();
        known.push_back(*k);
        os << *k;
        if(synonyms.size()>1) {
            os << " (";
            for(++k; k!=synonyms.end();) {
                known.push_back(*k);
                os << *k++;
                if(k!=synonyms.end()) {
                    os << ", ";
                }
            }
            os << ")";
        }
        os << ": " << o->getValueAsString();
        if(o->isDefault()) {
            os << " (default)";
        }
        os << endl;
    }
    return os;
}


// *************************************************************************
