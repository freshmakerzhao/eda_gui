/**
  ******************************************************************************
  * @file           : ProgressHelper.cpp
  * @author         : ksy
  * @description    : None
  * @attention      : None
  * @date           : 2024/10/28
  ******************************************************************************
  */
#include "ProgressHelper.h"
#include <QDebug>

bool ProgressHelper::showProgressDialog(QWidget *parent,
                                        const QString &title,
                                        const QString &labelText) {
    std::atomic<bool> isCanceled(false);
    QProgressDialog progressDialog(labelText, "Cancel", 0, 0, parent);  // 空文本和无进度范围
    progressDialog.setStyleSheet(".QProgressDialog { "
                                 "   color: black; "
                                 "   background: #f7f7f7; "
                                 "   border-top: 3px solid #e7e7e7;"  // 顶部边框
                                 "}");
    progressDialog.setFixedWidth(800);
    progressDialog.setWindowTitle(title);
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setMinimumDuration(0);     // 立即显示

    auto cancelConnection = connect(&progressDialog, &QProgressDialog::canceled, [&]() {
        isCanceled = true;
        progressDialog.close();
        qDebug() << "[Progress Dialog]" << "Cancel";
    });

    QTimer::singleShot(3000, &progressDialog, [&]() {
        //! 即使没有点击“取消”按钮，也会默认触发 canceled 信号
        //! 所以需要在计时结束断开信号槽
        QObject::disconnect(cancelConnection);
        progressDialog.close();
    });

    progressDialog.exec();

    return isCanceled;
}
