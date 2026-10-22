// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "../precomp.hpp"
#અન્તર્ભાવ "logtagmanager.hpp"
#અન્તર્ભાવ "logtagconfigparser.hpp"

નામાકાશ cv {
નામાકાશ utils {
નામાકાશ logging {


અચલ અક્ષર* LogTagManager::m_globalName = "global";


LogTagManager::LogTagManager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_globalLogTag(નવું LogTag(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<LogTagConfigParser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_globalLogTag.get());
}

LogTagManager::~LogTagManager()
{
}

શૂન્ય LogTagManager::setConfigString(અચલ std::string& configString, તાર્કિક apply /*true*/)
{
    m_config->parse(configString);
    જો (m_config->hasMalformed())
    {
        પાછા;
    }
    જો (!apply)
    {
        પાછા;
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
    અચલ auto& globalConfig = m_config->getGlobalConfig();
    m_globalLogTag->level = globalConfig.level;
    માટે (અચલ auto& config : m_config->getFirstPartConfigs())
    {
        setLevelByFirstPart(config.namePart, config.level);
    }
    માટે (અચલ auto& config : m_config->getAnyPartConfigs())
    {
        setLevelByAnyPart(config.namePart, config.level);
    }
    માટે (અચલ auto& config : m_config->getFullNameConfigs())
    {
        setLevelByFullName(config.namePart, config.level);
    }
}

LogTagConfigParser& LogTagManager::getConfigParser() અચલ
{
    પાછા *m_config;
}

શૂન્ય LogTagManager::assign(અચલ std::string& fullName, LogTag* ptr)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = સાચું;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    અચલ તાર્કિક isPtrChanged = (fullNameInfo.logTagPtr != ptr);
    જો (!isPtrChanged)
    {
        પાછા;
    }
    fullNameInfo.logTagPtr = ptr;
    જો (!ptr)
    {
        પાછા;
    }
    અચલ તાર્કિક hasAppliedFullNameConfig = internal_applyFullNameConfigToTag(fullNameInfo);
    જો (hasAppliedFullNameConfig)
    {
        પાછા;
    }
    internal_applyNamePartConfigToSpecificTag(result);
}

શૂન્ય LogTagManager::unassign(અચલ std::string& fullName)
{
    // Lock is inside assign() method.
    assign(fullName, nullptr);
}

LogTag* LogTagManager::get(અચલ std::string& fullName)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameInfo* fullNameInfoPtr = m_nameTable.getFullNameInfo(fullName);
    જો (fullNameInfoPtr && fullNameInfoPtr->logTagPtr)
    {
        પાછા fullNameInfoPtr->logTagPtr;
    }
    પાછા nullptr;
}

શૂન્ય LogTagManager::setLevelByFullName(અચલ std::string& fullName, LogLevel level)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = ખોટું;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    જો (fullNameInfo.parsedLevel.scope == MatchingScope::Full &&
        fullNameInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        પાછા;
    }
    // update the cached configured value.
    fullNameInfo.parsedLevel.scope = MatchingScope::Full;
    fullNameInfo.parsedLevel.level = level;
    // update the actual tag, if already registered.
    LogTag* logTagPtr = fullNameInfo.logTagPtr;
    જો (logTagPtr)
    {
        logTagPtr->level = level;
    }
}

શૂન્ય LogTagManager::setLevelByFirstPart(અચલ std::string& firstPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(firstPart, level, MatchingScope::FirstNamePart);
}

શૂન્ય LogTagManager::setLevelByAnyPart(અચલ std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(anyPart, level, MatchingScope::AnyNamePart);
}

શૂન્ય LogTagManager::setLevelByNamePart(અચલ std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    NamePartLookupResult result(namePart);
    result.m_findCrossReferences = સાચું;
    m_nameTable.addOrLookupNamePart(result);
    NamePartInfo& namePartInfo = *result.m_namePartInfoPtr;
    જો (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        પાછા;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    internal_applyNamePartConfigToMatchingTags(result);
}

std::vector<std::string> LogTagManager::splitNameParts(અચલ std::string& fullName)
{
    અચલ size_t npos = std::string::npos;
    અચલ size_t len = fullName.length();
    std::vector<std::string> nameParts;
    size_t start = 0u;
    જ્યારે (start < len)
    {
        size_t nextPeriod = fullName.find('.', start);
        જો (nextPeriod == npos)
        {
            nextPeriod = len;
        }
        જો (nextPeriod >= start + 1u)
        {
            nameParts.emplace_back(fullName.substr(start, nextPeriod - start));
        }
        start = nextPeriod + 1u;
    }
    પાછા nameParts;
}

તાર્કિક LogTagManager::internal_isNamePartMatch(MatchingScope scope, size_t matchingPos)
{
    પસંદગી (scope)
    {
    સ્થિતિ MatchingScope::FirstNamePart:
        પાછા (matchingPos == 0u);
    સ્થિતિ MatchingScope::AnyNamePart:
        પાછા સાચું;
    સ્થિતિ MatchingScope::None:
    સ્થિતિ MatchingScope::Full:
    મૂળભૂત:
        પાછા ખોટું;
    }
}

તાર્કિક LogTagManager::internal_applyFullNameConfigToTag(FullNameInfo& fullNameInfo)
{
    જો (!fullNameInfo.logTagPtr)
    {
        પાછા ખોટું;
    }
    જો (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
    {
        fullNameInfo.logTagPtr->level = fullNameInfo.parsedLevel.level;
        પાછા સાચું;
    }
    પાછા ખોટું;
}

તાર્કિક LogTagManager::internal_applyNamePartConfigToSpecificTag(FullNameLookupResult& fullNameResult)
{
    અચલ FullNameInfo& fullNameInfo = *fullNameResult.m_fullNameInfoPtr;
    LogTag* અચલ logTag = fullNameInfo.logTagPtr;
    જો (!logTag)
    {
        પાછા ખોટું;
    }
    CV_Assert(fullNameResult.m_findCrossReferences);
    અચલ auto& crossReferences = fullNameResult.m_crossReferences;
    અચલ size_t matching_Name_Part_ગણતરી = crossReferences.size();
    માટે (size_t k = 0u; k < matching_Name_Part_ગણતરી; ++k)
    {
        અચલ auto& match = crossReferences.at(k);
        અચલ auto& namePartInfo = *match.m_namePartInfo;
        અચલ auto& parsedLevel = namePartInfo.parsedLevel;
        અચલ auto scope = parsedLevel.scope;
        અચલ LogLevel level = parsedLevel.level;
        અચલ size_t matchingPos = match.m_matchingPos;
        અચલ તાર્કિક isMatch = internal_isNamePartMatch(scope, matchingPos);
        જો (isMatch)
        {
            logTag->level = level;
            પાછા સાચું;
        }
    }
    પાછા ખોટું;
}

શૂન્ય LogTagManager::internal_applyNamePartConfigToMatchingTags(NamePartLookupResult& namePartResult)
{
    CV_Assert(namePartResult.m_findCrossReferences);
    અચલ auto& crossReferences = namePartResult.m_crossReferences;
    અચલ size_t matching_Full_Name_ગણતરી = crossReferences.size();
    NamePartInfo& namePartInfo = *namePartResult.m_namePartInfoPtr;
    અચલ MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::Full);
    જો (scope == MatchingScope::None)
    {
        પાછા;
    }
    અચલ LogLevel level = namePartInfo.parsedLevel.level;
    માટે (size_t k = 0u; k < matching_Full_Name_ગણતરી; ++k)
    {
        અચલ auto& match = crossReferences.at(k);
        FullNameInfo& fullNameInfo = *match.m_fullNameInfo;
        LogTag* logTagPtr = fullNameInfo.logTagPtr;
        જો (!logTagPtr)
        {
            ચાલુ;
        }
        જો (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            ચાલુ;
        }
        અચલ size_t matchingPos = match.m_matchingPos;
        અચલ તાર્કિક isMatch = internal_isNamePartMatch(scope, matchingPos);
        જો (!isMatch)
        {
            ચાલુ;
        }
        logTagPtr->level = level;
    }
}

શૂન્ય LogTagManager::NameTable::addOrLookupFullName(FullNameLookupResult& result)
{
    અચલ auto fullNameIdAndFlag = internal_addOrLookupFullName(result.m_fullName);
    result.m_fullNameId = fullNameIdAndFlag.first;
    result.m_nameParts = LogTagManager::splitNameParts(result.m_fullName);
    internal_addOrLookupNameParts(result.m_nameParts, result.m_namePartIds);
    અચલ તાર્કિક isNew = fullNameIdAndFlag.second;
    જો (isNew)
    {
        internal_addCrossReference(result.m_fullNameId, result.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    result.m_fullNameInfoPtr = internal_getFullNameInfo(result.m_fullNameId);
    જો (result.m_findCrossReferences)
    {
        internal_findMatchingNamePartsForFullName(result);
    }
}

શૂન્ય LogTagManager::NameTable::addOrLookupNamePart(NamePartLookupResult& result)
{
    result.m_namePartId = internal_addOrLookupNamePart(result.m_namePart);
    result.m_namePartInfoPtr = internal_getNamePartInfo(result.m_namePartId);
    જો (result.m_findCrossReferences)
    {
        internal_findMatchingFullNamesForNamePart(result);
    }
}

std::pair<size_t, તાર્કિક> LogTagManager::NameTable::internal_addOrLookupFullName(અચલ std::string& fullName)
{
    અચલ auto fullNameIdIter = m_fullNameIds.find(fullName);
    જો (fullNameIdIter != m_fullNameIds.end())
    {
        પાછા std::make_pair(fullNameIdIter->second, ખોટું);
    }
    અચલ size_t fullNameId = m_fullNameInfos.size();
    m_fullNameInfos.emplace_back(FullNameInfo{});
    m_fullNameIds.emplace(fullName, fullNameId);
    પાછા std::make_pair(fullNameId, સાચું);
}

શૂન્ય LogTagManager::NameTable::internal_addOrLookupNameParts(અચલ std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    અચલ size_t name_Part_ગણતરી = nameParts.size();
    namePartIds.resize(name_Part_ગણતરી, ~(size_t)0u);
    માટે (size_t namePartIndex = 0u; namePartIndex < name_Part_ગણતરી; ++namePartIndex)
    {
        અચલ std::string& namePart = nameParts.at(namePartIndex);
        અચલ size_t namePartId = internal_addOrLookupNamePart(namePart);
        namePartIds.at(namePartIndex) = namePartId;
    }
}

size_t LogTagManager::NameTable::internal_addOrLookupNamePart(અચલ std::string& namePart)
{
    અચલ auto namePartIter = m_namePartIds.find(namePart);
    જો (namePartIter != m_namePartIds.end())
    {
        પાછા namePartIter->second;
    }
    અચલ size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    પાછા namePartId;
}

શૂન્ય LogTagManager::NameTable::internal_addCrossReference(size_t fullNameId, અચલ std::vector<size_t>& namePartIds)
{
    અચલ size_t name_Part_ગણતરી = namePartIds.size();
    માટે (size_t namePartPos = 0u; namePartPos < name_Part_ગણતરી; ++namePartPos)
    {
        અચલ size_t namePartId = namePartIds.at(namePartPos);
        m_fullNameToNamePartIds.emplace(fullNameId, std::make_pair(namePartId, namePartPos));
        m_namePartToFullNameIds.emplace(namePartId, std::make_pair(fullNameId, namePartPos));
    }
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::getFullNameInfo(અચલ std::string& fullName)
{
    અચલ auto fullNameIdIter = m_fullNameIds.find(fullName);
    જો (fullNameIdIter == m_fullNameIds.end())
    {
        પાછા nullptr;
    }
    અચલ size_t fullNameId = fullNameIdIter->second;
    પાછા internal_getFullNameInfo(fullNameId);
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::internal_getFullNameInfo(size_t fullNameId)
{
    પાછા std::addressof(m_fullNameInfos.at(fullNameId));
}

LogTagManager::NamePartInfo* LogTagManager::NameTable::internal_getNamePartInfo(size_t namePartId)
{
    પાછા std::addressof(m_namePartInfos.at(namePartId));
}

શૂન્ય LogTagManager::NameTable::internal_findMatchingNamePartsForFullName(FullNameLookupResult& fullNameResult)
{
    અચલ size_t fullNameId = fullNameResult.m_fullNameId;
    FullNameInfo* fullNameInfo = fullNameResult.m_fullNameInfoPtr;
    અચલ auto& namePartIds = fullNameResult.m_namePartIds;
    અચલ size_t name_Part_ગણતરી = namePartIds.size();
    auto& crossReferences = fullNameResult.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_ગણતરી);
    માટે (size_t matchingPos = 0u; matchingPos < name_Part_ગણતરી; ++matchingPos)
    {
        અચલ size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = internal_getNamePartInfo(namePartId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

શૂન્ય LogTagManager::NameTable::internal_findMatchingFullNamesForNamePart(NamePartLookupResult& result)
{
    અચલ size_t namePartId = result.m_namePartId;
    NamePartInfo* namePartInfo = result.m_namePartInfoPtr;
    અચલ size_t matching_Full_Name_ગણતરી = m_namePartToFullNameIds.ગણતરી_2(namePartId);
    std::vector<CrossReference>& crossReferences = result.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_Full_Name_ગણતરી);
    અચલ auto namePartToFullNameIterPair = m_namePartToFullNameIds.equal_range(result.m_namePartId);
    અચલ auto iterBegin = namePartToFullNameIterPair.first;
    અચલ auto iterEnd = namePartToFullNameIterPair.second;
    માટે (auto iter = iterBegin; iter != iterEnd; ++iter)
    {
        અચલ size_t fullNameId = iter->second.first;
        અચલ size_t matchingPos = iter->second.second;
        FullNameInfo* fullNameInfo = internal_getFullNameInfo(fullNameId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

}}} //namespace
