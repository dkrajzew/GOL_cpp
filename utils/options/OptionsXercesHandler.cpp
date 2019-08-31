/* *************************************************************************
   project:      multipurpose library
   subproject:   options library
   module:       OptionsXercesHandler
   purpose:      Handler for reading options from a configuration file
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
* compile only if XML is supported
* ======================================================================= */
#ifdef USE_XML_OPTIONS



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
#include <utils/xml/XMLConvert.h>
#include <iostream>
#include <string>
#include <utils/exceptions/InvalidArgument.h>
#include "OptionsXercesHandler.h"
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
OptionsXercesHandler::OptionsXercesHandler(OptionsCont *options,
    const std::string &file) : myOptions(options), myHadError(false), myFileName(file) {
}


OptionsXercesHandler::~OptionsXercesHandler() {
}


void
OptionsXercesHandler::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const Attributes& attrs) {
    myCurrentOptionName = XMLConvert::_2str(localname);
}


void
OptionsXercesHandler::characters(const XMLCh* const chars, const unsigned int length) {
    if(myOptions->contains(myCurrentOptionName)) {
        try {
            myOptions->setPathAware(myCurrentOptionName, XMLConvert::_2str(chars), myFileName);
        } catch(InvalidArgument &e) {
            cerr << "Error: " << e.msg() << endl;
            myHadError = true;
        } catch(...) {
        }
    }
    myCurrentOptionName = "";
}


void
OptionsXercesHandler::warning(const SAXParseException& exception) {
    XMLConvert::reportWarning(exception, myFileName);
}


void
OptionsXercesHandler::error(const SAXParseException& exception) {
    XMLConvert::reportError(exception, myFileName);
    myHadError = true;
}


void
OptionsXercesHandler::fatalError(const SAXParseException& exception) {
    XMLConvert::reportFatal(exception, myFileName);
    myHadError = true;
}


bool
OptionsXercesHandler::errorOccured() {
    return myHadError;
}

#endif // USE_XML_OPTIONS



// *************************************************************************
