AUX_DIST_GEN =                          \
        $(ac_aux_dir)/install-sh        \
        $(ac_aux_dir)/missing

CMAKE_FILES=                                   \
        CMakeLists.txt                         \
        cmake/FindSDL2.cmake                   \
        cmake/FindSDL2_mixer.cmake             \
        cmake/FindSDL2_net.cmake               \
        cmake/Findm.cmake                      \
        cmake/Findsamplerate.cmake             \
        cmake/config.h.cin

DOC_FILES=

EXTRA_DIST=                             \
        $(AUX_DIST_GEN)                 \
        $(CMAKE_FILES)                  \
        $(DOC_FILES)                    \
        .lvimrc                         \
        HACKING.md                      \
        TODO.md                         \
        win32/win_opendir.c             \
        win32/win_opendir.h

doomdocsdir = ${docdir}/../${PROGRAM_PREFIX}doom
doomdocs_DATA = $(DOC_FILES)

MAINTAINERCLEANFILES =  $(AUX_DIST_GEN)

SUBDIRS=textscreen midiproc opl pcsound data src

DIST_SUBDIRS=$(SUBDIRS)
