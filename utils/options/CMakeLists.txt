include_directories("../../")
set(optionslib_STAT_SRCS
   Option.cpp
   Option.h
   OptionsCont.cpp
   OptionsCont.h
   OptionsFileIO_CSV.cpp
   OptionsFileIO_CSV.h
   OptionsFileIO_XML.cpp
   OptionsFileIO_XML.h
   OptionsIO.cpp
   OptionsIO.h
   OptionsLoader.cpp
   OptionsLoader.h
   OptionsParser.cpp
   OptionsParser.h
   OptionsTypedFileIO.h
)

add_library(optionslib STATIC ${optionslib_STAT_SRCS})
set_property(TARGET optionslib PROPERTY PROJECT_LABEL "optionslib")
