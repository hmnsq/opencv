// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "../precomp.hpp"
#അന്തര്ഭാവ "logtagmanager.hpp"
#അന്തര്ഭാവ "logtagconfigparser.hpp"

നാമാകാശ cv {
നാമാകാശ utils {
നാമാകാശ logging {


സ്ഥിരം അക്ഷരം* LogTagManager::m_globalName = "global";


LogTagManager::LogTagManager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_globalLogTag(പുതിയ LogTag(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<LogTagConfigParser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_globalLogTag.get());
}

LogTagManager::~LogTagManager()
{
}

ശൂന്യം LogTagManager::setConfigString(സ്ഥിരം std::string& configString, തർക്കം apply /*true*/)
{
    m_config->parse(configString);
    എങ്കിൽ (m_config->hasMalformed())
    {
        തിരികെ;
    }
    എങ്കിൽ (!apply)
    {
        തിരികെ;
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
    സ്ഥിരം auto& globalConfig = m_config->getGlobalConfig();
    m_globalLogTag->level = globalConfig.level;
    വേണ്ടി (സ്ഥിരം auto& config : m_config->getFirstPartConfigs())
    {
        setLevelByFirstPart(config.namePart, config.level);
    }
    വേണ്ടി (സ്ഥിരം auto& config : m_config->getAnyPartConfigs())
    {
        setLevelByAnyPart(config.namePart, config.level);
    }
    വേണ്ടി (സ്ഥിരം auto& config : m_config->getFullNameConfigs())
    {
        setLevelByFullName(config.namePart, config.level);
    }
}

LogTagConfigParser& LogTagManager::getConfigParser() സ്ഥിരം
{
    തിരികെ *m_config;
}

ശൂന്യം LogTagManager::assign(സ്ഥിരം std::string& fullName, LogTag* ptr)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = സത്യം;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    സ്ഥിരം തർക്കം isPtrChanged = (fullNameInfo.logTagPtr != ptr);
    എങ്കിൽ (!isPtrChanged)
    {
        തിരികെ;
    }
    fullNameInfo.logTagPtr = ptr;
    എങ്കിൽ (!ptr)
    {
        തിരികെ;
    }
    സ്ഥിരം തർക്കം hasAppliedFullNameConfig = internal_applyFullNameConfigToTag(fullNameInfo);
    എങ്കിൽ (hasAppliedFullNameConfig)
    {
        തിരികെ;
    }
    internal_applyNamePartConfigToSpecificTag(result);
}

ശൂന്യം LogTagManager::unassign(സ്ഥിരം std::string& fullName)
{
    // Lock is inside assign() method.
    assign(fullName, nullptr);
}

LogTag* LogTagManager::get(സ്ഥിരം std::string& fullName)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameInfo* fullNameInfoPtr = m_nameTable.getFullNameInfo(fullName);
    എങ്കിൽ (fullNameInfoPtr && fullNameInfoPtr->logTagPtr)
    {
        തിരികെ fullNameInfoPtr->logTagPtr;
    }
    തിരികെ nullptr;
}

ശൂന്യം LogTagManager::setLevelByFullName(സ്ഥിരം std::string& fullName, LogLevel level)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = അസത്യം;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    എങ്കിൽ (fullNameInfo.parsedLevel.scope == MatchingScope::Full &&
        fullNameInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        തിരികെ;
    }
    // update the cached configured value.
    fullNameInfo.parsedLevel.scope = MatchingScope::Full;
    fullNameInfo.parsedLevel.level = level;
    // update the actual tag, if already registered.
    LogTag* logTagPtr = fullNameInfo.logTagPtr;
    എങ്കിൽ (logTagPtr)
    {
        logTagPtr->level = level;
    }
}

ശൂന്യം LogTagManager::setLevelByFirstPart(സ്ഥിരം std::string& firstPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(firstPart, level, MatchingScope::FirstNamePart);
}

ശൂന്യം LogTagManager::setLevelByAnyPart(സ്ഥിരം std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(anyPart, level, MatchingScope::AnyNamePart);
}

ശൂന്യം LogTagManager::setLevelByNamePart(സ്ഥിരം std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    NamePartLookupResult result(namePart);
    result.m_findCrossReferences = സത്യം;
    m_nameTable.addOrLookupNamePart(result);
    NamePartInfo& namePartInfo = *result.m_namePartInfoPtr;
    എങ്കിൽ (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        തിരികെ;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    internal_applyNamePartConfigToMatchingTags(result);
}

std::vector<std::string> LogTagManager::splitNameParts(സ്ഥിരം std::string& fullName)
{
    സ്ഥിരം size_t npos = std::string::npos;
    സ്ഥിരം size_t len = fullName.length();
    std::vector<std::string> nameParts;
    size_t start = 0u;
    ആകുമ്പോൾ (start < len)
    {
        size_t nextPeriod = fullName.find('.', start);
        എങ്കിൽ (nextPeriod == npos)
        {
            nextPeriod = len;
        }
        എങ്കിൽ (nextPeriod >= start + 1u)
        {
            nameParts.emplace_back(fullName.substr(start, nextPeriod - start));
        }
        start = nextPeriod + 1u;
    }
    തിരികെ nameParts;
}

തർക്കം LogTagManager::internal_isNamePartMatch(MatchingScope scope, size_t matchingPos)
{
    തിരഞ്ഞെടുപ്പ് (scope)
    {
    അവസ്ഥ MatchingScope::FirstNamePart:
        തിരികെ (matchingPos == 0u);
    അവസ്ഥ MatchingScope::AnyNamePart:
        തിരികെ സത്യം;
    അവസ്ഥ MatchingScope::None:
    അവസ്ഥ MatchingScope::Full:
    സ്ഥിരസ്ഥിതി:
        തിരികെ അസത്യം;
    }
}

തർക്കം LogTagManager::internal_applyFullNameConfigToTag(FullNameInfo& fullNameInfo)
{
    എങ്കിൽ (!fullNameInfo.logTagPtr)
    {
        തിരികെ അസത്യം;
    }
    എങ്കിൽ (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
    {
        fullNameInfo.logTagPtr->level = fullNameInfo.parsedLevel.level;
        തിരികെ സത്യം;
    }
    തിരികെ അസത്യം;
}

തർക്കം LogTagManager::internal_applyNamePartConfigToSpecificTag(FullNameLookupResult& fullNameResult)
{
    സ്ഥിരം FullNameInfo& fullNameInfo = *fullNameResult.m_fullNameInfoPtr;
    LogTag* സ്ഥിരം logTag = fullNameInfo.logTagPtr;
    എങ്കിൽ (!logTag)
    {
        തിരികെ അസത്യം;
    }
    CV_Assert(fullNameResult.m_findCrossReferences);
    സ്ഥിരം auto& crossReferences = fullNameResult.m_crossReferences;
    സ്ഥിരം size_t matching_Name_Part_എണ്ണം = crossReferences.size();
    വേണ്ടി (size_t k = 0u; k < matching_Name_Part_എണ്ണം; ++k)
    {
        സ്ഥിരം auto& match = crossReferences.at(k);
        സ്ഥിരം auto& namePartInfo = *match.m_namePartInfo;
        സ്ഥിരം auto& parsedLevel = namePartInfo.parsedLevel;
        സ്ഥിരം auto scope = parsedLevel.scope;
        സ്ഥിരം LogLevel level = parsedLevel.level;
        സ്ഥിരം size_t matchingPos = match.m_matchingPos;
        സ്ഥിരം തർക്കം isMatch = internal_isNamePartMatch(scope, matchingPos);
        എങ്കിൽ (isMatch)
        {
            logTag->level = level;
            തിരികെ സത്യം;
        }
    }
    തിരികെ അസത്യം;
}

ശൂന്യം LogTagManager::internal_applyNamePartConfigToMatchingTags(NamePartLookupResult& namePartResult)
{
    CV_Assert(namePartResult.m_findCrossReferences);
    സ്ഥിരം auto& crossReferences = namePartResult.m_crossReferences;
    സ്ഥിരം size_t matching_Full_Name_എണ്ണം = crossReferences.size();
    NamePartInfo& namePartInfo = *namePartResult.m_namePartInfoPtr;
    സ്ഥിരം MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::Full);
    എങ്കിൽ (scope == MatchingScope::None)
    {
        തിരികെ;
    }
    സ്ഥിരം LogLevel level = namePartInfo.parsedLevel.level;
    വേണ്ടി (size_t k = 0u; k < matching_Full_Name_എണ്ണം; ++k)
    {
        സ്ഥിരം auto& match = crossReferences.at(k);
        FullNameInfo& fullNameInfo = *match.m_fullNameInfo;
        LogTag* logTagPtr = fullNameInfo.logTagPtr;
        എങ്കിൽ (!logTagPtr)
        {
            തുടരുക;
        }
        എങ്കിൽ (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            തുടരുക;
        }
        സ്ഥിരം size_t matchingPos = match.m_matchingPos;
        സ്ഥിരം തർക്കം isMatch = internal_isNamePartMatch(scope, matchingPos);
        എങ്കിൽ (!isMatch)
        {
            തുടരുക;
        }
        logTagPtr->level = level;
    }
}

ശൂന്യം LogTagManager::NameTable::addOrLookupFullName(FullNameLookupResult& result)
{
    സ്ഥിരം auto fullNameIdAndFlag = internal_addOrLookupFullName(result.m_fullName);
    result.m_fullNameId = fullNameIdAndFlag.first;
    result.m_nameParts = LogTagManager::splitNameParts(result.m_fullName);
    internal_addOrLookupNameParts(result.m_nameParts, result.m_namePartIds);
    സ്ഥിരം തർക്കം isNew = fullNameIdAndFlag.second;
    എങ്കിൽ (isNew)
    {
        internal_addCrossReference(result.m_fullNameId, result.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    result.m_fullNameInfoPtr = internal_getFullNameInfo(result.m_fullNameId);
    എങ്കിൽ (result.m_findCrossReferences)
    {
        internal_findMatchingNamePartsForFullName(result);
    }
}

ശൂന്യം LogTagManager::NameTable::addOrLookupNamePart(NamePartLookupResult& result)
{
    result.m_namePartId = internal_addOrLookupNamePart(result.m_namePart);
    result.m_namePartInfoPtr = internal_getNamePartInfo(result.m_namePartId);
    എങ്കിൽ (result.m_findCrossReferences)
    {
        internal_findMatchingFullNamesForNamePart(result);
    }
}

std::pair<size_t, തർക്കം> LogTagManager::NameTable::internal_addOrLookupFullName(സ്ഥിരം std::string& fullName)
{
    സ്ഥിരം auto fullNameIdIter = m_fullNameIds.find(fullName);
    എങ്കിൽ (fullNameIdIter != m_fullNameIds.end())
    {
        തിരികെ std::make_pair(fullNameIdIter->second, അസത്യം);
    }
    സ്ഥിരം size_t fullNameId = m_fullNameInfos.size();
    m_fullNameInfos.emplace_back(FullNameInfo{});
    m_fullNameIds.emplace(fullName, fullNameId);
    തിരികെ std::make_pair(fullNameId, സത്യം);
}

ശൂന്യം LogTagManager::NameTable::internal_addOrLookupNameParts(സ്ഥിരം std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    സ്ഥിരം size_t name_Part_എണ്ണം = nameParts.size();
    namePartIds.resize(name_Part_എണ്ണം, ~(size_t)0u);
    വേണ്ടി (size_t namePartIndex = 0u; namePartIndex < name_Part_എണ്ണം; ++namePartIndex)
    {
        സ്ഥിരം std::string& namePart = nameParts.at(namePartIndex);
        സ്ഥിരം size_t namePartId = internal_addOrLookupNamePart(namePart);
        namePartIds.at(namePartIndex) = namePartId;
    }
}

size_t LogTagManager::NameTable::internal_addOrLookupNamePart(സ്ഥിരം std::string& namePart)
{
    സ്ഥിരം auto namePartIter = m_namePartIds.find(namePart);
    എങ്കിൽ (namePartIter != m_namePartIds.end())
    {
        തിരികെ namePartIter->second;
    }
    സ്ഥിരം size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    തിരികെ namePartId;
}

ശൂന്യം LogTagManager::NameTable::internal_addCrossReference(size_t fullNameId, സ്ഥിരം std::vector<size_t>& namePartIds)
{
    സ്ഥിരം size_t name_Part_എണ്ണം = namePartIds.size();
    വേണ്ടി (size_t namePartPos = 0u; namePartPos < name_Part_എണ്ണം; ++namePartPos)
    {
        സ്ഥിരം size_t namePartId = namePartIds.at(namePartPos);
        m_fullNameToNamePartIds.emplace(fullNameId, std::make_pair(namePartId, namePartPos));
        m_namePartToFullNameIds.emplace(namePartId, std::make_pair(fullNameId, namePartPos));
    }
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::getFullNameInfo(സ്ഥിരം std::string& fullName)
{
    സ്ഥിരം auto fullNameIdIter = m_fullNameIds.find(fullName);
    എങ്കിൽ (fullNameIdIter == m_fullNameIds.end())
    {
        തിരികെ nullptr;
    }
    സ്ഥിരം size_t fullNameId = fullNameIdIter->second;
    തിരികെ internal_getFullNameInfo(fullNameId);
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::internal_getFullNameInfo(size_t fullNameId)
{
    തിരികെ std::addressof(m_fullNameInfos.at(fullNameId));
}

LogTagManager::NamePartInfo* LogTagManager::NameTable::internal_getNamePartInfo(size_t namePartId)
{
    തിരികെ std::addressof(m_namePartInfos.at(namePartId));
}

ശൂന്യം LogTagManager::NameTable::internal_findMatchingNamePartsForFullName(FullNameLookupResult& fullNameResult)
{
    സ്ഥിരം size_t fullNameId = fullNameResult.m_fullNameId;
    FullNameInfo* fullNameInfo = fullNameResult.m_fullNameInfoPtr;
    സ്ഥിരം auto& namePartIds = fullNameResult.m_namePartIds;
    സ്ഥിരം size_t name_Part_എണ്ണം = namePartIds.size();
    auto& crossReferences = fullNameResult.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_എണ്ണം);
    വേണ്ടി (size_t matchingPos = 0u; matchingPos < name_Part_എണ്ണം; ++matchingPos)
    {
        സ്ഥിരം size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = internal_getNamePartInfo(namePartId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

ശൂന്യം LogTagManager::NameTable::internal_findMatchingFullNamesForNamePart(NamePartLookupResult& result)
{
    സ്ഥിരം size_t namePartId = result.m_namePartId;
    NamePartInfo* namePartInfo = result.m_namePartInfoPtr;
    സ്ഥിരം size_t matching_Full_Name_എണ്ണം = m_namePartToFullNameIds.എണ്ണം_2(namePartId);
    std::vector<CrossReference>& crossReferences = result.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_Full_Name_എണ്ണം);
    സ്ഥിരം auto namePartToFullNameIterPair = m_namePartToFullNameIds.equal_range(result.m_namePartId);
    സ്ഥിരം auto iterBegin = namePartToFullNameIterPair.first;
    സ്ഥിരം auto iterEnd = namePartToFullNameIterPair.second;
    വേണ്ടി (auto iter = iterBegin; iter != iterEnd; ++iter)
    {
        സ്ഥിരം size_t fullNameId = iter->second.first;
        സ്ഥിരം size_t matchingPos = iter->second.second;
        FullNameInfo* fullNameInfo = internal_getFullNameInfo(fullNameId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

}}} //namespace
