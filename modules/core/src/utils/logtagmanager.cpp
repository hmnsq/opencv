// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "../precomp.hpp"
#شامل "logtagmanager.hpp"
#شامل "logtagconfigparser.hpp"

فضاینام cv {
فضاینام utils {
فضاینام logging {


ثابت نویسه* LogTagManager::m_globalName = "global";


LogTagManager::LogTagManager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_globalLogTag(جدید LogTag(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<LogTagConfigParser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_globalLogTag.get());
}

LogTagManager::~LogTagManager()
{
}

تهی LogTagManager::setConfigString(ثابت std::string& configString, منطقی apply /*true*/)
{
    m_config->parse(configString);
    اگر (m_config->hasMalformed())
    {
        بازگشت;
    }
    اگر (!apply)
    {
        بازگشت;
    }
    // The following code is arranged with "priority by overwriting",
    // where when the same log tag has multiple matches, the last code
    // block has highest priority by literally overwriting the effects
    // from the earlier code blocks.
    //
    // Matching by full name has highest priority.
    // Matching by any name part has moderate priority.
    // Matching by first name part (prefix) has lowest priority.
    //
    ثابت auto& globalConfig = m_config->getGlobalConfig();
    m_globalLogTag->level = globalConfig.level;
    برای (ثابت auto& config : m_config->getFirstPartConfigs())
    {
        setLevelByFirstPart(config.namePart, config.level);
    }
    برای (ثابت auto& config : m_config->getAnyPartConfigs())
    {
        setLevelByAnyPart(config.namePart, config.level);
    }
    برای (ثابت auto& config : m_config->getFullNameConfigs())
    {
        setLevelByFullName(config.namePart, config.level);
    }
}

LogTagConfigParser& LogTagManager::getConfigParser() ثابت
{
    بازگشت *m_config;
}

تهی LogTagManager::assign(ثابت std::string& fullName, LogTag* ptr)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = درست;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    ثابت منطقی isPtrChanged = (fullNameInfo.logTagPtr != ptr);
    اگر (!isPtrChanged)
    {
        بازگشت;
    }
    fullNameInfo.logTagPtr = ptr;
    اگر (!ptr)
    {
        بازگشت;
    }
    ثابت منطقی hasAppliedFullNameConfig = internal_applyFullNameConfigToTag(fullNameInfo);
    اگر (hasAppliedFullNameConfig)
    {
        بازگشت;
    }
    internal_applyNamePartConfigToSpecificTag(result);
}

تهی LogTagManager::unassign(ثابت std::string& fullName)
{
    // Lock is inside assign() method.
    assign(fullName, nullptr);
}

LogTag* LogTagManager::get(ثابت std::string& fullName)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameInfo* fullNameInfoPtr = m_nameTable.getFullNameInfo(fullName);
    اگر (fullNameInfoPtr && fullNameInfoPtr->logTagPtr)
    {
        بازگشت fullNameInfoPtr->logTagPtr;
    }
    بازگشت nullptr;
}

تهی LogTagManager::setLevelByFullName(ثابت std::string& fullName, LogLevel level)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = نادرست;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    اگر (fullNameInfo.parsedLevel.scope == MatchingScope::Full &&
        fullNameInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        بازگشت;
    }
    // update the cached configured value.
    fullNameInfo.parsedLevel.scope = MatchingScope::Full;
    fullNameInfo.parsedLevel.level = level;
    // update the actual tag, if already registered.
    LogTag* logTagPtr = fullNameInfo.logTagPtr;
    اگر (logTagPtr)
    {
        logTagPtr->level = level;
    }
}

تهی LogTagManager::setLevelByFirstPart(ثابت std::string& firstPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(firstPart, level, MatchingScope::FirstNamePart);
}

تهی LogTagManager::setLevelByAnyPart(ثابت std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(anyPart, level, MatchingScope::AnyNamePart);
}

تهی LogTagManager::setLevelByNamePart(ثابت std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    NamePartLookupResult result(namePart);
    result.m_findCrossReferences = درست;
    m_nameTable.addOrLookupNamePart(result);
    NamePartInfo& namePartInfo = *result.m_namePartInfoPtr;
    اگر (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        بازگشت;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    internal_applyNamePartConfigToMatchingTags(result);
}

std::vector<std::string> LogTagManager::splitNameParts(ثابت std::string& fullName)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = fullName.length();
    std::vector<std::string> nameParts;
    size_t start = 0u;
    وقتی (start < len)
    {
        size_t nextPeriod = fullName.find('.', start);
        اگر (nextPeriod == npos)
        {
            nextPeriod = len;
        }
        اگر (nextPeriod >= start + 1u)
        {
            nameParts.emplace_back(fullName.substr(start, nextPeriod - start));
        }
        start = nextPeriod + 1u;
    }
    بازگشت nameParts;
}

