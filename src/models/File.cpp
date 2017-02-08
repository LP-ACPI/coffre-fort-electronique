//
// Created by Valentin on 05/12/16.
//

#include "File.h"

using namespace std;
using json=nlohmann::json;

bool File::isDirData() const
{return false;}


json File::to_json() const {
    json j = json{{"file", {
                {"path", path},
                {"name", name},
            }}
        };
    return j;
}



Data* File::from_json(const json js){
    cout << js.dump(1);
    File *outFile;
    outFile->setName(js["name"]);
    outFile->setName(js["path"]);
    return outFile;
}

void File::operator=(const File &file){
    cout << "data = file" << endl;
    name = file.getName();
    path = file.getPath();
}

std::ostream& operator<<(std::ostream &o,const File &f){
    o << "name: " << f.getName() << " (path:" << f.getPath() << ")" << endl;

    return o;
}


