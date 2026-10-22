// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "../precomp.hpp"
#अन्तर्भाव "logtagmanager.hpp"
#अन्तर्भाव "logtagconfigparser.hpp"

नामाकाश cv {
नामाकाश utils {
नामाकाश logging {


स्थिरांक अक्षर* LogTagManager::m_globalName = "global";


LogTagManager::LogTagManager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_globalLogTag(नवीन LogTag(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<LogTagConfigParser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_globalLogTag.get());
}

LogTagManager::~LogTagManager()
{
}

शून्य LogTagManager::setConfigString(स्थिरांक std::string& configString, तार्किक apply /*true*/)
{
    m_config->parse(configString);
    जर (m_config->hasMalformed())
    {
        परत;
    }
    जर (!apply)
    {
        परत;
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
    स्थिरांक स्वतः& globalConfig = m_config->getGlobalConfig();
    m_globalLogTag->level = globalConfig.level;
    साठी (स्थिरांक स्वतः& config : m_config->getFirstPartConfigs())
    {
        setLevelByFirstPart(config.namePart, config.level);
    }
    साठी (स्थिरांक स्वतः& config : m_config->getAnyPartConfigs())
    {
        setLevelByAnyPart(config.namePart, config.level);
    }
    साठी (स्थिरांक स्वतः& config : m_config->getFullNameConfigs())
    {
        setLevelByFullName(config.namePart, config.level);
    }
}

LogTagConfigParser& LogTagManager::getConfigParser() स्थिरांक
{
    परत *m_config;
}

शून्य LogTagManager::assign(स्थिरांक std::string& fullName, LogTag* ptr)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = सत्य;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    स्थिरांक तार्किक isPtrChanged = (fullNameInfo.logTagPtr != ptr);
    जर (!isPtrChanged)
    {
        परत;
    }
    fullNameInfo.logTagPtr = ptr;
    जर (!ptr)
    {
        परत;
    }
    स्थिरांक तार्किक hasAppliedFullNameConfig = internal_applyFullNameConfigToTag(fullNameInfo);
    जर (hasAppliedFullNameConfig)
    {
        परत;
    }
    internal_applyNamePartConfigToSpecificTag(result);
}

शून्य LogTagManager::unassign(स्थिरांक std::string& fullName)
{
    // Lock is inside assign() method.
    assign(fullName, nullptr);
}

LogTag* LogTagManager::get(स्थिरांक std::string& fullName)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameInfo* fullNameInfoPtr = m_nameTable.getFullNameInfo(fullName);
    जर (fullNameInfoPtr && fullNameInfoPtr->logTagPtr)
    {
        परत fullNameInfoPtr->logTagPtr;
    }
    परत nullptr;
}

शून्य LogTagManager::setLevelByFullName(स्थिरांक std::string& fullName, LogLevel level)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameLookupResult result(fullName);
    result.m_findCrossReferences = असत्य;
    m_nameTable.addOrLookupFullName(result);
    FullNameInfo& fullNameInfo = *result.m_fullNameInfoPtr;
    जर (fullNameInfo.parsedLevel.scope == MatchingScope::Full &&
        fullNameInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        परत;
    }
    // update the cached configured value.
    fullNameInfo.parsedLevel.scope = MatchingScope::Full;
    fullNameInfo.parsedLevel.level = level;
    // update the actual tag, if already registered.
    LogTag* logTagPtr = fullNameInfo.logTagPtr;
    जर (logTagPtr)
    {
        logTagPtr->level = level;
    }
}

शून्य LogTagManager::setLevelByFirstPart(स्थिरांक std::string& firstPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(firstPart, level, MatchingScope::FirstNamePart);
}

शून्य LogTagManager::setLevelByAnyPart(स्थिरांक std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(anyPart, level, MatchingScope::AnyNamePart);
}

शून्य LogTagManager::setLevelByNamePart(स्थिरांक std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    NamePartLookupResult result(namePart);
    result.m_findCrossReferences = सत्य;
    m_nameTable.addOrLookupNamePart(result);
    NamePartInfo& namePartInfo = *result.m_namePartInfoPtr;
    जर (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        परत;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    internal_applyNamePartConfigToMatchingTags(result);
}

std::vector<std::string> LogTagManager::splitNameParts(स्थिरांक std::string& fullName)
{
    स्थिरांक माप_प्रकार npos = std::string::npos;
    स्थिरांक माप_प्रकार len = fullName.length();
    std::vector<std::string> nameParts;
    माप_प्रकार start = 0u;
    पर्यंत (start < len)
    {
        माप_प्रकार nextPeriod = fullName.find('.', start);
        जर (nextPeriod == npos)
        {
            nextPeriod = len;
        }
        जर (nextPeriod >= start + 1u)
        {
            nameParts.emplace_back(fullName.substr(start, nextPeriod - start));
        }
        start = nextPeriod + 1u;
    }
    परत nameParts;
}

तार्किक LogTagManager::internal_isNamePartMatch(MatchingScope scope, माप_प्रकार matchingPos)
{
    निवड (scope)
    {
    स्थिती MatchingScope::FirstNamePart:
        परत (matchingPos == 0u);
    स्थिती MatchingScope::AnyNamePart:
        परत सत्य;
    स्थिती MatchingScope::None:
    स्थिती MatchingScope::Full:
    पूर्वनिर्धारित:
        परत असत्य;
    }
}

तार्किक LogTagManager::internal_applyFullNameConfigToTag(FullNameInfo& fullNameInfo)
{
    जर (!fullNameInfo.logTagPtr)
    {
        परत असत्य;
    }
    जर (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
    {
        fullNameInfo.logTagPtr->level = fullNameInfo.parsedLevel.level;
        परत सत्य;
    }
    परत असत्य;
}

तार्किक LogTagManager::internal_applyNamePartConfigToSpecificTag(FullNameLookupResult& fullNameResult)
{
    स्थिरांक FullNameInfo& fullNameInfo = *fullNameResult.m_fullNameInfoPtr;
    LogTag* स्थिरांक logTag = fullNameInfo.logTagPtr;
    जर (!logTag)
    {
        परत असत्य;
    }
    CV_Assert(fullNameResult.m_findCrossReferences);
    स्थिरांक स्वतः& crossReferences = fullNameResult.m_crossReferences;
    स्थिरांक माप_प्रकार matching_Name_Part_मोजणी = crossReferences.size();
    साठी (माप_प्रकार k = 0u; k < matching_Name_Part_मोजणी; ++k)
    {
        स्थिरांक स्वतः& match = crossReferences.at(k);
        स्थिरांक स्वतः& namePartInfo = *match.m_namePartInfo;
        स्थिरांक स्वतः& parsedLevel = namePartInfo.parsedLevel;
        स्थिरांक स्वतः scope = parsedLevel.scope;
        स्थिरांक LogLevel level = parsedLevel.level;
        स्थिरांक माप_प्रकार matchingPos = match.m_matchingPos;
        स्थिरांक तार्किक isMatch = internal_isNamePartMatch(scope, matchingPos);
        जर (isMatch)
        {
            logTag->level = level;
            परत सत्य;
        }
    }
    परत असत्य;
}

शून्य LogTagManager::internal_applyNamePartConfigToMatchingTags(NamePartLookupResult& namePartResult)
{
    CV_Assert(namePartResult.m_findCrossReferences);
    स्थिरांक स्वतः& crossReferences = namePartResult.m_crossReferences;
    स्थिरांक माप_प्रकार matching_Full_Name_मोजणी = crossReferences.size();
    NamePartInfo& namePartInfo = *namePartResult.m_namePartInfoPtr;
    स्थिरांक MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::Full);
    जर (scope == MatchingScope::None)
    {
        परत;
    }
    स्थिरांक LogLevel level = namePartInfo.parsedLevel.level;
    साठी (माप_प्रकार k = 0u; k < matching_Full_Name_मोजणी; ++k)
    {
        स्थिरांक स्वतः& match = crossReferences.at(k);
        FullNameInfo& fullNameInfo = *match.m_fullNameInfo;
        LogTag* logTagPtr = fullNameInfo.logTagPtr;
        जर (!logTagPtr)
        {
            सुरूठेवा;
        }
        जर (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            सुरूठेवा;
        }
        स्थिरांक माप_प्रकार matchingPos = match.m_matchingPos;
        स्थिरांक तार्किक isMatch = internal_isNamePartMatch(scope, matchingPos);
        जर (!isMatch)
        {
            सुरूठेवा;
        }
        logTagPtr->level = level;
    }
}

शून्य LogTagManager::NameTable::addOrLookupFullName(FullNameLookupResult& result)
{
    स्थिरांक स्वतः fullNameIdAndFlag = internal_addOrLookupFullName(result.m_fullName);
    result.m_fullNameId = fullNameIdAndFlag.first;
    result.m_nameParts = LogTagManager::splitNameParts(result.m_fullName);
    internal_addOrLookupNameParts(result.m_nameParts, result.m_namePartIds);
    स्थिरांक तार्किक isNew = fullNameIdAndFlag.second;
    जर (isNew)
    {
        internal_addCrossReference(result.m_fullNameId, result.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    result.m_fullNameInfoPtr = internal_getFullNameInfo(result.m_fullNameId);
    जर (result.m_findCrossReferences)
    {
        internal_findMatchingNamePartsForFullName(result);
    }
}

शून्य LogTagManager::NameTable::addOrLookupNamePart(NamePartLookupResult& result)
{
    result.m_namePartId = internal_addOrLookupNamePart(result.m_namePart);
    result.m_namePartInfoPtr = internal_getNamePartInfo(result.m_namePartId);
    जर (result.m_findCrossReferences)
    {
        internal_findMatchingFullNamesForNamePart(result);
    }
}

std::pair<माप_प्रकार, तार्किक> LogTagManager::NameTable::internal_addOrLookupFullName(स्थिरांक std::string& fullName)
{
    स्थिरांक स्वतः fullNameIdIter = m_fullNameIds.find(fullName);
    जर (fullNameIdIter != m_fullNameIds.end())
    {
        परत std::make_pair(fullNameIdIter->second, असत्य);
    }
    स्थिरांक माप_प्रकार fullNameId = m_fullNameInfos.size();
    m_fullNameInfos.emplace_back(FullNameInfo{});
    m_fullNameIds.emplace(fullName, fullNameId);
    परत std::make_pair(fullNameId, सत्य);
}

शून्य LogTagManager::NameTable::internal_addOrLookupNameParts(स्थिरांक std::vector<std::string>& nameParts,
    std::vector<माप_प्रकार>& namePartIds)
{
    स्थिरांक माप_प्रकार name_Part_मोजणी = nameParts.size();
    namePartIds.resize(name_Part_मोजणी, ~(माप_प्रकार)0u);
    साठी (माप_प्रकार namePartIndex = 0u; namePartIndex < name_Part_मोजणी; ++namePartIndex)
    {
        स्थिरांक std::string& namePart = nameParts.at(namePartIndex);
        स्थिरांक माप_प्रकार namePartId = internal_addOrLookupNamePart(namePart);
        namePartIds.at(namePartIndex) = namePartId;
    }
}

माप_प्रकार LogTagManager::NameTable::internal_addOrLookupNamePart(स्थिरांक std::string& namePart)
{
    स्थिरांक स्वतः namePartIter = m_namePartIds.find(namePart);
    जर (namePartIter != m_namePartIds.end())
    {
        परत namePartIter->second;
    }
    स्थिरांक माप_प्रकार namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    परत namePartId;
}

शून्य LogTagManager::NameTable::internal_addCrossReference(माप_प्रकार fullNameId, स्थिरांक std::vector<माप_प्रकार>& namePartIds)
{
    स्थिरांक माप_प्रकार name_Part_मोजणी = namePartIds.size();
    साठी (माप_प्रकार namePartPos = 0u; namePartPos < name_Part_मोजणी; ++namePartPos)
    {
        स्थिरांक माप_प्रकार namePartId = namePartIds.at(namePartPos);
        m_fullNameToNamePartIds.emplace(fullNameId, std::make_pair(namePartId, namePartPos));
        m_namePartToFullNameIds.emplace(namePartId, std::make_pair(fullNameId, namePartPos));
    }
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::getFullNameInfo(स्थिरांक std::string& fullName)
{
    स्थिरांक स्वतः fullNameIdIter = m_fullNameIds.find(fullName);
    जर (fullNameIdIter == m_fullNameIds.end())
    {
        परत nullptr;
    }
    स्थिरांक माप_प्रकार fullNameId = fullNameIdIter->second;
    परत internal_getFullNameInfo(fullNameId);
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::internal_getFullNameInfo(माप_प्रकार fullNameId)
{
    परत std::addressof(m_fullNameInfos.at(fullNameId));
}

LogTagManager::NamePartInfo* LogTagManager::NameTable::internal_getNamePartInfo(माप_प्रकार namePartId)
{
    परत std::addressof(m_namePartInfos.at(namePartId));
}

शून्य LogTagManager::NameTable::internal_findMatchingNamePartsForFullName(FullNameLookupResult& fullNameResult)
{
    स्थिरांक माप_प्रकार fullNameId = fullNameResult.m_fullNameId;
    FullNameInfo* fullNameInfo = fullNameResult.m_fullNameInfoPtr;
    स्थिरांक स्वतः& namePartIds = fullNameResult.m_namePartIds;
    स्थिरांक माप_प्रकार name_Part_मोजणी = namePartIds.size();
    स्वतः& crossReferences = fullNameResult.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_मोजणी);
    साठी (माप_प्रकार matchingPos = 0u; matchingPos < name_Part_मोजणी; ++matchingPos)
    {
        स्थिरांक माप_प्रकार namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = internal_getNamePartInfo(namePartId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

शून्य LogTagManager::NameTable::internal_findMatchingFullNamesForNamePart(NamePartLookupResult& result)
{
    स्थिरांक माप_प्रकार namePartId = result.m_namePartId;
    NamePartInfo* namePartInfo = result.m_namePartInfoPtr;
    स्थिरांक माप_प्रकार matching_Full_Name_मोजणी = m_namePartToFullNameIds.मोजणी_2(namePartId);
    std::vector<CrossReference>& crossReferences = result.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_Full_Name_मोजणी);
    स्थिरांक स्वतः namePartToFullNameIterPair = m_namePartToFullNameIds.equal_range(result.m_namePartId);
    स्थिरांक स्वतः iterBegin = namePartToFullNameIterPair.first;
    स्थिरांक स्वतः iterEnd = namePartToFullNameIterPair.second;
    साठी (स्वतः iter = iterBegin; iter != iterEnd; ++iter)
    {
        स्थिरांक माप_प्रकार fullNameId = iter->second.first;
        स्थिरांक माप_प्रकार matchingPos = iter->second.second;
        FullNameInfo* fullNameInfo = internal_getFullNameInfo(fullNameId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

}}} //namespace
