TEMPLATE = subdirs
SUBDIRS = miupnp \
    binlight \
    binlight.cp

CONFIG += ordered

OTHER_FILES += miportral.pri

binlight.cp.depends = binlight \
                   miupnp
