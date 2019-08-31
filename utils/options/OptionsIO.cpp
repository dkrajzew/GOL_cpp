/* *************************************************************************
   project:      multipurpose library
   subproject:   options library
   module:       OptionsIO
   purpose:      Static helper methods for parsing and loading of options
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
#include "OptionsCont.h"
#include "OptionsIO.h"
#include "OptionsParser.h"
#include <iostream>
#include <string>

#ifdef USE_XML_OPTIONS
#include "OptionsXercesHandler.h"
#include <utils/xml/XMLConvert.h>
#include <util/PlatformUtils.hpp>
#include <sax2/SAX2XMLReader.hpp>
#include <sax2/XMLReaderFactory.hpp>
using namespace XERCES_CPP_NAMESPACE;
#endif // USE_XML_OPTIONS

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
OptionsIO::parseAndLoad(OptionsCont &into, int argc, char **argv,
                        const std::string &configurationName,
                        bool , bool ) {
    bool ok = OptionsParser::parse(into, argc, argv);
    if(ok) into.remarkUnset();
    if(ok) ok = load(into, configurationName);
    if(ok) into.remarkUnset();
    if(ok) ok = OptionsParser::parse(into, argc, argv);
    return ok;
}


bool
OptionsIO::load(OptionsCont &into, const std::string &configurationName) {
    if(configurationName.length()==0) {
        return true;
    }
#ifdef USE_XML_OPTIONS
    try {
        XMLPlatformUtils::Initialize();
    } catch(const XMLException& toCatch) {
        cerr << "Error during initialization! Message:\n"
             << XMLConvert::_2str(toCatch.getMessage()) << endl;
        return false;
    }
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
    string file = into->getString(configurationName);
    OptionsXercesHandler handler(into, file);
    parser->setContentHandler(&handler);
    parser->setErrorHandler(&handler);
    try {
        parser->parse(file.c_str());
    } catch(const XMLException& e) {
        cerr << endl << "Error during parsing: '" << file << endl
             << "Exception message is:" << endl
             << XMLConvert::_2str(e.getMessage()) << "\n" << endl;
        XMLPlatformUtils::Terminate();
        return false;
    } catch(InvalidMethod &) {
        cerr << "Error: You have to set your enviroment variable." << endl; // !v2!
    } catch(...) {
        cerr << endl << "Error: Unexpected exception during parsing: '" << file << endl;
        XMLPlatformUtils::Terminate();
        return false;
    }
    return !handler.errorOccured();
#else
    // consume "into"
    into.contains("foo");
    return true;
#endif // USE_XML_OPTIONS
}


// *************************************************************************

