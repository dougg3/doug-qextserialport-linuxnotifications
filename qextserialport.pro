TEMPLATE = subdirs

include(doc/doc.pri)

module_extserialport_src.subdir = src
module_extserialport_src.target = module-extserialport-src

module_extserialport_examples.subdir = examples
module_extserialport_examples.target = module-extserialport-examples
module_extserialport_examples.depends = module_extserialport_src
!contains(QT_BUILD_PARTS,examples) {
    module_extserialport_examples.CONFIG += no_default_install no_default_target
}

module_extserialport_tests.subdir = tests
module_extserialport_tests.target = module-extserialport-tests
module_extserialport_tests.depends = module_extserialport_src
module_extserialport_tests.CONFIG = no_default_install
!contains(QT_BUILD_PARTS,tests) {
    module_extserialport_tests.CONFIG += no_default_target
}

SUBDIRS += module_extserialport_src \
           module_extserialport_examples \
           module_extserialport_tests
