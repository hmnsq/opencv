// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "../precomp.hpp"
#அந்தர்பாவ "logtagmanager.hpp"
#அந்தர்பாவ "logtagconfigparser.hpp"

நாமாகாஶ cv {
நாமாகாஶ utils {
நாமாகாஶ logging {


மாறிலி எழுத்து* LogTagManager::m_globalName = "global";


LogTagManager::LogTagManager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_globalLogTag(புதிய LogTag(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<LogTagConfigParser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_globalLogTag.get());
}

LogTagManager::~LogTagManager()
{
}

வெறுமை LogTagManager::setConfigString(மாறிலி std::string& configString, தருக்க apply /*true*/)
{
    m_config->parse(configString);
    எனில் (m_config->hasMalformed())
    {
        திரும்பு;
    }
    எனில் (!apply)
    {
        திரும்பு;
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
    மாறிலி auto& globalConfig = m_config->getGlobalConfig();
    m_globalLogTag->level = globalConfig.level;
    க்கு (மாறிலி auto& config : m_config->getFirstPartConfigs())
    {
        setLevelByFirstPart(config.namePart, config.level);
    }
    க்கு (மாறிலி auto& config : m_config->getAnyPartConfigs())
    {
        setLevelByAnyPart(config.namePart, config.level);
    }
    க்கு (மாறிலி auto& config : m_config->getFullNameConfigs())
    {
        setLevelByFullName(config.namePart, config.level);
    }
}

LogTagConfigParser& LogTagManager::getConfigParser() மாறிலி
{
    திரும்பு *m_config;
}

வெறுமை LogTagManager::assign(மாறிலி std::string& fullName, LogTag* ptr)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = உண்மை;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    மாறிலி தருக்க isPtrChanged = (fullNameInfo.logTagPtr != ptr);
    எனில் (!isPtrChanged)
    {
        திரும்பு;
    }
    fullNameInfo.logTagPtr = ptr;
    எனில் (!ptr)
    {
        திரும்பு;
    }
    மாறிலி தருக்க hasAppliedFullNameConfig = internal_applyFullNameConfigToTag(fullNameInfo);
    எனில் (hasAppliedFullNameConfig)
    {
        திரும்பு;
    }
    internal_applyNamePartConfigToSpecificTag(result);
}

வெறுமை LogTagManager::unassign(மாறிலி std::string& fullName)
{
    // Lock is inside assign() method.
    assign(fullName, nullptr);
}

LogTag* LogTagManager::get(மாறிலி std::string& fullName)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameInfo* fullNameInfoPtr = m_nameTable.getFullNameInfo(fullName);
    எனில் (fullNameInfoPtr && fullNameInfoPtr->logTagPtr)
    {
        திரும்பு fullNameInfoPtr->logTagPtr;
    }
    திரும்பு nullptr;
}

வெறுமை LogTagManager::setLevelByFullName(மாறிலி std::string& fullName, LogLevel level)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = பொய்;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    எனில் (fullNameInfo.parsedLevel.scope == MatchingScope::Full &&
        fullNameInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        திரும்பு;
    }
    // update the cached configured value.
    fullNameInfo.parsedLevel.scope = MatchingScope::Full;
    fullNameInfo.parsedLevel.level = level;
    // update the actual tag, if already registered.
    LogTag* logTagPtr = fullNameInfo.logTagPtr;
    எனில் (logTagPtr)
    {
        logTagPtr->level = level;
    }
}

வெறுமை LogTagManager::setLevelByFirstPart(மாறிலி std::string& firstPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(firstPart, level, MatchingScope::FirstNamePart);
}

வெறுமை LogTagManager::setLevelByAnyPart(மாறிலி std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(anyPart, level, MatchingScope::AnyNamePart);
}

வெறுமை LogTagManager::setLevelByNamePart(மாறிலி std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    NamePartLookupResult result(namePart);
    result.m_findCrossReferences = உண்மை;
    m_nameTable.addOrLookupNamePart(result);
    NamePartInfo& namePartInfo = *result.m_namePartInfoPtr;
    எனில் (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        திரும்பு;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    internal_applyNamePartConfigToMatchingTags(result);
}

std::vector<std::string> LogTagManager::splitNameParts(மாறிலி std::string& fullName)
{
    மாறிலி size_t npos = std::string::npos;
    மாறிலி size_t len = fullName.length();
    std::vector<std::string> nameParts;
    size_t start = 0u;
    போது (start < len)
    {
        size_t nextPeriod = fullName.find('.', start);
        எனில் (nextPeriod == npos)
        {
            nextPeriod = len;
        }
        எனில் (nextPeriod >= start + 1u)
        {
            nameParts.emplace_back(fullName.substr(start, nextPeriod - start));
        }
        start = nextPeriod + 1u;
    }
    திரும்பு nameParts;
}

தருக்க LogTagManager::internal_isNamePartMatch(MatchingScope scope, size_t matchingPos)
{
    தேர்வு (scope)
    {
    நிலை MatchingScope::FirstNamePart:
        திரும்பு (matchingPos == 0u);
    நிலை MatchingScope::AnyNamePart:
        திரும்பு உண்மை;
    நிலை MatchingScope::None:
    நிலை MatchingScope::Full:
    இயல்பு:
        திரும்பு பொய்;
    }
}

தருக்க LogTagManager::internal_applyFullNameConfigToTag(FullNameInfo& fullNameInfo)
{
    எனில் (!fullNameInfo.logTagPtr)
    {
        திரும்பு பொய்;
    }
    எனில் (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
    {
        fullNameInfo.logTagPtr->level = fullNameInfo.parsedLevel.level;
        திரும்பு உண்மை;
    }
    திரும்பு பொய்;
}

தருக்க LogTagManager::internal_applyNamePartConfigToSpecificTag(FullNameLookupResult& fullNameResult)
{
    மாறிலி FullNameInfo& fullNameInfo = *fullNameResult.m_fullNameInfoPtr;
    LogTag* மாறிலி logTag = fullNameInfo.logTagPtr;
    எனில் (!logTag)
    {
        திரும்பு பொய்;
    }
    CV_Assert(fullNameResult.m_findCrossReferences);
    மாறிலி auto& crossReferences = fullNameResult.m_crossReferences;
    மாறிலி size_t matching_Name_Part_மொத்தம் = crossReferences.size();
    க்கு (size_t k = 0u; k < matching_Name_Part_மொத்தம்; ++k)
    {
        மாறிலி auto& match = crossReferences.at(k);
        மாறிலி auto& namePartInfo = *match.m_namePartInfo;
        மாறிலி auto& parsedLevel = namePartInfo.parsedLevel;
        மாறிலி auto scope = parsedLevel.scope;
        மாறிலி LogLevel level = parsedLevel.level;
        மாறிலி size_t matchingPos = match.m_matchingPos;
        மாறிலி தருக்க isMatch = internal_isNamePartMatch(scope, matchingPos);
        எனில் (isMatch)
        {
            logTag->level = level;
            திரும்பு உண்மை;
        }
    }
    திரும்பு பொய்;
}

வெறுமை LogTagManager::internal_applyNamePartConfigToMatchingTags(NamePartLookupResult& namePartResult)
{
    CV_Assert(namePartResult.m_findCrossReferences);
    மாறிலி auto& crossReferences = namePartResult.m_crossReferences;
    மாறிலி size_t matching_Full_Name_மொத்தம் = crossReferences.size();
    NamePartInfo& namePartInfo = *namePartResult.m_namePartInfoPtr;
    மாறிலி MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::Full);
    எனில் (scope == MatchingScope::None)
    {
        திரும்பு;
    }
    மாறிலி LogLevel level = namePartInfo.parsedLevel.level;
    க்கு (size_t k = 0u; k < matching_Full_Name_மொத்தம்; ++k)
    {
        மாறிலி auto& match = crossReferences.at(k);
        FullNameInfo& fullNameInfo = *match.m_fullNameInfo;
        LogTag* logTagPtr = fullNameInfo.logTagPtr;
        எனில் (!logTagPtr)
        {
            தொடர்;
        }
        எனில் (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            தொடர்;
        }
        மாறிலி size_t matchingPos = match.m_matchingPos;
        மாறிலி தருக்க isMatch = internal_isNamePartMatch(scope, matchingPos);
        எனில் (!isMatch)
        {
            தொடர்;
        }
        logTagPtr->level = level;
    }
}

வெறுமை LogTagManager::NameTable::addOrLookupFullName(FullNameLookupResult& result)
{
    மாறிலி auto fullNameIdAndFlag = internal_addOrLookupFullName(result.m_fullName);
    result.m_fullNameId = fullNameIdAndFlag.first;
    result.m_nameParts = LogTagManager::splitNameParts(result.m_fullName);
    internal_addOrLookupNameParts(result.m_nameParts, result.m_namePartIds);
    மாறிலி தருக்க isNew = fullNameIdAndFlag.second;
    எனில் (isNew)
    {
        internal_addCrossReference(result.m_fullNameId, result.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    result.m_fullNameInfoPtr = internal_getFullNameInfo(result.m_fullNameId);
    எனில் (result.m_findCrossReferences)
    {
        internal_findMatchingNamePartsForFullName(result);
    }
}

வெறுமை LogTagManager::NameTable::addOrLookupNamePart(NamePartLookupResult& result)
{
    result.m_namePartId = internal_addOrLookupNamePart(result.m_namePart);
    result.m_namePartInfoPtr = internal_getNamePartInfo(result.m_namePartId);
    எனில் (result.m_findCrossReferences)
    {
        internal_findMatchingFullNamesForNamePart(result);
    }
}

std::pair<size_t, தருக்க> LogTagManager::NameTable::internal_addOrLookupFullName(மாறிலி std::string& fullName)
{
    மாறிலி auto fullNameIdIter = m_fullNameIds.find(fullName);
    எனில் (fullNameIdIter != m_fullNameIds.end())
    {
        திரும்பு std::make_pair(fullNameIdIter->second, பொய்);
    }
    மாறிலி size_t fullNameId = m_fullNameInfos.size();
    m_fullNameInfos.emplace_back(FullNameInfo{});
    m_fullNameIds.emplace(fullName, fullNameId);
    திரும்பு std::make_pair(fullNameId, உண்மை);
}

வெறுமை LogTagManager::NameTable::internal_addOrLookupNameParts(மாறிலி std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    மாறிலி size_t name_Part_மொத்தம் = nameParts.size();
    namePartIds.resize(name_Part_மொத்தம், ~(size_t)0u);
    க்கு (size_t namePartIndex = 0u; namePartIndex < name_Part_மொத்தம்; ++namePartIndex)
    {
        மாறிலி std::string& namePart = nameParts.at(namePartIndex);
        மாறிலி size_t namePartId = internal_addOrLookupNamePart(namePart);
        namePartIds.at(namePartIndex) = namePartId;
    }
}

size_t LogTagManager::NameTable::internal_addOrLookupNamePart(மாறிலி std::string& namePart)
{
    மாறிலி auto namePartIter = m_namePartIds.find(namePart);
    எனில் (namePartIter != m_namePartIds.end())
    {
        திரும்பு namePartIter->second;
    }
    மாறிலி size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    திரும்பு namePartId;
}

வெறுமை LogTagManager::NameTable::internal_addCrossReference(size_t fullNameId, மாறிலி std::vector<size_t>& namePartIds)
{
    மாறிலி size_t name_Part_மொத்தம் = namePartIds.size();
    க்கு (size_t namePartPos = 0u; namePartPos < name_Part_மொத்தம்; ++namePartPos)
    {
        மாறிலி size_t namePartId = namePartIds.at(namePartPos);
        m_fullNameToNamePartIds.emplace(fullNameId, std::make_pair(namePartId, namePartPos));
        m_namePartToFullNameIds.emplace(namePartId, std::make_pair(fullNameId, namePartPos));
    }
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::getFullNameInfo(மாறிலி std::string& fullName)
{
    மாறிலி auto fullNameIdIter = m_fullNameIds.find(fullName);
    எனில் (fullNameIdIter == m_fullNameIds.end())
    {
        திரும்பு nullptr;
    }
    மாறிலி size_t fullNameId = fullNameIdIter->second;
    திரும்பு internal_getFullNameInfo(fullNameId);
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::internal_getFullNameInfo(size_t fullNameId)
{
    திரும்பு std::addressof(m_fullNameInfos.at(fullNameId));
}

LogTagManager::NamePartInfo* LogTagManager::NameTable::internal_getNamePartInfo(size_t namePartId)
{
    திரும்பு std::addressof(m_namePartInfos.at(namePartId));
}

வெறுமை LogTagManager::NameTable::internal_findMatchingNamePartsForFullName(FullNameLookupResult& fullNameResult)
{
    மாறிலி size_t fullNameId = fullNameResult.m_fullNameId;
    FullNameInfo* fullNameInfo = fullNameResult.m_fullNameInfoPtr;
    மாறிலி auto& namePartIds = fullNameResult.m_namePartIds;
    மாறிலி size_t name_Part_மொத்தம் = namePartIds.size();
    auto& crossReferences = fullNameResult.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_மொத்தம்);
    க்கு (size_t matchingPos = 0u; matchingPos < name_Part_மொத்தம்; ++matchingPos)
    {
        மாறிலி size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = internal_getNamePartInfo(namePartId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

வெறுமை LogTagManager::NameTable::internal_findMatchingFullNamesForNamePart(NamePartLookupResult& result)
{
    மாறிலி size_t namePartId = result.m_namePartId;
    NamePartInfo* namePartInfo = result.m_namePartInfoPtr;
    மாறிலி size_t matching_Full_Name_மொத்தம் = m_namePartToFullNameIds.மொத்தம்_2(namePartId);
    std::vector<CrossReference>& crossReferences = result.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_Full_Name_மொத்தம்);
    மாறிலி auto namePartToFullNameIterPair = m_namePartToFullNameIds.equal_range(result.m_namePartId);
    மாறிலி auto iterBegin = namePartToFullNameIterPair.first;
    மாறிலி auto iterEnd = namePartToFullNameIterPair.second;
    க்கு (auto iter = iterBegin; iter != iterEnd; ++iter)
    {
        மாறிலி size_t fullNameId = iter->second.first;
        மாறிலி size_t matchingPos = iter->second.second;
        FullNameInfo* fullNameInfo = internal_getFullNameInfo(fullNameId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

}}} //namespace
