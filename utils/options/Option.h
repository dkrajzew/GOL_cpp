#ifndef Option_h
#define Option_h
/* *************************************************************************
   project:      multipurpose library
   subproject:   options library
   module:       Option
   purpose:      Definition of option types
   begin:		 03.03.2004
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
#include <string>

/* -------------------------------------------------------------------
 * (optional) memory checking
 * ----------------------------------------------------------------- */
#ifdef DEBUG_ALLOC_NVWA
#include <debug_new.h>
#endif



/* =========================================================================
 * class definitions
 * ======================================================================= */
/**
 * @class Option
 * @brief Base class fo a single option
 *
 * This class is the base class for options. It is not meant being used
 *  directly but only using derived classes, so the constructors are
 *  not public.
 * Still, the class is not an abstract class. Type-checking for the set
 *  options is done using methods from TypedValue and throwing exceptions
 *  rather than type-checking of the option during runtime.
 */
class Option {
public:
    /// @brief Destructor
    virtual ~Option();


    /** @brief Returns whether this option's value may be read
     *
     * The option's value may be read if either a default value was
     *  given or the user supplied a value.
	 * @return Whether the option has been set
     */
    bool isSet();


    /** @brief Returns whether this option's value is the default value
	* @return Whether the option has the defaulot value
	*/
    bool isDefault();


    /** @brief Allows setting this option
     *
     * After an option has been set by the user, it would normally throw an
     *  InvalidArgument-exception. To reallow setting the option, use this
     *  method.
     */
    void remarkSetable();


    /** @brief Returns the name of the type this option has
     *
     * Pure virtual, this method has to be implemented by the respective type-aware subclasses
	 * @return This option's value's type name
	 */
    virtual std::string getTypeName() = 0;


    /** @brief Returns whether this option is of the type "filename"
     *
     * Returns false unless overridden (in Option_Filename)
	 * @return Whether this options is a file name
	 */
    virtual bool isFileName() const;


    /** @brief Sets the current value to the given
     *
     * Pure virtual, this method has to be implemented by the respective type-aware subclasses
     * @param[in] value The value to set
     * @throw InvalidArgument if this option already has been set (see setSet())
     * @throw NotOfThisTypeException if it is not a string
	 */
    virtual void set(const std::string &value) = 0;


    /** @brief Adds a description (what appears in the help screen) to the option
     *
     * @param[in] desc The description to set
     * @param[in] semType The type of the value
	 */
    void setDescription(const std::string &desc, const std::string &semType);


    /** @brief Retuns the option's description
     * @return The option's description
	 */
    const std::string &getDescription() const;


    /** @brief Retuns the option's semantic type
     * @return The option's semantic type
	 */
    const std::string &getSemanticType() const;


    /** @brief Returns the value (if set) as a string
     *
     * Throws an exception if not set.
     * Pure virtual, this method has to be implemented by the respective type-aware subclasses
     * @return The value as a string, if set
	 */
    virtual std::string getValueAsString() const = 0;
    


protected:
    /** @brief constructor
     *
     * Use this constructor to build an option with a given type and no default value
     * Sets:
     * @arg myAmSetable to true
     * @arg myAmSet to false
     * @arg myHaveDefaultValue to hasDefault
     * @param[in] hasDefault Whether a default values has been supplied
     */
    Option(bool hasDefault=false);


    /** @brief copy constructor
	 * @param[in] s The option to copy
     */
    Option(const Option &s);


    /** @brief assignment operator
	 * @param[in] s The option to copy
	 */
    Option &operator=(const Option &s);


    /** @brief checks and marks further usage of this option on setting it
     *
     * Checks whether this option may be set using myAmSetable.
     * If not, throws an InvalidArgument-exception.
     * If yes, sets myAmSetable to false, myAmSet to true and myHaveDefaultValue to false
     */
    void setSet();



private:
    /// @brief Information whether a new value can be assigned
    bool myAmSetable;

    /// @brief Information whether this option's value may be read
    bool myAmSet;

    /// @brief Information whether this option's value is the one given optionally at initialisation
    bool myHaveDefaultValue;

    /// @brief The description (what appears in the help screen) of the option
    std::string myDescription;

    /// @brief The semantic type of the option
    std::string mySemanticType;


};



/**
 * @class Option_Integer
 * @brief A derivation of Option which accepts integer values.
 */
class Option_Integer : public Option {
public:
    /// @brief Constructor to use if no default is given
    Option_Integer();


    /** @brief Constructor to use if a default shall be set
	 * @param[in] value The value to set
	 */
    Option_Integer(int value);


    /** @brief Copying constructor
	 * @param[in] s The option to copy
	 */
    Option_Integer(const Option_Integer &s);


    /** @brief Assignment operator
	 * @param[in] s The option to copy
	 */
     Option_Integer &operator=(const Option_Integer &s);


    /// @brief Destructor
    ~Option_Integer();


    /** @brief Returns the name of the type this option has
	 * @return This option's value's type name
	 */
    std::string getTypeName();


    /** @brief Sets the current value to the given
     * @param[in] value The value to set
     * @throw InvalidArgument if this option already has been set (see setSet())
     * @throw NotOfThisTypeException if it is not a string
	 */
    void set(const std::string &value);


    /** @brief Returns the integer value
     * @return The option's value
     */
    int getValue() const;


    /** @brief Returns the value (if set) as a string
     *
     * Throws an exception if not set.
     * @return The value as a string, if set
	 */
    std::string getValueAsString() const;


