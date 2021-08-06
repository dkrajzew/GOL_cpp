#ifndef OptionsIO_h
#define OptionsIO_h
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
#include <string>

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
 * @class OptionsIO
 * @brief Static helper methods for parsing and loading of options.
 *
 * To enable loading of XML-configurations, USE_XERCES_XML must be defined.
 *  In this case, the configuration is loaded using the xerces-parser.
 */
class OptionsIO {
public:
    /** @brief Parses options from the command line and optionally loads options from a configuration file
	 * @param[in] into The options container to fill
	 * @param[in] argc The number of arguments given on the command line
	 * @param[in] argv The arguments given on the command line
  	 * @param[in] configOptionName The path to the configuration to load (XML)
	 * @param[in] continueOnError Continues even if an error occures while parsing
  	 * @param[in] acceptUnknown Unknown options do not throw an exception
     * @todo continueOnError is not used
     * @todo acceptUnknown is not used
     */
    static bool parseAndLoad(OptionsCont &into, int argc, char **argv,
		const std::string &configOptionName, bool continueOnError=false,
		bool acceptUnknown=false);


    /** @brief Writes the set options as an XML configuration file
     * @param[in] configName The name of the file to write the configuration to
     * @param[in] options The options container that includes the (set/parsed) options to write 
     */
    static void writeXMLConfiguration(const std::string &configName, const OptionsCont &options);


    /** @brief Writes the a template for a configuration file
     * @param configName The name of the file to write the template to
     * @param options The options container to write a template for 
     * @throws IOException If the file cannot be written
     */
    static void writeXMLTemplate(const std::string &configName, const OptionsCont &options);


    /** @brief Output operator
     * @param[in] os The output container to write
     * @param[in] options The options to print
     */
    static void printSetOptions(std::ostream &os, const OptionsCont &options, bool includeSynonyms, bool shortestFirst, bool skipDefault);


    /** @brief Prints the help screen
     *
     * First, the help header is printed. Then, the method iterates over the
     *  known options. In the end, the help tail is printed.
     * @param[in] os The stream to write to
     * @param[in] options The options to print
     * @param[in] maxWidth The maximum width of a line
     * @param[in] optionIndent The indent to use before writing an option
     * @param[in] divider The space between the option name and the description
     * @param[in] sectionIndent The indent to use before writing a section name
     */
    static void printHelp(std::ostream &os, const OptionsCont &options, size_t maxWidth=80, size_t optionIndent=2, size_t divider=2, size_t sectionIndent=1);



private:
    /** @brief Loads options from an XML configuration file
	 * @param[in] into The options container to fill
	 * @param[in] configurationName The path to the configuration to load (XML)
	 */
    static bool load(OptionsCont &into, const std::string &configurationName);



    /** @brief Returns the synomymes of an option as a help-formatted string 
     *
     * The synomymes are sorted by length.
     * @param[in] option The option to get the synonyms help string for
     * @return The options as a help-formatted string
     */
    static std::string getHelpFormattedSynonyms(const OptionsCont &options, const std::string &optionName);


};


// *************************************************************************
#endif
