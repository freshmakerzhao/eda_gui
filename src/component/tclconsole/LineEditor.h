#ifndef LINEEDITOR_H
#define LINEEDITOR_H

#include <QLineEdit>
#include <QMenu>

class LineEditor : public QLineEdit
{
    Q_OBJECT

public:
    explicit LineEditor(/*ParseHelper *helper, */QWidget *parent = 0);

private Q_SLOTS:
    void textInserted();
    void showContextMenu(const QPoint &pt);
    void clearHistory();

Q_SIGNALS:
    void textLineInserted(QString);

protected:
    void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE;
    bool focusNextPrevChild(bool next) Q_DECL_OVERRIDE;
    void autoComplete();

private:
    int index;
    QStringList lines;
    // ParseHelper *parseHelper;
};


#endif // LINEEDITOR_H