    /** Parses the given string assuming it contains an integer
     * @param[in] data The string to parse
     * @return the parsed integer
     * @throw runtime_error If the string could not been parsed to an integer
     */
    static int parseInteger(const char *data);



private:
    /// @brief The integer value (if set)
    int myValue;


};



/**
 * @class Option_Bool
 * @brief A derivation of Option which accepts boolean values.
 */
class Option_Bool : public Option {
public:
    /** @brief Constructor to use if no default is given
     * 
     * For boolean options, the default is always set to false
     */
    Option_Bool();


    /** @brief Copying constructor
	 * @param[in] s The option to copy
	 */
    Option_Bool(const Option_Bool &s);


    /** @brief Assignment operator
	 * @param[in] s The option to copy
	 */
    Option_Bool &operator=(const Option_Bool &s);


    /// @brief Destructor
    ~Option_Bool();


    /** @brief Returns the name of the type this option has
	 * @return This option's value's type name
	 */
    std::string getTypeName();


    /** @brief Sets the current value to the given
     * @param[in] value The value to set
     * @throw InvalidArgument if this option already has been set (see setSet())
     * @throw NotOfThisTypeException if it is not a string
	 */
    void set(const std::string &value);


    /** @brief Returns the boolean value
     * @return The option's value
     */
    bool getValue() const;


    /** @brief Returns the value (if set) as a string
     *
     * Throws an exception if not set.
     * @return The value as a string, if set
	 */
    std::string getValueAsString() const;



private:
    /// @brief The boolean value (if set)
    bool myValue;


};



/**
 * @class Option_Double
 * @brief A derivation of Option which accepts double values.
 */
class Option_Double : public Option {
public:
    /// @brief Constructor to use if no default is given
    Option_Double();


    /** @brief Constructor to use if a default shall be set
	 * @param[in] value The value to set
	 */
    Option_Double(double value);


    /** @brief Copying constructor 
	 * @param[in] s The option to copy
	 */
    Option_Double(const Option_Double &s);


    /** @brief Assignment operator
	 * @param[in] s The option to copy
	 */
    Option_Double &operator=(const Option_Double &s);


    /// @brief Destructor
    ~Option_Double();


    /** @brief Returns the name of the type this option has
	 * @return This option's value's type name
	 */
    std::string getTypeName();


    /** @brief Sets the current value to the given
     * @param[in] value The value to set
     * @throw InvalidArgument if this option already has been set (see setSet())
     * @throw NotOfThisTypeException if it is not a string
	 */
    void set(const std::string &value);


    /** @brief Returns the double value
     * @return The option's value
     */
    double getValue() const;


    /** @brief Returns the value (if set) as a string
     *
     * Throws an exception if not set.
     * @return The value as a string, if set
	 */
    std::string getValueAsString() const;


    /** Parses the given string assuming it contains a double
     * @param[in] data The string to parse
     * @return the parsed double
     * @throw runtime_error If the string could not been parsed to a double
     */
    static double parseDouble(const char *data);



private:
    /// @brief The double value (if set)
    double myValue;


};



/**
 * @class Option_String
 * @brief A derivation of Option which accepts string values.
 */
class Option_String : public Option {
public:
    /// @brief Constructor to use if no default is given
    Option_String();


    /** @brief Constructor to use if a default shall be set
	 * @param[in] value The value to set
	 */
    Option_String(const std::string &value);


    /** @brief Copying constructor
	 * @param[in] s The option to copy
	 */
    Option_String(const Option_String &s);


    /** @brief Assignment operator
	 * @param[in] s The option to copy
	 */
    Option_String &operator=(const Option_String &s);


    /// @brief Destructor
    ~Option_String();


    /** @brief Returns the name of the type this option has
	 * @return This option's value's type name
	 */
    virtual std::string getTypeName();


    /** @brief Sets the current value to the given
     * @param[in] value The value to set
     * @throw InvalidArgument if this option already has been set (see setSet())
     * @throw NotOfThisTypeException if it is not a string
	 */
    void set(const std::string &value);


    /** @brief Returns the string value
     * @return The option's value
     */
    const std::string &getValue() const;


    /** @brief Returns the value (if set) as a string
     *
     * Throws an exception if not set.
     * @return The value as a string, if set
	 */
    std::string getValueAsString() const;



private:
    /// @brief The string value (if set)
    std::string myValue;


};



/**
 * @class Option_FileName
 * @brief A derivation of Option which accepts filename values.
 */
class Option_FileName : public Option_String {
public:
    /// @brief Constructor to use if no default is given
    Option_FileName();


    /** @brief Constructor to use if a default shall be set
	 * @param[in] value The value to set
	 */
    Option_FileName(const std::string &value);


    /** @brief Copying constructor
	 * @param[in] s The option to copy
	 */
    Option_FileName(const Option_FileName &s);


    /** @brief Assignment operator
	 * @param[in] s The option to copy
	 */
    Option_FileName &operator=(const Option_FileName &s);


    /// @brief Destructor
    ~Option_FileName();


    /** @brief Returns whether this option is of the type "filename"
     * This overriding version returns always true
	 * @return Whether this is option stores a filename (true)
     */
    bool isFileName() const;


    /** @brief Returns the name of the type this option has
	 * @return This option's value's type name
	 */
    std::string getTypeName();


};



// *************************************************************************
#endif
