/**
 * @author Levi Armstrong
 * @date January 1, 2016
 *
 * @copyright Copyright (c) 2016, Southwest Research Institute
 *
 * @license Software License Agreement (Apache License)\n
 * \n
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at\n
 * \n
 * http://www.apache.org/licenses/LICENSE-2.0\n
 * \n
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef ROSPROJECT_H
#define ROSPROJECT_H


#include "ros_workspace_watcher.h"
#include "ros_project_manager.h"
#include "ros_project_nodes.h"
#include "ros_utils.h"

#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/target.h>
#include <projectexplorer/toolchain.h>
#include <projectexplorer/buildconfiguration.h>
#include <coreplugin/idocument.h>

#include <QFuture>

namespace ROSProjectManager {
namespace Internal {

class ROSProjectFile;

class ROSProject : public ProjectExplorer::Project
{
    Q_OBJECT

public:
    ROSProject(ROSManager *manager, const QString &filename);
    ~ROSProject() override;

    QString displayName() const override;
    ROSManager *projectManager() const override;
    QStringList files(FilesMode fileMode) const override;

    QStringList buildTargets() const;

    bool addIncludes(const QStringList &includePaths);
    bool setIncludes(const QStringList &includePaths);

    void refresh();

    QStringList projectIncludePaths() const;
    QStringList workspaceFiles() const;

    Utils::FileName buildDirectory() const;
    Utils::FileName sourceDirectory() const;

protected:
    Project::RestoreResult fromMap(const QVariantMap &map, QString *errorMessage);

private:
    bool saveProjectFile();
    void parseProjectFile();
    void refreshCppCodeModel();
    void repositoryChanged(const QString &repository);

    QString m_projectName;
    ROSWorkspaceWatcher *m_workspaceWatcher;
    QStringList m_watchDirectories;
    QStringList m_projectIncludePaths;
    QFuture<void> m_codeModelFuture;
};

class ROSProjectFile : public Core::IDocument
{
    Q_OBJECT

public:
    ROSProjectFile(ROSProject *parent, QString fileName);

    bool save(QString *errorString, const QString &fileName, bool autoSave) override;

    bool isModified() const override;
    bool isSaveAsAllowed() const override;

    ReloadBehavior reloadBehavior(ChangeTrigger state, ChangeType type) const override;
    bool reload(QString *errorString, ReloadFlag flag, ChangeType type) override;

private:
    ROSProject *m_project;
};



} // namespace Internal
} // namespace ROSProjectManager

#endif // ROSPROJECT_H
