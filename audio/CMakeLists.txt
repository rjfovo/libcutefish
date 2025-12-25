cmake_minimum_required(VERSION 3.16)
project(cutefishaudio LANGUAGES CXX)

# ----------------------------
# 编译选项和头文件
# ----------------------------
add_definitions(-DTRANSLATION_DOMAIN=\"cutefish_pulseaudio\")

set(USE_GSETTINGS OFF)
set(USE_GCONF OFF)

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# ----------------------------
# 源文件
# ----------------------------
set(AUDIO_SRCS
    card.cpp
    client.cpp
    context.cpp
    device.cpp
    maps.cpp
    operation.cpp
    port.cpp
    profile.cpp
    pulseaudio.cpp
    pulseobject.cpp
    sink.cpp
    sinkinput.cpp
    modulemanager.cpp
    source.cpp
    sourceoutput.cpp
    stream.cpp
    volumemonitor.cpp
    volumeobject.cpp
    debug.cpp
    server.cpp
    streamrestore.cpp
    module.cpp
    canberracontext.cpp
    speakertest.cpp
    qml/listitemmenu.cpp
    qml/plugin.cpp
    # qml/microphoneindicator.cpp
    # qml/volumeosd.cpp
    qml/volumefeedback.cpp
    model/sortfiltermodel.cpp
)

set(QML_FILES
    qml/PulseObjectFilterModel.qml
)

# ----------------------------
# 依赖
# ----------------------------
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    QuickControls2
    Qml
    Widgets
    DBus
)

# libcanberra
find_package(PkgConfig REQUIRED)

# 使用 pkg-config 查找 libcanberra
pkg_check_modules(Canberra REQUIRED libcanberra)

# SoundThemeFreedesktop 可能没有对应的 pkg-config 文件
# 我们可以手动检查或使用其他方法
find_path(SOUND_THEME_FREEDESKTOP_DIR index.theme
    PATHS
    /usr/share/sounds/freedesktop
    /usr/local/share/sounds/freedesktop
    REQUIRED
)

if(SOUND_THEME_FREEDESKTOP_DIR)
    set(SoundThemeFreedesktop_FOUND TRUE)
else()
    message(FATAL_ERROR "SoundThemeFreedesktop not found")
endif()

# PulseAudio
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPULSE libpulse REQUIRED IMPORTED_TARGET)
pkg_check_modules(LIBPULSE_MAINLOOP libpulse-mainloop-glib REQUIRED IMPORTED_TARGET)

# ----------------------------
# DBus interface
# ----------------------------
# 如果 audio 模块有 dbus xml 文件，这里可以使用 qt6_add_dbus_interface
# qt6_add_dbus_interface(AUDIO_SRCS file.xml interface_name)

# ----------------------------
# Qt6 QML MODULE
# ----------------------------
qt6_add_qml_module(cutefishaudio_qmlplugins
    URI Cutefish.Audio
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cutefish/Audio
    QML_FILES ${QML_FILES}
    SOURCES ${AUDIO_SRCS}
)

# ----------------------------
# 链接库
# ----------------------------
target_link_libraries(cutefishaudio_qmlplugins PRIVATE 
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Widgets
    Qt6::DBus
    Qt6::Quick
    ${Canberra_LIBRARIES}
    PkgConfig::LIBPULSE
    PkgConfig::LIBPULSE_MAINLOOP
)

# ----------------------------
# 安装（Qt6 qt6_add_qml_module 自动处理 qmldir & 安装路径）
# ----------------------------
# 不再需要手动 install()
