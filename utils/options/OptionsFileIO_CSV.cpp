/** ************************************************************************
   @project      options library
   @file         OptionsFileIO_CSV.cpp
   @since        26.08.2021
   @author       Daniel Krajzewicz (daniel@krajzewicz.de)
   @copyright    Eclipse Public License v2.0 (EPL v2.0), (c) Daniel Krajzewicz 2021-
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
#include "OptionsCont.h"
#include "OptionsFileIO_CSV.h"
#include <iostream>
#include <fstream>
#include <string>

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
OptionsFileIO_CSV::OptionsFileIO_CSV() {
}


OptionsFileIO_CSV::~OptionsFileIO_CSV() {
}


bool
OptionsFileIO_CSV::loadConfiguration(OptionsCont &into, const std::string &configurationName) {
    std::string line;
    std::string file = into.getString(configurationName);
    std::ifstream fdi(file.c_str());
    if(!fdi.good()) {
        std::cerr << std::endl << "Error: Could not open configuration file '" << file << "' for reading." << std::endl;
        return false;
    }
    while (getline(fdi, line, '\n')) {
        // trim
        do {
            size_t l = line.length();
            if(l==0) {
                break;
            }
            char c = line.at(l-1);
            if(c==' '||c=='\r'||c=='\n') {
                line = line.substr(0, l-1);
            } else {
                break;
            }
        } while(true);
        //
        size_t i1 = line.find(";");
        if(i1==std::string::npos) {
            continue;
        }
        size_t i2 = line.find(";", i1+1);
        if(i2==std::string::npos) {
            into.set(line.substr(0, i1), line.substr(i1+1));
        } else {
            into.set(line.substr(0, i1), line.substr(i1+1, i2-i1-1));
        }
    }
    return true;
}


void
OptionsFileIO_CSV::writeXMLConfiguration(const std::string &configName, const OptionsCont &options) {
    std::vector<std::string> optionNames = options.getSortedOptionNames();
    std::ofstream fdo(configName.c_str());
    for(std::vector<std::string>::const_iterator i=optionNames.begin(); i!=optionNames.end(); ++i) {
        std::string optionName = *i;
        if(options.isSet(optionName) && !options.isDefault(optionName)) {
            fdo << optionName << ";" << options.getValueAsString(optionName) << std::endl;
        }
    }
    fdo.close();
}


void
OptionsFileIO_CSV::writeXMLTemplate(const std::string &configName, const OptionsCont &options) {
    std::vector<std::string> optionNames = options.getSortedOptionNames();
    std::ofstream fdo(configName.c_str());
    for(std::vector<std::string>::const_iterator i=optionNames.begin(); i!=optionNames.end(); ++i) {
        std::string optionName = *i;
        fdo << optionName << ";" << std::endl;
    }
    fdo.close();
}


// *************************************************************************
