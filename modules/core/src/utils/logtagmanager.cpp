// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "../precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "logtagmanager.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "logtagconfigparser.hpp"

ਨਾਮਾਕਾਸ਼ cv {
ਨਾਮਾਕਾਸ਼ utils {
ਨਾਮਾਕਾਸ਼ logging {


ਅਚਲ ਅੱਖਰ* Log_ਟੈਗ_Manager::m_globalName = "global";


Log_ਟੈਗ_Manager::Log_ਟੈਗ_Manager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_global_Log_ਟੈਗ(ਨਵਾਂ Log_ਟੈਗ(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<Log_ਟੈਗ_Config_Parser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_global_Log_ਟੈਗ.get());
}

Log_ਟੈਗ_Manager::~Log_ਟੈਗ_Manager()
{
}

ਖਾਲੀ Log_ਟੈਗ_Manager::ਸੈੱਟ_Config_String(ਅਚਲ std::string& configString, ਤਾਰਕਿਕ apply /*true*/)
{
    m_config->parse(configString);
    ਜੇ (m_config->hasMalformed())
    {
        ਵਾਪਸ;
    }
    ਜੇ (!apply)
    {
        ਵਾਪਸ;
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
    ਅਚਲ ਸਵੈਚਾਲਿਤ& globalConfig = m_config->getGlobalConfig();
    m_global_Log_ਟੈਗ->level = globalConfig.level;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& config : m_config->get_ਪਹਿਲਾ_Part_Configs())
    {
        ਸੈੱਟ_Level_By_ਪਹਿਲਾ_Part(config.namePart, config.level);
    }
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& config : m_config->getAnyPartConfigs())
    {
        ਸੈੱਟ_Level_By_Any_Part(config.namePart, config.level);
    }
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& config : m_config->get_ਭਰਿਆ_Name_Configs())
    {
        ਸੈੱਟ_Level_By_ਭਰਿਆ_Name(config.namePart, config.level);
    }
}

Log_ਟੈਗ_Config_Parser& Log_ਟੈਗ_Manager::getConfigParser() ਅਚਲ
{
    ਵਾਪਸ *m_config;
}

ਖਾਲੀ Log_ਟੈਗ_Manager::assign(ਅਚਲ std::string& ਭਰਿਆ_Name, Log_ਟੈਗ* ptr)
{
    CV_TRACE_FUNCTION();
    ਤਾਲਾ_Type lock(m_mutex);
    ਭਰਿਆ_Name_Lookup_ਨਤੀਜਾ ਨਤੀਜਾ(ਭਰਿਆ_Name);
    ਨਤੀਜਾ.m_ਲੱਭੋ_Cross_References = ਸੱਚ;
    m_name_ਸਾਰਣੀ.ਜੋੜੋ_Or_Lookup_ਭਰਿਆ_Name(ਨਤੀਜਾ);
    ਭਰਿਆ_Name_Info& ਭਰਿਆ_Name_Info_2 = *ਨਤੀਜਾ.m_ਭਰਿਆ_Name_Info_Ptr;
    ਅਚਲ ਤਾਰਕਿਕ is_Ptr_ਬਦਲਿਆ = (ਭਰਿਆ_Name_Info_2.log_ਟੈਗ_Ptr != ptr);
    ਜੇ (!is_Ptr_ਬਦਲਿਆ)
    {
        ਵਾਪਸ;
    }
    ਭਰਿਆ_Name_Info_2.log_ਟੈਗ_Ptr = ptr;
    ਜੇ (!ptr)
    {
        ਵਾਪਸ;
    }
    ਅਚਲ ਤਾਰਕਿਕ has_Applied_ਭਰਿਆ_Name_Config = ਅੰਦਰੂਨੀ_apply_ਭਰਿਆ_Name_Config_ਮੰਜ਼ਿਲ_ਟੈਗ(ਭਰਿਆ_Name_Info_2);
    ਜੇ (has_Applied_ਭਰਿਆ_Name_Config)
    {
        ਵਾਪਸ;
    }
    ਅੰਦਰੂਨੀ_apply_Name_Part_Config_ਮੰਜ਼ਿਲ_Specific_ਟੈਗ(ਨਤੀਜਾ);
}

ਖਾਲੀ Log_ਟੈਗ_Manager::unassign(ਅਚਲ std::string& ਭਰਿਆ_Name)
{
    // Lock is inside assign() method.
    assign(ਭਰਿਆ_Name, ਖਾਲੀ_ਸੂਚਕ);
}

Log_ਟੈਗ* Log_ਟੈਗ_Manager::get(ਅਚਲ std::string& ਭਰਿਆ_Name)
{
    CV_TRACE_FUNCTION();
    ਤਾਲਾ_Type lock(m_mutex);
    ਭਰਿਆ_Name_Info* ਭਰਿਆ_Name_Info_Ptr = m_name_ਸਾਰਣੀ.get_ਭਰਿਆ_Name_Info(ਭਰਿਆ_Name);
    ਜੇ (ਭਰਿਆ_Name_Info_Ptr && ਭਰਿਆ_Name_Info_Ptr->log_ਟੈਗ_Ptr)
    {
        ਵਾਪਸ ਭਰਿਆ_Name_Info_Ptr->log_ਟੈਗ_Ptr;
    }
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
}

ਖਾਲੀ Log_ਟੈਗ_Manager::ਸੈੱਟ_Level_By_ਭਰਿਆ_Name(ਅਚਲ std::string& ਭਰਿਆ_Name, LogLevel level)
{
    CV_TRACE_FUNCTION();
    ਤਾਲਾ_Type lock(m_mutex);
    ਭਰਿਆ_Name_Lookup_ਨਤੀਜਾ ਨਤੀਜਾ(ਭਰਿਆ_Name);
    ਨਤੀਜਾ.m_ਲੱਭੋ_Cross_References = ਝੂਠ;
    m_name_ਸਾਰਣੀ.ਜੋੜੋ_Or_Lookup_ਭਰਿਆ_Name(ਨਤੀਜਾ);
    ਭਰਿਆ_Name_Info& ਭਰਿਆ_Name_Info_2 = *ਨਤੀਜਾ.m_ਭਰਿਆ_Name_Info_Ptr;
    ਜੇ (ਭਰਿਆ_Name_Info_2.parsedLevel.scope == MatchingScope::ਭਰਿਆ_2 &&
        ਭਰਿਆ_Name_Info_2.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        ਵਾਪਸ;
    }
    // update the cached configured value.
    ਭਰਿਆ_Name_Info_2.parsedLevel.scope = MatchingScope::ਭਰਿਆ_2;
    ਭਰਿਆ_Name_Info_2.parsedLevel.level = level;
    // update the actual tag, if already registered.
    Log_ਟੈਗ* log_ਟੈਗ_Ptr = ਭਰਿਆ_Name_Info_2.log_ਟੈਗ_Ptr;
    ਜੇ (log_ਟੈਗ_Ptr)
    {
        log_ਟੈਗ_Ptr->level = level;
    }
}

ਖਾਲੀ Log_ਟੈਗ_Manager::ਸੈੱਟ_Level_By_ਪਹਿਲਾ_Part(ਅਚਲ std::string& ਪਹਿਲਾ_Part, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    ਸੈੱਟ_Level_By_Name_Part(ਪਹਿਲਾ_Part, level, MatchingScope::ਪਹਿਲਾ_Name_Part);
}

ਖਾਲੀ Log_ਟੈਗ_Manager::ਸੈੱਟ_Level_By_Any_Part(ਅਚਲ std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    ਸੈੱਟ_Level_By_Name_Part(anyPart, level, MatchingScope::AnyNamePart);
}

ਖਾਲੀ Log_ਟੈਗ_Manager::ਸੈੱਟ_Level_By_Name_Part(ਅਚਲ std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    ਤਾਲਾ_Type lock(m_mutex);
    Name_Part_Lookup_ਨਤੀਜਾ ਨਤੀਜਾ(namePart);
    ਨਤੀਜਾ.m_ਲੱਭੋ_Cross_References = ਸੱਚ;
    m_name_ਸਾਰਣੀ.ਜੋੜੋ_Or_Lookup_Name_Part(ਨਤੀਜਾ);
    NamePartInfo& namePartInfo = *ਨਤੀਜਾ.m_namePartInfoPtr;
    ਜੇ (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        ਵਾਪਸ;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    ਅੰਦਰੂਨੀ_apply_Name_Part_Config_ਮੰਜ਼ਿਲ_Matching_ਟੈਗ_ਸੂਚੀ(ਨਤੀਜਾ);
}

std::vector<std::string> Log_ਟੈਗ_Manager::ਵੰਡੋ_Name_Parts(ਅਚਲ std::string& ਭਰਿਆ_Name)
{
    ਅਚਲ size_t npos = std::string::npos;
    ਅਚਲ size_t len = ਭਰਿਆ_Name.length();
    std::vector<std::string> nameParts;
    size_t ਸ਼ੁਰੂ = 0u;
    ਜਦੋਂ (ਸ਼ੁਰੂ < len)
    {
        size_t ਅਗਲਾ_Period = ਭਰਿਆ_Name.find('.', ਸ਼ੁਰੂ);
        ਜੇ (ਅਗਲਾ_Period == npos)
        {
            ਅਗਲਾ_Period = len;
        }
        ਜੇ (ਅਗਲਾ_Period >= ਸ਼ੁਰੂ + 1u)
        {
            nameParts.emplace_back(ਭਰਿਆ_Name.substr(ਸ਼ੁਰੂ, ਅਗਲਾ_Period - ਸ਼ੁਰੂ));
        }
        ਸ਼ੁਰੂ = ਅਗਲਾ_Period + 1u;
    }
    ਵਾਪਸ nameParts;
}

ਤਾਰਕਿਕ Log_ਟੈਗ_Manager::ਅੰਦਰੂਨੀ_is_Name_Part_ਮੇਲ(MatchingScope scope, size_t matchingPos)
{
    ਚੋਣ (scope)
    {
    ਹਾਲਤ MatchingScope::ਪਹਿਲਾ_Name_Part:
        ਵਾਪਸ (matchingPos == 0u);
    ਹਾਲਤ MatchingScope::AnyNamePart:
        ਵਾਪਸ ਸੱਚ;
    ਹਾਲਤ MatchingScope::None:
    ਹਾਲਤ MatchingScope::ਭਰਿਆ_2:
    ਮੂਲ:
        ਵਾਪਸ ਝੂਠ;
    }
}

ਤਾਰਕਿਕ Log_ਟੈਗ_Manager::ਅੰਦਰੂਨੀ_apply_ਭਰਿਆ_Name_Config_ਮੰਜ਼ਿਲ_ਟੈਗ(ਭਰਿਆ_Name_Info& ਭਰਿਆ_Name_Info_2)
{
    ਜੇ (!ਭਰਿਆ_Name_Info_2.log_ਟੈਗ_Ptr)
    {
        ਵਾਪਸ ਝੂਠ;
    }
    ਜੇ (ਭਰਿਆ_Name_Info_2.parsedLevel.scope == MatchingScope::ਭਰਿਆ_2)
    {
        ਭਰਿਆ_Name_Info_2.log_ਟੈਗ_Ptr->level = ਭਰਿਆ_Name_Info_2.parsedLevel.level;
        ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
}

ਤਾਰਕਿਕ Log_ਟੈਗ_Manager::ਅੰਦਰੂਨੀ_apply_Name_Part_Config_ਮੰਜ਼ਿਲ_Specific_ਟੈਗ(ਭਰਿਆ_Name_Lookup_ਨਤੀਜਾ& ਭਰਿਆ_Name_ਨਤੀਜਾ)
{
    ਅਚਲ ਭਰਿਆ_Name_Info& ਭਰਿਆ_Name_Info_2 = *ਭਰਿਆ_Name_ਨਤੀਜਾ.m_ਭਰਿਆ_Name_Info_Ptr;
    Log_ਟੈਗ* ਅਚਲ log_ਟੈਗ = ਭਰਿਆ_Name_Info_2.log_ਟੈਗ_Ptr;
    ਜੇ (!log_ਟੈਗ)
    {
        ਵਾਪਸ ਝੂਠ;
    }
    CV_Assert(ਭਰਿਆ_Name_ਨਤੀਜਾ.m_ਲੱਭੋ_Cross_References);
    ਅਚਲ ਸਵੈਚਾਲਿਤ& crossReferences = ਭਰਿਆ_Name_ਨਤੀਜਾ.m_crossReferences;
    ਅਚਲ size_t matching_Name_Part_ਗਿਣਤੀ = crossReferences.size();
    ਲਈ (size_t k = 0u; k < matching_Name_Part_ਗਿਣਤੀ; ++k)
    {
        ਅਚਲ ਸਵੈਚਾਲਿਤ& ਮੇਲ = crossReferences.at(k);
        ਅਚਲ ਸਵੈਚਾਲਿਤ& namePartInfo = *ਮੇਲ.m_namePartInfo;
        ਅਚਲ ਸਵੈਚਾਲਿਤ& parsedLevel = namePartInfo.parsedLevel;
        ਅਚਲ ਸਵੈਚਾਲਿਤ scope = parsedLevel.scope;
        ਅਚਲ LogLevel level = parsedLevel.level;
        ਅਚਲ size_t matchingPos = ਮੇਲ.m_matchingPos;
        ਅਚਲ ਤਾਰਕਿਕ is_ਮੇਲ = ਅੰਦਰੂਨੀ_is_Name_Part_ਮੇਲ(scope, matchingPos);
        ਜੇ (is_ਮੇਲ)
        {
            log_ਟੈਗ->level = level;
            ਵਾਪਸ ਸੱਚ;
        }
    }
    ਵਾਪਸ ਝੂਠ;
}

ਖਾਲੀ Log_ਟੈਗ_Manager::ਅੰਦਰੂਨੀ_apply_Name_Part_Config_ਮੰਜ਼ਿਲ_Matching_ਟੈਗ_ਸੂਚੀ(Name_Part_Lookup_ਨਤੀਜਾ& name_Part_ਨਤੀਜਾ)
{
    CV_Assert(name_Part_ਨਤੀਜਾ.m_ਲੱਭੋ_Cross_References);
    ਅਚਲ ਸਵੈਚਾਲਿਤ& crossReferences = name_Part_ਨਤੀਜਾ.m_crossReferences;
    ਅਚਲ size_t matching_ਭਰਿਆ_Name_ਗਿਣਤੀ = crossReferences.size();
    NamePartInfo& namePartInfo = *name_Part_ਨਤੀਜਾ.m_namePartInfoPtr;
    ਅਚਲ MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::ਭਰਿਆ_2);
    ਜੇ (scope == MatchingScope::None)
    {
        ਵਾਪਸ;
    }
    ਅਚਲ LogLevel level = namePartInfo.parsedLevel.level;
    ਲਈ (size_t k = 0u; k < matching_ਭਰਿਆ_Name_ਗਿਣਤੀ; ++k)
    {
        ਅਚਲ ਸਵੈਚਾਲਿਤ& ਮੇਲ = crossReferences.at(k);
        ਭਰਿਆ_Name_Info& ਭਰਿਆ_Name_Info_2 = *ਮੇਲ.m_ਭਰਿਆ_Name_Info;
        Log_ਟੈਗ* log_ਟੈਗ_Ptr = ਭਰਿਆ_Name_Info_2.log_ਟੈਗ_Ptr;
        ਜੇ (!log_ਟੈਗ_Ptr)
        {
            ਜਾਰੀ;
        }
        ਜੇ (ਭਰਿਆ_Name_Info_2.parsedLevel.scope == MatchingScope::ਭਰਿਆ_2)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            ਜਾਰੀ;
        }
        ਅਚਲ size_t matchingPos = ਮੇਲ.m_matchingPos;
        ਅਚਲ ਤਾਰਕਿਕ is_ਮੇਲ = ਅੰਦਰੂਨੀ_is_Name_Part_ਮੇਲ(scope, matchingPos);
        ਜੇ (!is_ਮੇਲ)
        {
            ਜਾਰੀ;
        }
        log_ਟੈਗ_Ptr->level = level;
    }
}

ਖਾਲੀ Log_ਟੈਗ_Manager::Name_ਸਾਰਣੀ::ਜੋੜੋ_Or_Lookup_ਭਰਿਆ_Name(ਭਰਿਆ_Name_Lookup_ਨਤੀਜਾ& ਨਤੀਜਾ)
{
    ਅਚਲ ਸਵੈਚਾਲਿਤ ਭਰਿਆ_Name_Id_And_Flag = ਅੰਦਰੂਨੀ_ਜੋੜੋ_Or_Lookup_ਭਰਿਆ_Name(ਨਤੀਜਾ.m_ਭਰਿਆ_Name);
    ਨਤੀਜਾ.m_ਭਰਿਆ_Name_Id = ਭਰਿਆ_Name_Id_And_Flag.first;
    ਨਤੀਜਾ.m_nameParts = Log_ਟੈਗ_Manager::ਵੰਡੋ_Name_Parts(ਨਤੀਜਾ.m_ਭਰਿਆ_Name);
    ਅੰਦਰੂਨੀ_ਜੋੜੋ_Or_Lookup_Name_Parts(ਨਤੀਜਾ.m_nameParts, ਨਤੀਜਾ.m_namePartIds);
    ਅਚਲ ਤਾਰਕਿਕ isNew = ਭਰਿਆ_Name_Id_And_Flag.second;
    ਜੇ (isNew)
    {
        ਅੰਦਰੂਨੀ_ਜੋੜੋ_Cross_Reference(ਨਤੀਜਾ.m_ਭਰਿਆ_Name_Id, ਨਤੀਜਾ.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    ਨਤੀਜਾ.m_ਭਰਿਆ_Name_Info_Ptr = ਅੰਦਰੂਨੀ_get_ਭਰਿਆ_Name_Info(ਨਤੀਜਾ.m_ਭਰਿਆ_Name_Id);
    ਜੇ (ਨਤੀਜਾ.m_ਲੱਭੋ_Cross_References)
    {
        ਅੰਦਰੂਨੀ_ਲੱਭੋ_Matching_Name_Parts_For_ਭਰਿਆ_Name(ਨਤੀਜਾ);
    }
}

ਖਾਲੀ Log_ਟੈਗ_Manager::Name_ਸਾਰਣੀ::ਜੋੜੋ_Or_Lookup_Name_Part(Name_Part_Lookup_ਨਤੀਜਾ& ਨਤੀਜਾ)
{
    ਨਤੀਜਾ.m_namePartId = ਅੰਦਰੂਨੀ_ਜੋੜੋ_Or_Lookup_Name_Part(ਨਤੀਜਾ.m_namePart);
    ਨਤੀਜਾ.m_namePartInfoPtr = ਅੰਦਰੂਨੀ_get_Name_Part_Info(ਨਤੀਜਾ.m_namePartId);
    ਜੇ (ਨਤੀਜਾ.m_ਲੱਭੋ_Cross_References)
    {
        ਅੰਦਰੂਨੀ_ਲੱਭੋ_Matching_ਭਰਿਆ_ਨਾਂ_For_Name_Part(ਨਤੀਜਾ);
    }
}

std::pair<size_t, ਤਾਰਕਿਕ> Log_ਟੈਗ_Manager::Name_ਸਾਰਣੀ::ਅੰਦਰੂਨੀ_ਜੋੜੋ_Or_Lookup_ਭਰਿਆ_Name(ਅਚਲ std::string& ਭਰਿਆ_Name)
{
    ਅਚਲ ਸਵੈਚਾਲਿਤ ਭਰਿਆ_Name_Id_Iter = m_ਭਰਿਆ_Name_Ids.find(ਭਰਿਆ_Name);
    ਜੇ (ਭਰਿਆ_Name_Id_Iter != m_ਭਰਿਆ_Name_Ids.end())
    {
        ਵਾਪਸ std::make_pair(ਭਰਿਆ_Name_Id_Iter->second, ਝੂਠ);
    }
    ਅਚਲ size_t ਭਰਿਆ_Name_Id = m_ਭਰਿਆ_Name_Infos.size();
    m_ਭਰਿਆ_Name_Infos.emplace_back(ਭਰਿਆ_Name_Info{});
    m_ਭਰਿਆ_Name_Ids.emplace(ਭਰਿਆ_Name, ਭਰਿਆ_Name_Id);
    ਵਾਪਸ std::make_pair(ਭਰਿਆ_Name_Id, ਸੱਚ);
}

ਖਾਲੀ Log_ਟੈਗ_Manager::Name_ਸਾਰਣੀ::ਅੰਦਰੂਨੀ_ਜੋੜੋ_Or_Lookup_Name_Parts(ਅਚਲ std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    ਅਚਲ size_t name_Part_ਗਿਣਤੀ = nameParts.size();
    namePartIds.resize(name_Part_ਗਿਣਤੀ, ~(size_t)0u);
    ਲਈ (size_t name_Part_ਸੂਚਕਾਂਕ = 0u; name_Part_ਸੂਚਕਾਂਕ < name_Part_ਗਿਣਤੀ; ++name_Part_ਸੂਚਕਾਂਕ)
    {
        ਅਚਲ std::string& namePart = nameParts.at(name_Part_ਸੂਚਕਾਂਕ);
        ਅਚਲ size_t namePartId = ਅੰਦਰੂਨੀ_ਜੋੜੋ_Or_Lookup_Name_Part(namePart);
        namePartIds.at(name_Part_ਸੂਚਕਾਂਕ) = namePartId;
    }
}

size_t Log_ਟੈਗ_Manager::Name_ਸਾਰਣੀ::ਅੰਦਰੂਨੀ_ਜੋੜੋ_Or_Lookup_Name_Part(ਅਚਲ std::string& namePart)
{
    ਅਚਲ ਸਵੈਚਾਲਿਤ namePartIter = m_namePartIds.find(namePart);
    ਜੇ (namePartIter != m_namePartIds.end())
    {
        ਵਾਪਸ namePartIter->second;
    }
    ਅਚਲ size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    ਵਾਪਸ namePartId;
}

ਖਾਲੀ Log_ਟੈਗ_Manager::Name_ਸਾਰਣੀ::ਅੰਦਰੂਨੀ_ਜੋੜੋ_Cross_Reference(size_t ਭਰਿਆ_Name_Id, ਅਚਲ std::vector<size_t>& namePartIds)
{
    ਅਚਲ size_t name_Part_ਗਿਣਤੀ = namePartIds.size();
    ਲਈ (size_t namePartPos = 0u; namePartPos < name_Part_ਗਿਣਤੀ; ++namePartPos)
    {
        ਅਚਲ size_t namePartId = namePartIds.at(namePartPos);
        m_ਭਰਿਆ_Name_ਮੰਜ਼ਿਲ_Name_Part_Ids.emplace(ਭਰਿਆ_Name_Id, std::make_pair(namePartId, namePartPos));
        m_name_Part_ਮੰਜ਼ਿਲ_ਭਰਿਆ_Name_Ids.emplace(namePartId, std::make_pair(ਭਰਿਆ_Name_Id, namePartPos));
    }
}

Log_ਟੈਗ_Manager::ਭਰਿਆ_Name_Info* Log_ਟੈਗ_Manager::Name_ਸਾਰਣੀ::get_ਭਰਿਆ_Name_Info(ਅਚਲ std::string& ਭਰਿਆ_Name)
{
    ਅਚਲ ਸਵੈਚਾਲਿਤ ਭਰਿਆ_Name_Id_Iter = m_ਭਰਿਆ_Name_Ids.find(ਭਰਿਆ_Name);
    ਜੇ (ਭਰਿਆ_Name_Id_Iter == m_ਭਰਿਆ_Name_Ids.end())
    {
        ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    ਅਚਲ size_t ਭਰਿਆ_Name_Id = ਭਰਿਆ_Name_Id_Iter->second;
    ਵਾਪਸ ਅੰਦਰੂਨੀ_get_ਭਰਿਆ_Name_Info(ਭਰਿਆ_Name_Id);
}

Log_ਟੈਗ_Manager::ਭਰਿਆ_Name_Info* Log_ਟੈਗ_Manager::Name_ਸਾਰਣੀ::ਅੰਦਰੂਨੀ_get_ਭਰਿਆ_Name_Info(size_t ਭਰਿਆ_Name_Id)
{
    ਵਾਪਸ std::addressof(m_ਭਰਿਆ_Name_Infos.at(ਭਰਿਆ_Name_Id));
}

Log_ਟੈਗ_Manager::NamePartInfo* Log_ਟੈਗ_Manager::Name_ਸਾਰਣੀ::ਅੰਦਰੂਨੀ_get_Name_Part_Info(size_t namePartId)
{
    ਵਾਪਸ std::addressof(m_namePartInfos.at(namePartId));
}

ਖਾਲੀ Log_ਟੈਗ_Manager::Name_ਸਾਰਣੀ::ਅੰਦਰੂਨੀ_ਲੱਭੋ_Matching_Name_Parts_For_ਭਰਿਆ_Name(ਭਰਿਆ_Name_Lookup_ਨਤੀਜਾ& ਭਰਿਆ_Name_ਨਤੀਜਾ)
{
    ਅਚਲ size_t ਭਰਿਆ_Name_Id = ਭਰਿਆ_Name_ਨਤੀਜਾ.m_ਭਰਿਆ_Name_Id;
    ਭਰਿਆ_Name_Info* ਭਰਿਆ_Name_Info_2 = ਭਰਿਆ_Name_ਨਤੀਜਾ.m_ਭਰਿਆ_Name_Info_Ptr;
    ਅਚਲ ਸਵੈਚਾਲਿਤ& namePartIds = ਭਰਿਆ_Name_ਨਤੀਜਾ.m_namePartIds;
    ਅਚਲ size_t name_Part_ਗਿਣਤੀ = namePartIds.size();
    ਸਵੈਚਾਲਿਤ& crossReferences = ਭਰਿਆ_Name_ਨਤੀਜਾ.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_ਗਿਣਤੀ);
    ਲਈ (size_t matchingPos = 0u; matchingPos < name_Part_ਗਿਣਤੀ; ++matchingPos)
    {
        ਅਚਲ size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = ਅੰਦਰੂਨੀ_get_Name_Part_Info(namePartId);
        crossReferences.emplace_back(CrossReference(ਭਰਿਆ_Name_Id, namePartId, matchingPos, ਭਰਿਆ_Name_Info_2, namePartInfo));
    }
}

ਖਾਲੀ Log_ਟੈਗ_Manager::Name_ਸਾਰਣੀ::ਅੰਦਰੂਨੀ_ਲੱਭੋ_Matching_ਭਰਿਆ_ਨਾਂ_For_Name_Part(Name_Part_Lookup_ਨਤੀਜਾ& ਨਤੀਜਾ)
{
    ਅਚਲ size_t namePartId = ਨਤੀਜਾ.m_namePartId;
    NamePartInfo* namePartInfo = ਨਤੀਜਾ.m_namePartInfoPtr;
    ਅਚਲ size_t matching_ਭਰਿਆ_Name_ਗਿਣਤੀ = m_name_Part_ਮੰਜ਼ਿਲ_ਭਰਿਆ_Name_Ids.ਗਿਣਤੀ_3(namePartId);
    std::vector<CrossReference>& crossReferences = ਨਤੀਜਾ.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_ਭਰਿਆ_Name_ਗਿਣਤੀ);
    ਅਚਲ ਸਵੈਚਾਲਿਤ name_Part_ਮੰਜ਼ਿਲ_ਭਰਿਆ_Name_Iter_Pair = m_name_Part_ਮੰਜ਼ਿਲ_ਭਰਿਆ_Name_Ids.equal_range(ਨਤੀਜਾ.m_namePartId);
    ਅਚਲ ਸਵੈਚਾਲਿਤ iterBegin = name_Part_ਮੰਜ਼ਿਲ_ਭਰਿਆ_Name_Iter_Pair.first;
    ਅਚਲ ਸਵੈਚਾਲਿਤ iter_ਸਿਰਾ = name_Part_ਮੰਜ਼ਿਲ_ਭਰਿਆ_Name_Iter_Pair.second;
    ਲਈ (ਸਵੈਚਾਲਿਤ iter = iterBegin; iter != iter_ਸਿਰਾ; ++iter)
    {
        ਅਚਲ size_t ਭਰਿਆ_Name_Id = iter->second.first;
        ਅਚਲ size_t matchingPos = iter->second.second;
        ਭਰਿਆ_Name_Info* ਭਰਿਆ_Name_Info_2 = ਅੰਦਰੂਨੀ_get_ਭਰਿਆ_Name_Info(ਭਰਿਆ_Name_Id);
        crossReferences.emplace_back(CrossReference(ਭਰਿਆ_Name_Id, namePartId, matchingPos, ਭਰਿਆ_Name_Info_2, namePartInfo));
    }
}

}}} //namespace
