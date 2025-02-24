//
// Created by DELL on 2025/2/21.
//

#include "SitesBlockFactory.h"

SitesBlockFactory& SitesBlockFactory::Instance() {
    static SitesBlockFactory instance;
    return instance;
}
void SitesBlockFactory::registerType(const std::string &type, SitesBlockFactory::CreateFunction createFunction) {
    creators[type] = createFunction;
}

SitesBlock* SitesBlockFactory::create(const std::string& type, const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
    auto it = creators.find(type);
    if(it != creators.end()) {
        return it->second(color, i, j, tile, name, index, pin);
    }
    return new SitesNull(color, i, j, tile, name, index);
}

SitesBlockFactory::SitesBlockFactory() {
    //注册site类型
    registerType("SLICEL",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string&) {
        return new SitesSliceL(color, i, j, tile, name, index);
    });
    registerType("SLICEM",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string&) {
        return new SitesSliceM(color, i, j, tile, name, index);
    });
    registerType("IOB33",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesIOB33(color, i, j, tile, name, index, pin);
    });
    registerType("BUFR",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string&) {
        return new SitesBUFR(color, i, j, tile, name, index);
    });
    registerType("BUFIO",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string&) {
        return new SitesBUFIO(color, i, j, tile, name, index);
    });
    registerType("IDELAYCTRL",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string&) {
        return new SitesIDELAYCTRL(color, i, j, tile, name, index);
    });
    registerType("OLOGICE3",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string&) {
        return new SitesOLOGICE3(color, i, j, tile, name, index);
    });
    registerType("ILOGICE3",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string&) {
        return new SitesILOGICE3(color, i, j, tile, name, index);
    });
    registerType("IDELAYE2",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string&) {
        return new SitesIDELAYE2(color, i, j, tile, name, index);
    });
    registerType("IOB33M",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesIOB33M(color, i, j, tile, name, index, pin);
    });
    registerType("IOB33S",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesIOB33S(color, i, j, tile, name, index, pin);
    });
    registerType("FIFO18E1",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesFIFO18E1(color, i, j, tile, name, index);
    });
    registerType("RAMBFIFO36E1",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesRAMB36E1(color, i, j, tile, name, index);
    });
    registerType("RAMB18E1",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesRAMB18E1(color, i, j, tile, name, index);
    });
    registerType("DSP48E1",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesDSP48E1(color, i, j, tile, name, index);
    });
    registerType("OUT_FIFO",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesOUTFIFO(color, i, j, tile, name, index);
    });
    registerType("IN_FIFO",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesINFIFO(color, i, j, tile, name, index);
    });
    registerType("MMCME2_ADV",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesMMCME2ADV(color, i, j, tile, name, index);
    });
    registerType("PLLE2_ADV",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesPLLE2ADV(color, i, j, tile, name, index);
    });
    registerType("PHASER_OUT_PHY",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesPhaserOUTPHY(color, i, j, tile, name, index);
    });
    registerType("PHASER_IN_PHY",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesPhaserINPHY(color, i, j, tile, name, index);
    });
    registerType("PHY_CONTROL",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesPHYControl(color, i, j, tile, name, index);
    });
    registerType("PHASER_REF",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesPhaserREF(color, i, j, tile, name, index);
    });
    registerType("OLOGICE3",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesOLOGICE3(color, i, j, tile, name, index);
    });
    registerType("ILOGICE3",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesILOGICE3(color, i, j, tile, name, index);
    });
    registerType("IDELAYE2",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesIDELAYE2(color, i, j, tile, name, index);
    });
    registerType("GTPE2_CHANNEL",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesGTP2Cannel(color, i, j, tile, name, index);
    });
    registerType("GTPE2_COMMON",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesGTP2Common(color, i, j, tile, name, index);
    });
    registerType("IPAD",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesIPAD(color, i, j, tile, name, index);
    });
    registerType("OPAD",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesOPAD(color, i, j, tile, name, index);
    });
    registerType("BUFGCTRL",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesBUFGCTRL(color, i, j, tile, name, index);
    });
    registerType("BUFHCE",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesBUFHCE(color, i, j, tile, name, index);
    });
    registerType("PCIE_2_1",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesPCIE2_1(color, i, j, tile, name, index);
    });
    registerType("BUFMRCE",[](const QColor &color, int i, int j, NormalTile tile, std::string& name, int index, std::string& pin) {
        return new SitesBUFMRCE(color, i, j, tile, name, index);
    });
}

