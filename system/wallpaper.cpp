#include "wallpaper.h"
#include <QFile>
#include <QDir>

Wallpaper::Wallpaper(QObject *parent)
    : QObject(parent)
{
    // Connect to DBus name owner changed so we can react if the settings service appears later
    QDBusConnection::sessionBus().connect(QStringLiteral("org.freedesktop.DBus"),
                                          QStringLiteral("/org/freedesktop/DBus"),
                                          QStringLiteral("org.freedesktop.DBus"),
                                          QStringLiteral("NameOwnerChanged"),
                                          this,
                                          SLOT(onNameOwnerChanged(QString,QString,QString)));

    createInterface();
}

Wallpaper::~Wallpaper()
{
    disconnectInterface();
}

void Wallpaper::createInterface()
{
    m_interface.reset(new QDBusInterface("com.cutefish.Settings",
                                         "/Theme", "com.cutefish.Theme",
                                         QDBusConnection::sessionBus(), this));
    
    if (m_interface && m_interface->isValid()) {
        connect(m_interface.data(), SIGNAL(wallpaperChanged(QString)), this, SLOT(onPathChanged(QString)));
        connect(m_interface.data(), SIGNAL(darkModeDimsWallpaperChanged()), this, SIGNAL(dimsWallpaperChanged()));
        connect(m_interface.data(), SIGNAL(backgroundTypeChanged()), this, SIGNAL(typeChanged()));
        connect(m_interface.data(), SIGNAL(backgroundColorChanged()), this, SIGNAL(colorChanged()));
    }
}

void Wallpaper::disconnectInterface()
{
    if (m_interface && m_interface->isValid()) {
        disconnect(m_interface.data(), SIGNAL(wallpaperChanged(QString)), this, SLOT(onPathChanged(QString)));
        disconnect(m_interface.data(), SIGNAL(darkModeDimsWallpaperChanged()), this, SIGNAL(dimsWallpaperChanged()));
        disconnect(m_interface.data(), SIGNAL(backgroundTypeChanged()), this, SIGNAL(typeChanged()));
        disconnect(m_interface.data(), SIGNAL(backgroundColorChanged()), this, SIGNAL(colorChanged()));
    }
    m_interface.reset();
}

int Wallpaper::type() const
{
    if (m_interface && m_interface->isValid())
        return m_interface->property("backgroundType").toInt();
    return 0;
}

QString Wallpaper::path() const
{
    QString wallpaperPath;
    
    if (m_interface && m_interface->isValid()) {
        wallpaperPath = m_interface->property("wallpaper").toString();
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
    if (m_interface && m_interface->isValid())
        return m_interface->property("darkModeDimsWallpaper").toBool();
    return false;
}

QString Wallpaper::color() const
{
    if (m_interface && m_interface->isValid())
        return m_interface->property("backgroundColor").toString();
    return "#2B8ADA";
}

void Wallpaper::onPathChanged(QString path)
{
    Q_UNUSED(path);

    emit pathChanged();
}

void Wallpaper::onNameOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner)
{
    Q_UNUSED(oldOwner);
    Q_UNUSED(newOwner);

    if (name != QLatin1String("com.cutefish.Settings"))
        return;

    // Reinitialize interface and emit change if available
    disconnectInterface();
    createInterface();
    
    if (m_interface && m_interface->isValid()) {
        emit pathChanged();
        emit dimsWallpaperChanged();
        emit typeChanged();
        emit colorChanged();
    }
}