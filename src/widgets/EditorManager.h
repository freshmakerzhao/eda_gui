/**
  ******************************************************************************
  * @file           : EditorManager.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/4/25
  ******************************************************************************
  */
#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include <QTabWidget>
#include <QPainter>
#include <QPainterPath>
#include "Editor.h"

class EditorManager : public QTabWidget
{
    Q_OBJECT
public:
    static EditorManager *instance(QWidget *parent = nullptr);

    void createEditorTab(const QString& path);

    /**
     * 执行操作前询问用户是否保存所有已打开的文件
     * @return 保存成功/失败
     */
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
     * @param op 操作选项
     */
    void editorEdit(const int op);

    void editorSave();

    void editorSaveAs();

    bool cleanEditorTab();

    /**
     * 当文本与已保存的不一致，设置Tab标记
     */
    void setSavePointFlag();

    /**
     * 当文本与已保存的一致，取消Tab标记
     */
    void resetSavePointFlag();

private slots:
    void onTabWidgetCurrentChanged(int index);

    void onTabWidgetTabCloseRequested(int index);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    EditorManager(QWidget *parent = nullptr);
    ~EditorManager();
};

#endif // EDITORMANAGER_H
