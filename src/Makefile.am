
SUBDIRS = doom setup

execgamesdir = ${bindir}

execgames_PROGRAMS = @PROGRAM_PREFIX@doom     \
                     @PROGRAM_PREFIX@server
EXTRA_PROGRAMS =

noinst_PROGRAMS = @PROGRAM_PREFIX@setup

SETUP_BINARIES = @PROGRAM_PREFIX@doom-setup$(EXEEXT)

execgames_SCRIPTS = $(SETUP_BINARIES)

AM_CFLAGS = -I$(top_srcdir)/textscreen            \
            -I$(top_srcdir)/opl                   \
            -I$(top_srcdir)/pcsound               \
            @SDLMIXER_CFLAGS@ @SDLNET_CFLAGS@

# Common source files used by absolutely everything:

COMMON_SOURCE_FILES=\
crispy.c             crispy.h              \
i_main.c                                   \
i_system.c           i_system.h            \
m_argv.c             m_argv.h              \
m_misc.c             m_misc.h

# Dedicated server (chocolate-server):

DEDSERV_FILES=\
d_dedicated.c                              \
d_iwad.c             d_iwad.h              \
d_mode.c             d_mode.h              \
deh_str.c            deh_str.h             \
i_timer.c            i_timer.h             \
m_config.c           m_config.h            \
z_native.c           z_zone.h

@PROGRAM_PREFIX@server_SOURCES=$(COMMON_SOURCE_FILES) $(DEDSERV_FILES)
@PROGRAM_PREFIX@server_LDADD = @LDFLAGS@ @SDLNET_LIBS@

# Source files used by the game binaries (chocolate-doom, etc.)

GAME_SOURCE_FILES=\
aes_prng.c           aes_prng.h            \
d_event.c            d_event.h             \
                     doomkeys.h            \
                     doomtype.h            \
d_iwad.c             d_iwad.h              \
d_loop.c             d_loop.h              \
d_mode.c             d_mode.h              \
                     d_ticcmd.h            \
deh_str.c            deh_str.h             \
gusconf.c            gusconf.h             \
i_cdmus.c            i_cdmus.h             \
i_glob.c             i_glob.h              \
i_input.c            i_input.h             \
i_joystick.c         i_joystick.h          \
                     i_swap.h              \
i_midipipe.c         i_midipipe.h          \
i_musicpack.c                              \
i_oplmusic.c                               \
i_pcsound.c                                \
i_sdlmusic.c                               \
i_sdlsound.c                               \
i_sound.c            i_sound.h             \
i_timer.c            i_timer.h             \
i_video.c            i_video.h             \
i_videohr.c          i_videohr.h           \
midifile.c           midifile.h            \
mus2mid.c            mus2mid.h             \
m_bbox.c             m_bbox.h              \
m_cheat.c            m_cheat.h             \
m_config.c           m_config.h            \
m_controls.c         m_controls.h          \
m_fixed.c            m_fixed.h             \
sha1.c               sha1.h                \
memio.c              memio.h               \
tables.c             tables.h              \
v_diskicon.c         v_diskicon.h          \
v_video.c            v_video.h             \
                     v_patch.h             \
v_trans.c            v_trans.h             \
w_checksum.c         w_checksum.h          \
w_main.c             w_main.h              \
w_wad.c              w_wad.h               \
w_file.c             w_file.h              \
w_file_stdc.c                              \
w_file_posix.c                             \
w_file_win32.c                             \
w_merge.c            w_merge.h             \
z_zone.c             z_zone.h

DEHACKED_SOURCE_FILES =                    \
deh_defs.h                                 \
deh_io.c             deh_io.h              \
deh_main.c           deh_main.h            \
deh_mapping.c        deh_mapping.h         \
deh_text.c

# Some games support dehacked patches, some don't:

SOURCE_FILES = $(COMMON_SOURCE_FILES) $(GAME_SOURCE_FILES)
SOURCE_FILES_WITH_DEH = $(SOURCE_FILES) $(DEHACKED_SOURCE_FILES)

