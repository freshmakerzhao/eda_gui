#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include <QTabWidget>
#include "Editor.h"

class EditorManager : public QTabWidget
{
    Q_OBJECT
public:
    static EditorManager *instance(QWidget *parent = nullptr);

    void createEditorTab(const QString& path);

    bool saveAllFiles();

    /**
     * 判断Tab中是否存在未保存的文件
     * @return
     */
    bool isModified();

    /**
     * 返回当前编辑器指针
     * @return
     */
    Editor *currentEditor();

public slots:
    /**
     * 编辑操作
     */
    void editorEdit(const int op);

    void editorSave();

    void editorSaveAs();

    bool cleanEditorTab();

private slots:
    void onTabWidgetCurrentChanged(int index);

    void onTabWidgetTabCloseRequested(int index);

private:
    EditorManager(QWidget *parent = nullptr);
    ~EditorManager();
};

#endif // EDITORMANAGER_H
