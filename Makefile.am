SUBDIRS = utils

EXTRA_DIST = msvc tests \
config.h config.h.in \
LICENSE README.md optionslib.doxyconf \
windows_config.h


doc: doxygen

doxygen:
	rm -rf docs/doxygen
	mkdir docs/doxygen
	doxygen sumo.doxyconf &> doxygen.log


dist-hook: abs_distdir=`pwd`/$(distdir)
dist-complete: 
	mv $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)-src-$(VERSION).tar.gz
	mv $(PACKAGE)-$(VERSION).zip $(PACKAGE)-src-$(VERSION).zip
	tar -xzf $(PACKAGE)-doc-$(VERSION).tar.gz $(distdir)
	tar -xzf $(PACKAGE)-tests-$(VERSION).tar.gz $(distdir)
	tar -xzf $(PACKAGE)-src-$(VERSION).tar.gz $(distdir)
	tar -czf $(PACKAGE)-all-$(VERSION).tar.gz $(distdir)
	rm -f $(PACKAGE)-all-$(VERSION).zip
	zip -r $(PACKAGE)-all-$(VERSION).zip $(distdir)
	$(am__remove_distdir)

bin_PROGRAMS = tester example

tester_SOURCES = tester.cpp

example_SOURCES = example.cpp

tester_LDADD = ./utils/options/liboptions.a

example_LDADD = ./utils/options/liboptions.a




