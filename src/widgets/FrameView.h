/**
  ******************************************************************************
  * @File           : FrameView.h
  * @Author         : zs
  * @Description    : None
  * @Date           : 23-11-9
  ******************************************************************************
  */

#ifndef GRID_VIEW_FRAMEVIEW_H
#define GRID_VIEW_FRAMEVIEW_H
#include "grid/view.h"
#include <QDialog>
#include <QWidget>
#include "blocks/SitesMods.h"
#include <QLabel>
#include "grid/ChipGridOperations.h"
#include <QApplication>
#include <QLineEdit>
#include <QStringListModel>
#include <QCompleter>


class QGraphicsScene;
class QSplitter;

class FrameView  : public QWidget {
Q_OBJECT

public:
    FrameView(const std::string& tileGridPath = "", const std::string& tileColorPathLocal = "",QString projectImplPath = "",QWidget *parent = nullptr);
    bool searchCell(const std::string &cell_name);
    View *view;
private:
    void setupMatrix();
    void populateScene();
    bool showTilesName = false;
    bool showSites = true;
    bool showClockRegion = false;
    QString tileJsonPath = "";
    QString usageJsonPath = "";
    ChipGridOperations viewer;
    QGraphicsScene *scene;
    // 搜索框提示词语
    QString searchTextLast = "";
    QCompleter searchCompleter;
    QStringList searchWordList;
    QStringListModel searchWordListModel;

    // 为在槽函数中调用
    QLabel *tileTypeValue;
    QLabel *colNumValue;
    QLabel *rowNumValue;
    QLabel *siteNameValue;
    QLabel *siteTypeValue;
    QLabel *NameValue;
    QLabel *TypeValue;
    QLineEdit *searchBox;

    void clearSearchWordList();
public slots:
    void showTileInfo(int x,int y);
    void showSiteInfo(int x,int y,bool sites_visible_status,int index);
    void showBelInfo(int x, int y, int site_index, bool bel_visible_status, int index, const std::string &bel_type, const std::string &name);
    void searchUpdate(const QString &text); //搜索框自动补全
    void showCell();
};


#endif //GRID_VIEW_FRAMEVIEW_H
