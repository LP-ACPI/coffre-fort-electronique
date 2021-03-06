//
// Created by Valentin on 05/12/16.
//

#include "Crypt.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <openssl/des.h>

using namespace std;

void Crypt::crypt_DES(char* entree, char* sortie, char* raw_key, int type){
    //Préparation de la clef
    DES_cblock key;
    DES_key_schedule schedule;
    memcpy( key, raw_key, CRYPT_KEY_SIZE);
    DES_set_odd_parity( &key );
    DES_set_key_checked( &key, &schedule );
    int n=0;
    DES_cfb64_encrypt( (unsigned char*) entree,
                       (unsigned char*) sortie,
                       CRYPT_BUFFER_SIZE,
                       &schedule,
                       &key,
                       &n,
                       type );
}
void Crypt::crypt_file_DES(string nom_source, string nom_destination, char* cle, int type)
{
    ifstream source(nom_source, ifstream::binary);
    //TODO: Ajouter un throw > Impossible de trouver/ouvrir le fichier source
    if(!source){
        cerr << strerror(errno) << endl;
        return;
    }

    ofstream destination(nom_destination, ofstream::binary);
    //TODO: Ajouter un throw > Impossible de trouver/ouvrir le fichier cible
    if(!destination) {
        cerr << strerror(errno) << endl;
        return;
    }

    char buffer_read[CRYPT_BUFFER_SIZE];
    char buffer_write[CRYPT_BUFFER_SIZE];
    int taille = source.seekg(0,ios::end).tellg();
    source.seekg(0,ios::beg);
    int lu = 0;
    while(lu < taille)
    {
        if(!source || !destination) break;//TODO: Ajouter un throw > Erreur pendant la lecture

        int nb_octet = CRYPT_BUFFER_SIZE;
        if(taille - lu < CRYPT_BUFFER_SIZE)
            nb_octet = taille - lu;

        source.read(buffer_read,nb_octet);
        crypt_DES(buffer_read,buffer_write,cle,type);
        //DEBUG pas de cryptage
        //memcpy( buffer_write, buffer_read, nb_octet);
        destination.write(buffer_write,nb_octet);
        lu = source.tellg();
    }
    source.close();
    destination.close();
}

char* Crypt::genKey()//TODO
{
    return NULL;
}
void Crypt::crypt(char* in, char* out, char* raw_key)
{
    crypt_DES(in,out,raw_key,DES_ENCRYPT);
}
void Crypt::decrypt(char* in, char* out, char* raw_key)
{
    crypt_DES(in,out,raw_key,DES_DECRYPT);
}
void Crypt::cryptFile(string nom_source, string nom_destination, char* cle)
{
    crypt_file_DES(nom_source,nom_destination,cle,DES_ENCRYPT);
}
void Crypt::decryptFile(string nom_source, string nom_destination, char* cle)
{
    crypt_file_DES(nom_source,nom_destination,cle,DES_DECRYPT);
}

