/** ************************************************************************
   @project      options library
   @file         OptionsFileIO_XML.cpp
   @since        26.08.2021
   @author       Daniel Krajzewicz (daniel@krajzewicz.de)
   @copyright    Eclipse Public License v2.0 (EPL v2.0), (c) Daniel Krajzewicz 2021-
   *********************************************************************** */

/* =========================================================================
* compile only if XML is supported
* ======================================================================= */
#ifdef USE_XERCES_XML


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
#include <sax2/Attributes.hpp>
#include <sax2/DefaultHandler.hpp>
#include "OptionsXercesHandler.h"
#include <util/PlatformUtils.hpp>
#include <sax2/SAX2XMLReader.hpp>
#include <sax2/XMLReaderFactory.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "OptionsXercesHandler.h"
#include "OptionsFileIO_XML.h"
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
using namespace XERCES_CPP_NAMESPACE;



/* =========================================================================
 * method definitions
 * ======================================================================= */
OptionsFileIO_XML::OptionsFileIO_XML() {
}


OptionsFileIO_XML::~OptionsFileIO_XML() {
}


bool
OptionsFileIO_XML::loadConfiguration(OptionsCont &into, const std::string &configurationName) {
#ifdef USE_XERCES_XML
    try {
        XMLPlatformUtils::Initialize();
    } catch(const XMLException& toCatch) {
        cerr << "Error during initialization! Message:" << std::endl << OptionsXercesHandler::convert(toCatch.getMessage()) << endl;
        return false;
    }
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
    string file = into.getString(configurationName);
    OptionsXercesHandler handler(into, file);
    parser->setContentHandler(&handler);
    parser->setErrorHandler(&handler);
    try {
        parser->parse(file.c_str());
    } catch(const XMLException& e) {
        std::cerr << std::endl << "Error during parsing: '" << file << "'." << std::endl
            << "Exception message is:" << std::endl
            << OptionsXercesHandler::convert(e.getMessage()) << std::endl;
        XMLPlatformUtils::Terminate();
        return false;
    } catch(...) {
        std::cerr << std::endl << "Error: Unexpected exception during parsing: '" << file << "':" << std::endl;
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


void
OptionsFileIO_XML::writeXMLConfiguration(const std::string &configName, const OptionsCont &options) {
    std::vector<std::string> optionNames = options.getSortedOptionNames();
    std::ofstream fdo(configName.c_str());
    fdo << "<configuration>" << std::endl;
    for(std::vector<std::string>::const_iterator i=optionNames.begin(); i!=optionNames.end(); ++i) {
        std::string optionName = *i;
        if(options.isSet(optionName) && !options.isDefault(optionName)) {
            fdo << "   <" << optionName << ">" << options.getValueAsString(optionName) << "</" << optionName << std::endl;
        }
    }
    fdo << "</configuration>" << std::endl;
    fdo.close();
}


void
OptionsFileIO_XML::writeXMLTemplate(const std::string &configName, const OptionsCont &options) {
    std::vector<std::string> optionNames = options.getSortedOptionNames();
    std::ofstream fdo(configName.c_str());
    fdo << "<configuration>" << std::endl;
    for(std::vector<std::string>::const_iterator i=optionNames.begin(); i!=optionNames.end(); ++i) {
        std::string optionName = *i;
        fdo << "   <" << optionName << "></" << optionName << std::endl;
    }
    fdo << "</configuration>" << std::endl;
    fdo.close();
}



#endif // USE_XERCES_XML



// *************************************************************************
