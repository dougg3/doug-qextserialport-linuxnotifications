TEMPLATE = subdirs

include(doc/doc.pri)

module_qesp_src.subdir = src
module_qesp_src.target = module-qesp-src

module_qesp_examples.subdir = examples
module_qesp_examples.target = module-qesp-examples
module_qesp_examples.depends = module_qesp_src
!contains(QT_BUILD_PARTS,examples) {
    module_qesp_examples.CONFIG += no_default_install no_default_target
}

module_qesp_tests.subdir = tests
module_qesp_tests.target = module-qesp-tests
module_qesp_tests.depends = module_qesp_src
module_qesp_tests.CONFIG = no_default_install
!contains(QT_BUILD_PARTS,tests) {
    module_qesp_tests.CONFIG += no_default_target
}

SUBDIRS += module_qesp_src \
           module_qesp_examples \
           module_qesp_tests
