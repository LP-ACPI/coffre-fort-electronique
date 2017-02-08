//
// Created by Valentin on 05/12/16.
//

#include "Directory.h"
#include "File.h"
#include "../services/ConfigManager.h"
#include <stdexcept>

using namespace std;
using json=nlohmann::json;

void Directory::addData(Data *data){
    if(!hasData(data))
        dataList.push_back(data);
}

void Directory::removeData(Data *data){
    if(hasData(data))
        dataList.remove(data);
}

Data& Directory::getDataAt(const unsigned position){
    if (dataList.size() > position){
        list<Data*>::iterator it = dataList.begin();
        advance(it, position);
        return *(*it);
    } else throw invalid_argument("Exception: pas de sous-données!");
}

bool Directory::hasData(const Data *data){
    bool found = false;
    for(auto &dt : dataList){
        if(dt == data) {
            found = true;
            break;
        }
    }
    return found;
}

list<Data*> Directory::getDataList() const {
    return dataList;
}

void Directory::setDataList(const std::list<Data*> newDataList){
    dataList.clear();
    for(Data *dt : newDataList){
        addData(dt);
    }
}


bool Directory::isDirData() const
{return true;}


void Directory::operator=(const Directory &dir){
    setDataList(dir.getDataList());
    setName(dir.getName());
    setPath(dir.getPath());
}

json Directory::to_json() const{
    json dataCol;
    for(Data *data : getDataList())
        dataCol = ConfigManager::getInstance().merge( dataCol, data->to_json() );

    json jOut = json{{getName(), {
                {"path", path},
                {"data", dataCol == NULL ? json::object():dataCol}
          }
    }};
    return jOut;
}

Data* Directory::from_json(const json j){
    cout << j;
    Directory *dir;
    dir->setName(j);
    dir->setPath(j["path"]);
    json jData = j["data"];
    for (json js : jData){
        Data *data;
        data = from_json(js);
        dir->addData(data);
    }
    return dir;
//        if(it.key() == "file"){
//            File file;
//            dir.addData(&file.from_json(it.value()));
//        }
//        else {
//            Directory sub_dir;
//            dir.addData(&sub_dir.from_json(it.value()));
//        }

//    return dir;

}


std::ostream& operator<<(std::ostream &o,const Directory &d){
    o << "name: " << d.getName() << " (path:" << d.getPath() << ")" << endl;
    o << "data: " << endl;
         for(Data *data : d.getDataList())
             o << "\t->" << *data;
    return o;
}

//Data& operator>>(const json &js, Directory &dir){

//                cout << js.dump(2);
//    json jDir = js["data"];
//    for (json::iterator it = jDir.begin(); it != jDir.end(); ++it){
//        Data* data;
//        it.value() >> *data;
//        dir.addData(data);
//    }
//    return dir;
//}
