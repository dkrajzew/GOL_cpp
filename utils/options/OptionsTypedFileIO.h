#ifndef OptionsTypedFileIO_h
#define OptionsTypedFileIO_h
/** ************************************************************************
   @project      options library
   @file         OptionsTypedFileIO.h
   @since        26.08.2021
   @author       Daniel Krajzewicz (daniel@krajzewicz.de)
   @copyright    Eclipse Public License v2.0 (EPL v2.0), (c) Daniel Krajzewicz 2004-2021
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
 * @class OptionsTypedFileIO
 * @brief A base class for loading/saving options from/to files
 */
class OptionsTypedFileIO {
public:
    /// @brief Constructor
    OptionsTypedFileIO();


    /// @brief Destructor
    virtual ~OptionsTypedFileIO();


    /** @brief Loads options from a configuration file
     * 
     * This method calls the protected member  _loadConfiguration(OptionsCont, String)
     * within a loop that realises hierarchical configurations.
     * 
     * @see _loadConfiguration(OptionsCont, String)
     * @param[in] into The options container to fill
     * @param[in] configOptionName The name of the option to get the path to the configuration to load from
     * @return Whether options could be loaded
     */
    virtual bool loadConfiguration(OptionsCont &into, const std::string &configOptionName);


    /** @brief Writes the set options as an XML configuration file
     * @param[in] configName The name of the file to write the configuration to
     * @param[in] options The options container that includes the (set/parsed) options to write 
     */
    virtual bool writeXMLConfiguration(const std::string &configName, const OptionsCont &options) = 0;


    /** @brief Writes the a template for a configuration file
     * @param configName The name of the file to write the template to
     * @param options The options container to write a template for 
     * @throws IOException If the file cannot be written
     */
    virtual bool writeXMLTemplate(const std::string &configName, const OptionsCont &options) = 0;


protected:
    /** @brief Loads options from a configuration file
     * @param[in] into The options container to fill
     * @param[in] configurationName The path to the configuration to load
     * @return Whether options could be loaded
     */
    virtual bool _loadConfiguration(OptionsCont &into, const std::string &configurationName) = 0;


};


// *************************************************************************
#endif
