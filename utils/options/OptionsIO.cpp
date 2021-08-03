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
#include "OptionsCont.h"
#include "OptionsIO.h"
#include "OptionsParser.h"
#include <iostream>
#include <string>

#ifdef USE_XERCES_XML
#include "OptionsXercesHandler.h"
#include <util/PlatformUtils.hpp>
#include <sax2/SAX2XMLReader.hpp>
#include <sax2/XMLReaderFactory.hpp>
using namespace XERCES_CPP_NAMESPACE;
#endif // USE_XERCES_XML

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
                        const std::string &configOptionName,
                        bool , bool ) {
    bool ok = OptionsParser::parse(into, argc, argv);
    if(ok) into.remarkUnset();
    if(ok) ok = load(into, configOptionName);
    if(ok) into.remarkUnset();
    if(ok) ok = OptionsParser::parse(into, argc, argv);
    return ok;
}


bool
OptionsIO::load(OptionsCont &into, const std::string &configOptionName) {
    if(configOptionName.length()==0) {
        return true;
    }
#ifdef USE_XERCES_XML
    try {
        XMLPlatformUtils::Initialize();
    } catch(const XMLException& toCatch) {
        cerr << "Error during initialization! Message:" << std::endl
             << OptionsXercesHandler::convert(toCatch.getMessage()) << endl;
        return false;
    }
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
    string file = into.getString(configOptionName);
    OptionsXercesHandler handler(into, file);
    parser->setContentHandler(&handler);
    parser->setErrorHandler(&handler);
    try {
        parser->parse(file.c_str());
    } catch(const XMLException& e) {
        std::cerr << std::endl << "Error during parsing: '" << file << std::endl
             << "Exception message is:" << std::endl
             << OptionsXercesHandler::convert(e.getMessage()) << "\n" << std::endl;
        XMLPlatformUtils::Terminate();
        return false;
    } catch(...) {
        std::cerr << std::endl << "Error: Unexpected exception during parsing: '" << file << std::endl;
        XMLPlatformUtils::Terminate();
        return false;
    }
    return !handler.errorOccured();
#else
    // consume "into"
    into.contains("foo");
    return true;
#endif // USE_XERCES_XML
}


// *************************************************************************

