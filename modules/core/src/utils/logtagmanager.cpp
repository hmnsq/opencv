// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"
#ضمن "logtagmanager.hpp"
#ضمن "logtagconfigparser.hpp"

نطاق cv {
نطاق utils {
نطاق logging {


ثابت حرف* LogTagManager::m_globalName = "global";


LogTagManager::LogTagManager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_globalLogTag(جديد LogTag(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<LogTagConfigParser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_globalLogTag.get());
}

LogTagManager::~LogTagManager()
{
}

فراغ LogTagManager::setConfigString(ثابت std::string& configString, منطقي apply /*true*/)
{
    m_config->parse(configString);
    ܐܢ (m_config->hasMalformed())
    {
        ܦܢܐ;
    }
    ܐܢ (!apply)
    {
        ܦܢܐ;
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
    لكل (ثابت auto& config : m_config->getFirstPartConfigs())
    {
        setLevelByFirstPart(config.namePart, config.level);
    }
    لكل (ثابت auto& config : m_config->getAnyPartConfigs())
    {
        setLevelByAnyPart(config.namePart, config.level);
    }
    لكل (ثابت auto& config : m_config->getFullNameConfigs())
    {
        setLevelByFullName(config.namePart, config.level);
    }
}

LogTagConfigParser& LogTagManager::getConfigParser() ثابت
{
    ܦܢܐ *m_config;
}

فراغ LogTagManager::assign(ثابت std::string& fullName, LogTag* ptr)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = ܫܪܪܐ;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    ثابت منطقي isPtrChanged = (fullNameInfo.logTagPtr != ptr);
    ܐܢ (!isPtrChanged)
    {
        ܦܢܐ;
    }
    fullNameInfo.logTagPtr = ptr;
    ܐܢ (!ptr)
    {
        ܦܢܐ;
    }
    ثابت منطقي hasAppliedFullNameConfig = internal_applyFullNameConfigToTag(fullNameInfo);
    ܐܢ (hasAppliedFullNameConfig)
    {
        ܦܢܐ;
    }
    internal_applyNamePartConfigToSpecificTag(result);
}

فراغ LogTagManager::unassign(ثابت std::string& fullName)
{
    // Lock is inside assign() method.
    assign(fullName, nullptr);
}

LogTag* LogTagManager::get(ثابت std::string& fullName)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameInfo* fullNameInfoPtr = m_nameTable.getFullNameInfo(fullName);
    ܐܢ (fullNameInfoPtr && fullNameInfoPtr->logTagPtr)
    {
        ܦܢܐ fullNameInfoPtr->logTagPtr;
    }
    ܦܢܐ nullptr;
}

فراغ LogTagManager::setLevelByFullName(ثابت std::string& fullName, LogLevel level)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = ܕܓܠܐ;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    ܐܢ (fullNameInfo.parsedLevel.scope == MatchingScope::Full &&
        fullNameInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        ܦܢܐ;
    }
    // update the cached configured value.
    fullNameInfo.parsedLevel.scope = MatchingScope::Full;
    fullNameInfo.parsedLevel.level = level;
    // update the actual tag, if already registered.
    LogTag* logTagPtr = fullNameInfo.logTagPtr;
    ܐܢ (logTagPtr)
    {
        logTagPtr->level = level;
    }
}

فراغ LogTagManager::setLevelByFirstPart(ثابت std::string& firstPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(firstPart, level, MatchingScope::FirstNamePart);
}

فراغ LogTagManager::setLevelByAnyPart(ثابت std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(anyPart, level, MatchingScope::AnyNamePart);
}

فراغ LogTagManager::setLevelByNamePart(ثابت std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    NamePartLookupResult result(namePart);
    result.m_findCrossReferences = ܫܪܪܐ;
    m_nameTable.addOrLookupNamePart(result);
    NamePartInfo& namePartInfo = *result.m_namePartInfoPtr;
    ܐܢ (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        ܦܢܐ;
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
    ܟܡܐ (start < len)
    {
        size_t nextPeriod = fullName.find('.', start);
        ܐܢ (nextPeriod == npos)
        {
            nextPeriod = len;
        }
        ܐܢ (nextPeriod >= start + 1u)
        {
            nameParts.emplace_back(fullName.substr(start, nextPeriod - start));
        }
        start = nextPeriod + 1u;
    }
    ܦܢܐ nameParts;
}

منطقي LogTagManager::internal_isNamePartMatch(MatchingScope scope, size_t matchingPos)
{
    بدل (scope)
    {
    حالة MatchingScope::FirstNamePart:
        ܦܢܐ (matchingPos == 0u);
    حالة MatchingScope::AnyNamePart:
        ܦܢܐ ܫܪܪܐ;
    حالة MatchingScope::None:
    حالة MatchingScope::Full:
    افتراضي:
        ܦܢܐ ܕܓܠܐ;
    }
}

منطقي LogTagManager::internal_applyFullNameConfigToTag(FullNameInfo& fullNameInfo)
{
    ܐܢ (!fullNameInfo.logTagPtr)
    {
        ܦܢܐ ܕܓܠܐ;
    }
    ܐܢ (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
    {
        fullNameInfo.logTagPtr->level = fullNameInfo.parsedLevel.level;
        ܦܢܐ ܫܪܪܐ;
    }
    ܦܢܐ ܕܓܠܐ;
}

منطقي LogTagManager::internal_applyNamePartConfigToSpecificTag(FullNameLookupResult& fullNameResult)
{
    ثابت FullNameInfo& fullNameInfo = *fullNameResult.m_fullNameInfoPtr;
    LogTag* ثابت logTag = fullNameInfo.logTagPtr;
    ܐܢ (!logTag)
    {
        ܦܢܐ ܕܓܠܐ;
    }
    CV_Assert(fullNameResult.m_findCrossReferences);
    ثابت auto& crossReferences = fullNameResult.m_crossReferences;
    ثابت size_t matchingNamePartCount = crossReferences.size();
    لكل (size_t k = 0u; k < matchingNamePartCount; ++k)
    {
        ثابت auto& match = crossReferences.at(k);
        ثابت auto& namePartInfo = *match.m_namePartInfo;
        ثابت auto& parsedLevel = namePartInfo.parsedLevel;
        ثابت auto scope = parsedLevel.scope;
        ثابت LogLevel level = parsedLevel.level;
        ثابت size_t matchingPos = match.m_matchingPos;
        ثابت منطقي isMatch = internal_isNamePartMatch(scope, matchingPos);
        ܐܢ (isMatch)
        {
            logTag->level = level;
            ܦܢܐ ܫܪܪܐ;
        }
    }
    ܦܢܐ ܕܓܠܐ;
}

فراغ LogTagManager::internal_applyNamePartConfigToMatchingTags(NamePartLookupResult& namePartResult)
{
    CV_Assert(namePartResult.m_findCrossReferences);
    ثابت auto& crossReferences = namePartResult.m_crossReferences;
    ثابت size_t matchingFullNameCount = crossReferences.size();
    NamePartInfo& namePartInfo = *namePartResult.m_namePartInfoPtr;
    ثابت MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::Full);
    ܐܢ (scope == MatchingScope::None)
    {
        ܦܢܐ;
    }
    ثابت LogLevel level = namePartInfo.parsedLevel.level;
    لكل (size_t k = 0u; k < matchingFullNameCount; ++k)
    {
        ثابت auto& match = crossReferences.at(k);
        FullNameInfo& fullNameInfo = *match.m_fullNameInfo;
        LogTag* logTagPtr = fullNameInfo.logTagPtr;
        ܐܢ (!logTagPtr)
        {
            استمر;
        }
        ܐܢ (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            استمر;
        }
        ثابت size_t matchingPos = match.m_matchingPos;
        ثابت منطقي isMatch = internal_isNamePartMatch(scope, matchingPos);
        ܐܢ (!isMatch)
        {
            استمر;
        }
        logTagPtr->level = level;
    }
}

فراغ LogTagManager::NameTable::addOrLookupFullName(FullNameLookupResult& result)
{
    ثابت auto fullNameIdAndFlag = internal_addOrLookupFullName(result.m_fullName);
    result.m_fullNameId = fullNameIdAndFlag.first;
    result.m_nameParts = LogTagManager::splitNameParts(result.m_fullName);
    internal_addOrLookupNameParts(result.m_nameParts, result.m_namePartIds);
    ثابت منطقي isNew = fullNameIdAndFlag.second;
    ܐܢ (isNew)
    {
        internal_addCrossReference(result.m_fullNameId, result.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    result.m_fullNameInfoPtr = internal_getFullNameInfo(result.m_fullNameId);
    ܐܢ (result.m_findCrossReferences)
    {
        internal_findMatchingNamePartsForFullName(result);
    }
}

فراغ LogTagManager::NameTable::addOrLookupNamePart(NamePartLookupResult& result)
{
    result.m_namePartId = internal_addOrLookupNamePart(result.m_namePart);
    result.m_namePartInfoPtr = internal_getNamePartInfo(result.m_namePartId);
    ܐܢ (result.m_findCrossReferences)
    {
        internal_findMatchingFullNamesForNamePart(result);
    }
}

std::pair<size_t, منطقي> LogTagManager::NameTable::internal_addOrLookupFullName(ثابت std::string& fullName)
{
    ثابت auto fullNameIdIter = m_fullNameIds.find(fullName);
    ܐܢ (fullNameIdIter != m_fullNameIds.end())
    {
        ܦܢܐ std::make_pair(fullNameIdIter->second, ܕܓܠܐ);
    }
    ثابت size_t fullNameId = m_fullNameInfos.size();
    m_fullNameInfos.emplace_back(FullNameInfo{});
    m_fullNameIds.emplace(fullName, fullNameId);
    ܦܢܐ std::make_pair(fullNameId, ܫܪܪܐ);
}

فراغ LogTagManager::NameTable::internal_addOrLookupNameParts(ثابت std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    ثابت size_t namePartCount = nameParts.size();
    namePartIds.resize(namePartCount, ~(size_t)0u);
    لكل (size_t namePartIndex = 0u; namePartIndex < namePartCount; ++namePartIndex)
    {
        ثابت std::string& namePart = nameParts.at(namePartIndex);
        ثابت size_t namePartId = internal_addOrLookupNamePart(namePart);
        namePartIds.at(namePartIndex) = namePartId;
    }
}

size_t LogTagManager::NameTable::internal_addOrLookupNamePart(ثابت std::string& namePart)
{
    ثابت auto namePartIter = m_namePartIds.find(namePart);
    ܐܢ (namePartIter != m_namePartIds.end())
    {
        ܦܢܐ namePartIter->second;
    }
    ثابت size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    ܦܢܐ namePartId;
}

فراغ LogTagManager::NameTable::internal_addCrossReference(size_t fullNameId, ثابت std::vector<size_t>& namePartIds)
{
    ثابت size_t namePartCount = namePartIds.size();
    لكل (size_t namePartPos = 0u; namePartPos < namePartCount; ++namePartPos)
    {
        ثابت size_t namePartId = namePartIds.at(namePartPos);
        m_fullNameToNamePartIds.emplace(fullNameId, std::make_pair(namePartId, namePartPos));
        m_namePartToFullNameIds.emplace(namePartId, std::make_pair(fullNameId, namePartPos));
    }
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::getFullNameInfo(ثابت std::string& fullName)
{
    ثابت auto fullNameIdIter = m_fullNameIds.find(fullName);
    ܐܢ (fullNameIdIter == m_fullNameIds.end())
    {
        ܦܢܐ nullptr;
    }
    ثابت size_t fullNameId = fullNameIdIter->second;
    ܦܢܐ internal_getFullNameInfo(fullNameId);
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::internal_getFullNameInfo(size_t fullNameId)
{
    ܦܢܐ std::addressof(m_fullNameInfos.at(fullNameId));
}

LogTagManager::NamePartInfo* LogTagManager::NameTable::internal_getNamePartInfo(size_t namePartId)
{
    ܦܢܐ std::addressof(m_namePartInfos.at(namePartId));
}

فراغ LogTagManager::NameTable::internal_findMatchingNamePartsForFullName(FullNameLookupResult& fullNameResult)
{
    ثابت size_t fullNameId = fullNameResult.m_fullNameId;
    FullNameInfo* fullNameInfo = fullNameResult.m_fullNameInfoPtr;
    ثابت auto& namePartIds = fullNameResult.m_namePartIds;
    ثابت size_t namePartCount = namePartIds.size();
    auto& crossReferences = fullNameResult.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(namePartCount);
    لكل (size_t matchingPos = 0u; matchingPos < namePartCount; ++matchingPos)
    {
        ثابت size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = internal_getNamePartInfo(namePartId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

فراغ LogTagManager::NameTable::internal_findMatchingFullNamesForNamePart(NamePartLookupResult& result)
{
    ثابت size_t namePartId = result.m_namePartId;
    NamePartInfo* namePartInfo = result.m_namePartInfoPtr;
    ثابت size_t matchingFullNameCount = m_namePartToFullNameIds.count(namePartId);
    std::vector<CrossReference>& crossReferences = result.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matchingFullNameCount);
    ثابت auto namePartToFullNameIterPair = m_namePartToFullNameIds.equal_range(result.m_namePartId);
    ثابت auto iterBegin = namePartToFullNameIterPair.first;
    ثابت auto iterEnd = namePartToFullNameIterPair.second;
    لكل (auto iter = iterBegin; iter != iterEnd; ++iter)
    {
        ثابت size_t fullNameId = iter->second.first;
        ثابت size_t matchingPos = iter->second.second;
        FullNameInfo* fullNameInfo = internal_getFullNameInfo(fullNameId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

}}} //namespace
