#ifndef OptionsFileIO_XML_h
#define OptionsFileIO_XML_h
/** ************************************************************************
   @project      options library
   @file         OptionsFileIO_XML.h
   @since        26.08.2021
   @author       Daniel Krajzewicz (daniel@krajzewicz.de)
   @copyright    Eclipse Public License v2.0 (EPL v2.0), (c) Daniel Krajzewicz 2004-2021
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
#include "OptionsTypedFileIO.h"

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
 * @class OptionsFileIO_XML
 * @brief A base class for loading/saving options from/to XML files
 */
class OptionsFileIO_XML : public OptionsTypedFileIO {
public:
    /// @brief Constructor
    OptionsFileIO_XML();


    /// @brief Destructor
    ~OptionsFileIO_XML();


    /** @brief Loads options from a configuration file
     * @param[in] into The options container to fill
     * @param[in] configurationName The path to the configuration to load
     */
    bool loadConfiguration(OptionsCont &into, const std::string &configurationName);


    /** @brief Writes the set options as an XML configuration file
     * @param[in] configName The name of the file to write the configuration to
     * @param[in] options The options container that includes the (set/parsed) options to write 
     */
    void writeXMLConfiguration(const std::string &configName, const OptionsCont &options);


    /** @brief Writes the a template for a configuration file
     * @param configName The name of the file to write the template to
     * @param options The options container to write a template for 
     * @throws IOException If the file cannot be written
     */
    void writeXMLTemplate(const std::string &configName, const OptionsCont &options);


};


#endif // USE_XERCES_XML


// *************************************************************************
#endif
