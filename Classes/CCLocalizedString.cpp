//
//  CCLocalizedString.cpp
//

#include "cocos2d.h"
USING_NS_CC;

#include <iostream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

#include "CCLocalizedString.h"

static map<std::string,std::string> localizedStrings;

void PurgeCCLocalizedStringCached()
{
    localizedStrings.clear();
}

const char * CCLocalizedString(const char * mKey)
{

    ccLanguageType curLanguage = CCApplication::sharedApplication()->getCurrentLanguage();
    const char * fileName;
    switch (curLanguage) {
        case kLanguageEnglish:
            fileName = "language_en";
            break;
        case kLanguageSpanish:
            fileName = "language_es";
            break;
        /**
         case for more localize
         */
        default:
            fileName = "language_en";
            break;
    }

    std::string resultStr;

    if (localizedStrings.empty())
    {
        // Initialize variables needed
        unsigned long fileSize = 0;
        unsigned char * fileContents = NULL;
        string line, fullPath, contents;

        // Get absolute path of file
        fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename( fileName );

        // Get data of file
        fileContents = CCFileUtils::sharedFileUtils()->getFileData( fullPath.c_str( ) , "rb", &fileSize );
        contents.assign(fileContents,fileContents+fileSize-1);

        // Create a string stream so that we can use getline( ) on it
        istringstream fileStringStream( contents );

        // Get file contents line by line
        while ( std::getline( fileStringStream, line ) )
        {
//            debug_Printf("line :%s",line.c_str());
            if (line.find("/*",0) == string::npos && line.find("//",0) == string::npos) //filter the valid string of one line
            {
//                debug_Printf("find the line not start with \" /* \"");
                std::string::size_type validPos= line.find('=',0);
                if ( validPos != string::npos)
                {
//                    debug_Printf("fimd the line contain \" = \"");
                    std::string keyStr = line.substr(0,validPos-1);
                    std::string subStr = line.substr(validPos+1,line.size()-1); // get valid string

                    //trim space
                    keyStr.erase(0, keyStr.find_first_not_of(" \t")); //
                    keyStr.erase(keyStr.find_last_not_of(" \t") + 1); //

                    subStr.erase(0, subStr.find_first_not_of(" \t")); //
                    subStr.erase(subStr.find_last_not_of(" \t") + 1); //

                    //trim \"
                    keyStr.erase(0, keyStr.find_first_not_of("\""));
                    keyStr.erase(keyStr.find_last_not_of("\"") + 1);

                    subStr.erase(0, subStr.find_first_not_of("\""));


                    //trim ; character and last \" character
                    subStr.erase(subStr.find_last_not_of(";") + 1);
                    subStr.erase(subStr.find_last_not_of("\"") + 1);


                    //replace line feed with \n
                    string::size_type pos(0);
                    string old_value("\\n");
                    if((pos=subStr.find(old_value))!=string::npos)
                    {
                        for(; pos!=string::npos; pos+=1)
                        {
                            if((pos=subStr.find(old_value,pos))!=string::npos)
                            {
                                subStr.erase(pos, 2);
                                subStr.insert(pos, 1, '\n');
                            }
                            else
                                break;
                        }
                    }

                    localizedStrings.insert(std::pair<std::string, std::string>(keyStr,subStr));

                    const char * keyChars = keyStr.c_str();

                    if (strcmp(keyChars,mKey) == 0) {
                        resultStr = subStr;
                    }
                }
            }
        }
        //must delete fileContents
        if (fileContents!= NULL) {
            delete [] fileContents;
            fileContents = NULL;
        }

        //return valid value
        if (resultStr.length() > 0){
            return (string(resultStr)).c_str();
        }
        return mKey;
    }
    else
    {
        std::map<std::string,std::string>::iterator itr = localizedStrings.find(std::string(mKey));
        if (itr != localizedStrings.end()) {
            resultStr = itr->second;
            if (resultStr.length() > 0)
            {
                return (itr->second).c_str();
            }
        }
    }
    return mKey;
}
