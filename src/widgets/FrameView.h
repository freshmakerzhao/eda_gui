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


class QGraphicsScene;
class QSplitter;

class FrameView  : public QWidget {
Q_OBJECT

public:
    FrameView(const std::string& tileGridPath = "", const std::string& tileColorPathLocal = "",QString projectImplPath = "",QWidget *parent = nullptr);
    View *view;
private:
    void setupMatrix();
    void populateScene();
    bool showTilesName = true;
    bool showSites = false;
    bool showClockRegion = false;
    QString tileJsonPath = "";
    QString usageJsonPath = "";
    ChipGridOperations viewer;
    QGraphicsScene *scene;

    // 为在槽函数中调用
    QLabel *tileTypeValue;
    QLabel *colNumValue;
    QLabel *rowNumValue;
    QLabel *siteNameValue;
    QLabel *siteTypeValue;
    QLabel *NameValue;
    QLabel *TypeValue;

public slots:
    void showTileInfo(int x,int y);
    void showSiteInfo(int x,int y,bool sites_visible_status,int index);
    void showBelInfo(int x, int y, int site_index, bool bel_visible_status, int index);

};


#endif //GRID_VIEW_FRAMEVIEW_H
