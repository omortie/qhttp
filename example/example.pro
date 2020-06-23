TEMPLATE = subdirs

SUBDIRS += helloworld
SUBDIRS += postcollector
SUBDIRS += basic-server
SUBDIRS += teamyarserver

contains(DEFINES, QHTTP_HAS_CLIENT) {
    SUBDIRS += keep-alive
}

