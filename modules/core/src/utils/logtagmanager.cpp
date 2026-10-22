// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "../precomp.hpp"
#అన్తర్భావ "logtagmanager.hpp"
#అన్తర్భావ "logtagconfigparser.hpp"

నామాకాశ cv {
నామాకాశ utils {
నామాకాశ logging {


స్థిరరాశి అక్షరం* Log_ట్యాగ్_Manager::m_globalName = "global";


Log_ట్యాగ్_Manager::Log_ట్యాగ్_Manager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_global_Log_ట్యాగ్(కొత్త Log_ట్యాగ్(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<Log_ట్యాగ్_Config_Parser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_global_Log_ట్యాగ్.get());
}

Log_ట్యాగ్_Manager::~Log_ట్యాగ్_Manager()
{
}

శూన్య Log_ట్యాగ్_Manager::సమితి_Config_String(స్థిరరాశి std::string& configString, తార్కిక apply /*true*/)
{
    m_config->parse(configString);
    ఒకవేళ (m_config->hasMalformed())
    {
        తిరిగి;
    }
    ఒకవేళ (!apply)
    {
        తిరిగి;
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
    స్థిరరాశి స్వయంచాలక& globalConfig = m_config->getGlobalConfig();
    m_global_Log_ట్యాగ్->level = globalConfig.level;
    కొరకు (స్థిరరాశి స్వయంచాలక& config : m_config->get_మొదటి_Part_Configs())
    {
        సమితి_Level_By_మొదటి_Part(config.namePart, config.level);
    }
    కొరకు (స్థిరరాశి స్వయంచాలక& config : m_config->getAnyPartConfigs())
    {
        సమితి_Level_By_Any_Part(config.namePart, config.level);
    }
    కొరకు (స్థిరరాశి స్వయంచాలక& config : m_config->get_నిండిన_Name_Configs())
    {
        సమితి_Level_By_నిండిన_Name(config.namePart, config.level);
    }
}

Log_ట్యాగ్_Config_Parser& Log_ట్యాగ్_Manager::getConfigParser() స్థిరరాశి
{
    తిరిగి *m_config;
}

శూన్య Log_ట్యాగ్_Manager::assign(స్థిరరాశి std::string& నిండిన_Name, Log_ట్యాగ్* ptr)
{
    CV_TRACE_FUNCTION();
    తాళం_Type lock(m_mutex);
    నిండిన_Name_Lookup_ఫలితం ఫలితం(నిండిన_Name);
    ఫలితం.m_కనుగొను_Cross_References = నిజం;
    m_name_పట్టిక.కూడు_Or_Lookup_నిండిన_Name(ఫలితం);
    నిండిన_Name_Info& నిండిన_Name_Info_2 = *ఫలితం.m_నిండిన_Name_Info_Ptr;
    స్థిరరాశి తార్కిక is_Ptr_మారింది = (నిండిన_Name_Info_2.log_ట్యాగ్_Ptr != ptr);
    ఒకవేళ (!is_Ptr_మారింది)
    {
        తిరిగి;
    }
    నిండిన_Name_Info_2.log_ట్యాగ్_Ptr = ptr;
    ఒకవేళ (!ptr)
    {
        తిరిగి;
    }
    స్థిరరాశి తార్కిక has_Applied_నిండిన_Name_Config = అంతర్గత_apply_నిండిన_Name_Config_గమ్యం_ట్యాగ్(నిండిన_Name_Info_2);
    ఒకవేళ (has_Applied_నిండిన_Name_Config)
    {
        తిరిగి;
    }
    అంతర్గత_apply_Name_Part_Config_గమ్యం_Specific_ట్యాగ్(ఫలితం);
}

శూన్య Log_ట్యాగ్_Manager::unassign(స్థిరరాశి std::string& నిండిన_Name)
{
    // Lock is inside assign() method.
    assign(నిండిన_Name, శూన్యసూచిక);
}

Log_ట్యాగ్* Log_ట్యాగ్_Manager::get(స్థిరరాశి std::string& నిండిన_Name)
{
    CV_TRACE_FUNCTION();
    తాళం_Type lock(m_mutex);
    నిండిన_Name_Info* నిండిన_Name_Info_Ptr = m_name_పట్టిక.get_నిండిన_Name_Info(నిండిన_Name);
    ఒకవేళ (నిండిన_Name_Info_Ptr && నిండిన_Name_Info_Ptr->log_ట్యాగ్_Ptr)
    {
        తిరిగి నిండిన_Name_Info_Ptr->log_ట్యాగ్_Ptr;
    }
    తిరిగి శూన్యసూచిక;
}

శూన్య Log_ట్యాగ్_Manager::సమితి_Level_By_నిండిన_Name(స్థిరరాశి std::string& నిండిన_Name, LogLevel level)
{
    CV_TRACE_FUNCTION();
    తాళం_Type lock(m_mutex);
    నిండిన_Name_Lookup_ఫలితం ఫలితం(నిండిన_Name);
    ఫలితం.m_కనుగొను_Cross_References = అబద్ధం;
    m_name_పట్టిక.కూడు_Or_Lookup_నిండిన_Name(ఫలితం);
    నిండిన_Name_Info& నిండిన_Name_Info_2 = *ఫలితం.m_నిండిన_Name_Info_Ptr;
    ఒకవేళ (నిండిన_Name_Info_2.parsedLevel.scope == MatchingScope::నిండిన_2 &&
        నిండిన_Name_Info_2.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        తిరిగి;
    }
    // update the cached configured value.
    నిండిన_Name_Info_2.parsedLevel.scope = MatchingScope::నిండిన_2;
    నిండిన_Name_Info_2.parsedLevel.level = level;
    // update the actual tag, if already registered.
    Log_ట్యాగ్* log_ట్యాగ్_Ptr = నిండిన_Name_Info_2.log_ట్యాగ్_Ptr;
    ఒకవేళ (log_ట్యాగ్_Ptr)
    {
        log_ట్యాగ్_Ptr->level = level;
    }
}

శూన్య Log_ట్యాగ్_Manager::సమితి_Level_By_మొదటి_Part(స్థిరరాశి std::string& మొదటి_Part, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    సమితి_Level_By_Name_Part(మొదటి_Part, level, MatchingScope::మొదటి_Name_Part);
}

శూన్య Log_ట్యాగ్_Manager::సమితి_Level_By_Any_Part(స్థిరరాశి std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    సమితి_Level_By_Name_Part(anyPart, level, MatchingScope::AnyNamePart);
}

శూన్య Log_ట్యాగ్_Manager::సమితి_Level_By_Name_Part(స్థిరరాశి std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    తాళం_Type lock(m_mutex);
    Name_Part_Lookup_ఫలితం ఫలితం(namePart);
    ఫలితం.m_కనుగొను_Cross_References = నిజం;
    m_name_పట్టిక.కూడు_Or_Lookup_Name_Part(ఫలితం);
    NamePartInfo& namePartInfo = *ఫలితం.m_namePartInfoPtr;
    ఒకవేళ (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        తిరిగి;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    అంతర్గత_apply_Name_Part_Config_గమ్యం_Matching_ట్యాగులు(ఫలితం);
}

std::vector<std::string> Log_ట్యాగ్_Manager::విభజించు_Name_Parts(స్థిరరాశి std::string& నిండిన_Name)
{
    స్థిరరాశి size_t npos = std::string::npos;
    స్థిరరాశి size_t len = నిండిన_Name.length();
    std::vector<std::string> nameParts;
    size_t ప్రారంభం = 0u;
    అయితే (ప్రారంభం < len)
    {
        size_t తదుపరి_Period = నిండిన_Name.find('.', ప్రారంభం);
        ఒకవేళ (తదుపరి_Period == npos)
        {
            తదుపరి_Period = len;
        }
        ఒకవేళ (తదుపరి_Period >= ప్రారంభం + 1u)
        {
            nameParts.emplace_back(నిండిన_Name.substr(ప్రారంభం, తదుపరి_Period - ప్రారంభం));
        }
        ప్రారంభం = తదుపరి_Period + 1u;
    }
    తిరిగి nameParts;
}

తార్కిక Log_ట్యాగ్_Manager::అంతర్గత_is_Name_Part_సరిపోలిక(MatchingScope scope, size_t matchingPos)
{
    ఎంపిక (scope)
    {
    స్థితి MatchingScope::మొదటి_Name_Part:
        తిరిగి (matchingPos == 0u);
    స్థితి MatchingScope::AnyNamePart:
        తిరిగి నిజం;
    స్థితి MatchingScope::None:
    స్థితి MatchingScope::నిండిన_2:
    అప్రమేయ:
        తిరిగి అబద్ధం;
    }
}

తార్కిక Log_ట్యాగ్_Manager::అంతర్గత_apply_నిండిన_Name_Config_గమ్యం_ట్యాగ్(నిండిన_Name_Info& నిండిన_Name_Info_2)
{
    ఒకవేళ (!నిండిన_Name_Info_2.log_ట్యాగ్_Ptr)
    {
        తిరిగి అబద్ధం;
    }
    ఒకవేళ (నిండిన_Name_Info_2.parsedLevel.scope == MatchingScope::నిండిన_2)
    {
        నిండిన_Name_Info_2.log_ట్యాగ్_Ptr->level = నిండిన_Name_Info_2.parsedLevel.level;
        తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
}

తార్కిక Log_ట్యాగ్_Manager::అంతర్గత_apply_Name_Part_Config_గమ్యం_Specific_ట్యాగ్(నిండిన_Name_Lookup_ఫలితం& నిండిన_Name_ఫలితం)
{
    స్థిరరాశి నిండిన_Name_Info& నిండిన_Name_Info_2 = *నిండిన_Name_ఫలితం.m_నిండిన_Name_Info_Ptr;
    Log_ట్యాగ్* స్థిరరాశి log_ట్యాగ్ = నిండిన_Name_Info_2.log_ట్యాగ్_Ptr;
    ఒకవేళ (!log_ట్యాగ్)
    {
        తిరిగి అబద్ధం;
    }
    CV_Assert(నిండిన_Name_ఫలితం.m_కనుగొను_Cross_References);
    స్థిరరాశి స్వయంచాలక& crossReferences = నిండిన_Name_ఫలితం.m_crossReferences;
    స్థిరరాశి size_t matching_Name_Part_లెక్క = crossReferences.size();
    కొరకు (size_t k = 0u; k < matching_Name_Part_లెక్క; ++k)
    {
        స్థిరరాశి స్వయంచాలక& సరిపోలిక = crossReferences.at(k);
        స్థిరరాశి స్వయంచాలక& namePartInfo = *సరిపోలిక.m_namePartInfo;
        స్థిరరాశి స్వయంచాలక& parsedLevel = namePartInfo.parsedLevel;
        స్థిరరాశి స్వయంచాలక scope = parsedLevel.scope;
        స్థిరరాశి LogLevel level = parsedLevel.level;
        స్థిరరాశి size_t matchingPos = సరిపోలిక.m_matchingPos;
        స్థిరరాశి తార్కిక is_సరిపోలిక = అంతర్గత_is_Name_Part_సరిపోలిక(scope, matchingPos);
        ఒకవేళ (is_సరిపోలిక)
        {
            log_ట్యాగ్->level = level;
            తిరిగి నిజం;
        }
    }
    తిరిగి అబద్ధం;
}

శూన్య Log_ట్యాగ్_Manager::అంతర్గత_apply_Name_Part_Config_గమ్యం_Matching_ట్యాగులు(Name_Part_Lookup_ఫలితం& name_Part_ఫలితం)
{
    CV_Assert(name_Part_ఫలితం.m_కనుగొను_Cross_References);
    స్థిరరాశి స్వయంచాలక& crossReferences = name_Part_ఫలితం.m_crossReferences;
    స్థిరరాశి size_t matching_నిండిన_Name_లెక్క = crossReferences.size();
    NamePartInfo& namePartInfo = *name_Part_ఫలితం.m_namePartInfoPtr;
    స్థిరరాశి MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::నిండిన_2);
    ఒకవేళ (scope == MatchingScope::None)
    {
        తిరిగి;
    }
    స్థిరరాశి LogLevel level = namePartInfo.parsedLevel.level;
    కొరకు (size_t k = 0u; k < matching_నిండిన_Name_లెక్క; ++k)
    {
        స్థిరరాశి స్వయంచాలక& సరిపోలిక = crossReferences.at(k);
        నిండిన_Name_Info& నిండిన_Name_Info_2 = *సరిపోలిక.m_నిండిన_Name_Info;
        Log_ట్యాగ్* log_ట్యాగ్_Ptr = నిండిన_Name_Info_2.log_ట్యాగ్_Ptr;
        ఒకవేళ (!log_ట్యాగ్_Ptr)
        {
            కొనసాగు;
        }
        ఒకవేళ (నిండిన_Name_Info_2.parsedLevel.scope == MatchingScope::నిండిన_2)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            కొనసాగు;
        }
        స్థిరరాశి size_t matchingPos = సరిపోలిక.m_matchingPos;
        స్థిరరాశి తార్కిక is_సరిపోలిక = అంతర్గత_is_Name_Part_సరిపోలిక(scope, matchingPos);
        ఒకవేళ (!is_సరిపోలిక)
        {
            కొనసాగు;
        }
        log_ట్యాగ్_Ptr->level = level;
    }
}

శూన్య Log_ట్యాగ్_Manager::Name_పట్టిక::కూడు_Or_Lookup_నిండిన_Name(నిండిన_Name_Lookup_ఫలితం& ఫలితం)
{
    స్థిరరాశి స్వయంచాలక నిండిన_Name_Id_And_Flag = అంతర్గత_కూడు_Or_Lookup_నిండిన_Name(ఫలితం.m_నిండిన_Name);
    ఫలితం.m_నిండిన_Name_Id = నిండిన_Name_Id_And_Flag.first;
    ఫలితం.m_nameParts = Log_ట్యాగ్_Manager::విభజించు_Name_Parts(ఫలితం.m_నిండిన_Name);
    అంతర్గత_కూడు_Or_Lookup_Name_Parts(ఫలితం.m_nameParts, ఫలితం.m_namePartIds);
    స్థిరరాశి తార్కిక isNew = నిండిన_Name_Id_And_Flag.second;
    ఒకవేళ (isNew)
    {
        అంతర్గత_కూడు_Cross_Reference(ఫలితం.m_నిండిన_Name_Id, ఫలితం.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    ఫలితం.m_నిండిన_Name_Info_Ptr = అంతర్గత_get_నిండిన_Name_Info(ఫలితం.m_నిండిన_Name_Id);
    ఒకవేళ (ఫలితం.m_కనుగొను_Cross_References)
    {
        అంతర్గత_కనుగొను_Matching_Name_Parts_For_నిండిన_Name(ఫలితం);
    }
}

శూన్య Log_ట్యాగ్_Manager::Name_పట్టిక::కూడు_Or_Lookup_Name_Part(Name_Part_Lookup_ఫలితం& ఫలితం)
{
    ఫలితం.m_namePartId = అంతర్గత_కూడు_Or_Lookup_Name_Part(ఫలితం.m_namePart);
    ఫలితం.m_namePartInfoPtr = అంతర్గత_get_Name_Part_Info(ఫలితం.m_namePartId);
    ఒకవేళ (ఫలితం.m_కనుగొను_Cross_References)
    {
        అంతర్గత_కనుగొను_Matching_నిండిన_పేర్లు_For_Name_Part(ఫలితం);
    }
}

std::pair<size_t, తార్కిక> Log_ట్యాగ్_Manager::Name_పట్టిక::అంతర్గత_కూడు_Or_Lookup_నిండిన_Name(స్థిరరాశి std::string& నిండిన_Name)
{
    స్థిరరాశి స్వయంచాలక నిండిన_Name_Id_Iter = m_నిండిన_Name_Ids.find(నిండిన_Name);
    ఒకవేళ (నిండిన_Name_Id_Iter != m_నిండిన_Name_Ids.end())
    {
        తిరిగి std::make_pair(నిండిన_Name_Id_Iter->second, అబద్ధం);
    }
    స్థిరరాశి size_t నిండిన_Name_Id = m_నిండిన_Name_Infos.size();
    m_నిండిన_Name_Infos.emplace_back(నిండిన_Name_Info{});
    m_నిండిన_Name_Ids.emplace(నిండిన_Name, నిండిన_Name_Id);
    తిరిగి std::make_pair(నిండిన_Name_Id, నిజం);
}

శూన్య Log_ట్యాగ్_Manager::Name_పట్టిక::అంతర్గత_కూడు_Or_Lookup_Name_Parts(స్థిరరాశి std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    స్థిరరాశి size_t name_Part_లెక్క = nameParts.size();
    namePartIds.resize(name_Part_లెక్క, ~(size_t)0u);
    కొరకు (size_t name_Part_సూచీ = 0u; name_Part_సూచీ < name_Part_లెక్క; ++name_Part_సూచీ)
    {
        స్థిరరాశి std::string& namePart = nameParts.at(name_Part_సూచీ);
        స్థిరరాశి size_t namePartId = అంతర్గత_కూడు_Or_Lookup_Name_Part(namePart);
        namePartIds.at(name_Part_సూచీ) = namePartId;
    }
}

size_t Log_ట్యాగ్_Manager::Name_పట్టిక::అంతర్గత_కూడు_Or_Lookup_Name_Part(స్థిరరాశి std::string& namePart)
{
    స్థిరరాశి స్వయంచాలక namePartIter = m_namePartIds.find(namePart);
    ఒకవేళ (namePartIter != m_namePartIds.end())
    {
        తిరిగి namePartIter->second;
    }
    స్థిరరాశి size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    తిరిగి namePartId;
}

శూన్య Log_ట్యాగ్_Manager::Name_పట్టిక::అంతర్గత_కూడు_Cross_Reference(size_t నిండిన_Name_Id, స్థిరరాశి std::vector<size_t>& namePartIds)
{
    స్థిరరాశి size_t name_Part_లెక్క = namePartIds.size();
    కొరకు (size_t namePartPos = 0u; namePartPos < name_Part_లెక్క; ++namePartPos)
    {
        స్థిరరాశి size_t namePartId = namePartIds.at(namePartPos);
        m_నిండిన_Name_గమ్యం_Name_Part_Ids.emplace(నిండిన_Name_Id, std::make_pair(namePartId, namePartPos));
        m_name_Part_గమ్యం_నిండిన_Name_Ids.emplace(namePartId, std::make_pair(నిండిన_Name_Id, namePartPos));
    }
}

Log_ట్యాగ్_Manager::నిండిన_Name_Info* Log_ట్యాగ్_Manager::Name_పట్టిక::get_నిండిన_Name_Info(స్థిరరాశి std::string& నిండిన_Name)
{
    స్థిరరాశి స్వయంచాలక నిండిన_Name_Id_Iter = m_నిండిన_Name_Ids.find(నిండిన_Name);
    ఒకవేళ (నిండిన_Name_Id_Iter == m_నిండిన_Name_Ids.end())
    {
        తిరిగి శూన్యసూచిక;
    }
    స్థిరరాశి size_t నిండిన_Name_Id = నిండిన_Name_Id_Iter->second;
    తిరిగి అంతర్గత_get_నిండిన_Name_Info(నిండిన_Name_Id);
}

Log_ట్యాగ్_Manager::నిండిన_Name_Info* Log_ట్యాగ్_Manager::Name_పట్టిక::అంతర్గత_get_నిండిన_Name_Info(size_t నిండిన_Name_Id)
{
    తిరిగి std::addressof(m_నిండిన_Name_Infos.at(నిండిన_Name_Id));
}

Log_ట్యాగ్_Manager::NamePartInfo* Log_ట్యాగ్_Manager::Name_పట్టిక::అంతర్గత_get_Name_Part_Info(size_t namePartId)
{
    తిరిగి std::addressof(m_namePartInfos.at(namePartId));
}

శూన్య Log_ట్యాగ్_Manager::Name_పట్టిక::అంతర్గత_కనుగొను_Matching_Name_Parts_For_నిండిన_Name(నిండిన_Name_Lookup_ఫలితం& నిండిన_Name_ఫలితం)
{
    స్థిరరాశి size_t నిండిన_Name_Id = నిండిన_Name_ఫలితం.m_నిండిన_Name_Id;
    నిండిన_Name_Info* నిండిన_Name_Info_2 = నిండిన_Name_ఫలితం.m_నిండిన_Name_Info_Ptr;
    స్థిరరాశి స్వయంచాలక& namePartIds = నిండిన_Name_ఫలితం.m_namePartIds;
    స్థిరరాశి size_t name_Part_లెక్క = namePartIds.size();
    స్వయంచాలక& crossReferences = నిండిన_Name_ఫలితం.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_లెక్క);
    కొరకు (size_t matchingPos = 0u; matchingPos < name_Part_లెక్క; ++matchingPos)
    {
        స్థిరరాశి size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = అంతర్గత_get_Name_Part_Info(namePartId);
        crossReferences.emplace_back(CrossReference(నిండిన_Name_Id, namePartId, matchingPos, నిండిన_Name_Info_2, namePartInfo));
    }
}

శూన్య Log_ట్యాగ్_Manager::Name_పట్టిక::అంతర్గత_కనుగొను_Matching_నిండిన_పేర్లు_For_Name_Part(Name_Part_Lookup_ఫలితం& ఫలితం)
{
    స్థిరరాశి size_t namePartId = ఫలితం.m_namePartId;
    NamePartInfo* namePartInfo = ఫలితం.m_namePartInfoPtr;
    స్థిరరాశి size_t matching_నిండిన_Name_లెక్క = m_name_Part_గమ్యం_నిండిన_Name_Ids.లెక్క_2(namePartId);
    std::vector<CrossReference>& crossReferences = ఫలితం.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_నిండిన_Name_లెక్క);
    స్థిరరాశి స్వయంచాలక name_Part_గమ్యం_నిండిన_Name_Iter_Pair = m_name_Part_గమ్యం_నిండిన_Name_Ids.equal_range(ఫలితం.m_namePartId);
    స్థిరరాశి స్వయంచాలక iterBegin = name_Part_గమ్యం_నిండిన_Name_Iter_Pair.first;
    స్థిరరాశి స్వయంచాలక iter_చివర = name_Part_గమ్యం_నిండిన_Name_Iter_Pair.second;
    కొరకు (స్వయంచాలక iter = iterBegin; iter != iter_చివర; ++iter)
    {
        స్థిరరాశి size_t నిండిన_Name_Id = iter->second.first;
        స్థిరరాశి size_t matchingPos = iter->second.second;
        నిండిన_Name_Info* నిండిన_Name_Info_2 = అంతర్గత_get_నిండిన_Name_Info(నిండిన_Name_Id);
        crossReferences.emplace_back(CrossReference(నిండిన_Name_Id, namePartId, matchingPos, నిండిన_Name_Info_2, namePartInfo));
    }
}

}}} //namespace
