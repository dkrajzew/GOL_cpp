## master
### New features
* [issue #6](https://github.com/dkrajzew/optionslib_java/issues/6) added CSV-configurations support
* Moved docs from the Wiki to an own folder

### Debugging / Refactoring
* extracted XML reading and writing methods from OptionsIO to OptionsTypedFileIO.h (interface) and its implementations



## version 1.2 (06.08.2021)

### New features
* writing templates
* writing configurations

### Debugging / Refactoring
* correcting misspelled "synonyms"
* improved error reporting
* API extensions
  * OptionsCont: added ```std::string OptionsCont::getValueAsString(const std::string &name) const``` for retrieving the string representation of a named option's value
  * OptionsCont: added ```std::string OptionsCont::getTypeName(const std::string &name) const``` for retrieving a named option's data type name
  * OptionsCont: added ```bool OptionsCont::isDefault(const std::string &name) const``` for retrieving whether the value of an option is its default value
  * OptionsCont: added ```const std::string &getSection(const std::string &optionName) const``` for retrieving the help section of an option
  * OptionsCont: added ```const std::string &getDescription(const std::string &optionName) const``` for retrieving an option's help description
  * OptionsCont: added ```const std::string &getHelpHead() const``` for retrieving the head of the help output
  * OptionsCont: added ```const std::string &getHelpTail() const``` for retrieving the tail of the help output

## version 1.0 (08.10.2019)
* base
