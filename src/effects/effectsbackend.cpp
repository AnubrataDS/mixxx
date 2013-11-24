#include <QtDebug>

#include "effects/effectsbackend.h"

EffectsBackend::EffectsBackend(QObject* pParent, QString name)
        : QObject(pParent),
          m_name(name) {
}

EffectsBackend::~EffectsBackend() {
}

const QString EffectsBackend::getName() const {
    return m_name;
}

void EffectsBackend::registerEffect(const QString id,
                                    const EffectManifest& manifest,
                                    EffectInstantiator pInstantiator) {
    if (m_registeredEffects.contains(id)) {
        qDebug() << "WARNING: Effect" << id << "already registered";
        return;
    }

    m_registeredEffects[id] = QPair<EffectManifest, EffectInstantiator>(
        manifest, pInstantiator);
}

const QSet<QString> EffectsBackend::getEffectIds() const {
    return QSet<QString>::fromList(m_registeredEffects.keys());
}

EffectManifest EffectsBackend::getManifest(const QString& effectId) const {
    if (!m_registeredEffects.contains(effectId)) {
        qDebug() << "WARNING: Effect" << effectId << "is not registered.";
        return EffectManifest();
    }
    return m_registeredEffects[effectId].first;
}

bool EffectsBackend::canInstantiateEffect(const QString& effectId) const {
    return m_registeredEffects.contains(effectId);
}

EffectPointer EffectsBackend::instantiateEffect(const QString& effectId) {
    if (!m_registeredEffects.contains(effectId)) {
        qDebug() << "WARNING: Effect" << effectId << "is not registered.";
        return EffectPointer();
    }
    QPair<EffectManifest, EffectInstantiator>& effectInfo = m_registeredEffects[effectId];
    return (*effectInfo.second)(this, effectInfo.first);
}
