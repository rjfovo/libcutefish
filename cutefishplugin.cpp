#include <QQmlExtensionPlugin>
#include <QQmlEngine>

QT_BEGIN_NAMESPACE

class CutefishPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char * uri) override {
        // 这是一个聚合插件，不注册任何类型
        // 它只是作为一个占位符，让 Qt 能够找到 cutefish 模块
        Q_UNUSED(uri);
    }
};

QT_END_NAMESPACE

#include "cutefishplugin.moc"
