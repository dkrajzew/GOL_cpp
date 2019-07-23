/* *************************************************************************
   project:      multipurpose library
   subproject:   options library
   module:       OptionsCont
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
    // check whether a synonyme already exists, if not, add the option to option's array
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
OptionsCont::addSynonyme(const std::string &name1, const std::string &name2) {
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
OptionsCont::setDescription(const std::string &name, const std::string &desc) {
    Option *o = getOption(name);
    o->setDescription(desc);
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
OptionsCont::getBool(const std::string &name) const {
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
OptionsCont::getDouble(const std::string &name) const {
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
OptionsCont::getString(const std::string &name) const {
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
OptionsCont::isSet(const std::string &name) const {
    Option *o = getOption(name);
    return o->isSet();
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
OptionsCont::getOption(const string &name) const {
    std::map<std::string, Option*>::const_iterator i = myOptionsMap.find(name);
    if(i==myOptionsMap.end()) {
        throw std::runtime_error("The option '" + name + "' is not known.");
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
OptionsCont::getSynonymes(const std::string &name) const {
    Option *o = getOption(name);
    return getSynonymes(o);
}


std::vector<std::string>
OptionsCont::getSynonymes(const Option* const option) const {
    vector<string> ret;
    for(std::map<std::string, Option*>::const_iterator i=myOptionsMap.begin(); i!=myOptionsMap.end(); i++) {
        if((*i).second==option) {
            ret.push_back((*i).first);
        }
    }
    return ret;
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


void
OptionsCont::printHelp(std::ostream &os, size_t optionIndent, size_t divider, size_t sectionIndent) const {
    // compute needed width
    size_t maxWidth = 0;
    for(std::vector<Option*>::const_iterator i=myOptions.begin(); i!=myOptions.end(); ++i) {
        std::string optNames = getHelpFormattedSynonymes(*i, optionIndent, divider);
        maxWidth = maxWidth<optNames.length() ? optNames.length() : maxWidth;
    }
    // build the indent
    std::string optionIndentSting, sectionIndentSting;
    for(size_t i=0; i<optionIndent; ++i) {
        optionIndentSting += " ";
    }
    for(size_t i=0; i<sectionIndent; ++i) {
        sectionIndentSting += " ";
    }
    // 
    os << myHelpHead;
    std::string lastSection;
    compareByLength c;
    for(std::vector<Option*>::const_iterator i=myOptions.begin(); i!=myOptions.end(); ++i) {
        // check whether a new section starts
        std::string optSection = myOption2Section.find(*i)->second;
        if(lastSection!=optSection) {
            lastSection = optSection;
            os << sectionIndentSting << lastSection << std::endl;
        }
        // write the option
        std::string optNames = getHelpFormattedSynonymes(*i, optionIndent, divider);
        // write the divider
        os << optionIndentSting << optNames;
        size_t owidth = optNames.length();
        // write the description
        size_t beg = 0;
        std::string desc = (*i)->getDescription();
        size_t offset = divider+maxWidth-owidth;
        while(beg<desc.length()) {
            for(size_t j=0; j<offset; ++j) {
                os << " ";
            }
            if(80-offset>=desc.length()-beg) {
                os << desc.substr(beg);
                beg = desc.length();
            } else {
                size_t end = desc.rfind(' ', beg+80-offset);
                os << desc.substr(beg, end);
                end = desc.length();
                os << std::endl;
            }
            offset = divider; // could "running description indent"
        }
        os << std::endl;
    }
    os << myHelpTail;
}

std::ostream &
operator<<(std::ostream &os, const OptionsCont &oc) {
    vector<string> known;
    known.reserve(oc.myOptionsMap.size());
    for(std::map<std::string, Option*>::const_iterator i=oc.myOptionsMap.begin(); i!=oc.myOptionsMap.end(); i++) {
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

std::string 
OptionsCont::getHelpFormattedSynonymes(const Option * const option, size_t optionIndent, size_t divider) const {
    compareByLength c;
    std::vector<std::string> synonymes = getSynonymes(option);
    std::sort(synonymes.begin(), synonymes.end(), c);
    std::ostringstream oss;
    for(std::vector<std::string>::const_iterator j=synonymes.begin(); j!=synonymes.end(); ++j) {
        // consider the - / --
        if((*j).length()==1) {
            oss << '-';
        } else {
            oss << "--";
        }
        oss << (*j);
        if(j!=synonymes.end()-1) {
            oss << ", ";
        }
    }
    return oss.str();
}




// *************************************************************************
