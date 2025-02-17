/**
  ******************************************************************************
  * @file           : Properties.h
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2025/1/15
  ******************************************************************************
  */
#include <QVBoxLayout>
#include <QTreeView>
#include <QStandardItem>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

class Properties : public QWidget
{
    Q_OBJECT
public:
    static Properties *instance(QWidget *parent = nullptr);

    void updateHardwareProperties(QJsonObject dataObj);

private slots:
    // 处理 dataChanged 信号的槽函数
    void onDataChanged(const QModelIndex &topLeft,
                       const QModelIndex &bottomRight);

private:
    Q_DISABLE_COPY(Properties)
    Properties(QWidget* parent = nullptr);
    ~Properties();

private:
    QTreeView *treeView;

    QStandardItemModel *_model = nullptr;
};
