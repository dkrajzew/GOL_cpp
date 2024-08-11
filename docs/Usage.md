# Interface
The major interface to the options is the class "OptionCont".

I usually use a static instance of the class and a method that fills it with options and that calls the parsing method to set the values given on the command line.

# Adding Options
An option is added to the OptionsCont using the "add" method:
```cpp
 void add(char abbr, Option *option);
 void add(const std::string &name, Option *option);
 void add(const std::string &name, char abbr, Option *option);
```

Please notice that neither the full name nor the abbreviation contain the leading "--" or "-", respectively. One usually does not use an abbreviation only.

The option is a new built instance of one of:
* Option_Bool
* Option_Integer
* Option_Double
* Option_String

The OptionsCont will delete the instance on own deletion.

Each of these type-aware option classes can get a value which will be used as default.

If you like to have more names for an option, e.g. --input-file, --input, and -f, you may additionally use the OptionsCont method "addSynonym":
```cpp
 myOptions.add("input-file", 'f', new Option_String());
 myOptions.addSynonym("input-file", "input");
```

# Parsing Options and Reading Configurations
The major interface for parsing options given on the command line and/or reading a configuration file is the method ```void OptionsIO::parseAndLoad(myOptions, argc, argv, configOptionName, bool continueOnError=false, bool acceptUnknown=false)```. Here ```configOptionName``` is the name of the option to read the name of the configuration file from. If no name is supported, no configuration is read. This is as well the case if the named option is not set.

# Retrieving Options
You directly ask for an option's value in a type-aware name:
```cpp
 int intValue = myOptions.getInteger("my-int-option");
 double doubleValue = myOptions.getDouble("my-double-option");
 bool boolValue = myOptions.getBool("my-bool-option");
 std::string strValue = myOptions.getString("my-string-option");
```

You can additionally ask whether an option exists (albeit you should know this) using ```bool contains(const std::string &name) const``` and whether an option is set (a value has been given) using ```bool isSet(const std::string &name) const```.

When asking for an option, I recommend to use the full name, so to ask for "help" and not for "?". "?" may be obvious, but simple characters have two disadvantages: a) you may get lost when having too many of them; b) when incrementally adding new options during development, it is often necessary to decide which one to abbreviate and which not. So the abbreviations change more often than the full names, requiring to adapt the code more often.

# Help and Sectioning
Usually, the help screen contains a short description about each option's purpose. Add such a description to a previously added option using the method ```void setDescription(const std::string &name, const std::string &desc)```. 

If you have many options, it may make sense to group them thematically. Use the ```void beginSection(const std::string &name)``` method to start a new section.

In addition, you may add a head and a tail to your help screen that precede and follow the list of options, respectively. This is done using ```void setHelpHeadAndTail(const std::string &head, const std::string &tail)```.

To print the help screen use the static method ```void printHelp(std::ostream &os, const OptionsCont &options, size_t optionIndent=3, size_t divider=4, size_t sectionIndent=1) const``` from the OptionsIO class.

# Further information
* [doxygen class documentation](https://www.krajzewicz.de/docs/optionslib_cpp/index.html)

