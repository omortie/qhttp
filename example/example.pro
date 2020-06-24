TEMPLATE = subdirs

SUBDIRS += helloworld \
 postcollector \
 basic-server \
 teamyarserver

contains(DEFINES, QHTTP_HAS_CLIENT) {
    SUBDIRS += keep-alive
}

