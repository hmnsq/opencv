// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ଅନ୍ତର୍ଭାଵ "../precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "logtagmanager.hpp"
#ଅନ୍ତର୍ଭାଵ "logtagconfigparser.hpp"

ନାମାକାଶ cv {
ନାମାକାଶ utils {
ନାମାକାଶ logging {


ଅଚଳ ଅକ୍ଷର* LogTagManager::m_globalName = "global";


LogTagManager::LogTagManager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_globalLogTag(ନୂତନ LogTag(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<LogTagConfigParser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_globalLogTag.get());
}

LogTagManager::~LogTagManager()
{
}

ଶୂନ୍ୟ LogTagManager::setConfigString(ଅଚଳ std::string& configString, ତାର୍କିକ apply /*true*/)
{
    m_config->parse(configString);
    ଯଦି (m_config->hasMalformed())
    {
        ଫେରନ୍ତୁ;
    }
    ଯଦି (!apply)
    {
        ଫେରନ୍ତୁ;
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
    ଅଚଳ auto& globalConfig = m_config->getGlobalConfig();
    m_globalLogTag->level = globalConfig.level;
    ପାଇଁ (ଅଚଳ auto& config : m_config->getFirstPartConfigs())
    {
        setLevelByFirstPart(config.namePart, config.level);
    }
    ପାଇଁ (ଅଚଳ auto& config : m_config->getAnyPartConfigs())
    {
        setLevelByAnyPart(config.namePart, config.level);
    }
    ପାଇଁ (ଅଚଳ auto& config : m_config->getFullNameConfigs())
    {
        setLevelByFullName(config.namePart, config.level);
    }
}

LogTagConfigParser& LogTagManager::getConfigParser() ଅଚଳ
{
    ଫେରନ୍ତୁ *m_config;
}

ଶୂନ୍ୟ LogTagManager::assign(ଅଚଳ std::string& fullName, LogTag* ptr)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = ସତ୍ୟ;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    ଅଚଳ ତାର୍କିକ isPtrChanged = (fullNameInfo.logTagPtr != ptr);
    ଯଦି (!isPtrChanged)
    {
        ଫେରନ୍ତୁ;
    }
    fullNameInfo.logTagPtr = ptr;
    ଯଦି (!ptr)
    {
        ଫେରନ୍ତୁ;
    }
    ଅଚଳ ତାର୍କିକ hasAppliedFullNameConfig = internal_applyFullNameConfigToTag(fullNameInfo);
    ଯଦି (hasAppliedFullNameConfig)
    {
        ଫେରନ୍ତୁ;
    }
    internal_applyNamePartConfigToSpecificTag(result);
}

ଶୂନ୍ୟ LogTagManager::unassign(ଅଚଳ std::string& fullName)
{
    // Lock is inside assign() method.
    assign(fullName, nullptr);
}

LogTag* LogTagManager::get(ଅଚଳ std::string& fullName)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameInfo* fullNameInfoPtr = m_nameTable.getFullNameInfo(fullName);
    ଯଦି (fullNameInfoPtr && fullNameInfoPtr->logTagPtr)
    {
        ଫେରନ୍ତୁ fullNameInfoPtr->logTagPtr;
    }
    ଫେରନ୍ତୁ nullptr;
}

ଶୂନ୍ୟ LogTagManager::setLevelByFullName(ଅଚଳ std::string& fullName, LogLevel level)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = ମିଥ୍ୟା;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    ଯଦି (fullNameInfo.parsedLevel.scope == MatchingScope::Full &&
        fullNameInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        ଫେରନ୍ତୁ;
    }
    // update the cached configured value.
    fullNameInfo.parsedLevel.scope = MatchingScope::Full;
    fullNameInfo.parsedLevel.level = level;
    // update the actual tag, if already registered.
    LogTag* logTagPtr = fullNameInfo.logTagPtr;
    ଯଦି (logTagPtr)
    {
        logTagPtr->level = level;
    }
}

ଶୂନ୍ୟ LogTagManager::setLevelByFirstPart(ଅଚଳ std::string& firstPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(firstPart, level, MatchingScope::FirstNamePart);
}

ଶୂନ୍ୟ LogTagManager::setLevelByAnyPart(ଅଚଳ std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(anyPart, level, MatchingScope::AnyNamePart);
}

ଶୂନ୍ୟ LogTagManager::setLevelByNamePart(ଅଚଳ std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    NamePartLookupResult result(namePart);
    result.m_findCrossReferences = ସତ୍ୟ;
    m_nameTable.addOrLookupNamePart(result);
    NamePartInfo& namePartInfo = *result.m_namePartInfoPtr;
    ଯଦି (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        ଫେରନ୍ତୁ;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    internal_applyNamePartConfigToMatchingTags(result);
}

std::vector<std::string> LogTagManager::splitNameParts(ଅଚଳ std::string& fullName)
{
    ଅଚଳ size_t npos = std::string::npos;
    ଅଚଳ size_t len = fullName.length();
    std::vector<std::string> nameParts;
    size_t start = 0u;
    ଯେତେବେଳେ (start < len)
    {
        size_t nextPeriod = fullName.find('.', start);
        ଯଦି (nextPeriod == npos)
        {
            nextPeriod = len;
        }
        ଯଦି (nextPeriod >= start + 1u)
        {
            nameParts.emplace_back(fullName.substr(start, nextPeriod - start));
        }
        start = nextPeriod + 1u;
    }
    ଫେରନ୍ତୁ nameParts;
}

ତାର୍କିକ LogTagManager::internal_isNamePartMatch(MatchingScope scope, size_t matchingPos)
{
    ବାଛ (scope)
    {
    ସ୍ଥିତି MatchingScope::FirstNamePart:
        ଫେରନ୍ତୁ (matchingPos == 0u);
    ସ୍ଥିତି MatchingScope::AnyNamePart:
        ଫେରନ୍ତୁ ସତ୍ୟ;
    ସ୍ଥିତି MatchingScope::None:
    ସ୍ଥିତି MatchingScope::Full:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
}

ତାର୍କିକ LogTagManager::internal_applyFullNameConfigToTag(FullNameInfo& fullNameInfo)
{
    ଯଦି (!fullNameInfo.logTagPtr)
    {
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଯଦି (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
    {
        fullNameInfo.logTagPtr->level = fullNameInfo.parsedLevel.level;
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ତାର୍କିକ LogTagManager::internal_applyNamePartConfigToSpecificTag(FullNameLookupResult& fullNameResult)
{
    ଅଚଳ FullNameInfo& fullNameInfo = *fullNameResult.m_fullNameInfoPtr;
    LogTag* ଅଚଳ logTag = fullNameInfo.logTagPtr;
    ଯଦି (!logTag)
    {
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    CV_Assert(fullNameResult.m_findCrossReferences);
    ଅଚଳ auto& crossReferences = fullNameResult.m_crossReferences;
    ଅଚଳ size_t matching_Name_Part_ଗଣନା = crossReferences.size();
    ପାଇଁ (size_t k = 0u; k < matching_Name_Part_ଗଣନା; ++k)
    {
        ଅଚଳ auto& match = crossReferences.at(k);
        ଅଚଳ auto& namePartInfo = *match.m_namePartInfo;
        ଅଚଳ auto& parsedLevel = namePartInfo.parsedLevel;
        ଅଚଳ auto scope = parsedLevel.scope;
        ଅଚଳ LogLevel level = parsedLevel.level;
        ଅଚଳ size_t matchingPos = match.m_matchingPos;
        ଅଚଳ ତାର୍କିକ isMatch = internal_isNamePartMatch(scope, matchingPos);
        ଯଦି (isMatch)
        {
            logTag->level = level;
            ଫେରନ୍ତୁ ସତ୍ୟ;
        }
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ଶୂନ୍ୟ LogTagManager::internal_applyNamePartConfigToMatchingTags(NamePartLookupResult& namePartResult)
{
    CV_Assert(namePartResult.m_findCrossReferences);
    ଅଚଳ auto& crossReferences = namePartResult.m_crossReferences;
    ଅଚଳ size_t matching_Full_Name_ଗଣନା = crossReferences.size();
    NamePartInfo& namePartInfo = *namePartResult.m_namePartInfoPtr;
    ଅଚଳ MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::Full);
    ଯଦି (scope == MatchingScope::None)
    {
        ଫେରନ୍ତୁ;
    }
    ଅଚଳ LogLevel level = namePartInfo.parsedLevel.level;
    ପାଇଁ (size_t k = 0u; k < matching_Full_Name_ଗଣନା; ++k)
    {
        ଅଚଳ auto& match = crossReferences.at(k);
        FullNameInfo& fullNameInfo = *match.m_fullNameInfo;
        LogTag* logTagPtr = fullNameInfo.logTagPtr;
        ଯଦି (!logTagPtr)
        {
            ଜାରି;
        }
        ଯଦି (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            ଜାରି;
        }
        ଅଚଳ size_t matchingPos = match.m_matchingPos;
        ଅଚଳ ତାର୍କିକ isMatch = internal_isNamePartMatch(scope, matchingPos);
        ଯଦି (!isMatch)
        {
            ଜାରି;
        }
        logTagPtr->level = level;
    }
}

ଶୂନ୍ୟ LogTagManager::NameTable::addOrLookupFullName(FullNameLookupResult& result)
{
    ଅଚଳ auto fullNameIdAndFlag = internal_addOrLookupFullName(result.m_fullName);
    result.m_fullNameId = fullNameIdAndFlag.first;
    result.m_nameParts = LogTagManager::splitNameParts(result.m_fullName);
    internal_addOrLookupNameParts(result.m_nameParts, result.m_namePartIds);
    ଅଚଳ ତାର୍କିକ isNew = fullNameIdAndFlag.second;
    ଯଦି (isNew)
    {
        internal_addCrossReference(result.m_fullNameId, result.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    result.m_fullNameInfoPtr = internal_getFullNameInfo(result.m_fullNameId);
    ଯଦି (result.m_findCrossReferences)
    {
        internal_findMatchingNamePartsForFullName(result);
    }
}

ଶୂନ୍ୟ LogTagManager::NameTable::addOrLookupNamePart(NamePartLookupResult& result)
{
    result.m_namePartId = internal_addOrLookupNamePart(result.m_namePart);
    result.m_namePartInfoPtr = internal_getNamePartInfo(result.m_namePartId);
    ଯଦି (result.m_findCrossReferences)
    {
        internal_findMatchingFullNamesForNamePart(result);
    }
}

std::pair<size_t, ତାର୍କିକ> LogTagManager::NameTable::internal_addOrLookupFullName(ଅଚଳ std::string& fullName)
{
    ଅଚଳ auto fullNameIdIter = m_fullNameIds.find(fullName);
    ଯଦି (fullNameIdIter != m_fullNameIds.end())
    {
        ଫେରନ୍ତୁ std::make_pair(fullNameIdIter->second, ମିଥ୍ୟା);
    }
    ଅଚଳ size_t fullNameId = m_fullNameInfos.size();
    m_fullNameInfos.emplace_back(FullNameInfo{});
    m_fullNameIds.emplace(fullName, fullNameId);
    ଫେରନ୍ତୁ std::make_pair(fullNameId, ସତ୍ୟ);
}

ଶୂନ୍ୟ LogTagManager::NameTable::internal_addOrLookupNameParts(ଅଚଳ std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    ଅଚଳ size_t name_Part_ଗଣନା = nameParts.size();
    namePartIds.resize(name_Part_ଗଣନା, ~(size_t)0u);
    ପାଇଁ (size_t namePartIndex = 0u; namePartIndex < name_Part_ଗଣନା; ++namePartIndex)
    {
        ଅଚଳ std::string& namePart = nameParts.at(namePartIndex);
        ଅଚଳ size_t namePartId = internal_addOrLookupNamePart(namePart);
        namePartIds.at(namePartIndex) = namePartId;
    }
}

size_t LogTagManager::NameTable::internal_addOrLookupNamePart(ଅଚଳ std::string& namePart)
{
    ଅଚଳ auto namePartIter = m_namePartIds.find(namePart);
    ଯଦି (namePartIter != m_namePartIds.end())
    {
        ଫେରନ୍ତୁ namePartIter->second;
    }
    ଅଚଳ size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    ଫେରନ୍ତୁ namePartId;
}

ଶୂନ୍ୟ LogTagManager::NameTable::internal_addCrossReference(size_t fullNameId, ଅଚଳ std::vector<size_t>& namePartIds)
{
    ଅଚଳ size_t name_Part_ଗଣନା = namePartIds.size();
    ପାଇଁ (size_t namePartPos = 0u; namePartPos < name_Part_ଗଣନା; ++namePartPos)
    {
        ଅଚଳ size_t namePartId = namePartIds.at(namePartPos);
        m_fullNameToNamePartIds.emplace(fullNameId, std::make_pair(namePartId, namePartPos));
        m_namePartToFullNameIds.emplace(namePartId, std::make_pair(fullNameId, namePartPos));
    }
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::getFullNameInfo(ଅଚଳ std::string& fullName)
{
    ଅଚଳ auto fullNameIdIter = m_fullNameIds.find(fullName);
    ଯଦି (fullNameIdIter == m_fullNameIds.end())
    {
        ଫେରନ୍ତୁ nullptr;
    }
    ଅଚଳ size_t fullNameId = fullNameIdIter->second;
    ଫେରନ୍ତୁ internal_getFullNameInfo(fullNameId);
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::internal_getFullNameInfo(size_t fullNameId)
{
    ଫେରନ୍ତୁ std::addressof(m_fullNameInfos.at(fullNameId));
}

LogTagManager::NamePartInfo* LogTagManager::NameTable::internal_getNamePartInfo(size_t namePartId)
{
    ଫେରନ୍ତୁ std::addressof(m_namePartInfos.at(namePartId));
}

ଶୂନ୍ୟ LogTagManager::NameTable::internal_findMatchingNamePartsForFullName(FullNameLookupResult& fullNameResult)
{
    ଅଚଳ size_t fullNameId = fullNameResult.m_fullNameId;
    FullNameInfo* fullNameInfo = fullNameResult.m_fullNameInfoPtr;
    ଅଚଳ auto& namePartIds = fullNameResult.m_namePartIds;
    ଅଚଳ size_t name_Part_ଗଣନା = namePartIds.size();
    auto& crossReferences = fullNameResult.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_ଗଣନା);
    ପାଇଁ (size_t matchingPos = 0u; matchingPos < name_Part_ଗଣନା; ++matchingPos)
    {
        ଅଚଳ size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = internal_getNamePartInfo(namePartId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

ଶୂନ୍ୟ LogTagManager::NameTable::internal_findMatchingFullNamesForNamePart(NamePartLookupResult& result)
{
    ଅଚଳ size_t namePartId = result.m_namePartId;
    NamePartInfo* namePartInfo = result.m_namePartInfoPtr;
    ଅଚଳ size_t matching_Full_Name_ଗଣନା = m_namePartToFullNameIds.ଗଣନା_3(namePartId);
    std::vector<CrossReference>& crossReferences = result.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_Full_Name_ଗଣନା);
    ଅଚଳ auto namePartToFullNameIterPair = m_namePartToFullNameIds.equal_range(result.m_namePartId);
    ଅଚଳ auto iterBegin = namePartToFullNameIterPair.first;
    ଅଚଳ auto iterEnd = namePartToFullNameIterPair.second;
    ପାଇଁ (auto iter = iterBegin; iter != iterEnd; ++iter)
    {
        ଅଚଳ size_t fullNameId = iter->second.first;
        ଅଚଳ size_t matchingPos = iter->second.second;
        FullNameInfo* fullNameInfo = internal_getFullNameInfo(fullNameId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

}}} //namespace
