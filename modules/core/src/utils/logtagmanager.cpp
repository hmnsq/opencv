// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "../precomp.hpp"
#شامل "logtagmanager.hpp"
#شامل "logtagconfigparser.hpp"

نامخلا cv {
نامخلا utils {
نامخلا logging {


ثابت حرف* Log_لیبل_Manager::m_globalName = "global";


Log_لیبل_Manager::Log_لیبل_Manager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_global_Log_لیبل(نیا Log_لیبل(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<Log_لیبل_Config_Parser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_global_Log_لیبل.get());
}

Log_لیبل_Manager::~Log_لیبل_Manager()
{
}

خالی Log_لیبل_Manager::سیٹ_Config_String(ثابت std::string& configString, منطقی apply /*true*/)
{
    m_config->parse(configString);
    اگر (m_config->hasMalformed())
    {
        واپس;
    }
    اگر (!apply)
    {
        واپس;
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
    ثابت خودکار& globalConfig = m_config->getGlobalConfig();
    m_global_Log_لیبل->level = globalConfig.level;
    برائے (ثابت خودکار& config : m_config->get_پہلا_Part_Configs())
    {
        سیٹ_Level_By_پہلا_Part(config.namePart, config.level);
    }
    برائے (ثابت خودکار& config : m_config->getAnyPartConfigs())
    {
        سیٹ_Level_By_Any_Part(config.namePart, config.level);
    }
    برائے (ثابت خودکار& config : m_config->get_بھرا_Name_Configs())
    {
        سیٹ_Level_By_بھرا_Name(config.namePart, config.level);
    }
}

Log_لیبل_Config_Parser& Log_لیبل_Manager::getConfigParser() ثابت
{
    واپس *m_config;
}

خالی Log_لیبل_Manager::assign(ثابت std::string& بھرا_Name, Log_لیبل* ptr)
{
    CV_TRACE_FUNCTION();
    تالا_Type lock(m_mutex);
    بھرا_Name_Lookup_نتیجہ نتیجہ(بھرا_Name);
    نتیجہ.m_ڈھونڈو_Cross_References = سچ;
    m_name_جدول.جوڑو_Or_Lookup_بھرا_Name(نتیجہ);
    بھرا_Name_Info& بھرا_Name_Info_2 = *نتیجہ.m_بھرا_Name_Info_Ptr;
    ثابت منطقی is_Ptr_بدلا = (بھرا_Name_Info_2.log_لیبل_Ptr != ptr);
    اگر (!is_Ptr_بدلا)
    {
        واپس;
    }
    بھرا_Name_Info_2.log_لیبل_Ptr = ptr;
    اگر (!ptr)
    {
        واپس;
    }
    ثابت منطقی has_Applied_بھرا_Name_Config = اندرونی_apply_بھرا_Name_Config_مقصد_لیبل(بھرا_Name_Info_2);
    اگر (has_Applied_بھرا_Name_Config)
    {
        واپس;
    }
    اندرونی_apply_Name_Part_Config_مقصد_Specific_لیبل(نتیجہ);
}

خالی Log_لیبل_Manager::unassign(ثابت std::string& بھرا_Name)
{
    // Lock is inside assign() method.
    assign(بھرا_Name, خالی_اشاریہ);
}

Log_لیبل* Log_لیبل_Manager::get(ثابت std::string& بھرا_Name)
{
    CV_TRACE_FUNCTION();
    تالا_Type lock(m_mutex);
    بھرا_Name_Info* بھرا_Name_Info_Ptr = m_name_جدول.get_بھرا_Name_Info(بھرا_Name);
    اگر (بھرا_Name_Info_Ptr && بھرا_Name_Info_Ptr->log_لیبل_Ptr)
    {
        واپس بھرا_Name_Info_Ptr->log_لیبل_Ptr;
    }
    واپس خالی_اشاریہ;
}

خالی Log_لیبل_Manager::سیٹ_Level_By_بھرا_Name(ثابت std::string& بھرا_Name, LogLevel level)
{
    CV_TRACE_FUNCTION();
    تالا_Type lock(m_mutex);
    بھرا_Name_Lookup_نتیجہ نتیجہ(بھرا_Name);
    نتیجہ.m_ڈھونڈو_Cross_References = جھوٹ;
    m_name_جدول.جوڑو_Or_Lookup_بھرا_Name(نتیجہ);
    بھرا_Name_Info& بھرا_Name_Info_2 = *نتیجہ.m_بھرا_Name_Info_Ptr;
    اگر (بھرا_Name_Info_2.parsedLevel.scope == MatchingScope::بھرا_2 &&
        بھرا_Name_Info_2.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        واپس;
    }
    // update the cached configured value.
    بھرا_Name_Info_2.parsedLevel.scope = MatchingScope::بھرا_2;
    بھرا_Name_Info_2.parsedLevel.level = level;
    // update the actual tag, if already registered.
    Log_لیبل* log_لیبل_Ptr = بھرا_Name_Info_2.log_لیبل_Ptr;
    اگر (log_لیبل_Ptr)
    {
        log_لیبل_Ptr->level = level;
    }
}

خالی Log_لیبل_Manager::سیٹ_Level_By_پہلا_Part(ثابت std::string& پہلا_Part, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    سیٹ_Level_By_Name_Part(پہلا_Part, level, MatchingScope::پہلا_Name_Part);
}

خالی Log_لیبل_Manager::سیٹ_Level_By_Any_Part(ثابت std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    سیٹ_Level_By_Name_Part(anyPart, level, MatchingScope::AnyNamePart);
}

خالی Log_لیبل_Manager::سیٹ_Level_By_Name_Part(ثابت std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    تالا_Type lock(m_mutex);
    Name_Part_Lookup_نتیجہ نتیجہ(namePart);
    نتیجہ.m_ڈھونڈو_Cross_References = سچ;
    m_name_جدول.جوڑو_Or_Lookup_Name_Part(نتیجہ);
    NamePartInfo& namePartInfo = *نتیجہ.m_namePartInfoPtr;
    اگر (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        واپس;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    اندرونی_apply_Name_Part_Config_مقصد_Matching_لیبل_فہرست(نتیجہ);
}

std::vector<std::string> Log_لیبل_Manager::بانٹو_Name_Parts(ثابت std::string& بھرا_Name)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = بھرا_Name.length();
    std::vector<std::string> nameParts;
    size_t آغاز = 0u;
    جب (آغاز < len)
    {
        size_t اگلا_Period = بھرا_Name.find('.', آغاز);
        اگر (اگلا_Period == npos)
        {
            اگلا_Period = len;
        }
        اگر (اگلا_Period >= آغاز + 1u)
        {
            nameParts.emplace_back(بھرا_Name.substr(آغاز, اگلا_Period - آغاز));
        }
        آغاز = اگلا_Period + 1u;
    }
    واپس nameParts;
}

منطقی Log_لیبل_Manager::اندرونی_is_Name_Part_مطابقت(MatchingScope scope, size_t matchingPos)
{
    انتخاب (scope)
    {
    حالت MatchingScope::پہلا_Name_Part:
        واپس (matchingPos == 0u);
    حالت MatchingScope::AnyNamePart:
        واپس سچ;
    حالت MatchingScope::None:
    حالت MatchingScope::بھرا_2:
    طےشدہ:
        واپس جھوٹ;
    }
}

منطقی Log_لیبل_Manager::اندرونی_apply_بھرا_Name_Config_مقصد_لیبل(بھرا_Name_Info& بھرا_Name_Info_2)
{
    اگر (!بھرا_Name_Info_2.log_لیبل_Ptr)
    {
        واپس جھوٹ;
    }
    اگر (بھرا_Name_Info_2.parsedLevel.scope == MatchingScope::بھرا_2)
    {
        بھرا_Name_Info_2.log_لیبل_Ptr->level = بھرا_Name_Info_2.parsedLevel.level;
        واپس سچ;
    }
    واپس جھوٹ;
}

منطقی Log_لیبل_Manager::اندرونی_apply_Name_Part_Config_مقصد_Specific_لیبل(بھرا_Name_Lookup_نتیجہ& بھرا_Name_نتیجہ)
{
    ثابت بھرا_Name_Info& بھرا_Name_Info_2 = *بھرا_Name_نتیجہ.m_بھرا_Name_Info_Ptr;
    Log_لیبل* ثابت log_لیبل = بھرا_Name_Info_2.log_لیبل_Ptr;
    اگر (!log_لیبل)
    {
        واپس جھوٹ;
    }
    CV_Assert(بھرا_Name_نتیجہ.m_ڈھونڈو_Cross_References);
    ثابت خودکار& crossReferences = بھرا_Name_نتیجہ.m_crossReferences;
    ثابت size_t matching_Name_Part_گنتی = crossReferences.size();
    برائے (size_t k = 0u; k < matching_Name_Part_گنتی; ++k)
    {
        ثابت خودکار& مطابقت = crossReferences.at(k);
        ثابت خودکار& namePartInfo = *مطابقت.m_namePartInfo;
        ثابت خودکار& parsedLevel = namePartInfo.parsedLevel;
        ثابت خودکار scope = parsedLevel.scope;
        ثابت LogLevel level = parsedLevel.level;
        ثابت size_t matchingPos = مطابقت.m_matchingPos;
        ثابت منطقی is_مطابقت = اندرونی_is_Name_Part_مطابقت(scope, matchingPos);
        اگر (is_مطابقت)
        {
            log_لیبل->level = level;
            واپس سچ;
        }
    }
    واپس جھوٹ;
}

خالی Log_لیبل_Manager::اندرونی_apply_Name_Part_Config_مقصد_Matching_لیبل_فہرست(Name_Part_Lookup_نتیجہ& name_Part_نتیجہ)
{
    CV_Assert(name_Part_نتیجہ.m_ڈھونڈو_Cross_References);
    ثابت خودکار& crossReferences = name_Part_نتیجہ.m_crossReferences;
    ثابت size_t matching_بھرا_Name_گنتی = crossReferences.size();
    NamePartInfo& namePartInfo = *name_Part_نتیجہ.m_namePartInfoPtr;
    ثابت MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::بھرا_2);
    اگر (scope == MatchingScope::None)
    {
        واپس;
    }
    ثابت LogLevel level = namePartInfo.parsedLevel.level;
    برائے (size_t k = 0u; k < matching_بھرا_Name_گنتی; ++k)
    {
        ثابت خودکار& مطابقت = crossReferences.at(k);
        بھرا_Name_Info& بھرا_Name_Info_2 = *مطابقت.m_بھرا_Name_Info;
        Log_لیبل* log_لیبل_Ptr = بھرا_Name_Info_2.log_لیبل_Ptr;
        اگر (!log_لیبل_Ptr)
        {
            جاری;
        }
        اگر (بھرا_Name_Info_2.parsedLevel.scope == MatchingScope::بھرا_2)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            جاری;
        }
        ثابت size_t matchingPos = مطابقت.m_matchingPos;
        ثابت منطقی is_مطابقت = اندرونی_is_Name_Part_مطابقت(scope, matchingPos);
        اگر (!is_مطابقت)
        {
            جاری;
        }
        log_لیبل_Ptr->level = level;
    }
}

خالی Log_لیبل_Manager::Name_جدول::جوڑو_Or_Lookup_بھرا_Name(بھرا_Name_Lookup_نتیجہ& نتیجہ)
{
    ثابت خودکار بھرا_Name_Id_And_Flag = اندرونی_جوڑو_Or_Lookup_بھرا_Name(نتیجہ.m_بھرا_Name);
    نتیجہ.m_بھرا_Name_Id = بھرا_Name_Id_And_Flag.first;
    نتیجہ.m_nameParts = Log_لیبل_Manager::بانٹو_Name_Parts(نتیجہ.m_بھرا_Name);
    اندرونی_جوڑو_Or_Lookup_Name_Parts(نتیجہ.m_nameParts, نتیجہ.m_namePartIds);
    ثابت منطقی isNew = بھرا_Name_Id_And_Flag.second;
    اگر (isNew)
    {
        اندرونی_جوڑو_Cross_Reference(نتیجہ.m_بھرا_Name_Id, نتیجہ.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    نتیجہ.m_بھرا_Name_Info_Ptr = اندرونی_get_بھرا_Name_Info(نتیجہ.m_بھرا_Name_Id);
    اگر (نتیجہ.m_ڈھونڈو_Cross_References)
    {
        اندرونی_ڈھونڈو_Matching_Name_Parts_For_بھرا_Name(نتیجہ);
    }
}

خالی Log_لیبل_Manager::Name_جدول::جوڑو_Or_Lookup_Name_Part(Name_Part_Lookup_نتیجہ& نتیجہ)
{
    نتیجہ.m_namePartId = اندرونی_جوڑو_Or_Lookup_Name_Part(نتیجہ.m_namePart);
    نتیجہ.m_namePartInfoPtr = اندرونی_get_Name_Part_Info(نتیجہ.m_namePartId);
    اگر (نتیجہ.m_ڈھونڈو_Cross_References)
    {
        اندرونی_ڈھونڈو_Matching_بھرا_نام_For_Name_Part(نتیجہ);
    }
}

std::pair<size_t, منطقی> Log_لیبل_Manager::Name_جدول::اندرونی_جوڑو_Or_Lookup_بھرا_Name(ثابت std::string& بھرا_Name)
{
    ثابت خودکار بھرا_Name_Id_Iter = m_بھرا_Name_Ids.find(بھرا_Name);
    اگر (بھرا_Name_Id_Iter != m_بھرا_Name_Ids.end())
    {
        واپس std::make_pair(بھرا_Name_Id_Iter->second, جھوٹ);
    }
    ثابت size_t بھرا_Name_Id = m_بھرا_Name_Infos.size();
    m_بھرا_Name_Infos.emplace_back(بھرا_Name_Info{});
    m_بھرا_Name_Ids.emplace(بھرا_Name, بھرا_Name_Id);
    واپس std::make_pair(بھرا_Name_Id, سچ);
}

خالی Log_لیبل_Manager::Name_جدول::اندرونی_جوڑو_Or_Lookup_Name_Parts(ثابت std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    ثابت size_t name_Part_گنتی = nameParts.size();
    namePartIds.resize(name_Part_گنتی, ~(size_t)0u);
    برائے (size_t name_Part_اشاریہ = 0u; name_Part_اشاریہ < name_Part_گنتی; ++name_Part_اشاریہ)
    {
        ثابت std::string& namePart = nameParts.at(name_Part_اشاریہ);
        ثابت size_t namePartId = اندرونی_جوڑو_Or_Lookup_Name_Part(namePart);
        namePartIds.at(name_Part_اشاریہ) = namePartId;
    }
}

size_t Log_لیبل_Manager::Name_جدول::اندرونی_جوڑو_Or_Lookup_Name_Part(ثابت std::string& namePart)
{
    ثابت خودکار namePartIter = m_namePartIds.find(namePart);
    اگر (namePartIter != m_namePartIds.end())
    {
        واپس namePartIter->second;
    }
    ثابت size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    واپس namePartId;
}

خالی Log_لیبل_Manager::Name_جدول::اندرونی_جوڑو_Cross_Reference(size_t بھرا_Name_Id, ثابت std::vector<size_t>& namePartIds)
{
    ثابت size_t name_Part_گنتی = namePartIds.size();
    برائے (size_t namePartPos = 0u; namePartPos < name_Part_گنتی; ++namePartPos)
    {
        ثابت size_t namePartId = namePartIds.at(namePartPos);
        m_بھرا_Name_مقصد_Name_Part_Ids.emplace(بھرا_Name_Id, std::make_pair(namePartId, namePartPos));
        m_name_Part_مقصد_بھرا_Name_Ids.emplace(namePartId, std::make_pair(بھرا_Name_Id, namePartPos));
    }
}

Log_لیبل_Manager::بھرا_Name_Info* Log_لیبل_Manager::Name_جدول::get_بھرا_Name_Info(ثابت std::string& بھرا_Name)
{
    ثابت خودکار بھرا_Name_Id_Iter = m_بھرا_Name_Ids.find(بھرا_Name);
    اگر (بھرا_Name_Id_Iter == m_بھرا_Name_Ids.end())
    {
        واپس خالی_اشاریہ;
    }
    ثابت size_t بھرا_Name_Id = بھرا_Name_Id_Iter->second;
    واپس اندرونی_get_بھرا_Name_Info(بھرا_Name_Id);
}

Log_لیبل_Manager::بھرا_Name_Info* Log_لیبل_Manager::Name_جدول::اندرونی_get_بھرا_Name_Info(size_t بھرا_Name_Id)
{
    واپس std::addressof(m_بھرا_Name_Infos.at(بھرا_Name_Id));
}

Log_لیبل_Manager::NamePartInfo* Log_لیبل_Manager::Name_جدول::اندرونی_get_Name_Part_Info(size_t namePartId)
{
    واپس std::addressof(m_namePartInfos.at(namePartId));
}

خالی Log_لیبل_Manager::Name_جدول::اندرونی_ڈھونڈو_Matching_Name_Parts_For_بھرا_Name(بھرا_Name_Lookup_نتیجہ& بھرا_Name_نتیجہ)
{
    ثابت size_t بھرا_Name_Id = بھرا_Name_نتیجہ.m_بھرا_Name_Id;
    بھرا_Name_Info* بھرا_Name_Info_2 = بھرا_Name_نتیجہ.m_بھرا_Name_Info_Ptr;
    ثابت خودکار& namePartIds = بھرا_Name_نتیجہ.m_namePartIds;
    ثابت size_t name_Part_گنتی = namePartIds.size();
    خودکار& crossReferences = بھرا_Name_نتیجہ.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_گنتی);
    برائے (size_t matchingPos = 0u; matchingPos < name_Part_گنتی; ++matchingPos)
    {
        ثابت size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = اندرونی_get_Name_Part_Info(namePartId);
        crossReferences.emplace_back(CrossReference(بھرا_Name_Id, namePartId, matchingPos, بھرا_Name_Info_2, namePartInfo));
    }
}

خالی Log_لیبل_Manager::Name_جدول::اندرونی_ڈھونڈو_Matching_بھرا_نام_For_Name_Part(Name_Part_Lookup_نتیجہ& نتیجہ)
{
    ثابت size_t namePartId = نتیجہ.m_namePartId;
    NamePartInfo* namePartInfo = نتیجہ.m_namePartInfoPtr;
    ثابت size_t matching_بھرا_Name_گنتی = m_name_Part_مقصد_بھرا_Name_Ids.گنتی_2(namePartId);
    std::vector<CrossReference>& crossReferences = نتیجہ.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_بھرا_Name_گنتی);
    ثابت خودکار name_Part_مقصد_بھرا_Name_Iter_Pair = m_name_Part_مقصد_بھرا_Name_Ids.equal_range(نتیجہ.m_namePartId);
    ثابت خودکار iterBegin = name_Part_مقصد_بھرا_Name_Iter_Pair.first;
    ثابت خودکار iter_انتہا = name_Part_مقصد_بھرا_Name_Iter_Pair.second;
    برائے (خودکار iter = iterBegin; iter != iter_انتہا; ++iter)
    {
        ثابت size_t بھرا_Name_Id = iter->second.first;
        ثابت size_t matchingPos = iter->second.second;
        بھرا_Name_Info* بھرا_Name_Info_2 = اندرونی_get_بھرا_Name_Info(بھرا_Name_Id);
        crossReferences.emplace_back(CrossReference(بھرا_Name_Id, namePartId, matchingPos, بھرا_Name_Info_2, namePartInfo));
    }
}

}}} //namespace
