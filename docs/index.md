# Home

**optionslib_cpp** (Options Library for C++) is a c++-library for parsing command line options.

It supports:
* type checking (if an option should get an integer as value, only ints are accepted, e.g.)
* named sections
* configurations

In the following, the features are described more detailed.


## Type Checking
Each option has a certain type what means that it can only store values of a given type. The following types are currently supported:
* bool
* integer
* double
* string

If the user enters a value for an option that cannot be parsed to the option's type, a runtime_error is thrown.


## Named Sections
If an application has a lot of options, it may make sense to group them by topic, e.g.:
```cpp
 Input options:
  -i, --input <FILE>       The file to read
  -f, --folder <FOLDER>    The folder to read
  -r, --recursive          Whether the folder shall be scanned recursive
 Output options:
  -o, --output <FILE>      The name of the file to write to
```

This can be done using "named sections". A new section is starting by calling the OptionsCont method "beginSection(&lt;NAME&gt;)".


## Configurations
The library can as well read configurations (option values) from a file. The contents of the file are stored into the options as would be done for values given on the command line.

Currently, XML configuration files are supported. The options are passed as characters within tags that are named as the options are. For the example above, a configuration file may look like this:
```xml
<config>
  <input><FILENAME_VALUE></input>
  <folder><FOLDER_VALUE></folder>
  <recursive><RECURSIVE_VALUE></recursive>
  <output><OUTPUT_VALUE></output>
<config>
```
As only those tags are parsed that have the name of a known option, the name of the root element is arbitrary - as long as it's not the name of an option.

