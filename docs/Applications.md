# example
A very basic example for using the options. It's basically a "Hello World"-application that allows you to give a name and a greet on the command line. The default for the name is "World" by default, but may be defined using the option --name &lt;NAME&gt;, or -n &lt;NAME&gt; for short. The default for the greet is "Hello" by default, but may be defined using the option --greet &lt;GREET&gt;, or -g &lt;GREET&gt; for short. Additionally, you may optionally change the number of times "&lt;GREET&gt; &lt;NAME&gt;!" is printed using the option --repeat (or -r for short).

Here's the application's main function:
```cpp
int
main(int argc, char *argv[]) {
    ReturnCodes ret = STAT_OK;
    // print how to get the help screen
    try {
        // parse options
        if(!getOptions(argc, argv)) {
            throw std::runtime_error("Please check your options.");
        }
        // check for additional (meta) options
        if(myOptions.getBool("help")) {
            // print the help screen
            OptionsIO::printHelp(std::cout, myOptions);
        } else {
            // do something
            std::string name = myOptions.getString("name");
            std::string greet = myOptions.getString("greet");
            int number = 1;
            if(myOptions.isSet("repeat")) {
                number = myOptions.getInteger("repeat");
            }
            for (int i=0; i<number; ++i) {
                std::cout << greet << " " << name << "!" << std::endl;
            }
        }
    } catch(std::runtime_error &e) {
        cerr << e.what() << endl;
        ret = STAT_READ_COMMENT;
    }
    if(ret!=STAT_OK) {
        cerr << "Quitting (on error)." << endl;
    }
    return ret;
}
```
So what is done is to set up the options, first, using the "getOptions" method. If it does not work for any reason, the application throws an exception which is caught at a later step. If the options could have been parsed, the application first checks whether the --help option was set. In this case, the application prints the help screen and quits - even if other options were given. Otherwise, the application prints the "Hello World" string, which may look different, if the greet string or the name string were changed using the according option. Whether it's printed only once (the default) or several times is determined by asking of the value of the --repeat option - only if it was set, otherwise the default  ```number = 1``` is used. Of course, one could already assign a 1 to the option, so it would be always set, like the case for name and greet. But this is an example only.

You may note that due to the type-sensitivity, we can directly ask for bools, strings and ints. Additionally, the library supports floats.

So, let's take a look how the options are defined and parsed within the "getOptions" method:
```cpp
bool
getOptions(int argc, char *argv[]) {
    myOptions.setHelpHeadAndTail("Usage: options_example [option]+\n\nOptions:\n","");
    // function
    myOptions.add("name", 'n', new Option_String("World"));
    myOptions.setDescription("name", "Defines how to call the user.");
    myOptions.add("greet", 'g', new Option_String("Hello"));
    myOptions.setDescription("greet", "Defines how to greet.");
    myOptions.add("repeat", 'r', new Option_Integer());
    myOptions.setDescription("repeat", "Sets an optional number of repetitions.");
    //
    myOptions.add("help", '?', new Option_Bool());
    myOptions.setDescription("help", "Prints this help screen.");
    // parse
    return OptionsIO::parseAndLoad(myOptions, argc, argv, "");
}
```

You may notice that the options container is a static object. Nope, I do not like static objects either, but that's the most convenient way. First, a header and a tail for the help screen is set. Then, we add the options. In the example above, each option has a name, "name" in the first case, and an abbreviation "n". The abbreviation is optional. Then, a type-aware child of the abstract class option is built and added. The constructor may get a value of the according type, which will be available as default value. You may notice that we did not give a default value for the "repeat" option. That's why we have to test in the main method whether it is set or not.

At the end, we call the static method "parseAndLoad". Actually, this is just parsing the given command line options - no file is loaded.

Yes, well, that's all. It's that ease to use the library.

# tester
The tester application is just for internal testing purposes. It reads a definitions file called "options.txt" which includes definitions of options and other things to set up the options container. Then the application performs the things defined by the [http://texttest.sourceforge.net/](TextTest) test system.



