####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = kmap
kmap_SOURCES = kmlogdlg.cpp kmoptabout.cpp kmhistory.cpp kmoptpaths.cpp kmoptstealth.cpp kmoptdecoy.cpp kmoptother.cpp kmoptscans.cpp kmoptiondlg.cpp kmconfig.cpp kmnmap.cpp kmapview.cpp kmnmapview.cpp kminfoview.cpp kmportview.cpp kmap.cpp main.cpp 
kmap_LDADD   = -lkdeui -lkdecore -lqt -lXext -lX11 $(LIBSOCKET)


EXTRA_DIST = main.cpp kmap.cpp kmap.h kmap.kdelnk kmap.xpm mini-kmap.xpm kmportview.cpp kmportview.h kminfoview.cpp kminfoview.h kmnmapview.cpp kmnmapview.h kmapview.cpp kmapview.h kmnmap.cpp kmnmap.h kmconfig.cpp kmconfig.h kmoptiondlg.cpp kmoptiondlg.h kmoptscans.cpp kmoptscans.h kmoptother.cpp kmoptother.h kmoptdecoy.cpp kmoptdecoy.h kmoptstealth.cpp kmoptstealth.h kmoptpaths.cpp kmoptpaths.h kmhistory.cpp kmhistory.h kmoptabout.cpp kmoptabout.h kmcommon.h kmlogdlg.cpp kmlogdlg.h 

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) kmap.kdelnk $(kde_appsdir)/Applications/kmap.kdelnk
	$(mkinstalldirs) $(kde_icondir)/
	$(INSTALL_DATA) kmap.xpm $(kde_icondir)/kmap.xpm
	$(mkinstalldirs) $(kde_minidir)/
	$(INSTALL_DATA) mini-kmap.xpm $(kde_minidir)/kmap.xpm

uninstall-local:
	-rm -f $(kde_appsdir)/Applications/kmap.kdelnk
	-rm -f $(kde_icondir)/kmap.xpm
	-rm -f $(kde_minidir)/kmap.xpm

####### kdevelop will overwrite this part!!! (end)############
# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to.
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)
# claim, which subdirectories you want to install
# you can add here more. This one gets installed
bin_PROGRAMS = kmap

kmap_METASOURCES = USE_AUTOMOC

# the library search path.
kmap_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(kmap_METASOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	$(XGETTEXT) -C -ki18n -x $(KDEDIR)/include/kde.pot `find . -name \*.h -o -name \*.cpp -o -name \*.cc` -o ../po/kmap.pot

