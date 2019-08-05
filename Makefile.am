SUBDIRS = utils msvc

EXTRA_DIST = README.md ChangeLog AUTHORS CONTRIBUTING.md LICENSE NOTICE.md sumo.doxyconf

doc: doxygen

doxygen:
	rm -rf docs/doxygen
	mkdir docs/doxygen
	doxygen sumo.doxyconf &> doxygen.log

man:
	rm -rf docs/man
	mkdir docs/man
	help2man -N -n "tests application for the options library" tester > docs/man/tester.1
	help2man -N -n "example application for the options library" example > docs/man/example.1

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

tester$(EXEEXT) example$(EXEEXT):
	$(MAKE) -C src $@
