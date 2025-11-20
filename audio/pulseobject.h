/*
    SPDX-FileCopyrightText: 2014-2015 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef PULSEOBJECT_H
#define PULSEOBJECT_H

#include "debug.h"
#include <QObject>
#include <QVariant>  // 添加 QVariant 头文件
#include <QMap>      // 添加 QMap 头文件
#include <QString>

#include <pulse/introspect.h>

namespace QPulseAudio
{
class Context;

class PulseObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint32 index READ index CONSTANT)
    Q_PROPERTY(QString iconName READ iconName CONSTANT)
    Q_PROPERTY(QVariantMap properties READ properties NOTIFY propertiesChanged)
public:
    template<typename PAInfo>
    void updatePulseObject(PAInfo *info)
    {
        m_index = info->index;

        QVariantMap properties;
        void *it = nullptr;
        while (const char *key = pa_proplist_iterate(info->proplist, &it)) {
            Q_ASSERT(key);
            const char *value = pa_proplist_gets(info->proplist, key);
            if (!value) {
                qCDebug(PLASMAPA) << "property" << key << "not a string";
                continue;
            }
            Q_ASSERT(value);
            // 修复：将 QString 显式转换为 QVariant
            properties.insert(QString::fromUtf8(key), QVariant(QString::fromUtf8(value)));
        }

        // 修复：使用自定义比较函数来比较 QVariantMap
        if (!arePropertiesEqual(m_properties, properties)) {
            m_properties = properties;
            Q_EMIT propertiesChanged();
        }
    }

    quint32 index() const;
    QString iconName() const;
    QVariantMap properties() const;

Q_SIGNALS:
    void propertiesChanged();

protected:
    explicit PulseObject(QObject *parent);
    ~PulseObject() override;

    Context *context() const;
    quint32 m_index;
    QVariantMap m_properties;

private:
    // 修复：添加自定义比较函数
    bool arePropertiesEqual(const QVariantMap &map1, const QVariantMap &map2) const
    {
        if (map1.size() != map2.size()) {
            return false;
        }
        
        for (auto it = map1.constBegin(); it != map1.constEnd(); ++it) {
            auto map2It = map2.constFind(it.key());
            if (map2It == map2.constEnd() || map2It.value() != it.value()) {
                return false;
            }
        }
        
        return true;
    }

    // Ensure that we get properly parented.
    PulseObject();
};

} // QPulseAudio

#endif // PULSEOBJECT_H