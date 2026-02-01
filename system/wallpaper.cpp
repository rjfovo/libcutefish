#include "wallpaper.h"
#include <QFile>
#include <QDir>

Wallpaper::Wallpaper(QObject *parent)
    : QObject(parent)
    , m_interface("com.cutefish.Settings",
                  "/Theme", "com.cutefish.Theme",
                  QDBusConnection::sessionBus(), this)
{
    if (m_interface.isValid()) {
        connect(&m_interface, SIGNAL(wallpaperChanged(QString)), this, SLOT(onPathChanged(QString)));
        connect(&m_interface, SIGNAL(darkModeDimsWallpaperChanged()), this, SIGNAL(dimsWallpaperChanged()));
        connect(&m_interface, SIGNAL(backgroundTypeChanged()), this, SIGNAL(typeChanged()));
        connect(&m_interface, SIGNAL(backgroundColorChanged()), this, SIGNAL(colorChanged()));
    }
}

int Wallpaper::type() const
{
    if (!m_interface.isValid())
        return 0;
    return m_interface.property("backgroundType").toInt();
}

QString Wallpaper::path() const
{
    QString wallpaperPath;
    
    if (m_interface.isValid()) {
        wallpaperPath = m_interface.property("wallpaper").toString();
    }
    
    // 如果从 D-Bus 获取的路径为空，或者 D-Bus 接口无效，使用默认壁纸
    if (wallpaperPath.isEmpty()) {
        QString defaultWallpaper = "/usr/share/backgrounds/cutefishos/default.jpg";
        if (QFile::exists(defaultWallpaper)) {
            return defaultWallpaper;
        }
        // 如果默认壁纸也不存在，尝试其他可能的路径
        QString fallbackWallpaper = "/usr/share/wallpapers/cutefishos/default.jpg";
        if (QFile::exists(fallbackWallpaper)) {
            return fallbackWallpaper;
        }
        // 最后尝试任何存在的壁纸文件
        QDir wallpaperDir("/usr/share/backgrounds/cutefishos");
        if (wallpaperDir.exists()) {
            QStringList filters;
            filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp";
            QStringList files = wallpaperDir.entryList(filters, QDir::Files);
            if (!files.isEmpty()) {
                return wallpaperDir.absoluteFilePath(files.first());
            }
        }
        // 如果所有尝试都失败，返回空字符串
        return QString();
    }
    
    return wallpaperPath;
}

bool Wallpaper::dimsWallpaper() const
{
    if (!m_interface.isValid())
        return false;
    return m_interface.property("darkModeDimsWallpaper").toBool();
}

QString Wallpaper::color() const
{
    if (!m_interface.isValid())
        return "#2B8ADA";
    return m_interface.property("backgroundColor").toString();
}

void Wallpaper::onPathChanged(QString path)
{
    Q_UNUSED(path);

    emit pathChanged();
}
