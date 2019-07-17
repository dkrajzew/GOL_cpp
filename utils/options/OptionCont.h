#ifndef OptionCont_h
#define OptionCont_h
/* *************************************************************************
   project:      multipurpose library
   subproject:   options library
   module:       OptionCont
   purpose:      Container for options
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
#include <map>
#include <string>
#include <vector>

/* -------------------------------------------------------------------
 * (optional) memory checking
 * ----------------------------------------------------------------- */
#ifdef DEBUG_ALLOC_NVWA
#include <debug_new.h>
#endif


/* =========================================================================
 * class declarations
 * ======================================================================= */
class Option;
class Option_String;


/* =========================================================================
 * class definitions
 * ======================================================================= */
/**
 * @class OptionCont
 * Container for options.
 */
class OptionCont {
public:
    /// @brief Constructor
    OptionCont();


    /// @brief Destructor
    ~OptionCont();


    /** @brief Registers an option under an abbreviation
	 * @param[in] abbr The option's abbreviated name
	 * @param[in] option The option
     */
    void add(char abbr, Option *option);


    /** @brief Registers an option under the given name
	 * @param[in] name The option's name
	 * @param[in] option The option
	 */
    void add(const std::string &name, Option *option);


    /** @brief Registers an option under the given abbreviation and the given name
	 * @param[in] name The option's name
	 * @param[in] abbr The option's abbreviated name
	 * @param[in] option The option
     */
    void add(const std::string &name, char abbr, Option *option);


    /** @brief Registers a known option under the other synonyme
	 * @param[in] name1 The name the option was already known under
	 * @param[in] name2 The synonyme to register
	 */
    void addSynonyme(const std::string &name1, const std::string &name2);


    /** @brief Returns the integer value of the named option
     * @param[in] name The name of the option to retrieve the value from
	 * @return The named option's value
	 */
    int getInteger(const std::string &name) const;


    /** @brief Returns the float value of the named option
	 * @param[in] name The name of the option to retrieve the value from
	 * @return The named option's value
	 */
    double getDouble(const std::string &name) const;


    /** @brief Returns the boolean value of the named option
	 * @param[in] name The name of the option to retrieve the value from
	 * @return The named option's value
	 */
    bool getBool(const std::string &name) const;


    /** @brief Returns the string value of the named option
	 * @param[in] name The name of the option to retrieve the value from
	 * @return The named option's value
	 */
    const std::string &getString(const std::string &name) const;


    /** @brief Returns the information whether the option is set
	 * @param[in] name The name of the option to check
	 * @return Whether the option has a value set
	 */
    bool isSet(const std::string &name) const;


    /** @brief Returns the information whether the option is a boolean option
	 * @param[in] name The name of the option to check
	 * @return Whether the option stores a bool
	 */
    bool isBool(const std::string &name) const;


    /** @brief Sets the given value to the given option
     * @param[in] name The name of the option to set
	 * @param[in] value The value to set
     */
    void set(const std::string &name, const std::string &value);


    /** @brief Sets the given value to the given option (boolean options only)
	 * @param[in] name The name of the option to set
	 * @param[in] value The value to set
	 */
    void set(const std::string &name, bool value=true);


    /** @brief Returns the list of synonymes to the given option name
	 * @param[in] name The name of the option
	 * @return List of this option's names
	 */
    std::vector<std::string> getSynonymes(const std::string &name) const;


    /** @brief Returns the list of names of the given option
	 * @param[in] option The option to retrieve her names
	 * @return List of this option's names
	 */
    std::vector<std::string> getSynonymes(const Option* const option) const;


    /// @brief Remarks all options as unset
    void remarkUnset();


    /** @brief Returns the information whether the named option is known
	 * @param[in] name The name of the option
	 * @return Whether the option is known
	 */
    bool contains(const std::string &name) const;


    /** @brief Sets the description for an already added option
     *
     * The description is what appears in the help menu
	 * @param[in] name The name of the option
	 * @param[in] desc The description of the option
	 */
    void setDescription(const std::string &name, const std::string &desc);


    /** @brief Output operator
     * @param[out] os The stream to write to
     * @param[in] oc The output container to write
     * @return The stream to write to
	 */
    friend std::ostream &operator<<(std::ostream &os, const OptionCont &oc);


    /** @brief Prints the help screen
     *
     * First, the help header is printed. Then, the method iterates over the
     *  known options. In the end, the help tail is printed.
     * @param[out] os The stream to write to
	 */
    void printHelp(std::ostream &os, size_t optionIndent=3, size_t divider=4) const;



private:
    /** @brief Returns the option; throws an exception when not existing
	 * @param[in] name The name of the option
	 * @return The option if known
	 * @throw InvalidArgument If the option is not known
	 */
    Option *getOption(const std::string &name) const;


    /** @brief Returns the option or 0 when not existing
	 * @param[in] name The name of the option
	 * @return The option if known, 0 otherwise
	 */
    Option *getOptionSecure(const std::string &name) const;


    /** @brief Converts the character into a string
 	 * @param[in] abbr The abbreviated name
	 * @return The abbreviated name as a string
	 */
    std::string convert(char abbr);


    /** @brief A string-by-length comperator
     */
    struct compareByLength {
        bool operator()(const std::string& first, const std::string& second) {
            return first.size() < second.size();
        }
    };


    /** @brief Returns the synomymes of an option as a help-formatted string 
     *
     * The synomymes are sorted by length.
     * @param[in] option The option to get the synonymes help string for
     * @param[in] optionIndent The indentation of options
     * @param[in] divider The space between the options and the help string
     * @return The options as a help-formatted string
     */
    std::string getHelpFormattedSynonymes(const Option * const option, size_t optionIndent, size_t divider) const;



private:
    /// @brief Definition of a map from option names to options
    typedef std::map<std::string, Option*> ContType;

    /// @brief Definition of a list of options
    typedef std::vector<Option*> AddressContType;


private:
    /// @brief A map from option names to options
    ContType myOptionsMap;

    /// @brief The list of known options
    AddressContType myOptions;


private:
    /// @brief Invalidated copy constructor
    OptionCont(const OptionCont &s);

    /// @brief Invalidated assignment operator
    OptionCont &operator=(const OptionCont &s);


};


// *************************************************************************
#endif

