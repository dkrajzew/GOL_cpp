/* *************************************************************************
   project:      multipurpose library
   subproject:   options library
   module:       OptionsLoader
   purpose:      Static helpers for parsing options from command line
   begin:        21.02.2004
   copyright:    (C) Daniel Krajzewicz
   email:        daniel@krajzewicz.de
   *************************************************************************
   Version:
   Remarks:
   *************************************************************************
   ToDo:
   @todo catch InvalidArguments
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
#include <cstring>
#include <iostream>
#include <sstream>
#include "Option.h"
#include "OptionCont.h"
#include "OptionsParser.h"

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
bool
OptionsParser::parse(OptionCont &into, int argc, char **argv, bool continueOnError, bool acceptUnknown) {
    bool ok = true;
    for(int pos=1; pos<argc;) {
        // try to parse a token combination
        int add = parse(into, argv, pos, argc);
        // check whether an error occured
        if(add<0) {
            // error occured
            ok = false;
            // maybe we want to continue although an error occured
            if(continueOnError) {
                pos += -add;
            } else {
                return false;
            }
        } else {
            // ok, go on with the next combination
            pos += add;
        }
    }
    // return whether parsing had errors or not
    return ok;
}


int
OptionsParser::parse(OptionCont &into, char **args, int pos, int argc) {
    // an option name indicator must have at least two characters
    if(strlen(args[pos])>=2) {
        if(args[pos][0]=='-'&&args[pos][1]!='-') {
            // the next combination is an abbreviation
            return parseAbbreviation(into, args, pos, argc);
        } else if(args[pos][0]=='-'&&args[pos][1]=='-') {
            // the next combination is a full name argument
            return parseFull(into, args, pos, argc);
        }
    }
    // no option
    string msg = string("Unrecognized option '") + string(args[pos]) + string("'.");
    if(pos>2&&args[pos-1][0]=='-'&&args[pos-2][0]=='-') {
        msg = msg + string("\n Propably forgot a parameter for '") + string(args[pos-2]) + string("'.");
    }
    throw std::runtime_error(msg);
}


int
OptionsParser::parseAbbreviation(OptionCont &into, char **args, int pos, int argc) {
    string options = string(args[pos]).substr(1);
    size_t len = options.length();
    vector<string> usingParameter;
    // go through the combination
    unsigned int i = 0;
    for(; i<len&&options[i]!='='; ++i) {
        // check whether the name is a bool
        if(!into.isBool(convert(options[i]))) {
            // if not, then add it to the list of options that need a parameter
            char buf[2];
            buf[0] = options[i];
            buf[1] = 0;
            string tmp = buf;
            usingParameter.push_back(tmp);
        } else {
            // otherwise simply set it
            into.set(convert(options[i]));
        }
    }
    // check options that need a parameter
    if(usingParameter.size()==0) {
        // if no one was needed, return ok for parsing
        return 1;
    }
    if(usingParameter.size()>1) {
        // if there is more than one report an error
        ostringstream tmp;
        tmp << "All of the following options need a value: ";
        for(vector<string>::iterator j=usingParameter.begin(); j!=usingParameter.end();) {
            tmp << *j;
            if(j!=usingParameter.end())
                tmp << ", ";
            ++j;
        }
        tmp << ends;
        string msg = tmp.str();
        throw std::runtime_error(msg);
    }
    // assume one token is used
    int tokens = 1;
    // otherwise (one needed a parameter)
    string param;
    if(options[i]=='=') {
        // maybe the parameter is within the string
        param = options.substr(i+1);
    }
    if(param==""&&pos+1>=argc) {
        // no parameter was given, report an error
        throw std::runtime_error(string("Option '") + usingParameter.front() + string("' needs a value."));
    }
    if(param=="") {
        // use the next token as option value
        param = args[pos+1];
        tokens = 2;
    }
    // ok
    into.set(usingParameter.front(), param);
    return tokens;
}


int
OptionsParser::parseFull(OptionCont &into,
                         char **args, int pos, int argc) {
    string option = string(args[pos]).substr(2);
    string value;
    // check whether the value is given within the same token
    size_t idx = option.find('=');
    if(idx!=string::npos) {
        value = option.substr(idx+1);
        option = option.substr(0, idx);
    }
    // check whether it is a boolean option
    if(into.isBool(option)) {
        if(value!="") {
            // if a value has been given, inform the user
            throw std::runtime_error(string("Option '") + option + string("' does not need a value."));
        }
        into.set(option);
        return 1;
    }
    // otherwise (parameter needed)
    if(value!="") {
        // ok, value was given within the same token
        into.set(option, value);
        return 1;
    }
    if(pos+1>=argc) {
        // there is no further parameter, report an error
        throw std::runtime_error(string("Parameter '") + option + string("' needs a value."));
    }
    // ok, use the next one
    into.set(option, string(args[pos+1]));
    return 2;
}


string
OptionsParser::convert(char *str) {
    return string(str);
}


string
OptionsParser::convert(char abbr) {
    char buf[2];
    buf[0] = abbr;
    buf[1] = 0;
    return string(buf);
}


// *************************************************************************

