
TEMPLATE = subdirs

SUBDIRS += library

!CONFIG(no_tests) {
    SUBDIRS += tests
    tests.depends = library
}

