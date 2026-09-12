# Memcheck zapisi povezani sa igrom Cascadia++

Izvorni log: `memcheck.log`

Broj izdvojenih zapisa: **75**. Ovo nije broj potvrđenih grešaka u igri.

Izdvojeni su celi zapisi koji u tragu poziva pominju prepoznate izvorne fajlove igre. Podudaranje naziva fajla ne dokazuje uzrok greške. `main.cpp` i generisani Qt fajlovi nisu kriterijum, jer se pojavljuju i u inicijalizaciji biblioteka. Uključene su sve kategorije takvih zapisa, pa i `still reachable`. Ostali zapisi ostaju u izvornom logu.

## Zapis 1: Memorija klasifikovana kao indirektno izgubljena

Početni red u izvornom logu: **1751**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 2 bytes in 1 blocks are indirectly lost in loss record 12 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x698296E: strdup (strdup.c:42)
==28627==    by 0x1D69C4B0: drmGetVersion (in /usr/lib/x86_64-linux-gnu/libdrm.so.2.131.0)
==28627==    by 0x16F04206: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0072F: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x140257C9: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x140265F3: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x1402D202: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF7162: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF761E: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x53A785B: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 2: Memorija klasifikovana kao indirektno izgubljena

Početni red u izvornom logu: **4718**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 7 bytes in 1 blocks are indirectly lost in loss record 117 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x698296E: strdup (strdup.c:42)
==28627==    by 0x1D69C491: drmGetVersion (in /usr/lib/x86_64-linux-gnu/libdrm.so.2.131.0)
==28627==    by 0x16F04206: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0072F: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x140257C9: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x140265F3: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x1402D202: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF7162: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF761E: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x53A785B: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 3: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **14085**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:18`.

Originalni zapis:

```text
==28627== 9 bytes in 1 blocks are still reachable in loss record 437 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x698296E: strdup (strdup.c:42)
==28627==    by 0x6C30221: ??? (in /usr/lib/x86_64-linux-gnu/libfontconfig.so.1.16.1)
==28627==    by 0x6C2B8D8: ??? (in /usr/lib/x86_64-linux-gnu/libfontconfig.so.1.16.1)
==28627==    by 0x5AC9711: QFontconfigDatabase::setupFontEngine(QFontEngineFT*, QFontDef const&) const (in /usr/lib/x86_64-linux-gnu/libQt6Gui.so.6.10.2)
==28627==    by 0x5ACA5E0: QFontconfigDatabase::fontEngine(QFontDef const&, void*) (in /usr/lib/x86_64-linux-gnu/libQt6Gui.so.6.10.2)
==28627==    by 0x58E3808: QFontDatabasePrivate::loadSingleEngine(int, QFontDef const&, QtFontFamily*, QtFontFoundry*, QtFontStyle*, QtFontSize*) (in /usr/lib/x86_64-linux-gnu/libQt6Gui.so.6.10.2)
==28627==    by 0x58E3BB5: QFontDatabasePrivate::loadEngine(int, QFontDef const&, QtFontFamily*, QtFontFoundry*, QtFontStyle*, QtFontSize*) (in /usr/lib/x86_64-linux-gnu/libQt6Gui.so.6.10.2)
==28627==    by 0x58ECB15: QFontDatabasePrivate::findFont(QFontDef const&, int, bool) (in /usr/lib/x86_64-linux-gnu/libQt6Gui.so.6.10.2)
==28627==    by 0x58EDB72: QFontDatabasePrivate::load(QFontPrivate const*, int) (in /usr/lib/x86_64-linux-gnu/libQt6Gui.so.6.10.2)
==28627==    by 0x58CD123: QFontPrivate::engineForScript(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Gui.so.6.10.2)
==28627==    by 0x5901007: QFontMetrics::height() const (in /usr/lib/x86_64-linux-gnu/libQt6Gui.so.6.10.2)
==28627==    by 0x4F25C7A: QStatusBar::reformat() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4F264F9: QStatusBar::setSizeGripEnabled(bool) (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x40A259F: Ui_CascadiaGame::setupUi(QMainWindow*) (ui_mainwindow.h:91)
==28627==    by 0x409ECAC: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:18)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 4: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **14105**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 9 bytes in 1 blocks are still reachable in loss record 438 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79668: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8DE03: pa_log_set_ident (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8E004: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8E0A2: pa_log_levelv_meta (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8ECE4: pa_log_level_meta (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A7F969: pa_config_parse (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A804D2: pa_client_conf_load (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC376C: pa_context_new_with_proplist (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A9229: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 5: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **16217**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:16`.

Originalni zapis:

```text
==28627== 12 bytes in 1 blocks are still reachable in loss record 518 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x6E13679: g_malloc (in /usr/lib/x86_64-linux-gnu/libglib-2.0.so.0.8800.0)
==28627==    by 0x6E32309: g_strdup (in /usr/lib/x86_64-linux-gnu/libglib-2.0.so.0.8800.0)
==28627==    by 0xD4F5A04: pango_font_description_merge (in /usr/lib/x86_64-linux-gnu/libpango-1.0.so.0.5700.0)
==28627==    by 0xCF49E4D: gtk_style_context_get_font (in /usr/lib/x86_64-linux-gnu/libgtk-3.so.0.2420.32)
==28627==    by 0xCC5FC71: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/platformthemes/libqgtk3.so)
==28627==    by 0xCC60224: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/platformthemes/libqgtk3.so)
==28627==    by 0x56820C6: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Gui.so.6.10.2)
==28627==    by 0x568B997: QGuiApplication::font() (in /usr/lib/x86_64-linux-gnu/libQt6Gui.so.6.10.2)
==28627==    by 0x58CD314: QFont::QFont() (in /usr/lib/x86_64-linux-gnu/libQt6Gui.so.6.10.2)
==28627==    by 0x4D234AD: QWidgetPrivate::QWidgetPrivate(QtPrivate_6_10_2) (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4EA9737: QMainWindow::QMainWindow(QWidget*, QFlags<Qt::WindowType>) (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x409EC2B: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:16)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 6: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **70074**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 16 bytes in 1 blocks are still reachable in loss record 2,443 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9EE69: pa_queue_push (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA07C0: pa_pstream_send_packet (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3577: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA36DA: pa_pstream_send_tagstruct_with_creds (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E0C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 7: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **163702**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 34 bytes in 1 blocks are still reachable in loss record 5,789 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79190: pa_xmemdup (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD2C: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CAF5: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA0752: pa_pstream_send_packet (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3577: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA36DA: pa_pstream_send_tagstruct_with_creds (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E0C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 8: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **163730**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 34 bytes in 1 blocks are still reachable in loss record 5,790 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79190: pa_xmemdup (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD2C: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CB15: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9EDF9: pa_queue_push (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA07C0: pa_pstream_send_packet (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3577: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA36DA: pa_pstream_send_tagstruct_with_creds (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E0C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 9: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **167187**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 35 bytes in 1 blocks are still reachable in loss record 5,892 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79190: pa_xmemdup (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD2C: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CAB5: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9E3FB: pa_packet_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9E4EE: pa_packet_new_data (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3555: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA36DA: pa_pstream_send_tagstruct_with_creds (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E0C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 10: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **171400**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 36 bytes in 1 blocks are still reachable in loss record 6,017 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79190: pa_xmemdup (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD2C: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CE65: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8A104: pa_hashmap_put (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BDC751: pa_proplist_sets (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC3643: pa_context_new_with_proplist (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A9229: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 11: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **175972**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 37 bytes in 1 blocks are still reachable in loss record 6,152 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79190: pa_xmemdup (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD2C: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BCD755: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD1DDC: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC7BB0: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A92EE: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 12: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **198470**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 40 bytes in 1 blocks are still reachable in loss record 6,889 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB5DAC: pa_mutex_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB600B: pa_static_mutex_get (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CED4: pa_once_begin (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8BB33: pa_init_i18n (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD10F1: pa_mainloop_new (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A920F: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 13: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **198491**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 40 bytes in 1 blocks are still reachable in loss record 6,890 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB5DAC: pa_mutex_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB600B: pa_static_mutex_get (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CED4: pa_once_begin (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D043: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8A104: pa_hashmap_put (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BDC751: pa_proplist_sets (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC3643: pa_context_new_with_proplist (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A9229: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 14: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **198514**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 40 bytes in 1 blocks are still reachable in loss record 6,891 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB5DAC: pa_mutex_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB600B: pa_static_mutex_get (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CED4: pa_once_begin (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8E099: pa_log_levelv_meta (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8ECE4: pa_log_level_meta (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A7F969: pa_config_parse (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A804D2: pa_client_conf_load (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC376C: pa_context_new_with_proplist (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A9229: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 15: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **198538**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 40 bytes in 1 blocks are still reachable in loss record 6,892 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB5DAC: pa_mutex_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB600B: pa_static_mutex_get (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CED4: pa_once_begin (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8B763: pa_page_size (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9B57F: pa_mempool_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC37EF: pa_context_new_with_proplist (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A9229: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 16: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **198560**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 40 bytes in 1 blocks are still reachable in loss record 6,893 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB5DAC: pa_mutex_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB600B: pa_static_mutex_get (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CED4: pa_once_begin (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D043: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB00EA: pa_tagstruct_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5D00: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 17: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **198586**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 40 bytes in 1 blocks are still reachable in loss record 6,894 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB5DAC: pa_mutex_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB600B: pa_static_mutex_get (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CED4: pa_once_begin (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D043: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9E3FB: pa_packet_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9E4EE: pa_packet_new_data (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3555: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA36DA: pa_pstream_send_tagstruct_with_creds (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E0C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 18: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **198615**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 40 bytes in 1 blocks are still reachable in loss record 6,895 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB5DAC: pa_mutex_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB600B: pa_static_mutex_get (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CED4: pa_once_begin (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D043: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA0752: pa_pstream_send_packet (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3577: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA36DA: pa_pstream_send_tagstruct_with_creds (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E0C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 19: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **198643**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 40 bytes in 1 blocks are still reachable in loss record 6,896 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB5DAC: pa_mutex_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB600B: pa_static_mutex_get (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CED4: pa_once_begin (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D043: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9EDF9: pa_queue_push (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA07C0: pa_pstream_send_packet (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3577: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA36DA: pa_pstream_send_tagstruct_with_creds (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E0C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 20: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **198672**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 40 bytes in 1 blocks are still reachable in loss record 6,897 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB5DAC: pa_mutex_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB600B: pa_static_mutex_get (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CED4: pa_once_begin (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D043: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9FF56: pa_pdispatch_register_reply (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E2A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 21: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **198698**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 40 bytes in 1 blocks are still reachable in loss record 6,898 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB5DAC: pa_mutex_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB600B: pa_static_mutex_get (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CED4: pa_once_begin (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D043: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD1DDC: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC7BB0: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A92EE: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 22: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **203181**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 41 bytes in 1 blocks are still reachable in loss record 7,034 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79190: pa_xmemdup (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD2C: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB00C5: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB00EA: pa_tagstruct_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5D00: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 23: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **203207**.

Fajlovi igre: `controller.cpp:9`.

Originalni zapis:

```text
==28627== 41 bytes in 1 blocks are still reachable in loss record 7,035 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42FDB1F: malloc (rtld-malloc.h:56)
==28627==    by 0x42FDB1F: strdup (strdup.c:42)
==28627==    by 0x42E9AAF: _dl_load_cache_lookup (dl-cache.c:498)
==28627==    by 0x42DBE27: _dl_map_new_object (dl-load.c:2073)
==28627==    by 0x42D49FC: openaux (dl-deps.c:64)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D4E2F: _dl_map_object_deps (dl-deps.c:232)
==28627==    by 0x42DFE17: dl_open_worker_begin (dl-open.c:609)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0xC3685B0: QtWaylandClient::QWaylandDecorationFactory::create(QString const&, QList<QString> const&) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A5B28: QtWaylandClient::QWaylandWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3968C5: QtWaylandClient::QWaylandShmWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A7C2A: QtWaylandClient::QWaylandWindow::initWindow() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A92F7: QtWaylandClient::QWaylandWindow::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0x4D3B7C6: QWidgetPrivate::show_sys() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3BA20: QWidgetPrivate::show_helper() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3E432: QWidgetPrivate::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x403CF25: Controller::Controller(QObject*) (controller.cpp:9)
```

## Zapis 24: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **203241**.

Fajlovi igre: `controller.cpp:9`.

Originalni zapis:

```text
==28627== 41 bytes in 1 blocks are still reachable in loss record 7,036 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42DEFD8: malloc (rtld-malloc.h:56)
==28627==    by 0x42DEFD8: _dl_new_object (dl-object.c:199)
==28627==    by 0x42D9EA0: _dl_map_object_from_fd (dl-load.c:1065)
==28627==    by 0x42DB989: _dl_map_new_object (dl-load.c:2206)
==28627==    by 0x42D49FC: openaux (dl-deps.c:64)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D4E2F: _dl_map_object_deps (dl-deps.c:232)
==28627==    by 0x42DFE17: dl_open_worker_begin (dl-open.c:609)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0xC3685B0: QtWaylandClient::QWaylandDecorationFactory::create(QString const&, QList<QString> const&) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A5B28: QtWaylandClient::QWaylandWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3968C5: QtWaylandClient::QWaylandShmWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A7C2A: QtWaylandClient::QWaylandWindow::initWindow() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A92F7: QtWaylandClient::QWaylandWindow::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0x4D3B7C6: QWidgetPrivate::show_sys() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3BA20: QWidgetPrivate::show_helper() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3E432: QWidgetPrivate::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x403CF25: Controller::Controller(QObject*) (controller.cpp:9)
```

## Zapis 25: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **207543**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 42 bytes in 1 blocks are still reachable in loss record 7,163 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79190: pa_xmemdup (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD2C: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CAD5: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9FF56: pa_pdispatch_register_reply (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E2A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 26: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **207655**.

Fajlovi igre: `settings.cpp:9`.

Originalni zapis:

```text
==28627== 43 bytes in 1 blocks are still reachable in loss record 7,167 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42FDB1F: malloc (rtld-malloc.h:56)
==28627==    by 0x42FDB1F: strdup (strdup.c:42)
==28627==    by 0x42E9AAF: _dl_load_cache_lookup (dl-cache.c:498)
==28627==    by 0x42DBE27: _dl_map_new_object (dl-load.c:2073)
==28627==    by 0x42E0460: dl_open_worker_begin (dl-open.c:535)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x1DA7B615: ???
==28627==    by 0x1DA8A199: ???
==28627==    by 0x1DA96423: ???
==28627==    by 0x16F07B14: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0CD1A: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0FAB3: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x140257C9: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x140265F3: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x1402D202: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF7162: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF761E: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x53A785B: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
```

## Zapis 27: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **207689**.

Fajlovi igre: `settings.cpp:9`.

Originalni zapis:

```text
==28627== 43 bytes in 1 blocks are still reachable in loss record 7,168 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42DEFD8: malloc (rtld-malloc.h:56)
==28627==    by 0x42DEFD8: _dl_new_object (dl-object.c:199)
==28627==    by 0x42D9EA0: _dl_map_object_from_fd (dl-load.c:1065)
==28627==    by 0x42DB989: _dl_map_new_object (dl-load.c:2206)
==28627==    by 0x42E0460: dl_open_worker_begin (dl-open.c:535)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x1DA7B615: ???
==28627==    by 0x1DA8A199: ???
==28627==    by 0x1DA96423: ???
==28627==    by 0x16F07B14: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0CD1A: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0FAB3: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x140257C9: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x140265F3: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x1402D202: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF7162: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF761E: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x53A785B: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
```

## Zapis 28: Memorija klasifikovana kao indirektno izgubljena

Početni red u izvornom logu: **219866**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 45 bytes in 1 blocks are indirectly lost in loss record 7,527 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x698296E: strdup (strdup.c:42)
==28627==    by 0x1D69C4CF: drmGetVersion (in /usr/lib/x86_64-linux-gnu/libdrm.so.2.131.0)
==28627==    by 0x16F04206: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0072F: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x140257C9: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x140265F3: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x1402D202: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF7162: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF761E: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x53A785B: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 29: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **246097**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 48 bytes in 1 blocks are still reachable in loss record 8,480 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8A1A9: pa_hashmap_put (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BDC751: pa_proplist_sets (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC3643: pa_context_new_with_proplist (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A9229: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 30: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **246116**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 48 bytes in 1 blocks are still reachable in loss record 8,481 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8A1A9: pa_hashmap_put (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79A11: pa_proplist_sets (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA34A1: pa_init_proplist (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC596C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7A9EBA1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9FE0F: pa_pdispatch_run (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC522A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AA4AC1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA594E: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD860F: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 31: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **246143**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 48 bytes in 1 blocks are still reachable in loss record 8,482 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8A1A9: pa_hashmap_put (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79A11: pa_proplist_sets (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3462: pa_init_proplist (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC596C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7A9EBA1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9FE0F: pa_pdispatch_run (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC522A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AA4AC1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA594E: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD860F: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 32: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **246170**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 48 bytes in 1 blocks are still reachable in loss record 8,483 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8A1A9: pa_hashmap_put (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79A11: pa_proplist_sets (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA342A: pa_init_proplist (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC596C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7A9EBA1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9FE0F: pa_pdispatch_run (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC522A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AA4AC1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA594E: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD860F: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 33: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **246197**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 48 bytes in 1 blocks are still reachable in loss record 8,484 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8A1A9: pa_hashmap_put (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79A11: pa_proplist_sets (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA33ED: pa_init_proplist (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC596C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7A9EBA1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9FE0F: pa_pdispatch_run (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC522A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AA4AC1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA594E: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD860F: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 34: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **246224**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 48 bytes in 1 blocks are still reachable in loss record 8,485 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8A1A9: pa_hashmap_put (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79A11: pa_proplist_sets (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA33B6: pa_init_proplist (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC596C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7A9EBA1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9FE0F: pa_pdispatch_run (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC522A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AA4AC1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA594E: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD860F: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 35: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **246251**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 48 bytes in 1 blocks are still reachable in loss record 8,486 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8A1A9: pa_hashmap_put (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79A11: pa_proplist_sets (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3381: pa_init_proplist (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC596C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7A9EBA1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9FE0F: pa_pdispatch_run (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC522A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AA4AC1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA594E: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD860F: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 36: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **246278**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 48 bytes in 1 blocks are still reachable in loss record 8,487 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8A1A9: pa_hashmap_put (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A79A11: pa_proplist_sets (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3342: pa_init_proplist (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC596C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7A9EBA1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9FE0F: pa_pdispatch_run (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC522A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AA4AC1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA594E: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD860F: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 37: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **286540**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 64 bytes in 1 blocks are still reachable in loss record 9,886 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA0029: pa_pdispatch_register_reply (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E2A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 38: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **286562**.

Fajlovi igre: `controller.cpp:9`.

Originalni zapis:

```text
==28627== 64 bytes in 1 blocks are still reachable in loss record 9,887 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42E055B: malloc (rtld-malloc.h:56)
==28627==    by 0x42E055B: resize_scopes (dl-open.c:294)
==28627==    by 0x42E055B: dl_open_worker_begin (dl-open.c:687)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0xC3685B0: QtWaylandClient::QWaylandDecorationFactory::create(QString const&, QList<QString> const&) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A5B28: QtWaylandClient::QWaylandWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3968C5: QtWaylandClient::QWaylandShmWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A7C2A: QtWaylandClient::QWaylandWindow::initWindow() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A92F7: QtWaylandClient::QWaylandWindow::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0x4D3B7C6: QWidgetPrivate::show_sys() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3BA20: QWidgetPrivate::show_helper() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3E432: QWidgetPrivate::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x403CF25: Controller::Controller(QObject*) (controller.cpp:9)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 39: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **290708**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 73 bytes in 1 blocks are still reachable in loss record 10,027 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42FDB1F: malloc (rtld-malloc.h:56)
==28627==    by 0x42FDB1F: strdup (strdup.c:42)
==28627==    by 0x42DB90D: _dl_map_new_object (dl-load.c:2139)
==28627==    by 0x42E0460: dl_open_worker_begin (dl-open.c:535)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x53A7837: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 40: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **290736**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 73 bytes in 1 blocks are still reachable in loss record 10,028 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42DEFD8: malloc (rtld-malloc.h:56)
==28627==    by 0x42DEFD8: _dl_new_object (dl-object.c:199)
==28627==    by 0x42D9EA0: _dl_map_object_from_fd (dl-load.c:1065)
==28627==    by 0x42DB989: _dl_map_new_object (dl-load.c:2206)
==28627==    by 0x42E0460: dl_open_worker_begin (dl-open.c:535)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x53A7837: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 41: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **291050**.

Fajlovi igre: `controller.cpp:9`.

Originalni zapis:

```text
==28627== 78 bytes in 1 blocks are still reachable in loss record 10,047 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42FDB1F: malloc (rtld-malloc.h:56)
==28627==    by 0x42FDB1F: strdup (strdup.c:42)
==28627==    by 0x42DB90D: _dl_map_new_object (dl-load.c:2139)
==28627==    by 0x42E0460: dl_open_worker_begin (dl-open.c:535)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0xC3685B0: QtWaylandClient::QWaylandDecorationFactory::create(QString const&, QList<QString> const&) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A5B28: QtWaylandClient::QWaylandWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3968C5: QtWaylandClient::QWaylandShmWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A7C2A: QtWaylandClient::QWaylandWindow::initWindow() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A92F7: QtWaylandClient::QWaylandWindow::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0x4D3B7C6: QWidgetPrivate::show_sys() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3BA20: QWidgetPrivate::show_helper() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3E432: QWidgetPrivate::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x403CF25: Controller::Controller(QObject*) (controller.cpp:9)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 42: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **291081**.

Fajlovi igre: `controller.cpp:9`.

Originalni zapis:

```text
==28627== 78 bytes in 1 blocks are still reachable in loss record 10,048 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42DEFD8: malloc (rtld-malloc.h:56)
==28627==    by 0x42DEFD8: _dl_new_object (dl-object.c:199)
==28627==    by 0x42D9EA0: _dl_map_object_from_fd (dl-load.c:1065)
==28627==    by 0x42DB989: _dl_map_new_object (dl-load.c:2206)
==28627==    by 0x42E0460: dl_open_worker_begin (dl-open.c:535)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0xC3685B0: QtWaylandClient::QWaylandDecorationFactory::create(QString const&, QList<QString> const&) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A5B28: QtWaylandClient::QWaylandWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3968C5: QtWaylandClient::QWaylandShmWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A7C2A: QtWaylandClient::QWaylandWindow::initWindow() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A92F7: QtWaylandClient::QWaylandWindow::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0x4D3B7C6: QWidgetPrivate::show_sys() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3BA20: QWidgetPrivate::show_helper() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3E432: QWidgetPrivate::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x403CF25: Controller::Controller(QObject*) (controller.cpp:9)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 43: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **334050**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 88 bytes in 1 blocks are still reachable in loss record 11,645 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x6BE7CCA: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD1E79: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC7BB0: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A92EE: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 44: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **355094**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 96 bytes in 1 blocks are still reachable in loss record 12,436 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA08C1: pa_pstream_send_packet (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3577: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA36DA: pa_pstream_send_tagstruct_with_creds (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E0C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 45: Memorija klasifikovana kao definitivno izgubljena

Početni red u izvornom logu: **359606**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 110 (56 direct, 54 indirect) bytes in 1 blocks are definitely lost in loss record 12,594 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x1D69C45E: drmGetVersion (in /usr/lib/x86_64-linux-gnu/libdrm.so.2.131.0)
==28627==    by 0x16F04206: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0072F: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x140257C9: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x140265F3: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x1402D202: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF7162: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF761E: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x53A785B: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 46: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **372451**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 152 bytes in 1 blocks are still reachable in loss record 13,060 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9E471: pa_packet_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9E4EE: pa_packet_new_data (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3555: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA36DA: pa_pstream_send_tagstruct_with_creds (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E0C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 47: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **372476**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 152 bytes in 1 blocks are still reachable in loss record 13,061 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9E471: pa_packet_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9E4EE: pa_packet_new_data (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3555: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5986: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7A9EBA1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9FE0F: pa_pdispatch_run (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC522A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AA4AC1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA594E: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD860F: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 48: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **373678**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 168 bytes in 1 blocks are still reachable in loss record 13,104 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB0129: pa_tagstruct_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5D00: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 49: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **373700**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 168 bytes in 1 blocks are still reachable in loss record 13,105 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x7A78F5A: pa_xmalloc (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB0129: pa_tagstruct_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5933: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7A9EBA1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9FE0F: pa_pdispatch_run (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC522A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AA4AC1: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA594E: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD860F: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 50: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **374708**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 184 bytes in 1 blocks are still reachable in loss record 13,144 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x7A324AF: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A331A1: dbus_message_new_error (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A2392C: dbus_connection_send_with_reply (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A23CF3: dbus_connection_send_with_reply_and_block (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A23F9F: dbus_bus_register (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A24288: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x1DA7E9F9: ???
==28627==    by 0x1DA8147E: ???
==28627==    by 0xBF89500: ???
==28627==    by 0xBF4BAF6: ???
==28627==    by 0xBF43D7A: ???
==28627==    by 0xBF441AD: ???
==28627==    by 0xBF5EBBD: ???
==28627==    by 0x53F6EDC: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53F7544: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A8D18: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 51: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **374738**.

Fajlovi igre: `settings.cpp:9`.

Originalni zapis:

```text
==28627== 184 bytes in 1 blocks are still reachable in loss record 13,145 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x7A324AF: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A33726: _dbus_message_loader_queue_messages (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A3ED5B: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A3EF3F: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A3F28B: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A3F80D: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A21F42: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A380BF: dbus_pending_call_block (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A23D0D: dbus_connection_send_with_reply_and_block (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A23F9F: dbus_bus_register (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A24288: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x1DA7E9F9: ???
==28627==    by 0x1DA8147E: ???
==28627==    by 0xBF89500: ???
==28627==    by 0xBF4BAF6: ???
==28627==    by 0xBF43D7A: ???
==28627==    by 0xBF441AD: ???
==28627==    by 0xBF5EBBD: ???
==28627==    by 0x53F6EDC: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53F7544: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A8D18: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
```

## Zapis 52: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **376501**.

Fajlovi igre: `settings.cpp:9`.

Originalni zapis:

```text
==28627== 223 bytes in 1 blocks are still reachable in loss record 13,209 of 13,936
==28627==    at 0x4B2407F: realloc (vg_replace_malloc.c:1804)
==28627==    by 0x7A40A64: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A41910: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A4274C: _dbus_string_copy_len (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A45248: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A2C8B0: _dbus_type_writer_write_basic (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A319A1: dbus_message_iter_append_basic (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A33220: dbus_message_new_error (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A2392C: dbus_connection_send_with_reply (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A23CF3: dbus_connection_send_with_reply_and_block (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A23F9F: dbus_bus_register (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A24288: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x1DA7E9F9: ???
==28627==    by 0x1DA8147E: ???
==28627==    by 0xBF89500: ???
==28627==    by 0xBF4BAF6: ???
==28627==    by 0xBF43D7A: ???
==28627==    by 0xBF441AD: ???
==28627==    by 0xBF5EBBD: ???
==28627==    by 0x53F6EDC: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53F7544: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A8D18: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
```

## Zapis 53: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **378380**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 256 bytes in 4 blocks are still reachable in loss record 13,274 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42E055B: malloc (rtld-malloc.h:56)
==28627==    by 0x42E055B: resize_scopes (dl-open.c:294)
==28627==    by 0x42E055B: dl_open_worker_begin (dl-open.c:687)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x53A7837: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 54: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **380443**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 320 bytes in 1 blocks are still reachable in loss record 13,343 of 13,936
==28627==    at 0x4B2407F: realloc (vg_replace_malloc.c:1804)
==28627==    by 0x7A41D36: _dbus_string_lengthen (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A2C1A5: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A2C65F: _dbus_type_writer_recurse (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A2CACA: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A2DB73: _dbus_header_set_field_basic (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A2DCEB: ??? (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A319AC: dbus_message_iter_append_basic (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A320C7: dbus_message_append_args_valist (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A3233E: dbus_message_append_args (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x7A2501F: dbus_bus_name_has_owner (in /usr/lib/x86_64-linux-gnu/libdbus-1.so.3.38.3)
==28627==    by 0x1DA81185: ???
==28627==    by 0xBF89500: ???
==28627==    by 0xBF4BAF6: ???
==28627==    by 0xBF43D7A: ???
==28627==    by 0xBF441AD: ???
==28627==    by 0xBF5EBBD: ???
==28627==    by 0x53F6EDC: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53F7544: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A8D18: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
```

## Zapis 55: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **381484**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 352 bytes in 4 blocks are still reachable in loss record 13,380 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42DDDC8: malloc (rtld-malloc.h:56)
==28627==    by 0x42DDDC8: add_dependency (dl-lookup.c:686)
==28627==    by 0x42DDDC8: _dl_lookup_symbol_x (dl-lookup.c:856)
==28627==    by 0x42E2050: resolve_map (dl-reloc.c:190)
==28627==    by 0x42E2050: elf_machine_rela (dl-machine.h:280)
==28627==    by 0x42E2050: elf_dynamic_do_Rela (do-rel.h:138)
==28627==    by 0x42E2050: _dl_relocate_object_no_relro (dl-reloc.c:296)
==28627==    by 0x42E5580: _dl_relocate_object (dl-reloc.c:346)
==28627==    by 0x42DFFCA: _dl_open_relocate_one_object (dl-open.c:452)
==28627==    by 0x42DFFCA: dl_open_worker_begin (dl-open.c:682)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x53A7837: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 56: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **382027**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 384 bytes in 3 blocks are still reachable in loss record 13,399 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42D3DCA: malloc (rtld-malloc.h:56)
==28627==    by 0x42D3DCA: _dl_close_worker.part.0 (dl-close.c:360)
==28627==    by 0x42D471B: _dl_close_worker (dl-close.c:120)
==28627==    by 0x42D471B: _dl_close (dl-close.c:793)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x69666D2: dlclose@@GLIBC_2.34 (dlclose.c:31)
==28627==    by 0x1DA87363: ???
==28627==    by 0x1DA956B6: ???
==28627==    by 0x16F079B8: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF531E: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EDE982: av_buffer_unref (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x1402660E: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x1402D202: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF7162: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF761E: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x53A785B: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 57: Memorija klasifikovana kao moguće izgubljena

Početni red u izvornom logu: **386906**.

Fajlovi igre: `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 720 bytes in 1 blocks are possibly lost in loss record 13,575 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42E6A90: malloc (rtld-malloc.h:56)
==28627==    by 0x42E6A90: _dl_resize_dtv (dl-tls.c:569)
==28627==    by 0x42E7883: _dl_update_slotinfo (dl-tls.c:894)
==28627==    by 0x42E03E6: update_tls_slotinfo (dl-open.c:408)
==28627==    by 0x42E03E6: dl_open_worker_begin (dl-open.c:730)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x1DA7B615: ???
==28627==    by 0x1DA8A199: ???
==28627==    by 0x1DA96423: ???
==28627==    by 0x16F07B14: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0CD1A: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0FAB3: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x140257C9: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x140265F3: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x1402D202: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF7162: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF761E: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x53A785B: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
```

## Zapis 58: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **387593**.

Fajlovi igre: `controller.cpp:9`.

Originalni zapis:

```text
==28627== 912 bytes in 2 blocks are still reachable in loss record 13,601 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x42E92BD: calloc (rtld-malloc.h:44)
==28627==    by 0x42E92BD: _dl_check_map_versions (dl-version.c:280)
==28627==    by 0x42E010C: dl_open_worker_begin (dl-open.c:617)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0xC3685B0: QtWaylandClient::QWaylandDecorationFactory::create(QString const&, QList<QString> const&) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A5B28: QtWaylandClient::QWaylandWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3968C5: QtWaylandClient::QWaylandShmWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A7C2A: QtWaylandClient::QWaylandWindow::initWindow() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A92F7: QtWaylandClient::QWaylandWindow::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0x4D3B7C6: QWidgetPrivate::show_sys() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3BA20: QWidgetPrivate::show_helper() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3E432: QWidgetPrivate::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x403CF25: Controller::Controller(QObject*) (controller.cpp:9)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 59: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **389741**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 1,184 bytes in 13 blocks are still reachable in loss record 13,681 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42D3DCA: malloc (rtld-malloc.h:56)
==28627==    by 0x42D3DCA: _dl_close_worker.part.0 (dl-close.c:360)
==28627==    by 0x42D471B: _dl_close_worker (dl-close.c:120)
==28627==    by 0x42D471B: _dl_close (dl-close.c:793)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x69666D2: dlclose@@GLIBC_2.34 (dlclose.c:31)
==28627==    by 0x1DA995B2: ???
==28627==    by 0x16F0CE20: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0FAB3: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x140257C9: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x140265F3: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x1402D202: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF7162: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF761E: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x53A785B: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 60: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **389955**.

Fajlovi igre: `controller.cpp:9`.

Originalni zapis:

```text
==28627== 1,255 bytes in 1 blocks are still reachable in loss record 13,690 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x42DECAC: calloc (rtld-malloc.h:44)
==28627==    by 0x42DECAC: _dl_new_object (dl-object.c:92)
==28627==    by 0x42D9EA0: _dl_map_object_from_fd (dl-load.c:1065)
==28627==    by 0x42DB989: _dl_map_new_object (dl-load.c:2206)
==28627==    by 0x42D49FC: openaux (dl-deps.c:64)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D4E2F: _dl_map_object_deps (dl-deps.c:232)
==28627==    by 0x42DFE17: dl_open_worker_begin (dl-open.c:609)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0xC3685B0: QtWaylandClient::QWaylandDecorationFactory::create(QString const&, QList<QString> const&) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A5B28: QtWaylandClient::QWaylandWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3968C5: QtWaylandClient::QWaylandShmWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A7C2A: QtWaylandClient::QWaylandWindow::initWindow() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A92F7: QtWaylandClient::QWaylandWindow::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0x4D3B7C6: QWidgetPrivate::show_sys() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3BA20: QWidgetPrivate::show_helper() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3E432: QWidgetPrivate::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x403CF25: Controller::Controller(QObject*) (controller.cpp:9)
```

## Zapis 61: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **389989**.

Fajlovi igre: `settings.cpp:9`.

Originalni zapis:

```text
==28627== 1,257 bytes in 1 blocks are still reachable in loss record 13,691 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x42DECAC: calloc (rtld-malloc.h:44)
==28627==    by 0x42DECAC: _dl_new_object (dl-object.c:92)
==28627==    by 0x42D9EA0: _dl_map_object_from_fd (dl-load.c:1065)
==28627==    by 0x42DB989: _dl_map_new_object (dl-load.c:2206)
==28627==    by 0x42E0460: dl_open_worker_begin (dl-open.c:535)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x1DA7B615: ???
==28627==    by 0x1DA8A199: ???
==28627==    by 0x1DA96423: ???
==28627==    by 0x16F07B14: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0CD1A: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0FAB3: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x140257C9: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x140265F3: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x1402D202: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF7162: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF761E: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x53A785B: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
```

## Zapis 62: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **390376**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 1,313 bytes in 1 blocks are still reachable in loss record 13,704 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x42DECAC: calloc (rtld-malloc.h:44)
==28627==    by 0x42DECAC: _dl_new_object (dl-object.c:92)
==28627==    by 0x42D9EA0: _dl_map_object_from_fd (dl-load.c:1065)
==28627==    by 0x42DB989: _dl_map_new_object (dl-load.c:2206)
==28627==    by 0x42E0460: dl_open_worker_begin (dl-open.c:535)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x53A7837: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 63: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **390405**.

Fajlovi igre: `controller.cpp:9`.

Originalni zapis:

```text
==28627== 1,318 bytes in 1 blocks are still reachable in loss record 13,705 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x42DECAC: calloc (rtld-malloc.h:44)
==28627==    by 0x42DECAC: _dl_new_object (dl-object.c:92)
==28627==    by 0x42D9EA0: _dl_map_object_from_fd (dl-load.c:1065)
==28627==    by 0x42DB989: _dl_map_new_object (dl-load.c:2206)
==28627==    by 0x42E0460: dl_open_worker_begin (dl-open.c:535)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0xC3685B0: QtWaylandClient::QWaylandDecorationFactory::create(QString const&, QList<QString> const&) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A5B28: QtWaylandClient::QWaylandWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3968C5: QtWaylandClient::QWaylandShmWindow::createDecoration() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A7C2A: QtWaylandClient::QWaylandWindow::initWindow() (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0xC3A92F7: QtWaylandClient::QWaylandWindow::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6WaylandClient.so.6.10.2)
==28627==    by 0x4D3B7C6: QWidgetPrivate::show_sys() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3BA20: QWidgetPrivate::show_helper() (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x4D3E432: QWidgetPrivate::setVisible(bool) (in /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.10.2)
==28627==    by 0x403CF25: Controller::Controller(QObject*) (controller.cpp:9)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 64: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **392905**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 2,914 bytes in 70 blocks are still reachable in loss record 13,792 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42FDB1F: malloc (rtld-malloc.h:56)
==28627==    by 0x42FDB1F: strdup (strdup.c:42)
==28627==    by 0x42E9AAF: _dl_load_cache_lookup (dl-cache.c:498)
==28627==    by 0x42DBE27: _dl_map_new_object (dl-load.c:2073)
==28627==    by 0x42D49FC: openaux (dl-deps.c:64)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D4E2F: _dl_map_object_deps (dl-deps.c:232)
==28627==    by 0x42DFE17: dl_open_worker_begin (dl-open.c:609)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x53A7837: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 65: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **392937**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 2,914 bytes in 70 blocks are still reachable in loss record 13,793 of 13,936
==28627==    at 0x4B1C858: malloc (vg_replace_malloc.c:447)
==28627==    by 0x42DEFD8: malloc (rtld-malloc.h:56)
==28627==    by 0x42DEFD8: _dl_new_object (dl-object.c:199)
==28627==    by 0x42D9EA0: _dl_map_object_from_fd (dl-load.c:1065)
==28627==    by 0x42DB989: _dl_map_new_object (dl-load.c:2206)
==28627==    by 0x42D49FC: openaux (dl-deps.c:64)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D4E2F: _dl_map_object_deps (dl-deps.c:232)
==28627==    by 0x42DFE17: dl_open_worker_begin (dl-open.c:609)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x53A7837: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 66: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **393820**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 4,136 bytes in 1 blocks are still reachable in loss record 13,824 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x7A794C2: pa_xmalloc0 (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD21: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CE65: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8A104: pa_hashmap_put (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BDC751: pa_proplist_sets (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC3643: pa_context_new_with_proplist (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A9229: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 67: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **393842**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 4,136 bytes in 1 blocks are still reachable in loss record 13,825 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x7A794C2: pa_xmalloc0 (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD21: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB00C5: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AB00EA: pa_tagstruct_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5D00: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 68: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **393867**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 4,136 bytes in 1 blocks are still reachable in loss record 13,826 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x7A794C2: pa_xmalloc0 (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD21: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CAB5: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9E3FB: pa_packet_new (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9E4EE: pa_packet_new_data (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3555: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA36DA: pa_pstream_send_tagstruct_with_creds (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E0C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 69: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **393895**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 4,136 bytes in 1 blocks are still reachable in loss record 13,827 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x7A794C2: pa_xmalloc0 (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD21: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CAF5: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA0752: pa_pstream_send_packet (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3577: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA36DA: pa_pstream_send_tagstruct_with_creds (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E0C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 70: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **393922**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 4,136 bytes in 1 blocks are still reachable in loss record 13,828 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x7A794C2: pa_xmalloc0 (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD21: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CB15: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9EDF9: pa_queue_push (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA07C0: pa_pstream_send_packet (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA3577: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7AA36DA: pa_pstream_send_tagstruct_with_creds (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E0C: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 71: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **393950**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 4,136 bytes in 1 blocks are still reachable in loss record 13,829 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x7A794C2: pa_xmalloc0 (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD21: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9CAD5: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A9FF56: pa_pdispatch_register_reply (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BC5E2A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC718A: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7AACBEA: ??? (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD8799: pa_mainloop_dispatch (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BD8D4E: pa_mainloop_iterate (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A926E: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 72: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **393975**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 4,136 bytes in 1 blocks are still reachable in loss record 13,830 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x7A794C2: pa_xmalloc0 (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x7A8CD21: pa_flist_new_with_name (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BCD755: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x7A9D049: pa_run_once (in /usr/lib/x86_64-linux-gnu/pulseaudio/libpulsecommon-17.0.so)
==28627==    by 0x6BD1DDC: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x6BC7BB0: ??? (in /usr/lib/x86_64-linux-gnu/libpulse.so.0.24.3)
==28627==    by 0x53A92EE: QPlatformAudioDevices::create() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A9403: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x6970513: __pthread_once_slow.isra.0 (pthread_once.c:116)
==28627==    by 0x6970588: pthread_once@@GLIBC_2.34 (pthread_once.c:143)
==28627==    by 0x53A4865: QPlatformMediaIntegration::audioDevices() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B9029: QMediaDevices::audioOutputs() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53BAB13: QMediaDevices::defaultAudioOutput() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x5373C2C: QAudioOutput::QAudioOutput(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40AE020: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 73: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **394093**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 4,224 bytes in 10 blocks are still reachable in loss record 13,836 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x42E92BD: calloc (rtld-malloc.h:44)
==28627==    by 0x42E92BD: _dl_check_map_versions (dl-version.c:280)
==28627==    by 0x42E010C: dl_open_worker_begin (dl-open.c:617)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x1DA7B615: ???
==28627==    by 0x1DA8A199: ???
==28627==    by 0x1DA96423: ???
==28627==    by 0x16F07B14: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0CD1A: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16F0FAB3: ??? (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x16EF5FBD: av_hwdevice_ctx_create (in /usr/lib/x86_64-linux-gnu/libavutil.so.60.8.100)
==28627==    by 0x140257C9: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x140265F3: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x1402D202: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF7162: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x13FF761E: ??? (in /usr/lib/x86_64-linux-gnu/qt6/plugins/multimedia/libffmpegmediaplugin.so)
==28627==    by 0x53A785B: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
```

## Zapis 74: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **396594**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 25,296 bytes in 71 blocks are still reachable in loss record 13,923 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x42E92BD: calloc (rtld-malloc.h:44)
==28627==    by 0x42E92BD: _dl_check_map_versions (dl-version.c:280)
==28627==    by 0x42E010C: dl_open_worker_begin (dl-open.c:617)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x53A7837: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

## Zapis 75: Memorija koja je i dalje dostupna preko pokazivača

Početni red u izvornom logu: **396831**.

Fajlovi igre: `controller.cpp:7`, `mainWindow.cpp:23`, `settings.cpp:9`.

Originalni zapis:

```text
==28627== 87,894 bytes in 70 blocks are still reachable in loss record 13,931 of 13,936
==28627==    at 0x4B23E43: calloc (vg_replace_malloc.c:1678)
==28627==    by 0x42DECAC: calloc (rtld-malloc.h:44)
==28627==    by 0x42DECAC: _dl_new_object (dl-object.c:92)
==28627==    by 0x42D9EA0: _dl_map_object_from_fd (dl-load.c:1065)
==28627==    by 0x42DB989: _dl_map_new_object (dl-load.c:2206)
==28627==    by 0x42D49FC: openaux (dl-deps.c:64)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D4E2F: _dl_map_object_deps (dl-deps.c:232)
==28627==    by 0x42DFE17: dl_open_worker_begin (dl-open.c:609)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF20B: dl_open_worker (dl-open.c:762)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42DF749: _dl_open (dl-open.c:900)
==28627==    by 0x6966EA3: dlopen_doit (dlopen.c:56)
==28627==    by 0x42D3512: _dl_catch_exception (dl-catch.c:241)
==28627==    by 0x42D3658: _dl_catch_error (dl-catch.c:260)
==28627==    by 0x6966992: _dlerror_run (dlerror.c:138)
==28627==    by 0x6966F5A: dlopen_implementation (dlopen.c:71)
==28627==    by 0x6966F5A: dlopen@@GLIBC_2.34 (dlopen.c:81)
==28627==    by 0x62F1ECE: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2B7E: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x62F2EA7: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x60FA0FF: QFactoryLoader::instance(int) const (in /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.10.2)
==28627==    by 0x53A7837: ??? (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53A7D01: QPlatformMediaIntegration::instance() (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x53B326D: QMediaPlayer::QMediaPlayer(QObject*) (in /usr/lib/x86_64-linux-gnu/libQt6Multimedia.so.6.10.2)
==28627==    by 0x40ADFDF: Settings::Settings(QWidget*) (settings.cpp:9)
==28627==    by 0x409ED4C: MainWindow::MainWindow(QWidget*) (mainWindow.cpp:23)
==28627==    by 0x403CEDF: Controller::Controller(QObject*) (controller.cpp:7)
==28627==    by 0x401F872: main (main.cpp:11)
```

