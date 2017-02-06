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

void File::operator=(const File &file){
    cout << "data = file" << endl;
    name = file.getName();
    path = file.getPath();
}
