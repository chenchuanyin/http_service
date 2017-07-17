//
// Created by silver chan on 2017/7/14.
//

#ifndef __DEFINE_H__
#define __DEFINE_H__

#define Params std::map<std::string, Poco::Any>

std::vector<int> definedSources = {0, 1, 2, 3, 15};
std::vector<int> definedCategorys = {0, 1, 2, 3, 4};
std::vector<std::string> definedCategoryTypes = {"", "kuyin_singer", "kuyin_song", "kuyin_singersong", "kuyin_songlist"};
std::vector<int> definedCopyrights = {0, 1};
std::vector<int> definedCopyrightTypes = {0, 1, 2, 4, 8, 9, 10, 11, 12};
std::vector<int> definedSearchTypes = {1, 2, 3};
std::vector<int> definedCorrects = {0, 1};
std::vector<int> definedSemantics = {0, 1};

#endif // __DEFINE_H__
