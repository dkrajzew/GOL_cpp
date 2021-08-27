/** ************************************************************************
   @project      options library
   @file         OptionsTypedFileIO.cpp
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
#include "OptionsTypedFileIO.h"
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
OptionsTypedFileIO::OptionsTypedFileIO() {
}


OptionsTypedFileIO::~OptionsTypedFileIO() {
}


bool
OptionsTypedFileIO::loadConfiguration(OptionsCont &into, const std::string &configOptionName) {
    const std::string &parentName = into.getParentConfigurationName();
    std::string fileName = into.getString(configOptionName);
    bool ok = true;
    do {
        if(parentName!="") {
            into.remarkUnset(parentName);
        }
        ok &= _loadConfiguration(into, fileName);
        if(parentName!="" && !into.isDefault(parentName)) {
            fileName = into.getString(parentName);
        } else {
            fileName = "";
        }
    } while (ok && fileName!="");
    return ok;
}



// *************************************************************************