منطقی LogTagManager::internal_isNamePartMatch(MatchingScope scope, size_t matchingPos)
{
    انتخاب (scope)
    {
    حالت MatchingScope::FirstNamePart:
        بازگشت (matchingPos == 0u);
    حالت MatchingScope::AnyNamePart:
        بازگشت درست;
    حالت MatchingScope::None:
    حالت MatchingScope::Full:
    پیشفرض:
        بازگشت نادرست;
    }
}

منطقی LogTagManager::internal_applyFullNameConfigToTag(FullNameInfo& fullNameInfo)
{
    اگر (!fullNameInfo.logTagPtr)
    {
        بازگشت نادرست;
    }
    اگر (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
    {
        fullNameInfo.logTagPtr->level = fullNameInfo.parsedLevel.level;
        بازگشت درست;
    }
    بازگشت نادرست;
}

منطقی LogTagManager::internal_applyNamePartConfigToSpecificTag(FullNameLookupResult& fullNameResult)
{
    ثابت FullNameInfo& fullNameInfo = *fullNameResult.m_fullNameInfoPtr;
    LogTag* ثابت logTag = fullNameInfo.logTagPtr;
    اگر (!logTag)
    {
        بازگشت نادرست;
    }
    CV_Assert(fullNameResult.m_findCrossReferences);
    ثابت auto& crossReferences = fullNameResult.m_crossReferences;
    ثابت size_t matching_Name_Part_شمار = crossReferences.size();
    برای (size_t k = 0u; k < matching_Name_Part_شمار; ++k)
    {
        ثابت auto& match = crossReferences.at(k);
        ثابت auto& namePartInfo = *match.m_namePartInfo;
        ثابت auto& parsedLevel = namePartInfo.parsedLevel;
        ثابت auto scope = parsedLevel.scope;
        ثابت LogLevel level = parsedLevel.level;
        ثابت size_t matchingPos = match.m_matchingPos;
        ثابت منطقی isMatch = internal_isNamePartMatch(scope, matchingPos);
        اگر (isMatch)
        {
            logTag->level = level;
            بازگشت درست;
        }
    }
    بازگشت نادرست;
}

تهی LogTagManager::internal_applyNamePartConfigToMatchingTags(NamePartLookupResult& namePartResult)
{
    CV_Assert(namePartResult.m_findCrossReferences);
    ثابت auto& crossReferences = namePartResult.m_crossReferences;
    ثابت size_t matching_Full_Name_شمار = crossReferences.size();
    NamePartInfo& namePartInfo = *namePartResult.m_namePartInfoPtr;
    ثابت MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::Full);
    اگر (scope == MatchingScope::None)
    {
        بازگشت;
    }
    ثابت LogLevel level = namePartInfo.parsedLevel.level;
    برای (size_t k = 0u; k < matching_Full_Name_شمار; ++k)
    {
        ثابت auto& match = crossReferences.at(k);
        FullNameInfo& fullNameInfo = *match.m_fullNameInfo;
        LogTag* logTagPtr = fullNameInfo.logTagPtr;
        اگر (!logTagPtr)
        {
            ادامه;
        }
        اگر (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            ادامه;
        }
        ثابت size_t matchingPos = match.m_matchingPos;
        ثابت منطقی isMatch = internal_isNamePartMatch(scope, matchingPos);
        اگر (!isMatch)
        {
            ادامه;
        }
        logTagPtr->level = level;
    }
}

