/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: http://www.qt-project.org/
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**************************************************************************/

#ifndef MAEMOREMOTEMOUNTER_H
#define MAEMOREMOTEMOUNTER_H

#include "maemomountspecification.h"

#include <projectexplorer/devicesupport/idevice.h>
#include <utils/fileutils.h>

#include <QList>
#include <QObject>
#include <QProcess>
#include <QSharedPointer>
#include <QString>

QT_FORWARD_DECLARE_CLASS(QTimer)

namespace QSsh {
class SshConnection;
class SshRemoteProcess;
}

namespace ProjectExplorer { class Profile; }
namespace RemoteLinux { class RemoteLinuxUsedPortsGatherer; }
namespace Utils { class PortList; }

namespace Madde {
namespace Internal {

class MaemoRemoteMounter : public QObject
{
    Q_OBJECT
public:
    MaemoRemoteMounter(QObject *parent);
    ~MaemoRemoteMounter();

    // Must already be connected.
    void setConnection(QSsh::SshConnection *connection,
        const ProjectExplorer::IDevice::ConstPtr &devConf);

    void setProfile(const ProjectExplorer::Profile *profile);
    void addMountSpecification(const MaemoMountSpecification &mountSpec,
        bool mountAsRoot);
    bool hasValidMountSpecifications() const;
    void resetMountSpecifications() { m_mountSpecs.clear(); }
    void mount(Utils::PortList *freePorts,
        const RemoteLinux::RemoteLinuxUsedPortsGatherer *portsGatherer);
    void unmount();
    void stop();

signals:
    void mounted();
    void unmounted();
    void error(const QString &reason);
    void reportProgress(const QString &progressOutput);
    void debugOutput(const QString &output);

private slots:
    void handleUtfsClientsStarted();
    void handleUtfsClientsFinished(int exitStatus);
    void handleUnmountProcessFinished(int exitStatus);
    void handleUtfsServerError(QProcess::ProcessError procError);
    void handleUtfsServerFinished(int exitCode,
        QProcess::ExitStatus exitStatus);
    void handleUtfsServerTimeout();
    void handleUtfsServerStderr();
    void startUtfsServers();

private:
    enum State {
        Inactive, Unmounting, UtfsClientsStarting, UtfsClientsStarted,
            UtfsServersStarted
    };

    void setState(State newState);

    void startUtfsClients();
    void killUtfsServer(QProcess *proc);
    void killAllUtfsServers();
    QString utfsClientOnDevice() const;
    Utils::FileName utfsServer() const;

    QTimer * const m_utfsServerTimer;

    struct MountInfo {
        MountInfo(const MaemoMountSpecification &m, bool root)
            : mountSpec(m), mountAsRoot(root), remotePort(-1) {}
        MaemoMountSpecification mountSpec;
        bool mountAsRoot;
        int remotePort;
    };

    QSsh::SshConnection *m_connection;
    ProjectExplorer::IDevice::ConstPtr m_devConf;
    QList<MountInfo> m_mountSpecs;
    QSharedPointer<QSsh::SshRemoteProcess> m_mountProcess;
    QSharedPointer<QSsh::SshRemoteProcess> m_unmountProcess;

    typedef QSharedPointer<QProcess> ProcPtr;
    QList<ProcPtr> m_utfsServers;

    Utils::PortList *m_freePorts;
    const RemoteLinux::RemoteLinuxUsedPortsGatherer *m_portsGatherer;
    bool m_remoteMountsAllowed;
    Utils::FileName m_maddeRoot;

    State m_state;
};

} // namespace Internal
} // namespace Madde

#endif // MAEMOREMOTEMOUNTER_H
