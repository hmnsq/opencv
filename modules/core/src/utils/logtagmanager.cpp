// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "../precomp.hpp"
#समावेश "logtagmanager.hpp"
#समावेश "logtagconfigparser.hpp"

namespace cv {
namespace utils {
namespace logging {


स्थिर अक्षर* LogTagManager::m_globalName = "global";


LogTagManager::LogTagManager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_globalLogTag(नया LogTag(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<LogTagConfigParser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_globalLogTag.लो());
}

LogTagManager::~LogTagManager()
{
}

व्योम LogTagManager::setConfigString(स्थिर std::string& configString, bool apply /*true*/)
{
    m_config->parse(configString);
    अगर (m_config->hasMalformed())
    {
        वापस;
    }
    अगर (!apply)
    {
        वापस;
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
    स्थिर स्वतः& globalConfig = m_config->getGlobalConfig();
    m_globalLogTag->level = globalConfig.level;
    क्रम (स्थिर स्वतः& config : m_config->getFirstPartConfigs())
    {
        setLevelByFirstPart(config.namePart, config.level);
    }
    क्रम (स्थिर स्वतः& config : m_config->getAnyPartConfigs())
    {
        setLevelByAnyPart(config.namePart, config.level);
    }
    क्रम (स्थिर स्वतः& config : m_config->getFullNameConfigs())
    {
        setLevelByFullName(config.namePart, config.level);
    }
}

LogTagConfigParser& LogTagManager::getConfigParser() स्थिर
{
    वापस *m_config;
}

व्योम LogTagManager::assign(स्थिर std::string& fullName, LogTag* ptr)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    Full_Name_Lookup_परिणाम परिणाम(fullName);
    परिणाम.m_findCrossReferences = true;
    m_nameTable.addOrLookupFullName(परिणाम);
    FullNameInfo& fullNameInfo = *परिणाम.m_fullNameInfoPtr;
    स्थिर bool isPtrChanged = (fullNameInfo.logTagPtr != ptr);
    अगर (!isPtrChanged)
    {
        वापस;
    }
    fullNameInfo.logTagPtr = ptr;
    अगर (!ptr)
    {
        वापस;
    }
    स्थिर bool hasAppliedFullNameConfig = internal_applyFullNameConfigToTag(fullNameInfo);
    अगर (hasAppliedFullNameConfig)
    {
        वापस;
    }
    internal_applyNamePartConfigToSpecificTag(परिणाम);
}

व्योम LogTagManager::unassign(स्थिर std::string& fullName)
{
    // Lock is inside assign() method.
    assign(fullName, nullptr);
}

LogTag* LogTagManager::लो(स्थिर std::string& fullName)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    FullNameInfo* fullNameInfoPtr = m_nameTable.getFullNameInfo(fullName);
    अगर (fullNameInfoPtr && fullNameInfoPtr->logTagPtr)
    {
        वापस fullNameInfoPtr->logTagPtr;
    }
    वापस nullptr;
}

व्योम LogTagManager::setLevelByFullName(स्थिर std::string& fullName, LogLevel level)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    Full_Name_Lookup_परिणाम परिणाम(fullName);
    परिणाम.m_findCrossReferences = false;
    m_nameTable.addOrLookupFullName(परिणाम);
    FullNameInfo& fullNameInfo = *परिणाम.m_fullNameInfoPtr;
    अगर (fullNameInfo.parsedLevel.scope == MatchingScope::Full &&
        fullNameInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        वापस;
    }
    // update the cached configured value.
    fullNameInfo.parsedLevel.scope = MatchingScope::Full;
    fullNameInfo.parsedLevel.level = level;
    // update the actual tag, if already registered.
    LogTag* logTagPtr = fullNameInfo.logTagPtr;
    अगर (logTagPtr)
    {
        logTagPtr->level = level;
    }
}

व्योम LogTagManager::setLevelByFirstPart(स्थिर std::string& firstPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(firstPart, level, MatchingScope::FirstNamePart);
}

व्योम LogTagManager::setLevelByAnyPart(स्थिर std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    setLevelByNamePart(anyPart, level, MatchingScope::AnyNamePart);
}

व्योम LogTagManager::setLevelByNamePart(स्थिर std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    LockType lock(m_mutex);
    Name_Part_Lookup_परिणाम परिणाम(namePart);
    परिणाम.m_findCrossReferences = true;
    m_nameTable.addOrLookupNamePart(परिणाम);
    NamePartInfo& namePartInfo = *परिणाम.m_namePartInfoPtr;
    अगर (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        वापस;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    internal_applyNamePartConfigToMatchingTags(परिणाम);
}

std::vector<std::string> LogTagManager::splitNameParts(स्थिर std::string& fullName)
{
    स्थिर माप_प्रकार npos = std::string::npos;
    स्थिर माप_प्रकार len = fullName.length();
    std::vector<std::string> nameParts;
    माप_प्रकार start = 0u;
    जबतक (start < len)
    {
        माप_प्रकार अग्र_Period = fullName.find('.', start);
        अगर (अग्र_Period == npos)
        {
            अग्र_Period = len;
        }
        अगर (अग्र_Period >= start + 1u)
        {
            nameParts.emplace_back(fullName.substr(start, अग्र_Period - start));
        }
        start = अग्र_Period + 1u;
    }
    वापस nameParts;
}

bool LogTagManager::internal_isNamePartMatch(MatchingScope scope, माप_प्रकार matchingPos)
{
    चयन (scope)
    {
    हाल MatchingScope::FirstNamePart:
        वापस (matchingPos == 0u);
    हाल MatchingScope::AnyNamePart:
        वापस true;
    हाल MatchingScope::None:
    हाल MatchingScope::Full:
    शेष:
        वापस false;
    }
}

bool LogTagManager::internal_applyFullNameConfigToTag(FullNameInfo& fullNameInfo)
{
    अगर (!fullNameInfo.logTagPtr)
    {
        वापस false;
    }
    अगर (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
    {
        fullNameInfo.logTagPtr->level = fullNameInfo.parsedLevel.level;
        वापस true;
    }
    वापस false;
}

bool LogTagManager::internal_applyNamePartConfigToSpecificTag(Full_Name_Lookup_परिणाम& full_Name_परिणाम)
{
    स्थिर FullNameInfo& fullNameInfo = *full_Name_परिणाम.m_fullNameInfoPtr;
    LogTag* स्थिर logTag = fullNameInfo.logTagPtr;
    अगर (!logTag)
    {
        वापस false;
    }
    CV_Assert(full_Name_परिणाम.m_findCrossReferences);
    स्थिर स्वतः& crossReferences = full_Name_परिणाम.m_crossReferences;
    स्थिर माप_प्रकार matching_Name_Part_गिनती = crossReferences.size();
    क्रम (माप_प्रकार k = 0u; k < matching_Name_Part_गिनती; ++k)
    {
        स्थिर स्वतः& match = crossReferences.at(k);
        स्थिर स्वतः& namePartInfo = *match.m_namePartInfo;
        स्थिर स्वतः& parsedLevel = namePartInfo.parsedLevel;
        स्थिर स्वतः scope = parsedLevel.scope;
        स्थिर LogLevel level = parsedLevel.level;
        स्थिर माप_प्रकार matchingPos = match.m_matchingPos;
        स्थिर bool isMatch = internal_isNamePartMatch(scope, matchingPos);
        अगर (isMatch)
        {
            logTag->level = level;
            वापस true;
        }
    }
    वापस false;
}

व्योम LogTagManager::internal_applyNamePartConfigToMatchingTags(Name_Part_Lookup_परिणाम& name_Part_परिणाम)
{
    CV_Assert(name_Part_परिणाम.m_findCrossReferences);
    स्थिर स्वतः& crossReferences = name_Part_परिणाम.m_crossReferences;
    स्थिर माप_प्रकार matching_Full_Name_गिनती = crossReferences.size();
    NamePartInfo& namePartInfo = *name_Part_परिणाम.m_namePartInfoPtr;
    स्थिर MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::Full);
    अगर (scope == MatchingScope::None)
    {
        वापस;
    }
    स्थिर LogLevel level = namePartInfo.parsedLevel.level;
    क्रम (माप_प्रकार k = 0u; k < matching_Full_Name_गिनती; ++k)
    {
        स्थिर स्वतः& match = crossReferences.at(k);
        FullNameInfo& fullNameInfo = *match.m_fullNameInfo;
        LogTag* logTagPtr = fullNameInfo.logTagPtr;
        अगर (!logTagPtr)
        {
            जारी;
        }
        अगर (fullNameInfo.parsedLevel.scope == MatchingScope::Full)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            जारी;
        }
        स्थिर माप_प्रकार matchingPos = match.m_matchingPos;
        स्थिर bool isMatch = internal_isNamePartMatch(scope, matchingPos);
        अगर (!isMatch)
        {
            जारी;
        }
        logTagPtr->level = level;
    }
}

व्योम LogTagManager::NameTable::addOrLookupFullName(Full_Name_Lookup_परिणाम& परिणाम)
{
    स्थिर स्वतः fullNameIdAndFlag = internal_addOrLookupFullName(परिणाम.m_fullName);
    परिणाम.m_fullNameId = fullNameIdAndFlag.first;
    परिणाम.m_nameParts = LogTagManager::splitNameParts(परिणाम.m_fullName);
    internal_addOrLookupNameParts(परिणाम.m_nameParts, परिणाम.m_namePartIds);
    स्थिर bool isNew = fullNameIdAndFlag.second;
    अगर (isNew)
    {
        internal_addCrossReference(परिणाम.m_fullNameId, परिणाम.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    परिणाम.m_fullNameInfoPtr = internal_getFullNameInfo(परिणाम.m_fullNameId);
    अगर (परिणाम.m_findCrossReferences)
    {
        internal_findMatchingNamePartsForFullName(परिणाम);
    }
}

व्योम LogTagManager::NameTable::addOrLookupNamePart(Name_Part_Lookup_परिणाम& परिणाम)
{
    परिणाम.m_namePartId = internal_addOrLookupNamePart(परिणाम.m_namePart);
    परिणाम.m_namePartInfoPtr = internal_getNamePartInfo(परिणाम.m_namePartId);
    अगर (परिणाम.m_findCrossReferences)
    {
        internal_findMatchingFullNamesForNamePart(परिणाम);
    }
}

std::pair<माप_प्रकार, bool> LogTagManager::NameTable::internal_addOrLookupFullName(स्थिर std::string& fullName)
{
    स्थिर स्वतः fullNameIdIter = m_fullNameIds.find(fullName);
    अगर (fullNameIdIter != m_fullNameIds.end())
    {
        वापस std::make_pair(fullNameIdIter->second, false);
    }
    स्थिर माप_प्रकार fullNameId = m_fullNameInfos.size();
    m_fullNameInfos.emplace_back(FullNameInfo{});
    m_fullNameIds.emplace(fullName, fullNameId);
    वापस std::make_pair(fullNameId, true);
}

व्योम LogTagManager::NameTable::internal_addOrLookupNameParts(स्थिर std::vector<std::string>& nameParts,
    std::vector<माप_प्रकार>& namePartIds)
{
    स्थिर माप_प्रकार name_Part_गिनती = nameParts.size();
    namePartIds.resize(name_Part_गिनती, ~(माप_प्रकार)0u);
    क्रम (माप_प्रकार name_Part_सूचकांक = 0u; name_Part_सूचकांक < name_Part_गिनती; ++name_Part_सूचकांक)
    {
        स्थिर std::string& namePart = nameParts.at(name_Part_सूचकांक);
        स्थिर माप_प्रकार namePartId = internal_addOrLookupNamePart(namePart);
        namePartIds.at(name_Part_सूचकांक) = namePartId;
    }
}

माप_प्रकार LogTagManager::NameTable::internal_addOrLookupNamePart(स्थिर std::string& namePart)
{
    स्थिर स्वतः namePartIter = m_namePartIds.find(namePart);
    अगर (namePartIter != m_namePartIds.end())
    {
        वापस namePartIter->second;
    }
    स्थिर माप_प्रकार namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    वापस namePartId;
}

व्योम LogTagManager::NameTable::internal_addCrossReference(माप_प्रकार fullNameId, स्थिर std::vector<माप_प्रकार>& namePartIds)
{
    स्थिर माप_प्रकार name_Part_गिनती = namePartIds.size();
    क्रम (माप_प्रकार namePartPos = 0u; namePartPos < name_Part_गिनती; ++namePartPos)
    {
        स्थिर माप_प्रकार namePartId = namePartIds.at(namePartPos);
        m_fullNameToNamePartIds.emplace(fullNameId, std::make_pair(namePartId, namePartPos));
        m_namePartToFullNameIds.emplace(namePartId, std::make_pair(fullNameId, namePartPos));
    }
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::getFullNameInfo(स्थिर std::string& fullName)
{
    स्थिर स्वतः fullNameIdIter = m_fullNameIds.find(fullName);
    अगर (fullNameIdIter == m_fullNameIds.end())
    {
        वापस nullptr;
    }
    स्थिर माप_प्रकार fullNameId = fullNameIdIter->second;
    वापस internal_getFullNameInfo(fullNameId);
}

LogTagManager::FullNameInfo* LogTagManager::NameTable::internal_getFullNameInfo(माप_प्रकार fullNameId)
{
    वापस std::addressof(m_fullNameInfos.at(fullNameId));
}

LogTagManager::NamePartInfo* LogTagManager::NameTable::internal_getNamePartInfo(माप_प्रकार namePartId)
{
    वापस std::addressof(m_namePartInfos.at(namePartId));
}

व्योम LogTagManager::NameTable::internal_findMatchingNamePartsForFullName(Full_Name_Lookup_परिणाम& full_Name_परिणाम)
{
    स्थिर माप_प्रकार fullNameId = full_Name_परिणाम.m_fullNameId;
    FullNameInfo* fullNameInfo = full_Name_परिणाम.m_fullNameInfoPtr;
    स्थिर स्वतः& namePartIds = full_Name_परिणाम.m_namePartIds;
    स्थिर माप_प्रकार name_Part_गिनती = namePartIds.size();
    स्वतः& crossReferences = full_Name_परिणाम.m_crossReferences;
    crossReferences.सफा();
    crossReferences.reserve(name_Part_गिनती);
    क्रम (माप_प्रकार matchingPos = 0u; matchingPos < name_Part_गिनती; ++matchingPos)
    {
        स्थिर माप_प्रकार namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = internal_getNamePartInfo(namePartId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

व्योम LogTagManager::NameTable::internal_findMatchingFullNamesForNamePart(Name_Part_Lookup_परिणाम& परिणाम)
{
    स्थिर माप_प्रकार namePartId = परिणाम.m_namePartId;
    NamePartInfo* namePartInfo = परिणाम.m_namePartInfoPtr;
    स्थिर माप_प्रकार matching_Full_Name_गिनती = m_namePartToFullNameIds.गिनती_2(namePartId);
    std::vector<CrossReference>& crossReferences = परिणाम.m_crossReferences;
    crossReferences.सफा();
    crossReferences.reserve(matching_Full_Name_गिनती);
    स्थिर स्वतः namePartToFullNameIterPair = m_namePartToFullNameIds.equal_range(परिणाम.m_namePartId);
    स्थिर स्वतः iterBegin = namePartToFullNameIterPair.first;
    स्थिर स्वतः iterEnd = namePartToFullNameIterPair.second;
    क्रम (स्वतः iter = iterBegin; iter != iterEnd; ++iter)
    {
        स्थिर माप_प्रकार fullNameId = iter->second.first;
        स्थिर माप_प्रकार matchingPos = iter->second.second;
        FullNameInfo* fullNameInfo = internal_getFullNameInfo(fullNameId);
        crossReferences.emplace_back(CrossReference(fullNameId, namePartId, matchingPos, fullNameInfo, namePartInfo));
    }
}

}}} //namespace
