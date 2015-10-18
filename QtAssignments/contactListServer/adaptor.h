/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -c Adaptor -a adaptor.h:adaptor.cpp com.contact.info.xml
 *
 * qdbusxml2cpp is Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef ADAPTOR_H_1388059236
#define ADAPTOR_H_1388059236

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "contactinfo.h"
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface com.contact.info
 */
class Adaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.contact.info")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.contact.info\">\n"
"    <method name=\"GetContactDetails\">\n"
"      <annotation value=\"contactList\" name=\"org.qtproject.QtDBus.QtTypeName.Out0\"/>\n"
"      <arg direction=\"out\" type=\"a(sss)\" name=\"list\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    Adaptor(QObject *parent);
    virtual ~Adaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    contactList GetContactDetails();
Q_SIGNALS: // SIGNALS
};

#endif
