#ifndef OptionsParser_h
#define OptionsParser_h
/* *************************************************************************
   project:      multipurpose library
   subproject:   options library
   module:       OptionsLoader
   purpose:      Static helpers for parsing options from command line
   begin:        03.03.2004
   copyright:    (C) Daniel Krajzewicz
   email:        daniel@krajzewicz.de
   *************************************************************************
   Version:
   Remarks:
   *************************************************************************
   ToDo:
   @todo catch InvalidArguments 
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
class OptionCont;



/* =========================================================================
 * class definitions
 * ======================================================================= */
/**
 * @class OptionsParser
 * @brief Static helpers for parsing options from command line.
 */
class OptionsParser {
public:
    /** @brief Parses the given options into the given container
	 * @param[in] into The options container to fill
	 * @param[in] argc The number of arguments given on the command line
	 * @param[in] argv The arguments given on the command line
	 * @param[in] continueOnError Continues even if an error occures while parsing
	 * @param[in] acceptUnknown Unknown options do not throw an exception
	 */
    static bool parse(OptionCont &into, int argc, char **argv,
		bool continueOnError=true, bool acceptUnknown=false);


private:
    /** @brief Parses a single option into the container
	 * @param[in] into The options container to fill
	 * @param[in] argv The arguments given on the command line
	 * @param[in] pos The current position within the arguments to parse from
	 * @param[in] argc The number of arguments given on the command line
	 * @return The number of arguments to proceed
	 */
    static int parse(OptionCont &into, char **argv, int pos, int argc);


    /** @brief Parses a single, abbreviated option into the container
	 * @param[in] into The options container to fill
	 * @param[in] argv The arguments given on the command line
	 * @param[in] pos The current position within the arguments to parse from
	 * @param[in] argc The number of arguments given on the command line
	 * @return The number of arguments to proceed
	 */
    static int parseAbbreviation(OptionCont &into, char **argv, int pos, int argc);


    /** @brief Parses a single, fully-named option into the container
	 * @param[in] into The options container to fill
	 * @param[in] argv The arguments given on the command line
	 * @param[in] pos The current position within the arguments to parse from
	 * @param[in] argc The number of arguments given on the command line
	 * @return The number of arguments to proceed
	 */
    static int parseFull(OptionCont &into, char **argv, int pos, int argc);


    /** @brief Converts the given char* into a string
	 * @param[in] str The string to convert
	 * @return The converted string
	 */
    static std::string convert(char *str);


    /** @brief Converts the given char into a string
	 * @param[in] str The char to convert
	 * @return The converted char
	 */
    static std::string convert(char abbr);


};


// *************************************************************************
#endif
