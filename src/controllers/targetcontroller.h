#ifndef TARGETCONTROLLER_H
#define TARGETCONTROLLER_H

#include "../models/FtpTarget.h"
#include "../models/Target.h"

#include <list>

class TargetController
{
    static TargetController instance;

    TargetController()
    {}

public:
    static TargetController getInstance()
    {     return instance;  }

    std::map<std::string,std::string> favoriteFtpTargetToInfoMap(std::string);
    std::map<std::string,std::string> favoriteNormalTargetToInfoMap(std::string);
   std::string getFavoriteTargetsType(std::string);

    Target *getNormalTargetFromInfoMap(std::map<std::string,std::string>);
    FtpTarget *getFtpTargetFromInfoMap(std::map<std::string,std::string>);
    void updateFavoriteNormalTarget(std::map<std::string,std::string>);
    void updateFavoriteFtpTarget(std::map<std::string,std::string>);
    void addNewFavoriteNormalTarget(std::map<std::string,std::string>);
    void addNewFavoriteFtpTarget(std::map<std::string,std::string>);
    void deleteFavoriteTarget(std::string);

    std::list<std::string> listlFavoriteNormalTargetTags();
    std::list<std::string> listFavoriteFtpTargetTags();
    std::map<std::string,std::string> targetCouplesTagKey();

};

#endif // TARGETCONTROLLER_H