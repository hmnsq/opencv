// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "../precomp.hpp"
#अन्तर्भाव "logtagmanager.hpp"
#अन्तर्भाव "logtagconfigparser.hpp"

नामाकाश cv {
नामाकाश utils {
नामाकाश logging {


अचर वर्ण* LogTagManager::m_globalName = "global";


LogTagManager::LogTagManager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_globalLogTag(नूतन LogTag(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<LogTagConfigParser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_globalLogTag.get());
}

LogTagManager::~LogTagManager()
{
}

शून्य LogTagManager::setConfigString(अचर std::string& configString, तार्किक apply /*true*/)
{
    m_config->parse(configString);
    यदि (m_config->hasMalformed())
    {
        फर्काऊ;
    }
    यदि (!apply)
    {
        फर्काऊ;
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
    अचर स्वतः& globalConfig = m_config->getGlobalConfig();
    m_globalLogTag->level = globalConfig.level;
    लागि (अचर स्वतः& config : m_config->getFirstPartConfigs())
    {
        setLevelByFirstPart(config.namePart, config.level);
    }
    लागि (अचर स्वतः& config : m_config->getAnyPartConfigs())
    {
        setLevelByAnyPart(config.namePart, config.level);
    }
    लागि (अचर स्वतः& config : m_config->getFullNameConfigs())
    {
        setLevelByFullName(config.namePart, config.level);
    }
}

LogTagConfigParser& LogTagManager::getConfigParser() अचर
{
    फर्काऊ *m_config;
}

शून्य LogTagManager::assign(अचर std::string& fullName, LogTag* ptr)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = सत्य;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    अचर तार्किक isPtrChanged = (fullNameInfo.logTagPtr != ptr);
    यदि (!isPtrChanged)
    {
        फर्काऊ;
    }
    fullNameInfo.logTagPtr = ptr;
    यदि (!ptr)
    {
        फर्काऊ;
    }
    अचर तार्किक hasAppliedFullNameConfig = internal_applyFullNameConfigToTag(fullNameInfo);
    यदि (hasAppliedFullNameConfig)
    {
        फर्काऊ;
    }
    internal_applyNamePartConfigToSpecificTag(result);
}

शून्य LogTagManager::unassign(अचर std::string& fullName)
{
    // Lock is inside assign() method.
    assign(fullName, nullptr);
}

LogTag* LogTagManager::get(अचर std::string& fullName)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameInfo* fullNameInfoPtr = m_nameTable.getFullNameInfo(fullName);
    यदि (fullNameInfoPtr && fullNameInfoPtr->logTagPtr)
    {
        फर्काऊ fullNameInfoPtr->logTagPtr;
    }
    फर्काऊ nullptr;
}

शून्य LogTagManager::setLevelByFullName(अचर std::string& fullName, LogLevel level)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = असत्य;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    यदि (fullNameInfo.parsedLevel.scope == MatchingScope::Full &&
        fullNameInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        फर्काऊ;
    }
    // update the cached configured value.
    fullNameInfo.parsedLevel.scope = MatchingScope::Full;
    fullNameInfo.parsedLevel.level = level;
    // update the actual tag, if already registered.
    LogTag* logTagPtr = fullNameInfo.logTagPtr;
    यदि (logTagPtr)
    {
        logTagPtr->level = level;
    }
}

शून्य LogTagManager::setLevelByFirstPart(अचर std::string& firstPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(firstPart, level, MatchingScope::FirstNamePart);
}

शून्य LogTagManager::setLevelByAnyPart(अचर std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(anyPart, level, MatchingScope::AnyNamePart);
}

शून्य LogTagManager::setLevelByNamePart(अचर std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    NamePartLookupResult result(namePart);
    result.m_findCrossReferences = सत्य;
    m_nameTable.addOrLookupNamePart(result);
    NamePartInfo& namePartInfo = *result.m_namePartInfoPtr;
    यदि (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        फर्काऊ;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    internal_applyNamePartConfigToMatchingTags(result);
}

std::vector<std::string> LogTagManager::splitNameParts(अचर std::string& fullName)
{
    अचर माप_प्रकार npos = std::string::npos;
    अचर माप_प्रकार len = fullName.length();
    std::vector<std::string> nameParts;
    माप_प्रकार start = 0u;
    जबसम्म (start < len)
    {
        माप_प्रकार nextPeriod = fullName.find('.', start);
        यदि (nextPeriod == npos)
        {
            nextPeriod = len;
        }
        यदि (nextPeriod >= start + 1u)
        {
            nameParts.emplace_back(fullName.substr(start, nextPeriod - start));
        }
        start = nextPeriod + 1u;
    }
    फर्काऊ nameParts;
}

तार्किक LogTagManager::internal_isNamePartMatch(MatchingScope scope, माप_प्रकार matchingPos)
{
    विकल्प (scope)
    {
    पक्ष MatchingScope::FirstNamePart:
        फर्काऊ (matchingPos == 0u);
    पक्ष MatchingScope::AnyNamePart:
        फर्काऊ सत्य;
    पक्ष MatchingScope::None:
    पक्ष MatchingScope::Full:
    स्वभाव:
        फर्काऊ असत्य;
    }
}

तार्किक LogTagManager::internal_applyFullNameConfigToTag(FullNameInfo& fullNameInfo)
{
    यदि (!fullNameInfo.logTagPtr)
    {
        फर्काऊ असत्य;
    }
    यदि (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
    {
        fullNameInfo.logTagPtr->level = fullNameInfo.parsedLevel.level;
        फर्काऊ सत्य;
    }
    फर्काऊ असत्य;
}

तार्किक LogTagManager::internal_applyNamePartConfigToSpecificTag(FullNameLookupResult& fullNameResult)
{
    अचर FullNameInfo& fullNameInfo = *fullNameResult.m_fullNameInfoPtr;
    LogTag* अचर logTag = fullNameInfo.logTagPtr;
    यदि (!logTag)
    {
        फर्काऊ असत्य;
    }
    CV_Assert(fullNameResult.m_findCrossReferences);
    अचर स्वतः& crossReferences = fullNameResult.m_crossReferences;
    अचर माप_प्रकार matching_Name_Part_गन्ती = crossReferences.size();
    लागि (माप_प्रकार k = 0u; k < matching_Name_Part_गन्ती; ++k)
    {
        अचर स्वतः& match = crossReferences.at(k);
        अचर स्वतः& namePartInfo = *match.m_namePartInfo;
        अचर स्वतः& parsedLevel = namePartInfo.parsedLevel;
        अचर स्वतः scope = parsedLevel.scope;
        अचर LogLevel level = parsedLevel.level;
        अचर माप_प्रकार matchingPos = match.m_matchingPos;
        अचर तार्किक isMatch = internal_isNamePartMatch(scope, matchingPos);
        यदि (isMatch)
        {
            logTag->level = level;
            फर्काऊ सत्य;
        }
    }
    फर्काऊ असत्य;
}

शून्य LogTagManager::internal_applyNamePartConfigToMatchingTags(NamePartLookupResult& namePartResult)
{
    CV_Assert(namePartResult.m_findCrossReferences);
    अचर स्वतः& crossReferences = namePartResult.m_crossReferences;
    अचर माप_प्रकार matching_Full_Name_गन्ती = crossReferences.size();
    NamePartInfo& namePartInfo = *namePartResult.m_namePartInfoPtr;
    अचर MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::Full);
    यदि (scope == MatchingScope::None)
    {
        फर्काऊ;
    }
    अचर LogLevel level = namePartInfo.parsedLevel.level;
    लागि (माप_प्रकार k = 0u; k < matching_Full_Name_गन्ती; ++k)
    {
        अचर स्वतः& match = crossReferences.at(k);
        FullNameInfo& fullNameInfo = *match.m_fullNameInfo;
        LogTag* logTagPtr = fullNameInfo.logTagPtr;
        यदि (!logTagPtr)
        {
            जारी;
        }
        यदि (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            जारी;
        }
        अचर माप_प्रकार matchingPos = match.m_matchingPos;
        अचर तार्किक isMatch = internal_isNamePartMatch(scope, matchingPos);
        यदि (!isMatch)
        {
            जारी;
        }
        logTagPtr->level = level;
    }
}

शून्य LogTagManager::NameTable::addOrLookupFullName(FullNameLookupResult& result)
{
    अचर स्वतः fullNameIdAndFlag = internal_addOrLookupFullName(result.m_fullName);
    result.m_fullNameId = fullNameIdAndFlag.first;
    result.m_nameParts = LogTagManager::splitNameParts(result.m_fullName);
    internal_addOrLookupNameParts(result.m_nameParts, result.m_namePartIds);
    अचर तार्किक isNew = fullNameIdAndFlag.second;
    यदि (isNew)
    {
        internal_addCrossReference(result.m_fullNameId, result.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    result.m_fullNameInfoPtr = internal_getFullNameInfo(result.m_fullNameId);
    यदि (result.m_findCrossReferences)
    {
        internal_findMatchingNamePartsForFullName(result);
    }
}

शून्य LogTagManager::NameTable::addOrLookupNamePart(NamePartLookupResult& result)
{
    result.m_namePartId = internal_addOrLookupNamePart(result.m_namePart);
    result.m_namePartInfoPtr = internal_getNamePartInfo(result.m_namePartId);
    यदि (result.m_findCrossReferences)
    {
        internal_findMatchingFullNamesForNamePart(result);
    }
}

std::pair<माप_प्रकार, तार्किक> LogTagManager::NameTable::internal_addOrLookupFullName(अचर std::string& fullName)
{
    अचर स्वतः fullNameIdIter = m_fullNameIds.find(fullName);
    यदि (fullNameIdIter != m_fullNameIds.end())
    {
        फर्काऊ std::make_pair(fullNameIdIter->second, असत्य);
    }
    अचर माप_प्रकार fullNameId = m_fullNameInfos.size();
    m_fullNameInfos.emplace_back(FullNameInfo{});
    m_fullNameIds.emplace(fullName, fullNameId);
    फर्काऊ std::make_pair(fullNameId, सत्य);
}

शून्य LogTagManager::NameTable::internal_addOrLookupNameParts(अचर std::vector<std::string>& nameParts,
    std::vector<माप_प्रकार>& namePartIds)
{
    अचर माप_प्रकार name_Part_गन्ती = nameParts.size();
    namePartIds.resize(name_Part_गन्ती, ~(माप_प्रकार)0u);
    लागि (माप_प्रकार namePartIndex = 0u; namePartIndex < name_Part_गन्ती; ++namePartIndex)
    {
        अचर std::string& namePart = nameParts.at(namePartIndex);
        अचर माप_प्रकार namePartId = internal_addOrLookupNamePart(namePart);
        namePartIds.at(namePartIndex) = namePartId;
    }
}

माप_प्रकार LogTagManager::NameTable::internal_addOrLookupNamePart(अचर std::string& namePart)
{
    अचर स्वतः namePartIter = m_namePartIds.find(namePart);
    यदि (namePartIter != m_namePartIds.end())
    {
        फर्काऊ namePartIter->second;
    }
    अचर माप_प्रकार namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    फर्काऊ namePartId;
}

शून्य LogTagManager::NameTable::internal_addCrossReference(माप_प्रकार fullNameId, अचर std::vector<माप_प्रकार>& namePartIds)
{
    अचर माप_प्रकार name_Part_गन्ती = namePartIds.size();
    लागि (माप_प्रकार namePartPos = 0u; namePartPos < name_Part_गन्ती; ++namePartPos)
    {
        अचर माप_प्रकार namePartId = namePartIds.at(namePartPos);
        m_fullNameToNamePartIds.emplace(fullNameId, std::make_pair(namePartId, namePartPos));
        m_namePartToFullNameIds.emplace(namePartId, std::make_pair(fullNameId, namePartPos));
    }
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::getFullNameInfo(अचर std::string& fullName)
{
    अचर स्वतः fullNameIdIter = m_fullNameIds.find(fullName);
    यदि (fullNameIdIter == m_fullNameIds.end())
    {
        फर्काऊ nullptr;
    }
    अचर माप_प्रकार fullNameId = fullNameIdIter->second;
    फर्काऊ internal_getFullNameInfo(fullNameId);
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::internal_getFullNameInfo(माप_प्रकार fullNameId)
{
    फर्काऊ std::addressof(m_fullNameInfos.at(fullNameId));
}

LogTagManager::NamePartInfo* LogTagManager::NameTable::internal_getNamePartInfo(माप_प्रकार namePartId)
{
    फर्काऊ std::addressof(m_namePartInfos.at(namePartId));
}

शून्य LogTagManager::NameTable::internal_findMatchingNamePartsForFullName(FullNameLookupResult& fullNameResult)
{
    अचर माप_प्रकार fullNameId = fullNameResult.m_fullNameId;
    FullNameInfo* fullNameInfo = fullNameResult.m_fullNameInfoPtr;
    अचर स्वतः& namePartIds = fullNameResult.m_namePartIds;
    अचर माप_प्रकार name_Part_गन्ती = namePartIds.size();
    स्वतः& crossReferences = fullNameResult.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_गन्ती);
    लागि (माप_प्रकार matchingPos = 0u; matchingPos < name_Part_गन्ती; ++matchingPos)
    {
        अचर माप_प्रकार namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = internal_getNamePartInfo(namePartId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

शून्य LogTagManager::NameTable::internal_findMatchingFullNamesForNamePart(NamePartLookupResult& result)
{
    अचर माप_प्रकार namePartId = result.m_namePartId;
    NamePartInfo* namePartInfo = result.m_namePartInfoPtr;
    अचर माप_प्रकार matching_Full_Name_गन्ती = m_namePartToFullNameIds.गन्ती_2(namePartId);
    std::vector<CrossReference>& crossReferences = result.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_Full_Name_गन्ती);
    अचर स्वतः namePartToFullNameIterPair = m_namePartToFullNameIds.equal_range(result.m_namePartId);
    अचर स्वतः iterBegin = namePartToFullNameIterPair.first;
    अचर स्वतः iterEnd = namePartToFullNameIterPair.second;
    लागि (स्वतः iter = iterBegin; iter != iterEnd; ++iter)
    {
        अचर माप_प्रकार fullNameId = iter->second.first;
        अचर माप_प्रकार matchingPos = iter->second.second;
        FullNameInfo* fullNameInfo = internal_getFullNameInfo(fullNameId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

}}} //namespace