EXTRA_LIBS =                                               \
               $(top_builddir)/dmr/target/debug/libdmr.a   \
               $(top_builddir)/textscreen/libtextscreen.a  \
               $(top_builddir)/pcsound/libpcsound.a        \
               $(top_builddir)/opl/libopl.a                \
               @LDFLAGS@                                   \
               @SDL_LIBS@                                  \
               @SDLMIXER_LIBS@                             \
               @SDLNET_LIBS@

if HAVE_WINDRES
@PROGRAM_PREFIX@doom_SOURCES=$(SOURCE_FILES_WITH_DEH) resource.rc
else
@PROGRAM_PREFIX@doom_SOURCES=$(SOURCE_FILES_WITH_DEH)
endif

@PROGRAM_PREFIX@doom_LDADD = doom/libdoom.a $(EXTRA_LIBS)


$(SETUP_BINARIES): @PROGRAM_PREFIX@setup$(EXEEXT)
	cp @PROGRAM_PREFIX@setup$(EXEEXT) $@

# Make "make" aware of convenience libraries in subdirectories

doom/libdoom.a:
	$(MAKE) -C doom

# Source files needed for chocolate-setup:

SETUP_FILES=                               \
deh_str.c            deh_str.h             \
d_mode.c             d_mode.h              \
d_iwad.c             d_iwad.h              \
i_timer.c            i_timer.h             \
m_config.c           m_config.h            \
m_controls.c         m_controls.h          \
z_native.c           z_zone.h

if HAVE_WINDRES
@PROGRAM_PREFIX@setup_SOURCES=$(SETUP_FILES) $(COMMON_SOURCE_FILES) setup-res.rc
else
@PROGRAM_PREFIX@setup_SOURCES=$(SETUP_FILES) $(COMMON_SOURCE_FILES)
endif
@PROGRAM_PREFIX@setup_LDADD = setup/libsetup.a                             \
                        $(top_builddir)/dmr/target/debug/libdmr.a   \
                        $(top_builddir)/textscreen/libtextscreen.a   \
                        @LDFLAGS@ @SDL_LIBS@ @SDLMIXER_LIBS@ @SDLNET_LIBS@

EXTRA_DIST =                        \
        CMakeLists.txt              \
        icon.c                      \
        Doom_Screensaver.desktop.in \
        manifest.xml

metainfodir = $(prefix)/share/metainfo
metainfo_DATA =                             \
        @PACKAGE_RDNS@.Doom.metainfo.xml

@PACKAGE_RDNS@.Doom.metainfo.xml : Doom.metainfo.xml
	cp Doom.metainfo.xml $@


appdir = $(prefix)/share/applications
app_DATA =                                 \
           @PACKAGE_RDNS@.Doom.desktop

@PACKAGE_RDNS@.Doom.desktop : Doom.desktop
	cp Doom.desktop $@

screensaverdir = $(prefix)/share/applications/screensavers
screensaver_DATA = @PACKAGE_RDNS@.Doom_Screensaver.desktop

@PACKAGE_RDNS@.Doom_Screensaver.desktop: Doom_Screensaver.desktop
	cp Doom_Screensaver.desktop $@

CLEANFILES = $(execgames_SCRIPTS) $(app_DATA) $(screensaver_DATA)

.rc.o:
	$(WINDRES) $< -o $@
%.o : %.rc
	$(WINDRES) $< -o $@

if HAVE_PYTHON

icon.c : $(top_builddir)/data/doom.png
	$(top_builddir)/data/convert-icon $(top_builddir)/data/doom.png $@

endif

midiread : midifile.c
	$(CC) -DTEST $(CFLAGS) @LDFLAGS@ midifile.c -o $@

MUS2MID_SRC_FILES = mus2mid.c memio.c z_native.c i_system.c m_argv.c m_misc.c
mus2mid : $(MUS2MID_SRC_FILES)
	$(CC) -DSTANDALONE -I$(top_builddir) $(CFLAGS) @LDFLAGS@ \
              $(MUS2MID_SRC_FILES) -o $@

