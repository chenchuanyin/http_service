//
// Created by silver chan on 2017/7/14.
//

#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <vector>
#include <string>

extern std::vector<int> definedSources;
extern std::vector<int> definedCategorys;
extern std::vector<std::string> definedCategoryTypes;
extern std::vector<int> definedCopyrights;
extern std::vector<int> definedCopyrightTypes;
extern std::vector<int> definedSearchTypes;
extern std::vector<int> definedCorrects;
extern std::vector<int> definedSemantics;

enum DefinedErrorCodeType {
    SUCCESS = 0,
    NO_SUPPORT_METHOD = 2,
    UNKNOWN_METHOD = 5,
    UNKNOWN_ERROR = 6
};

#endif // __DEFINE_H__
