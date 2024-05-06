#include "EditorManager.h"
#include "mainwindow.h"
#include "dialog/CustomMessageBox.h"

EditorManager *EditorManager::instance(QWidget *parent)
{
    static EditorManager *_instance = nullptr;
    if (!_instance) {
        _instance = new EditorManager(parent);
    }
    return _instance;
}

void EditorManager::createEditorTab(const QString &path)
{
    if(path.isEmpty()) {// 取消打开文件
        return;
    }

    for (int i = 0; i < this->count(); ++i) { // 不重复打开文件
        if(this->widget(i)->property("filePath").toString() == path) {
            this->setCurrentIndex(i);
            return;
        }
    }

    Editor *editor = new Editor(this); // 创建对象
    editor->setProperty("filePath", path);
    if (!editor->openFile(path)) {
        CustomMessageBox::showError(MainWindow::instance(), "Failed", "Cannot open File.");
        delete editor;
        return;
    }
    this->addTab(editor, QFileInfo(path).fileName()); // 添加tab
    this->setCurrentIndex(this->count() - 1); // 设置当前文件的索引

    MainWindow::instance()->updateActionState();
}

/**
 * 执行操作前询问用户是否保存所有已打开的文件
 * @return 保存成功/失败
 */
bool EditorManager::saveAllFiles()
{
    if (isModified()) {
        CustomMessageBox::StandardButton btn = CustomMessageBox::showTwoOptionQuestion(MainWindow::instance(), "Warning", "There are unsaved files,"
                                                                             "  save and execute?",
                                                            QMessageBox::Yes, QMessageBox::No);
        if (btn == QMessageBox::No) {
            return false;
        }
    }

    for (int i = 0; i < this->count(); ++i) {
        Editor *editor = qobject_cast<Editor*>(this->widget(i));
        if (editor->isModified()) {
            if(!editor->saveFile()) {
                return false;
            }
        }
    }

    MainWindow::instance()->updateActionState();
    return true;
}

/**
 * 判断Tab中是否存在未保存的文件
 * @return
 */
bool EditorManager::isModified()
{
    for (int i = 0; i < this->count(); ++i) {
        Editor *editor = qobject_cast<Editor*>(this->widget(i));
        if (editor->isModified()) {
            return true;
        }
    }
    return false;
}

/**
 * 返回当前编辑器指针
 * @return
 */
Editor *EditorManager::currentEditor()
{
    return (Editor*) this->currentWidget();
}

/**
 * 编辑操作
 */
void EditorManager::editorEdit(const int op)
{
    Editor *editor = qobject_cast<Editor*>(this->currentWidget());
    if (!editor) {
        return;
    }

    switch (op) {
    case 0 :
        editor->cut();
        break;
    case 1 :
        editor->copy();
        break;
    case 2 :
        editor->paste();
        break;
    case 3 :
        editor->undo();
        break;
    case 4 :
        editor->redo();
        break;
    default:
        break;
    }

    MainWindow::instance()->updateActionState();
}

void EditorManager::editorSave()
{
    Editor *editor = (Editor*) this->currentWidget();

    if(editor) {
        if(editor->saveFile()) {
            qDebug() << "Save Success";
            // TODO
        }
    }

    MainWindow::instance()->updateActionState();
}

void EditorManager::editorSaveAs()
{
    Editor *editor = (Editor*) this->currentWidget();
    if(editor) {
        if(editor->saveAsFile()) {
            qDebug() << "Save_As Success";
            // TODO
        }
    }

    MainWindow::instance()->updateActionState();
}

bool EditorManager::cleanEditorTab()
{
    if (isModified()) {
        CustomMessageBox::StandardButton btn = CustomMessageBox::showTwoOptionQuestion(MainWindow::instance(), "Warning", "The document has been modified.\n"
                                                                                               "Do you want to save your changes?",
                                                                              QMessageBox::Yes, QMessageBox::No);
        if (btn == QMessageBox::No) {
            return false;
        }
    }

    while (this->count() > 0) {
        Editor *editor = qobject_cast<Editor*>(this->widget(0));
        delete editor;
    }
    return true;
}

void EditorManager::onTabWidgetCurrentChanged(int index)
{
    Q_UNUSED(index);
    MainWindow::instance()->updateActionState();
}

void EditorManager::onTabWidgetTabCloseRequested(int index)
{
    Editor *editor = qobject_cast<Editor*>(this->widget(index));
    if (editor->isModified()) {
        // qDebug() << "File" << index << "has been Modified";
        CustomMessageBox::StandardButton btn = CustomMessageBox::showQuestion(MainWindow::instance(), "Warning", "The document has been modified.\n"
                                                                                               "Do you want to save your changes?",
                                                                              QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (btn == QMessageBox::Yes) {
            if(editor->saveFile()) {
                qDebug() << "Save Success";
                // TODO
            }
        } else if (btn == QMessageBox::Cancel) {
            // 对话框的关闭按钮是与QMessageBox::question里面最后一个值绑定的
            return;
        }
    }
    this->removeTab(index);
    delete editor;

    MainWindow::instance()->updateActionState();
}

EditorManager::EditorManager(QWidget *parent)
    : QTabWidget(parent)
{
    setMovable(true);
    setTabsClosable(true);
    connect(this, &QTabWidget::tabCloseRequested, this, &EditorManager::onTabWidgetTabCloseRequested);
    connect(this, &QTabWidget::currentChanged, this, &EditorManager::onTabWidgetCurrentChanged);
}

EditorManager::~EditorManager() {}
