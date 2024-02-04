#include "RatingsManager.h"

#include <Geode/utils/web.hpp>
#include <chrono>
#include "Utils.h"

using namespace geode::prelude;
using json = nlohmann::json;

std::map<int, GDDLRating> RatingsManager::demonMap;
std::map<int, int> RatingsManager::ratingsCache;

GDDLRating RatingsManager::parseJson(std::string response) {
    json levelData = json::parse(response);
    return GDDLRating(levelData);
}

/**
 * Cached json format:
 * {
 *  cached: unixTimestamp;
 *  list: [{ID: int, Rating: int}] (list returned from the GDDL API)
 * }
 *
 * If the list is over a week old, don't load data from it
 */
void RatingsManager::populateFromSave() {
    if (!Utils::fileExists(cachedListPath))
        return;
    std::ifstream f(cachedListPath);
    json data = json::parse(f);
    const int cachedTimestamp = data["cached"];
    const int currentTimestamp = Utils::getCurrentTimestamp();
    if (currentTimestamp - cachedTimestamp < 86400 * 7) { // list less than 7 days old, load it
        for (auto idRatingPair: data["list"]) {
            const int id = idRatingPair["ID"];
            const int rating = idRatingPair["Rating"];
            ratingsCache[id] = rating;
        }
    }
}

void RatingsManager::cacheList() {
    json cachedList;
    cachedList["cached"] = Utils::getCurrentTimestamp();
    std::vector<json> idRatingPairs;
    json element;
    for (auto [id, rating]: ratingsCache) {
        element["ID"] = id;
        element["Rating"] = rating;
        idRatingPairs.push_back(element);
    }
    cachedList["list"] = idRatingPairs;
    std::fstream settingsFile;
    settingsFile.open(cachedListPath, std::ios::out);
    settingsFile << cachedList;
    settingsFile.close();
}

int RatingsManager::getDemonTier(int id) { return !demonMap.contains(id) ? -1 : demonMap[id].roundedRating; }

std::optional<GDDLRating> RatingsManager::getRating(int id) {
    if (!demonMap.contains(id))
        return {};
    return demonMap[id];
}

std::string RatingsManager::getRequestUrl(int id) {
    std::string requestURL = "https://gdladder.com/api/level?levelID=" + std::to_string(id);
    return requestURL;
}

bool RatingsManager::addRatingFromResponse(int id, std::string response) {
    if (response.empty())
        return false;
    GDDLRating rating = parseJson(response);
    demonMap[id] = rating;
    return true;
}

void RatingsManager::cacheRatings(const std::string &response) {
    json ratingsData = json::parse(response);
    for (auto element: ratingsData) {
        const int id = element["ID"];
        const float rating = element["Rating"].is_null() ? -1.0f : static_cast<float>(element["Rating"]);
        const int roundedRating = round(rating);
        ratingsCache[id] = roundedRating;
    }
    cacheList();
}

std::map<int, int> RatingsManager::getTierStats() {
    GameLevelManager *levelManager = GameLevelManager::sharedState();
    cocos2d::CCArray *completedLevels = levelManager->getCompletedLevels(false);
    std::map<int, int> tierStats;
    CCObject *obj;
    CCARRAY_FOREACH(completedLevels, obj) {
        auto level = dynamic_cast<GJGameLevel *>(obj);
        if (level->m_normalPercent == 100 && level->m_stars == 10) {
            int cachedTier = -1;
            if (ratingsCache.contains(level->m_levelID)) {
                cachedTier = ratingsCache[level->m_levelID];
            }
            tierStats[cachedTier]++;
        }
    }
    return tierStats;
}
bool RatingsManager::alreadyCached() {
    populateFromSave();
    return !ratingsCache.empty();
}
GJSearchObject *RatingsManager::searchForTier(const int tier, const bool completed, const int page) {
    std::string idList;
    GameLevelManager *levelManager = GameLevelManager::sharedState();
    const cocos2d::CCArray *completedLevels = levelManager->getCompletedLevels(false);
    CCObject *obj;
    int levelIndex = 0;
    CCARRAY_FOREACH(completedLevels, obj) {
        const auto level = dynamic_cast<GJGameLevel *>(obj);
        const int levelTier = ratingsCache[level->m_levelID];
        // ReSharper disable once CppTooWideScopeInitStatement
        const bool levelCompleted = level->m_normalPercent == 100;
        if (levelTier == tier && completed == levelCompleted) {
            if (levelIndex >= (page - 1) * 10 && levelIndex < page * 10) {
                idList += std::to_string(level->m_levelID) + ',';
            } else if (levelIndex >= page * 10) {
                break;
            }
            ++levelIndex;
        }
    }
    idList.pop_back();
    idList += "&gameVersion=22";
    return GJSearchObject::create(SearchType::Type19, idList);
}

void RatingsManager::setupSearch(const int tier, const TierSearchType searchType) {
    searchedTier = tier;
    tierSearchType = searchType;
    searchingForTier = true;
}

bool RatingsManager::isSearchingForTier() {
    return searchingForTier;
}

GJSearchObject *RatingsManager::getSearchPage(const int page) {
    return searchForTier(searchedTier, true, page);
}

void RatingsManager::stopSearch() {
    searchingForTier = false;
}