تهی LogTagManager::NameTable::addOrLookupFullName(FullNameLookupResult& result)
{
    ثابت auto fullNameIdAndFlag = internal_addOrLookupFullName(result.m_fullName);
    result.m_fullNameId = fullNameIdAndFlag.first;
    result.m_nameParts = LogTagManager::splitNameParts(result.m_fullName);
    internal_addOrLookupNameParts(result.m_nameParts, result.m_namePartIds);
    ثابت منطقی isNew = fullNameIdAndFlag.second;
    اگر (isNew)
    {
        internal_addCrossReference(result.m_fullNameId, result.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    result.m_fullNameInfoPtr = internal_getFullNameInfo(result.m_fullNameId);
    اگر (result.m_findCrossReferences)
    {
        internal_findMatchingNamePartsForFullName(result);
    }
}

تهی LogTagManager::NameTable::addOrLookupNamePart(NamePartLookupResult& result)
{
    result.m_namePartId = internal_addOrLookupNamePart(result.m_namePart);
    result.m_namePartInfoPtr = internal_getNamePartInfo(result.m_namePartId);
    اگر (result.m_findCrossReferences)
    {
        internal_findMatchingFullNamesForNamePart(result);
    }
}

std::pair<size_t, منطقی> LogTagManager::NameTable::internal_addOrLookupFullName(ثابت std::string& fullName)
{
    ثابت auto fullNameIdIter = m_fullNameIds.find(fullName);
    اگر (fullNameIdIter != m_fullNameIds.end())
    {
        بازگشت std::make_pair(fullNameIdIter->second, نادرست);
    }
    ثابت size_t fullNameId = m_fullNameInfos.size();
    m_fullNameInfos.emplace_back(FullNameInfo{});
    m_fullNameIds.emplace(fullName, fullNameId);
    بازگشت std::make_pair(fullNameId, درست);
}

تهی LogTagManager::NameTable::internal_addOrLookupNameParts(ثابت std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    ثابت size_t name_Part_شمار = nameParts.size();
    namePartIds.resize(name_Part_شمار, ~(size_t)0u);
    برای (size_t namePartIndex = 0u; namePartIndex < name_Part_شمار; ++namePartIndex)
    {
        ثابت std::string& namePart = nameParts.at(namePartIndex);
        ثابت size_t namePartId = internal_addOrLookupNamePart(namePart);
        namePartIds.at(namePartIndex) = namePartId;
    }
}

size_t LogTagManager::NameTable::internal_addOrLookupNamePart(ثابت std::string& namePart)
{
    ثابت auto namePartIter = m_namePartIds.find(namePart);
    اگر (namePartIter != m_namePartIds.end())
    {
        بازگشت namePartIter->second;
    }
    ثابت size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    بازگشت namePartId;
}

تهی LogTagManager::NameTable::internal_addCrossReference(size_t fullNameId, ثابت std::vector<size_t>& namePartIds)
{
    ثابت size_t name_Part_شمار = namePartIds.size();
    برای (size_t namePartPos = 0u; namePartPos < name_Part_شمار; ++namePartPos)
    {
        ثابت size_t namePartId = namePartIds.at(namePartPos);
        m_fullNameToNamePartIds.emplace(fullNameId, std::make_pair(namePartId, namePartPos));
        m_namePartToFullNameIds.emplace(namePartId, std::make_pair(fullNameId, namePartPos));
    }
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::getFullNameInfo(ثابت std::string& fullName)
{
    ثابت auto fullNameIdIter = m_fullNameIds.find(fullName);
    اگر (fullNameIdIter == m_fullNameIds.end())
    {
        بازگشت nullptr;
    }
    ثابت size_t fullNameId = fullNameIdIter->second;
    بازگشت internal_getFullNameInfo(fullNameId);
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::internal_getFullNameInfo(size_t fullNameId)
{
    بازگشت std::addressof(m_fullNameInfos.at(fullNameId));
}

LogTagManager::NamePartInfo* LogTagManager::NameTable::internal_getNamePartInfo(size_t namePartId)
{
    بازگشت std::addressof(m_namePartInfos.at(namePartId));
}

تهی LogTagManager::NameTable::internal_findMatchingNamePartsForFullName(FullNameLookupResult& fullNameResult)
{
    ثابت size_t fullNameId = fullNameResult.m_fullNameId;
    FullNameInfo* fullNameInfo = fullNameResult.m_fullNameInfoPtr;
    ثابت auto& namePartIds = fullNameResult.m_namePartIds;
    ثابت size_t name_Part_شمار = namePartIds.size();
    auto& crossReferences = fullNameResult.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_شمار);
    برای (size_t matchingPos = 0u; matchingPos < name_Part_شمار; ++matchingPos)
    {
        ثابت size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = internal_getNamePartInfo(namePartId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

تهی LogTagManager::NameTable::internal_findMatchingFullNamesForNamePart(NamePartLookupResult& result)
{
    ثابت size_t namePartId = result.m_namePartId;
    NamePartInfo* namePartInfo = result.m_namePartInfoPtr;
    ثابت size_t matching_Full_Name_شمار = m_namePartToFullNameIds.شمار_2(namePartId);
    std::vector<CrossReference>& crossReferences = result.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_Full_Name_شمار);
    ثابت auto namePartToFullNameIterPair = m_namePartToFullNameIds.equal_range(result.m_namePartId);
    ثابت auto iterBegin = namePartToFullNameIterPair.first;
    ثابت auto iterEnd = namePartToFullNameIterPair.second;
    برای (auto iter = iterBegin; iter != iterEnd; ++iter)
    {
        ثابت size_t fullNameId = iter->second.first;
        ثابت size_t matchingPos = iter->second.second;
        FullNameInfo* fullNameInfo = internal_getFullNameInfo(fullNameId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

}}} //namespace
