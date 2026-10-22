// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "../precomp.hpp"
#अन्तर्भाव "logtagmanager.hpp"
#अन्तर्भाव "logtagconfigparser.hpp"

नामाकाश cv {
नामाकाश utils {
नामाकाश logging {


ध्रुव वर्ण* Log_चिह्नकम्_Manager::m_globalName = "global";


Log_चिह्नकम्_Manager::Log_चिह्नकम्_Manager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_global_Log_चिह्नकम्(नूतन Log_चिह्नकम्(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<Log_चिह्नकम्_Config_Parser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_global_Log_चिह्नकम्.get());
}

Log_चिह्नकम्_Manager::~Log_चिह्नकम्_Manager()
{
}

शून्य Log_चिह्नकम्_Manager::समुच्चयः_Config_String(ध्रुव std::string& configString, तार्किक apply /*true*/)
{
    m_config->parse(configString);
    यदि (m_config->hasMalformed())
    {
        प्रत्यागम;
    }
    यदि (!apply)
    {
        प्रत्यागम;
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
    ध्रुव स्वचालित& globalConfig = m_config->getGlobalConfig();
    m_global_Log_चिह्नकम्->level = globalConfig.level;
    प्रति (ध्रुव स्वचालित& config : m_config->getFirstPartConfigs())
    {
        समुच्चयः_Level_By_First_Part(config.namePart, config.level);
    }
    प्रति (ध्रुव स्वचालित& config : m_config->getAnyPartConfigs())
    {
        समुच्चयः_Level_By_Any_Part(config.namePart, config.level);
    }
    प्रति (ध्रुव स्वचालित& config : m_config->get_पूर्णम्_Name_Configs())
    {
        समुच्चयः_Level_By_पूर्णम्_Name(config.namePart, config.level);
    }
}

Log_चिह्नकम्_Config_Parser& Log_चिह्नकम्_Manager::getConfigParser() ध्रुव
{
    प्रत्यागम *m_config;
}

शून्य Log_चिह्नकम्_Manager::assign(ध्रुव std::string& पूर्णम्_Name, Log_चिह्नकम्* ptr)
{
    CV_TRACE_FUNCTION();
    तालकम्_Type lock(m_mutex);
    पूर्णम्_Name_Lookup_परिणाम परिणाम(पूर्णम्_Name);
    परिणाम.m_findCrossReferences = सत्य;
    m_name_सारणी.सङ्कलय_Or_Lookup_पूर्णम्_Name(परिणाम);
    पूर्णम्_Name_Info& पूर्णम्_Name_Info_2 = *परिणाम.m_पूर्णम्_Name_Info_Ptr;
    ध्रुव तार्किक is_Ptr_परिवर्तितम् = (पूर्णम्_Name_Info_2.log_चिह्नकम्_Ptr != ptr);
    यदि (!is_Ptr_परिवर्तितम्)
    {
        प्रत्यागम;
    }
    पूर्णम्_Name_Info_2.log_चिह्नकम्_Ptr = ptr;
    यदि (!ptr)
    {
        प्रत्यागम;
    }
    ध्रुव तार्किक has_Applied_पूर्णम्_Name_Config = आन्तरिकम्_apply_पूर्णम्_Name_Config_गन्तव्यम्_चिह्नकम्(पूर्णम्_Name_Info_2);
    यदि (has_Applied_पूर्णम्_Name_Config)
    {
        प्रत्यागम;
    }
    आन्तरिकम्_apply_Name_Part_Config_गन्तव्यम्_Specific_चिह्नकम्(परिणाम);
}

शून्य Log_चिह्नकम्_Manager::unassign(ध्रुव std::string& पूर्णम्_Name)
{
    // Lock is inside assign() method.
    assign(पूर्णम्_Name, शून्यसूचकः);
}

Log_चिह्नकम्* Log_चिह्नकम्_Manager::get(ध्रुव std::string& पूर्णम्_Name)
{
    CV_TRACE_FUNCTION();
    तालकम्_Type lock(m_mutex);
    पूर्णम्_Name_Info* पूर्णम्_Name_Info_Ptr = m_name_सारणी.get_पूर्णम्_Name_Info(पूर्णम्_Name);
    यदि (पूर्णम्_Name_Info_Ptr && पूर्णम्_Name_Info_Ptr->log_चिह्नकम्_Ptr)
    {
        प्रत्यागम पूर्णम्_Name_Info_Ptr->log_चिह्नकम्_Ptr;
    }
    प्रत्यागम शून्यसूचकः;
}

शून्य Log_चिह्नकम्_Manager::समुच्चयः_Level_By_पूर्णम्_Name(ध्रुव std::string& पूर्णम्_Name, LogLevel level)
{
    CV_TRACE_FUNCTION();
    तालकम्_Type lock(m_mutex);
    पूर्णम्_Name_Lookup_परिणाम परिणाम(पूर्णम्_Name);
    परिणाम.m_findCrossReferences = असत्य;
    m_name_सारणी.सङ्कलय_Or_Lookup_पूर्णम्_Name(परिणाम);
    पूर्णम्_Name_Info& पूर्णम्_Name_Info_2 = *परिणाम.m_पूर्णम्_Name_Info_Ptr;
    यदि (पूर्णम्_Name_Info_2.parsedLevel.scope == MatchingScope::पूर्णम्_2 &&
        पूर्णम्_Name_Info_2.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        प्रत्यागम;
    }
    // update the cached configured value.
    पूर्णम्_Name_Info_2.parsedLevel.scope = MatchingScope::पूर्णम्_2;
    पूर्णम्_Name_Info_2.parsedLevel.level = level;
    // update the actual tag, if already registered.
    Log_चिह्नकम्* log_चिह्नकम्_Ptr = पूर्णम्_Name_Info_2.log_चिह्नकम्_Ptr;
    यदि (log_चिह्नकम्_Ptr)
    {
        log_चिह्नकम्_Ptr->level = level;
    }
}

शून्य Log_चिह्नकम्_Manager::समुच्चयः_Level_By_First_Part(ध्रुव std::string& firstPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    समुच्चयः_Level_By_Name_Part(firstPart, level, MatchingScope::FirstNamePart);
}

शून्य Log_चिह्नकम्_Manager::समुच्चयः_Level_By_Any_Part(ध्रुव std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    समुच्चयः_Level_By_Name_Part(anyPart, level, MatchingScope::AnyNamePart);
}

शून्य Log_चिह्नकम्_Manager::समुच्चयः_Level_By_Name_Part(ध्रुव std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    तालकम्_Type lock(m_mutex);
    Name_Part_Lookup_परिणाम परिणाम(namePart);
    परिणाम.m_findCrossReferences = सत्य;
    m_name_सारणी.सङ्कलय_Or_Lookup_Name_Part(परिणाम);
    NamePartInfo& namePartInfo = *परिणाम.m_namePartInfoPtr;
    यदि (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        प्रत्यागम;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    आन्तरिकम्_apply_Name_Part_Config_गन्तव्यम्_Matching_चिह्नकानि(परिणाम);
}

std::vector<std::string> Log_चिह्नकम्_Manager::विभज_Name_Parts(ध्रुव std::string& पूर्णम्_Name)
{
    ध्रुव size_t npos = std::string::npos;
    ध्रुव size_t len = पूर्णम्_Name.length();
    std::vector<std::string> nameParts;
    size_t start = 0u;
    यावत् (start < len)
    {
        size_t अग्र_Period = पूर्णम्_Name.find('.', start);
        यदि (अग्र_Period == npos)
        {
            अग्र_Period = len;
        }
        यदि (अग्र_Period >= start + 1u)
        {
            nameParts.emplace_back(पूर्णम्_Name.substr(start, अग्र_Period - start));
        }
        start = अग्र_Period + 1u;
    }
    प्रत्यागम nameParts;
}

तार्किक Log_चिह्नकम्_Manager::आन्तरिकम्_is_Name_Part_मेलनम्(MatchingScope scope, size_t matchingPos)
{
    विकल्प (scope)
    {
    पक्ष MatchingScope::FirstNamePart:
        प्रत्यागम (matchingPos == 0u);
    पक्ष MatchingScope::AnyNamePart:
        प्रत्यागम सत्य;
    पक्ष MatchingScope::None:
    पक्ष MatchingScope::पूर्णम्_2:
    स्वभाव:
        प्रत्यागम असत्य;
    }
}

तार्किक Log_चिह्नकम्_Manager::आन्तरिकम्_apply_पूर्णम्_Name_Config_गन्तव्यम्_चिह्नकम्(पूर्णम्_Name_Info& पूर्णम्_Name_Info_2)
{
    यदि (!पूर्णम्_Name_Info_2.log_चिह्नकम्_Ptr)
    {
        प्रत्यागम असत्य;
    }
    यदि (पूर्णम्_Name_Info_2.parsedLevel.scope == MatchingScope::पूर्णम्_2)
    {
        पूर्णम्_Name_Info_2.log_चिह्नकम्_Ptr->level = पूर्णम्_Name_Info_2.parsedLevel.level;
        प्रत्यागम सत्य;
    }
    प्रत्यागम असत्य;
}

तार्किक Log_चिह्नकम्_Manager::आन्तरिकम्_apply_Name_Part_Config_गन्तव्यम्_Specific_चिह्नकम्(पूर्णम्_Name_Lookup_परिणाम& पूर्णम्_Name_परिणाम)
{
    ध्रुव पूर्णम्_Name_Info& पूर्णम्_Name_Info_2 = *पूर्णम्_Name_परिणाम.m_पूर्णम्_Name_Info_Ptr;
    Log_चिह्नकम्* ध्रुव log_चिह्नकम् = पूर्णम्_Name_Info_2.log_चिह्नकम्_Ptr;
    यदि (!log_चिह्नकम्)
    {
        प्रत्यागम असत्य;
    }
    CV_Assert(पूर्णम्_Name_परिणाम.m_findCrossReferences);
    ध्रुव स्वचालित& crossReferences = पूर्णम्_Name_परिणाम.m_crossReferences;
    ध्रुव size_t matching_Name_Part_गणनम् = crossReferences.size();
    प्रति (size_t k = 0u; k < matching_Name_Part_गणनम्; ++k)
    {
        ध्रुव स्वचालित& मेलनम् = crossReferences.at(k);
        ध्रुव स्वचालित& namePartInfo = *मेलनम्.m_namePartInfo;
        ध्रुव स्वचालित& parsedLevel = namePartInfo.parsedLevel;
        ध्रुव स्वचालित scope = parsedLevel.scope;
        ध्रुव LogLevel level = parsedLevel.level;
        ध्रुव size_t matchingPos = मेलनम्.m_matchingPos;
        ध्रुव तार्किक is_मेलनम् = आन्तरिकम्_is_Name_Part_मेलनम्(scope, matchingPos);
        यदि (is_मेलनम्)
        {
            log_चिह्नकम्->level = level;
            प्रत्यागम सत्य;
        }
    }
    प्रत्यागम असत्य;
}

शून्य Log_चिह्नकम्_Manager::आन्तरिकम्_apply_Name_Part_Config_गन्तव्यम्_Matching_चिह्नकानि(Name_Part_Lookup_परिणाम& name_Part_परिणाम)
{
    CV_Assert(name_Part_परिणाम.m_findCrossReferences);
    ध्रुव स्वचालित& crossReferences = name_Part_परिणाम.m_crossReferences;
    ध्रुव size_t matching_पूर्णम्_Name_गणनम् = crossReferences.size();
    NamePartInfo& namePartInfo = *name_Part_परिणाम.m_namePartInfoPtr;
    ध्रुव MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::पूर्णम्_2);
    यदि (scope == MatchingScope::None)
    {
        प्रत्यागम;
    }
    ध्रुव LogLevel level = namePartInfo.parsedLevel.level;
    प्रति (size_t k = 0u; k < matching_पूर्णम्_Name_गणनम्; ++k)
    {
        ध्रुव स्वचालित& मेलनम् = crossReferences.at(k);
        पूर्णम्_Name_Info& पूर्णम्_Name_Info_2 = *मेलनम्.m_पूर्णम्_Name_Info;
        Log_चिह्नकम्* log_चिह्नकम्_Ptr = पूर्णम्_Name_Info_2.log_चिह्नकम्_Ptr;
        यदि (!log_चिह्नकम्_Ptr)
        {
            अनुवर्त;
        }
        यदि (पूर्णम्_Name_Info_2.parsedLevel.scope == MatchingScope::पूर्णम्_2)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            अनुवर्त;
        }
        ध्रुव size_t matchingPos = मेलनम्.m_matchingPos;
        ध्रुव तार्किक is_मेलनम् = आन्तरिकम्_is_Name_Part_मेलनम्(scope, matchingPos);
        यदि (!is_मेलनम्)
        {
            अनुवर्त;
        }
        log_चिह्नकम्_Ptr->level = level;
    }
}

शून्य Log_चिह्नकम्_Manager::Name_सारणी::सङ्कलय_Or_Lookup_पूर्णम्_Name(पूर्णम्_Name_Lookup_परिणाम& परिणाम)
{
    ध्रुव स्वचालित पूर्णम्_Name_Id_And_Flag = आन्तरिकम्_सङ्कलय_Or_Lookup_पूर्णम्_Name(परिणाम.m_पूर्णम्_Name);
    परिणाम.m_पूर्णम्_Name_Id = पूर्णम्_Name_Id_And_Flag.first;
    परिणाम.m_nameParts = Log_चिह्नकम्_Manager::विभज_Name_Parts(परिणाम.m_पूर्णम्_Name);
    आन्तरिकम्_सङ्कलय_Or_Lookup_Name_Parts(परिणाम.m_nameParts, परिणाम.m_namePartIds);
    ध्रुव तार्किक isNew = पूर्णम्_Name_Id_And_Flag.second;
    यदि (isNew)
    {
        आन्तरिकम्_सङ्कलय_Cross_Reference(परिणाम.m_पूर्णम्_Name_Id, परिणाम.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    परिणाम.m_पूर्णम्_Name_Info_Ptr = आन्तरिकम्_get_पूर्णम्_Name_Info(परिणाम.m_पूर्णम्_Name_Id);
    यदि (परिणाम.m_findCrossReferences)
    {
        आन्तरिकम्_find_Matching_Name_Parts_For_पूर्णम्_Name(परिणाम);
    }
}

शून्य Log_चिह्नकम्_Manager::Name_सारणी::सङ्कलय_Or_Lookup_Name_Part(Name_Part_Lookup_परिणाम& परिणाम)
{
    परिणाम.m_namePartId = आन्तरिकम्_सङ्कलय_Or_Lookup_Name_Part(परिणाम.m_namePart);
    परिणाम.m_namePartInfoPtr = आन्तरिकम्_get_Name_Part_Info(परिणाम.m_namePartId);
    यदि (परिणाम.m_findCrossReferences)
    {
        आन्तरिकम्_find_Matching_पूर्णम्_नामानि_For_Name_Part(परिणाम);
    }
}

std::pair<size_t, तार्किक> Log_चिह्नकम्_Manager::Name_सारणी::आन्तरिकम्_सङ्कलय_Or_Lookup_पूर्णम्_Name(ध्रुव std::string& पूर्णम्_Name)
{
    ध्रुव स्वचालित पूर्णम्_Name_Id_Iter = m_पूर्णम्_Name_Ids.find(पूर्णम्_Name);
    यदि (पूर्णम्_Name_Id_Iter != m_पूर्णम्_Name_Ids.end())
    {
        प्रत्यागम std::make_pair(पूर्णम्_Name_Id_Iter->second, असत्य);
    }
    ध्रुव size_t पूर्णम्_Name_Id = m_पूर्णम्_Name_Infos.size();
    m_पूर्णम्_Name_Infos.emplace_back(पूर्णम्_Name_Info{});
    m_पूर्णम्_Name_Ids.emplace(पूर्णम्_Name, पूर्णम्_Name_Id);
    प्रत्यागम std::make_pair(पूर्णम्_Name_Id, सत्य);
}

शून्य Log_चिह्नकम्_Manager::Name_सारणी::आन्तरिकम्_सङ्कलय_Or_Lookup_Name_Parts(ध्रुव std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    ध्रुव size_t name_Part_गणनम् = nameParts.size();
    namePartIds.resize(name_Part_गणनम्, ~(size_t)0u);
    प्रति (size_t name_Part_सूचकांक = 0u; name_Part_सूचकांक < name_Part_गणनम्; ++name_Part_सूचकांक)
    {
        ध्रुव std::string& namePart = nameParts.at(name_Part_सूचकांक);
        ध्रुव size_t namePartId = आन्तरिकम्_सङ्कलय_Or_Lookup_Name_Part(namePart);
        namePartIds.at(name_Part_सूचकांक) = namePartId;
    }
}

size_t Log_चिह्नकम्_Manager::Name_सारणी::आन्तरिकम्_सङ्कलय_Or_Lookup_Name_Part(ध्रुव std::string& namePart)
{
    ध्रुव स्वचालित namePartIter = m_namePartIds.find(namePart);
    यदि (namePartIter != m_namePartIds.end())
    {
        प्रत्यागम namePartIter->second;
    }
    ध्रुव size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    प्रत्यागम namePartId;
}

शून्य Log_चिह्नकम्_Manager::Name_सारणी::आन्तरिकम्_सङ्कलय_Cross_Reference(size_t पूर्णम्_Name_Id, ध्रुव std::vector<size_t>& namePartIds)
{
    ध्रुव size_t name_Part_गणनम् = namePartIds.size();
    प्रति (size_t namePartPos = 0u; namePartPos < name_Part_गणनम्; ++namePartPos)
    {
        ध्रुव size_t namePartId = namePartIds.at(namePartPos);
        m_पूर्णम्_Name_गन्तव्यम्_Name_Part_Ids.emplace(पूर्णम्_Name_Id, std::make_pair(namePartId, namePartPos));
        m_name_Part_गन्तव्यम्_पूर्णम्_Name_Ids.emplace(namePartId, std::make_pair(पूर्णम्_Name_Id, namePartPos));
    }
}

Log_चिह्नकम्_Manager::पूर्णम्_Name_Info* Log_चिह्नकम्_Manager::Name_सारणी::get_पूर्णम्_Name_Info(ध्रुव std::string& पूर्णम्_Name)
{
    ध्रुव स्वचालित पूर्णम्_Name_Id_Iter = m_पूर्णम्_Name_Ids.find(पूर्णम्_Name);
    यदि (पूर्णम्_Name_Id_Iter == m_पूर्णम्_Name_Ids.end())
    {
        प्रत्यागम शून्यसूचकः;
    }
    ध्रुव size_t पूर्णम्_Name_Id = पूर्णम्_Name_Id_Iter->second;
    प्रत्यागम आन्तरिकम्_get_पूर्णम्_Name_Info(पूर्णम्_Name_Id);
}

Log_चिह्नकम्_Manager::पूर्णम्_Name_Info* Log_चिह्नकम्_Manager::Name_सारणी::आन्तरिकम्_get_पूर्णम्_Name_Info(size_t पूर्णम्_Name_Id)
{
    प्रत्यागम std::addressof(m_पूर्णम्_Name_Infos.at(पूर्णम्_Name_Id));
}

Log_चिह्नकम्_Manager::NamePartInfo* Log_चिह्नकम्_Manager::Name_सारणी::आन्तरिकम्_get_Name_Part_Info(size_t namePartId)
{
    प्रत्यागम std::addressof(m_namePartInfos.at(namePartId));
}

शून्य Log_चिह्नकम्_Manager::Name_सारणी::आन्तरिकम्_find_Matching_Name_Parts_For_पूर्णम्_Name(पूर्णम्_Name_Lookup_परिणाम& पूर्णम्_Name_परिणाम)
{
    ध्रुव size_t पूर्णम्_Name_Id = पूर्णम्_Name_परिणाम.m_पूर्णम्_Name_Id;
    पूर्णम्_Name_Info* पूर्णम्_Name_Info_2 = पूर्णम्_Name_परिणाम.m_पूर्णम्_Name_Info_Ptr;
    ध्रुव स्वचालित& namePartIds = पूर्णम्_Name_परिणाम.m_namePartIds;
    ध्रुव size_t name_Part_गणनम् = namePartIds.size();
    स्वचालित& crossReferences = पूर्णम्_Name_परिणाम.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_गणनम्);
    प्रति (size_t matchingPos = 0u; matchingPos < name_Part_गणनम्; ++matchingPos)
    {
        ध्रुव size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = आन्तरिकम्_get_Name_Part_Info(namePartId);
        crossReferences.emplace_back(CrossReference(पूर्णम्_Name_Id, namePartId, matchingPos, पूर्णम्_Name_Info_2, namePartInfo));
    }
}

शून्य Log_चिह्नकम्_Manager::Name_सारणी::आन्तरिकम्_find_Matching_पूर्णम्_नामानि_For_Name_Part(Name_Part_Lookup_परिणाम& परिणाम)
{
    ध्रुव size_t namePartId = परिणाम.m_namePartId;
    NamePartInfo* namePartInfo = परिणाम.m_namePartInfoPtr;
    ध्रुव size_t matching_पूर्णम्_Name_गणनम् = m_name_Part_गन्तव्यम्_पूर्णम्_Name_Ids.गणनम्_2(namePartId);
    std::vector<CrossReference>& crossReferences = परिणाम.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_पूर्णम्_Name_गणनम्);
    ध्रुव स्वचालित name_Part_गन्तव्यम्_पूर्णम्_Name_Iter_Pair = m_name_Part_गन्तव्यम्_पूर्णम्_Name_Ids.equal_range(परिणाम.m_namePartId);
    ध्रुव स्वचालित iterBegin = name_Part_गन्तव्यम्_पूर्णम्_Name_Iter_Pair.first;
    ध्रुव स्वचालित iter_अन्तः = name_Part_गन्तव्यम्_पूर्णम्_Name_Iter_Pair.second;
    प्रति (स्वचालित iter = iterBegin; iter != iter_अन्तः; ++iter)
    {
        ध्रुव size_t पूर्णम्_Name_Id = iter->second.first;
        ध्रुव size_t matchingPos = iter->second.second;
        पूर्णम्_Name_Info* पूर्णम्_Name_Info_2 = आन्तरिकम्_get_पूर्णम्_Name_Info(पूर्णम्_Name_Id);
        crossReferences.emplace_back(CrossReference(पूर्णम्_Name_Id, namePartId, matchingPos, पूर्णम्_Name_Info_2, namePartInfo));
    }
}

}}} //namespace
