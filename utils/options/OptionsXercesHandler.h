#ifndef OptionsXercesHandler_h
#define OptionsXercesHandler_h
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

/* -------------------------------------------------------------------
 * (optional) memory checking
 * ----------------------------------------------------------------- */
#ifdef DEBUG_ALLOC_NVWA
#include <debug_new.h>
#endif


/* =========================================================================
 * class declarations
 * ======================================================================= */
class OptionsCont;


/* =========================================================================
 * class definitions
 * ======================================================================= */
/**
 * @class OptionsXercesHandler
 * @brief Handler for reading options from a configuration file.
 *
 * To enable loading of XML-configurations, USE_XMLmyOptions must be defined.
 *  In this case, OptionsXercesHandler is used as a handler for the xerces-library.
 */
class OptionsXercesHandler : public XERCES_CPP_NAMESPACE_QUALIFIER DefaultHandler {
public:
    /** @brief Constructor
	 * @param[in] options The options to fill
	 * @param[in] file The configuration file to load
     */
    OptionsXercesHandler(OptionsCont *options, const std::string &file);


    /// @brief Destructor
    ~OptionsXercesHandler();



    //{ Handlers for the SAX ContentHandler interface

    /** @brief Called is an element begins
	 */
    void startElement(const XMLCh* const uri, const XMLCh* const localname,
                      const XMLCh* const qname,
                      const XERCES_CPP_NAMESPACE_QUALIFIER Attributes& attrs);


    /** @brief Called if characters shall be processed */
    void characters(const XMLCh* const chars, const unsigned int length);
    //}



    //{ Handlers for the SAX ErrorHandler interface

    /// @brief Called if a warning has to be generated
    void warning(const XERCES_CPP_NAMESPACE_QUALIFIER  SAXParseException& exception);


    /// @brief Called if an error occured
    void error(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& exception);


    /// @brief Called if a fatal error occured
    void fatalError(const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& exception);
    //}


    /** @brief returns whether an error occured
	 * @return Whether an error occured while loading the configuration file
	 */
    bool errorOccured();


private:
    /// @brief The options to fill
    OptionsCont *myOptions;

    /// @brief The name of the current option to set
    std::string myCurrentOptionName;

    /// @brief Information whether an error occured
    bool myHadError;

    /// @brief The name of the processed file
    std::string myFileName;

};


#endif // USE_XML_OPTIONS


// *************************************************************************
#endif
