// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "../precomp.hpp"
#ಅನ್ತರ್ಭಾವ "logtagmanager.hpp"
#ಅನ್ತರ್ಭಾವ "logtagconfigparser.hpp"

ನಾಮಾಕಾಶ cv {
ನಾಮಾಕಾಶ utils {
ನಾಮಾಕಾಶ logging {


ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* Log_ಟ್ಯಾಗ್_Manager::m_globalName = "global";


Log_ಟ್ಯಾಗ್_Manager::Log_ಟ್ಯಾಗ್_Manager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_global_Log_ಟ್ಯಾಗ್(ಹೊಸ Log_ಟ್ಯಾಗ್(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<Log_ಟ್ಯಾಗ್_Config_Parser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_global_Log_ಟ್ಯಾಗ್.get());
}

Log_ಟ್ಯಾಗ್_Manager::~Log_ಟ್ಯಾಗ್_Manager()
{
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::ಗಣ_Config_String(ಸ್ಥಿರಾಂಕ std::string& configString, ತಾರ್ಕಿಕ apply /*true*/)
{
    m_config->parse(configString);
    ಆದರೆ (m_config->hasMalformed())
    {
        ಹಿಂತಿರುಗಿ;
    }
    ಆದರೆ (!apply)
    {
        ಹಿಂತಿರುಗಿ;
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
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& globalConfig = m_config->getGlobalConfig();
    m_global_Log_ಟ್ಯಾಗ್->level = globalConfig.level;
    ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& config : m_config->get_ಮೊದಲ_Part_Configs())
    {
        ಗಣ_Level_By_ಮೊದಲ_Part(config.namePart, config.level);
    }
    ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& config : m_config->getAnyPartConfigs())
    {
        ಗಣ_Level_By_Any_Part(config.namePart, config.level);
    }
    ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& config : m_config->get_ತುಂಬಿದ_Name_Configs())
    {
        ಗಣ_Level_By_ತುಂಬಿದ_Name(config.namePart, config.level);
    }
}

Log_ಟ್ಯಾಗ್_Config_Parser& Log_ಟ್ಯಾಗ್_Manager::getConfigParser() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ *m_config;
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::assign(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name, Log_ಟ್ಯಾಗ್* ptr)
{
    CV_TRACE_FUNCTION();
    ಬೀಗ_Type lock(m_mutex);
    ತುಂಬಿದ_Name_Lookup_ಫಲಿತಾಂಶ ಫಲಿತಾಂಶ(ತುಂಬಿದ_Name);
    ಫಲಿತಾಂಶ.m_ಕಂಡುಹಿಡಿ_Cross_References = ನಿಜ;
    m_name_ಕೋಷ್ಟಕ.ಕೂಡಿಸು_Or_Lookup_ತುಂಬಿದ_Name(ಫಲಿತಾಂಶ);
    ತುಂಬಿದ_Name_Info& ತುಂಬಿದ_Name_Info_2 = *ಫಲಿತಾಂಶ.m_ತುಂಬಿದ_Name_Info_Ptr;
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ is_Ptr_ಬದಲಾಯಿತು = (ತುಂಬಿದ_Name_Info_2.log_ಟ್ಯಾಗ್_Ptr != ptr);
    ಆದರೆ (!is_Ptr_ಬದಲಾಯಿತು)
    {
        ಹಿಂತಿರುಗಿ;
    }
    ತುಂಬಿದ_Name_Info_2.log_ಟ್ಯಾಗ್_Ptr = ptr;
    ಆದರೆ (!ptr)
    {
        ಹಿಂತಿರುಗಿ;
    }
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ has_Applied_ತುಂಬಿದ_Name_Config = ಆಂತರಿಕ_apply_ತುಂಬಿದ_Name_Config_ಗಮ್ಯ_ಟ್ಯಾಗ್(ತುಂಬಿದ_Name_Info_2);
    ಆದರೆ (has_Applied_ತುಂಬಿದ_Name_Config)
    {
        ಹಿಂತಿರುಗಿ;
    }
    ಆಂತರಿಕ_apply_Name_Part_Config_ಗಮ್ಯ_Specific_ಟ್ಯಾಗ್(ಫಲಿತಾಂಶ);
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::unassign(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name)
{
    // Lock is inside assign() method.
    assign(ತುಂಬಿದ_Name, ಶೂನ್ಯಸೂಚಕ);
}

Log_ಟ್ಯಾಗ್* Log_ಟ್ಯಾಗ್_Manager::get(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name)
{
    CV_TRACE_FUNCTION();
    ಬೀಗ_Type lock(m_mutex);
    ತುಂಬಿದ_Name_Info* ತುಂಬಿದ_Name_Info_Ptr = m_name_ಕೋಷ್ಟಕ.get_ತುಂಬಿದ_Name_Info(ತುಂಬಿದ_Name);
    ಆದರೆ (ತುಂಬಿದ_Name_Info_Ptr && ತುಂಬಿದ_Name_Info_Ptr->log_ಟ್ಯಾಗ್_Ptr)
    {
        ಹಿಂತಿರುಗಿ ತುಂಬಿದ_Name_Info_Ptr->log_ಟ್ಯಾಗ್_Ptr;
    }
    ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::ಗಣ_Level_By_ತುಂಬಿದ_Name(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name, LogLevel level)
{
    CV_TRACE_FUNCTION();
    ಬೀಗ_Type lock(m_mutex);
    ತುಂಬಿದ_Name_Lookup_ಫಲಿತಾಂಶ ಫಲಿತಾಂಶ(ತುಂಬಿದ_Name);
    ಫಲಿತಾಂಶ.m_ಕಂಡುಹಿಡಿ_Cross_References = ಸುಳ್ಳು;
    m_name_ಕೋಷ್ಟಕ.ಕೂಡಿಸು_Or_Lookup_ತುಂಬಿದ_Name(ಫಲಿತಾಂಶ);
    ತುಂಬಿದ_Name_Info& ತುಂಬಿದ_Name_Info_2 = *ಫಲಿತಾಂಶ.m_ತುಂಬಿದ_Name_Info_Ptr;
    ಆದರೆ (ತುಂಬಿದ_Name_Info_2.parsedLevel.scope == MatchingScope::ತುಂಬಿದ_2 &&
        ತುಂಬಿದ_Name_Info_2.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        ಹಿಂತಿರುಗಿ;
    }
    // update the cached configured value.
    ತುಂಬಿದ_Name_Info_2.parsedLevel.scope = MatchingScope::ತುಂಬಿದ_2;
    ತುಂಬಿದ_Name_Info_2.parsedLevel.level = level;
    // update the actual tag, if already registered.
    Log_ಟ್ಯಾಗ್* log_ಟ್ಯಾಗ್_Ptr = ತುಂಬಿದ_Name_Info_2.log_ಟ್ಯಾಗ್_Ptr;
    ಆದರೆ (log_ಟ್ಯಾಗ್_Ptr)
    {
        log_ಟ್ಯಾಗ್_Ptr->level = level;
    }
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::ಗಣ_Level_By_ಮೊದಲ_Part(ಸ್ಥಿರಾಂಕ std::string& ಮೊದಲ_Part, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    ಗಣ_Level_By_Name_Part(ಮೊದಲ_Part, level, MatchingScope::ಮೊದಲ_Name_Part);
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::ಗಣ_Level_By_Any_Part(ಸ್ಥಿರಾಂಕ std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    ಗಣ_Level_By_Name_Part(anyPart, level, MatchingScope::AnyNamePart);
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::ಗಣ_Level_By_Name_Part(ಸ್ಥಿರಾಂಕ std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    ಬೀಗ_Type lock(m_mutex);
    Name_Part_Lookup_ಫಲಿತಾಂಶ ಫಲಿತಾಂಶ(namePart);
    ಫಲಿತಾಂಶ.m_ಕಂಡುಹಿಡಿ_Cross_References = ನಿಜ;
    m_name_ಕೋಷ್ಟಕ.ಕೂಡಿಸು_Or_Lookup_Name_Part(ಫಲಿತಾಂಶ);
    NamePartInfo& namePartInfo = *ಫಲಿತಾಂಶ.m_namePartInfoPtr;
    ಆದರೆ (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        ಹಿಂತಿರುಗಿ;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    ಆಂತರಿಕ_apply_Name_Part_Config_ಗಮ್ಯ_Matching_ಟ್ಯಾಗುಗಳು(ಫಲಿತಾಂಶ);
}

std::vector<std::string> Log_ಟ್ಯಾಗ್_Manager::ವಿಭಜಿಸು_Name_Parts(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name)
{
    ಸ್ಥಿರಾಂಕ size_t npos = std::string::npos;
    ಸ್ಥಿರಾಂಕ size_t len = ತುಂಬಿದ_Name.length();
    std::vector<std::string> nameParts;
    size_t ಆರಂಭ = 0u;
    ಆಗ (ಆರಂಭ < len)
    {
        size_t ಮುಂದಿನ_Period = ತುಂಬಿದ_Name.find('.', ಆರಂಭ);
        ಆದರೆ (ಮುಂದಿನ_Period == npos)
        {
            ಮುಂದಿನ_Period = len;
        }
        ಆದರೆ (ಮುಂದಿನ_Period >= ಆರಂಭ + 1u)
        {
            nameParts.emplace_back(ತುಂಬಿದ_Name.substr(ಆರಂಭ, ಮುಂದಿನ_Period - ಆರಂಭ));
        }
        ಆರಂಭ = ಮುಂದಿನ_Period + 1u;
    }
    ಹಿಂತಿರುಗಿ nameParts;
}

ತಾರ್ಕಿಕ Log_ಟ್ಯಾಗ್_Manager::ಆಂತರಿಕ_is_Name_Part_ಹೊಂದಿಕೆ(MatchingScope scope, size_t matchingPos)
{
    ಆಯ್ಕೆ (scope)
    {
    ಸ್ಥಿತಿ MatchingScope::ಮೊದಲ_Name_Part:
        ಹಿಂತಿರುಗಿ (matchingPos == 0u);
    ಸ್ಥಿತಿ MatchingScope::AnyNamePart:
        ಹಿಂತಿರುಗಿ ನಿಜ;
    ಸ್ಥಿತಿ MatchingScope::None:
    ಸ್ಥಿತಿ MatchingScope::ತುಂಬಿದ_2:
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
}

ತಾರ್ಕಿಕ Log_ಟ್ಯಾಗ್_Manager::ಆಂತರಿಕ_apply_ತುಂಬಿದ_Name_Config_ಗಮ್ಯ_ಟ್ಯಾಗ್(ತುಂಬಿದ_Name_Info& ತುಂಬಿದ_Name_Info_2)
{
    ಆದರೆ (!ತುಂಬಿದ_Name_Info_2.log_ಟ್ಯಾಗ್_Ptr)
    {
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    ಆದರೆ (ತುಂಬಿದ_Name_Info_2.parsedLevel.scope == MatchingScope::ತುಂಬಿದ_2)
    {
        ತುಂಬಿದ_Name_Info_2.log_ಟ್ಯಾಗ್_Ptr->level = ತುಂಬಿದ_Name_Info_2.parsedLevel.level;
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

ತಾರ್ಕಿಕ Log_ಟ್ಯಾಗ್_Manager::ಆಂತರಿಕ_apply_Name_Part_Config_ಗಮ್ಯ_Specific_ಟ್ಯಾಗ್(ತುಂಬಿದ_Name_Lookup_ಫಲಿತಾಂಶ& ತುಂಬಿದ_Name_ಫಲಿತಾಂಶ)
{
    ಸ್ಥಿರಾಂಕ ತುಂಬಿದ_Name_Info& ತುಂಬಿದ_Name_Info_2 = *ತುಂಬಿದ_Name_ಫಲಿತಾಂಶ.m_ತುಂಬಿದ_Name_Info_Ptr;
    Log_ಟ್ಯಾಗ್* ಸ್ಥಿರಾಂಕ log_ಟ್ಯಾಗ್ = ತುಂಬಿದ_Name_Info_2.log_ಟ್ಯಾಗ್_Ptr;
    ಆದರೆ (!log_ಟ್ಯಾಗ್)
    {
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    CV_Assert(ತುಂಬಿದ_Name_ಫಲಿತಾಂಶ.m_ಕಂಡುಹಿಡಿ_Cross_References);
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& crossReferences = ತುಂಬಿದ_Name_ಫಲಿತಾಂಶ.m_crossReferences;
    ಸ್ಥಿರಾಂಕ size_t matching_Name_Part_ಎಣಿಕೆ = crossReferences.size();
    ಗಾಗಿ (size_t k = 0u; k < matching_Name_Part_ಎಣಿಕೆ; ++k)
    {
        ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& ಹೊಂದಿಕೆ = crossReferences.at(k);
        ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& namePartInfo = *ಹೊಂದಿಕೆ.m_namePartInfo;
        ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& parsedLevel = namePartInfo.parsedLevel;
        ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ scope = parsedLevel.scope;
        ಸ್ಥಿರಾಂಕ LogLevel level = parsedLevel.level;
        ಸ್ಥಿರಾಂಕ size_t matchingPos = ಹೊಂದಿಕೆ.m_matchingPos;
        ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ is_ಹೊಂದಿಕೆ = ಆಂತರಿಕ_is_Name_Part_ಹೊಂದಿಕೆ(scope, matchingPos);
        ಆದರೆ (is_ಹೊಂದಿಕೆ)
        {
            log_ಟ್ಯಾಗ್->level = level;
            ಹಿಂತಿರುಗಿ ನಿಜ;
        }
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::ಆಂತರಿಕ_apply_Name_Part_Config_ಗಮ್ಯ_Matching_ಟ್ಯಾಗುಗಳು(Name_Part_Lookup_ಫಲಿತಾಂಶ& name_Part_ಫಲಿತಾಂಶ)
{
    CV_Assert(name_Part_ಫಲಿತಾಂಶ.m_ಕಂಡುಹಿಡಿ_Cross_References);
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& crossReferences = name_Part_ಫಲಿತಾಂಶ.m_crossReferences;
    ಸ್ಥಿರಾಂಕ size_t matching_ತುಂಬಿದ_Name_ಎಣಿಕೆ = crossReferences.size();
    NamePartInfo& namePartInfo = *name_Part_ಫಲಿತಾಂಶ.m_namePartInfoPtr;
    ಸ್ಥಿರಾಂಕ MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::ತುಂಬಿದ_2);
    ಆದರೆ (scope == MatchingScope::None)
    {
        ಹಿಂತಿರುಗಿ;
    }
    ಸ್ಥಿರಾಂಕ LogLevel level = namePartInfo.parsedLevel.level;
    ಗಾಗಿ (size_t k = 0u; k < matching_ತುಂಬಿದ_Name_ಎಣಿಕೆ; ++k)
    {
        ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& ಹೊಂದಿಕೆ = crossReferences.at(k);
        ತುಂಬಿದ_Name_Info& ತುಂಬಿದ_Name_Info_2 = *ಹೊಂದಿಕೆ.m_ತುಂಬಿದ_Name_Info;
        Log_ಟ್ಯಾಗ್* log_ಟ್ಯಾಗ್_Ptr = ತುಂಬಿದ_Name_Info_2.log_ಟ್ಯಾಗ್_Ptr;
        ಆದರೆ (!log_ಟ್ಯಾಗ್_Ptr)
        {
            ಮುಂದುವರಿ;
        }
        ಆದರೆ (ತುಂಬಿದ_Name_Info_2.parsedLevel.scope == MatchingScope::ತುಂಬಿದ_2)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            ಮುಂದುವರಿ;
        }
        ಸ್ಥಿರಾಂಕ size_t matchingPos = ಹೊಂದಿಕೆ.m_matchingPos;
        ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ is_ಹೊಂದಿಕೆ = ಆಂತರಿಕ_is_Name_Part_ಹೊಂದಿಕೆ(scope, matchingPos);
        ಆದರೆ (!is_ಹೊಂದಿಕೆ)
        {
            ಮುಂದುವರಿ;
        }
        log_ಟ್ಯಾಗ್_Ptr->level = level;
    }
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::Name_ಕೋಷ್ಟಕ::ಕೂಡಿಸು_Or_Lookup_ತುಂಬಿದ_Name(ತುಂಬಿದ_Name_Lookup_ಫಲಿತಾಂಶ& ಫಲಿತಾಂಶ)
{
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ ತುಂಬಿದ_Name_Id_And_Flag = ಆಂತರಿಕ_ಕೂಡಿಸು_Or_Lookup_ತುಂಬಿದ_Name(ಫಲಿತಾಂಶ.m_ತುಂಬಿದ_Name);
    ಫಲಿತಾಂಶ.m_ತುಂಬಿದ_Name_Id = ತುಂಬಿದ_Name_Id_And_Flag.first;
    ಫಲಿತಾಂಶ.m_nameParts = Log_ಟ್ಯಾಗ್_Manager::ವಿಭಜಿಸು_Name_Parts(ಫಲಿತಾಂಶ.m_ತುಂಬಿದ_Name);
    ಆಂತರಿಕ_ಕೂಡಿಸು_Or_Lookup_Name_Parts(ಫಲಿತಾಂಶ.m_nameParts, ಫಲಿತಾಂಶ.m_namePartIds);
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ isNew = ತುಂಬಿದ_Name_Id_And_Flag.second;
    ಆದರೆ (isNew)
    {
        ಆಂತರಿಕ_ಕೂಡಿಸು_Cross_Reference(ಫಲಿತಾಂಶ.m_ತುಂಬಿದ_Name_Id, ಫಲಿತಾಂಶ.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    ಫಲಿತಾಂಶ.m_ತುಂಬಿದ_Name_Info_Ptr = ಆಂತರಿಕ_get_ತುಂಬಿದ_Name_Info(ಫಲಿತಾಂಶ.m_ತುಂಬಿದ_Name_Id);
    ಆದರೆ (ಫಲಿತಾಂಶ.m_ಕಂಡುಹಿಡಿ_Cross_References)
    {
        ಆಂತರಿಕ_ಕಂಡುಹಿಡಿ_Matching_Name_Parts_For_ತುಂಬಿದ_Name(ಫಲಿತಾಂಶ);
    }
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::Name_ಕೋಷ್ಟಕ::ಕೂಡಿಸು_Or_Lookup_Name_Part(Name_Part_Lookup_ಫಲಿತಾಂಶ& ಫಲಿತಾಂಶ)
{
    ಫಲಿತಾಂಶ.m_namePartId = ಆಂತರಿಕ_ಕೂಡಿಸು_Or_Lookup_Name_Part(ಫಲಿತಾಂಶ.m_namePart);
    ಫಲಿತಾಂಶ.m_namePartInfoPtr = ಆಂತರಿಕ_get_Name_Part_Info(ಫಲಿತಾಂಶ.m_namePartId);
    ಆದರೆ (ಫಲಿತಾಂಶ.m_ಕಂಡುಹಿಡಿ_Cross_References)
    {
        ಆಂತರಿಕ_ಕಂಡುಹಿಡಿ_Matching_ತುಂಬಿದ_ಹೆಸರುಗಳು_For_Name_Part(ಫಲಿತಾಂಶ);
    }
}

std::pair<size_t, ತಾರ್ಕಿಕ> Log_ಟ್ಯಾಗ್_Manager::Name_ಕೋಷ್ಟಕ::ಆಂತರಿಕ_ಕೂಡಿಸು_Or_Lookup_ತುಂಬಿದ_Name(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name)
{
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ ತುಂಬಿದ_Name_Id_Iter = m_ತುಂಬಿದ_Name_Ids.find(ತುಂಬಿದ_Name);
    ಆದರೆ (ತುಂಬಿದ_Name_Id_Iter != m_ತುಂಬಿದ_Name_Ids.end())
    {
        ಹಿಂತಿರುಗಿ std::make_pair(ತುಂಬಿದ_Name_Id_Iter->second, ಸುಳ್ಳು);
    }
    ಸ್ಥಿರಾಂಕ size_t ತುಂಬಿದ_Name_Id = m_ತುಂಬಿದ_Name_Infos.size();
    m_ತುಂಬಿದ_Name_Infos.emplace_back(ತುಂಬಿದ_Name_Info{});
    m_ತುಂಬಿದ_Name_Ids.emplace(ತುಂಬಿದ_Name, ತುಂಬಿದ_Name_Id);
    ಹಿಂತಿರುಗಿ std::make_pair(ತುಂಬಿದ_Name_Id, ನಿಜ);
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::Name_ಕೋಷ್ಟಕ::ಆಂತರಿಕ_ಕೂಡಿಸು_Or_Lookup_Name_Parts(ಸ್ಥಿರಾಂಕ std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    ಸ್ಥಿರಾಂಕ size_t name_Part_ಎಣಿಕೆ = nameParts.size();
    namePartIds.resize(name_Part_ಎಣಿಕೆ, ~(size_t)0u);
    ಗಾಗಿ (size_t name_Part_ಸೂಚ್ಯಂಕ = 0u; name_Part_ಸೂಚ್ಯಂಕ < name_Part_ಎಣಿಕೆ; ++name_Part_ಸೂಚ್ಯಂಕ)
    {
        ಸ್ಥಿರಾಂಕ std::string& namePart = nameParts.at(name_Part_ಸೂಚ್ಯಂಕ);
        ಸ್ಥಿರಾಂಕ size_t namePartId = ಆಂತರಿಕ_ಕೂಡಿಸು_Or_Lookup_Name_Part(namePart);
        namePartIds.at(name_Part_ಸೂಚ್ಯಂಕ) = namePartId;
    }
}

size_t Log_ಟ್ಯಾಗ್_Manager::Name_ಕೋಷ್ಟಕ::ಆಂತರಿಕ_ಕೂಡಿಸು_Or_Lookup_Name_Part(ಸ್ಥಿರಾಂಕ std::string& namePart)
{
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ namePartIter = m_namePartIds.find(namePart);
    ಆದರೆ (namePartIter != m_namePartIds.end())
    {
        ಹಿಂತಿರುಗಿ namePartIter->second;
    }
    ಸ್ಥಿರಾಂಕ size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    ಹಿಂತಿರುಗಿ namePartId;
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::Name_ಕೋಷ್ಟಕ::ಆಂತರಿಕ_ಕೂಡಿಸು_Cross_Reference(size_t ತುಂಬಿದ_Name_Id, ಸ್ಥಿರಾಂಕ std::vector<size_t>& namePartIds)
{
    ಸ್ಥಿರಾಂಕ size_t name_Part_ಎಣಿಕೆ = namePartIds.size();
    ಗಾಗಿ (size_t namePartPos = 0u; namePartPos < name_Part_ಎಣಿಕೆ; ++namePartPos)
    {
        ಸ್ಥಿರಾಂಕ size_t namePartId = namePartIds.at(namePartPos);
        m_ತುಂಬಿದ_Name_ಗಮ್ಯ_Name_Part_Ids.emplace(ತುಂಬಿದ_Name_Id, std::make_pair(namePartId, namePartPos));
        m_name_Part_ಗಮ್ಯ_ತುಂಬಿದ_Name_Ids.emplace(namePartId, std::make_pair(ತುಂಬಿದ_Name_Id, namePartPos));
    }
}

Log_ಟ್ಯಾಗ್_Manager::ತುಂಬಿದ_Name_Info* Log_ಟ್ಯಾಗ್_Manager::Name_ಕೋಷ್ಟಕ::get_ತುಂಬಿದ_Name_Info(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name)
{
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ ತುಂಬಿದ_Name_Id_Iter = m_ತುಂಬಿದ_Name_Ids.find(ತುಂಬಿದ_Name);
    ಆದರೆ (ತುಂಬಿದ_Name_Id_Iter == m_ತುಂಬಿದ_Name_Ids.end())
    {
        ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
    }
    ಸ್ಥಿರಾಂಕ size_t ತುಂಬಿದ_Name_Id = ತುಂಬಿದ_Name_Id_Iter->second;
    ಹಿಂತಿರುಗಿ ಆಂತರಿಕ_get_ತುಂಬಿದ_Name_Info(ತುಂಬಿದ_Name_Id);
}

Log_ಟ್ಯಾಗ್_Manager::ತುಂಬಿದ_Name_Info* Log_ಟ್ಯಾಗ್_Manager::Name_ಕೋಷ್ಟಕ::ಆಂತರಿಕ_get_ತುಂಬಿದ_Name_Info(size_t ತುಂಬಿದ_Name_Id)
{
    ಹಿಂತಿರುಗಿ std::addressof(m_ತುಂಬಿದ_Name_Infos.at(ತುಂಬಿದ_Name_Id));
}

Log_ಟ್ಯಾಗ್_Manager::NamePartInfo* Log_ಟ್ಯಾಗ್_Manager::Name_ಕೋಷ್ಟಕ::ಆಂತರಿಕ_get_Name_Part_Info(size_t namePartId)
{
    ಹಿಂತಿರುಗಿ std::addressof(m_namePartInfos.at(namePartId));
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::Name_ಕೋಷ್ಟಕ::ಆಂತರಿಕ_ಕಂಡುಹಿಡಿ_Matching_Name_Parts_For_ತುಂಬಿದ_Name(ತುಂಬಿದ_Name_Lookup_ಫಲಿತಾಂಶ& ತುಂಬಿದ_Name_ಫಲಿತಾಂಶ)
{
    ಸ್ಥಿರಾಂಕ size_t ತುಂಬಿದ_Name_Id = ತುಂಬಿದ_Name_ಫಲಿತಾಂಶ.m_ತುಂಬಿದ_Name_Id;
    ತುಂಬಿದ_Name_Info* ತುಂಬಿದ_Name_Info_2 = ತುಂಬಿದ_Name_ಫಲಿತಾಂಶ.m_ತುಂಬಿದ_Name_Info_Ptr;
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& namePartIds = ತುಂಬಿದ_Name_ಫಲಿತಾಂಶ.m_namePartIds;
    ಸ್ಥಿರಾಂಕ size_t name_Part_ಎಣಿಕೆ = namePartIds.size();
    ಸ್ವಯಂಚಾಲಿತ& crossReferences = ತುಂಬಿದ_Name_ಫಲಿತಾಂಶ.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_ಎಣಿಕೆ);
    ಗಾಗಿ (size_t matchingPos = 0u; matchingPos < name_Part_ಎಣಿಕೆ; ++matchingPos)
    {
        ಸ್ಥಿರಾಂಕ size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = ಆಂತರಿಕ_get_Name_Part_Info(namePartId);
        crossReferences.emplace_back(CrossReference(ತುಂಬಿದ_Name_Id, namePartId, matchingPos, ತುಂಬಿದ_Name_Info_2, namePartInfo));
    }
}

ಶೂನ್ಯ Log_ಟ್ಯಾಗ್_Manager::Name_ಕೋಷ್ಟಕ::ಆಂತರಿಕ_ಕಂಡುಹಿಡಿ_Matching_ತುಂಬಿದ_ಹೆಸರುಗಳು_For_Name_Part(Name_Part_Lookup_ಫಲಿತಾಂಶ& ಫಲಿತಾಂಶ)
{
    ಸ್ಥಿರಾಂಕ size_t namePartId = ಫಲಿತಾಂಶ.m_namePartId;
    NamePartInfo* namePartInfo = ಫಲಿತಾಂಶ.m_namePartInfoPtr;
    ಸ್ಥಿರಾಂಕ size_t matching_ತುಂಬಿದ_Name_ಎಣಿಕೆ = m_name_Part_ಗಮ್ಯ_ತುಂಬಿದ_Name_Ids.ಎಣಿಕೆ_3(namePartId);
    std::vector<CrossReference>& crossReferences = ಫಲಿತಾಂಶ.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_ತುಂಬಿದ_Name_ಎಣಿಕೆ);
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ name_Part_ಗಮ್ಯ_ತುಂಬಿದ_Name_Iter_Pair = m_name_Part_ಗಮ್ಯ_ತುಂಬಿದ_Name_Ids.equal_range(ಫಲಿತಾಂಶ.m_namePartId);
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ iterBegin = name_Part_ಗಮ್ಯ_ತುಂಬಿದ_Name_Iter_Pair.first;
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ iter_ಕೊನೆ = name_Part_ಗಮ್ಯ_ತುಂಬಿದ_Name_Iter_Pair.second;
    ಗಾಗಿ (ಸ್ವಯಂಚಾಲಿತ iter = iterBegin; iter != iter_ಕೊನೆ; ++iter)
    {
        ಸ್ಥಿರಾಂಕ size_t ತುಂಬಿದ_Name_Id = iter->second.first;
        ಸ್ಥಿರಾಂಕ size_t matchingPos = iter->second.second;
        ತುಂಬಿದ_Name_Info* ತುಂಬಿದ_Name_Info_2 = ಆಂತರಿಕ_get_ತುಂಬಿದ_Name_Info(ತುಂಬಿದ_Name_Id);
        crossReferences.emplace_back(CrossReference(ತುಂಬಿದ_Name_Id, namePartId, matchingPos, ತುಂಬಿದ_Name_Info_2, namePartInfo));
    }
}

}}} //namespace
