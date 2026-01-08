#include <qqml.h>
#include "wallpaper.h"

extern "C" Q_DECL_EXPORT void qml_register_types_cutefish_system()
{
    qmlRegisterType<Wallpaper>("cutefish.system", 1, 0, "Wallpaper");
}
