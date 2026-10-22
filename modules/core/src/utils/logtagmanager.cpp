// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"
#ضمن "logtagmanager.hpp"
#ضمن "logtagconfigparser.hpp"

نطاق cv {
نطاق utils {
نطاق logging {


ثابت حرف* Log_ٹیگ_Manager::m_globalName = "global";


Log_ٹیگ_Manager::Log_ٹیگ_Manager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_global_Log_ٹیگ(جديد Log_ٹیگ(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<Log_ٹیگ_Config_Parser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_global_Log_ٹیگ.get());
}

Log_ٹیگ_Manager::~Log_ٹیگ_Manager()
{
}

فراغ Log_ٹیگ_Manager::سیٹ_Config_String(ثابت std::string& configString, منطقي apply /*true*/)
{
    m_config->parse(configString);
    إذا (m_config->hasMalformed())
    {
        أرجع;
    }
    إذا (!apply)
    {
        أرجع;
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
    m_global_Log_ٹیگ->level = globalConfig.level;
    لكل (ثابت خودکار& config : m_config->get_پہلا_Part_Configs())
    {
        سیٹ_Level_By_پہلا_Part(config.namePart, config.level);
    }
    لكل (ثابت خودکار& config : m_config->getAnyPartConfigs())
    {
        سیٹ_Level_By_Any_Part(config.namePart, config.level);
    }
    لكل (ثابت خودکار& config : m_config->get_بھریا_Name_Configs())
    {
        سیٹ_Level_By_بھریا_Name(config.namePart, config.level);
    }
}

Log_ٹیگ_Config_Parser& Log_ٹیگ_Manager::getConfigParser() ثابت
{
    أرجع *m_config;
}

فراغ Log_ٹیگ_Manager::assign(ثابت std::string& بھریا_Name, Log_ٹیگ* ptr)
{
    CV_TRACE_FUNCTION();
    جندرا_Type lock(m_mutex);
    بھریا_Name_Lookup_نتیجہ نتیجہ(بھریا_Name);
    نتیجہ.m_لبھو_Cross_References = سچ;
    m_name_جدول.جوڑو_Or_Lookup_بھریا_Name(نتیجہ);
    بھریا_Name_Info& بھریا_Name_Info_2 = *نتیجہ.m_بھریا_Name_Info_Ptr;
    ثابت منطقي is_Ptr_بدلیا = (بھریا_Name_Info_2.log_ٹیگ_Ptr != ptr);
    إذا (!is_Ptr_بدلیا)
    {
        أرجع;
    }
    بھریا_Name_Info_2.log_ٹیگ_Ptr = ptr;
    إذا (!ptr)
    {
        أرجع;
    }
    ثابت منطقي has_Applied_بھریا_Name_Config = اندرونی_apply_بھریا_Name_Config_منزل_ٹیگ(بھریا_Name_Info_2);
    إذا (has_Applied_بھریا_Name_Config)
    {
        أرجع;
    }
    اندرونی_apply_Name_Part_Config_منزل_Specific_ٹیگ(نتیجہ);
}

فراغ Log_ٹیگ_Manager::unassign(ثابت std::string& بھریا_Name)
{
    // Lock is inside assign() method.
    assign(بھریا_Name, خالی_سوچک);
}

Log_ٹیگ* Log_ٹیگ_Manager::get(ثابت std::string& بھریا_Name)
{
    CV_TRACE_FUNCTION();
    جندرا_Type lock(m_mutex);
    بھریا_Name_Info* بھریا_Name_Info_Ptr = m_name_جدول.get_بھریا_Name_Info(بھریا_Name);
    إذا (بھریا_Name_Info_Ptr && بھریا_Name_Info_Ptr->log_ٹیگ_Ptr)
    {
        أرجع بھریا_Name_Info_Ptr->log_ٹیگ_Ptr;
    }
    أرجع خالی_سوچک;
}

فراغ Log_ٹیگ_Manager::سیٹ_Level_By_بھریا_Name(ثابت std::string& بھریا_Name, LogLevel level)
{
    CV_TRACE_FUNCTION();
    جندرا_Type lock(m_mutex);
    بھریا_Name_Lookup_نتیجہ نتیجہ(بھریا_Name);
    نتیجہ.m_لبھو_Cross_References = خطأ;
    m_name_جدول.جوڑو_Or_Lookup_بھریا_Name(نتیجہ);
    بھریا_Name_Info& بھریا_Name_Info_2 = *نتیجہ.m_بھریا_Name_Info_Ptr;
    إذا (بھریا_Name_Info_2.parsedLevel.scope == MatchingScope::بھریا_2 &&
        بھریا_Name_Info_2.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        أرجع;
    }
    // update the cached configured value.
    بھریا_Name_Info_2.parsedLevel.scope = MatchingScope::بھریا_2;
    بھریا_Name_Info_2.parsedLevel.level = level;
    // update the actual tag, if already registered.
    Log_ٹیگ* log_ٹیگ_Ptr = بھریا_Name_Info_2.log_ٹیگ_Ptr;
    إذا (log_ٹیگ_Ptr)
    {
        log_ٹیگ_Ptr->level = level;
    }
}

فراغ Log_ٹیگ_Manager::سیٹ_Level_By_پہلا_Part(ثابت std::string& پہلا_Part, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    سیٹ_Level_By_Name_Part(پہلا_Part, level, MatchingScope::پہلا_Name_Part);
}

فراغ Log_ٹیگ_Manager::سیٹ_Level_By_Any_Part(ثابت std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    سیٹ_Level_By_Name_Part(anyPart, level, MatchingScope::AnyNamePart);
}

فراغ Log_ٹیگ_Manager::سیٹ_Level_By_Name_Part(ثابت std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    جندرا_Type lock(m_mutex);
    Name_Part_Lookup_نتیجہ نتیجہ(namePart);
    نتیجہ.m_لبھو_Cross_References = سچ;
    m_name_جدول.جوڑو_Or_Lookup_Name_Part(نتیجہ);
    NamePartInfo& namePartInfo = *نتیجہ.m_namePartInfoPtr;
    إذا (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        أرجع;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    اندرونی_apply_Name_Part_Config_منزل_Matching_ٹیگ_سوچی(نتیجہ);
}

std::vector<std::string> Log_ٹیگ_Manager::ونڈو_Name_Parts(ثابت std::string& بھریا_Name)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = بھریا_Name.length();
    std::vector<std::string> nameParts;
    size_t شروع = 0u;
    طالما (شروع < len)
    {
        size_t اگلا_Period = بھریا_Name.find('.', شروع);
        إذا (اگلا_Period == npos)
        {
            اگلا_Period = len;
        }
        إذا (اگلا_Period >= شروع + 1u)
        {
            nameParts.emplace_back(بھریا_Name.substr(شروع, اگلا_Period - شروع));
        }
        شروع = اگلا_Period + 1u;
    }
    أرجع nameParts;
}

منطقي Log_ٹیگ_Manager::اندرونی_is_Name_Part_میل(MatchingScope scope, size_t matchingPos)
{
    بدل (scope)
    {
    حالة MatchingScope::پہلا_Name_Part:
        أرجع (matchingPos == 0u);
    حالة MatchingScope::AnyNamePart:
        أرجع سچ;
    حالة MatchingScope::None:
    حالة MatchingScope::بھریا_2:
    افتراضي:
        أرجع خطأ;
    }
}

منطقي Log_ٹیگ_Manager::اندرونی_apply_بھریا_Name_Config_منزل_ٹیگ(بھریا_Name_Info& بھریا_Name_Info_2)
{
    إذا (!بھریا_Name_Info_2.log_ٹیگ_Ptr)
    {
        أرجع خطأ;
    }
    إذا (بھریا_Name_Info_2.parsedLevel.scope == MatchingScope::بھریا_2)
    {
        بھریا_Name_Info_2.log_ٹیگ_Ptr->level = بھریا_Name_Info_2.parsedLevel.level;
        أرجع سچ;
    }
    أرجع خطأ;
}

منطقي Log_ٹیگ_Manager::اندرونی_apply_Name_Part_Config_منزل_Specific_ٹیگ(بھریا_Name_Lookup_نتیجہ& بھریا_Name_نتیجہ)
{
    ثابت بھریا_Name_Info& بھریا_Name_Info_2 = *بھریا_Name_نتیجہ.m_بھریا_Name_Info_Ptr;
    Log_ٹیگ* ثابت log_ٹیگ = بھریا_Name_Info_2.log_ٹیگ_Ptr;
    إذا (!log_ٹیگ)
    {
        أرجع خطأ;
    }
    CV_Assert(بھریا_Name_نتیجہ.m_لبھو_Cross_References);
    ثابت خودکار& crossReferences = بھریا_Name_نتیجہ.m_crossReferences;
    ثابت size_t matching_Name_Part_گنتی = crossReferences.size();
    لكل (size_t k = 0u; k < matching_Name_Part_گنتی; ++k)
    {
        ثابت خودکار& میل = crossReferences.at(k);
        ثابت خودکار& namePartInfo = *میل.m_namePartInfo;
        ثابت خودکار& parsedLevel = namePartInfo.parsedLevel;
        ثابت خودکار scope = parsedLevel.scope;
        ثابت LogLevel level = parsedLevel.level;
        ثابت size_t matchingPos = میل.m_matchingPos;
        ثابت منطقي is_میل = اندرونی_is_Name_Part_میل(scope, matchingPos);
        إذا (is_میل)
        {
            log_ٹیگ->level = level;
            أرجع سچ;
        }
    }
    أرجع خطأ;
}

فراغ Log_ٹیگ_Manager::اندرونی_apply_Name_Part_Config_منزل_Matching_ٹیگ_سوچی(Name_Part_Lookup_نتیجہ& name_Part_نتیجہ)
{
    CV_Assert(name_Part_نتیجہ.m_لبھو_Cross_References);
    ثابت خودکار& crossReferences = name_Part_نتیجہ.m_crossReferences;
    ثابت size_t matching_بھریا_Name_گنتی = crossReferences.size();
    NamePartInfo& namePartInfo = *name_Part_نتیجہ.m_namePartInfoPtr;
    ثابت MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::بھریا_2);
    إذا (scope == MatchingScope::None)
    {
        أرجع;
    }
    ثابت LogLevel level = namePartInfo.parsedLevel.level;
    لكل (size_t k = 0u; k < matching_بھریا_Name_گنتی; ++k)
    {
        ثابت خودکار& میل = crossReferences.at(k);
        بھریا_Name_Info& بھریا_Name_Info_2 = *میل.m_بھریا_Name_Info;
        Log_ٹیگ* log_ٹیگ_Ptr = بھریا_Name_Info_2.log_ٹیگ_Ptr;
        إذا (!log_ٹیگ_Ptr)
        {
            استمر;
        }
        إذا (بھریا_Name_Info_2.parsedLevel.scope == MatchingScope::بھریا_2)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            استمر;
        }
        ثابت size_t matchingPos = میل.m_matchingPos;
        ثابت منطقي is_میل = اندرونی_is_Name_Part_میل(scope, matchingPos);
        إذا (!is_میل)
        {
            استمر;
        }
        log_ٹیگ_Ptr->level = level;
    }
}

فراغ Log_ٹیگ_Manager::Name_جدول::جوڑو_Or_Lookup_بھریا_Name(بھریا_Name_Lookup_نتیجہ& نتیجہ)
{
    ثابت خودکار بھریا_Name_Id_And_Flag = اندرونی_جوڑو_Or_Lookup_بھریا_Name(نتیجہ.m_بھریا_Name);
    نتیجہ.m_بھریا_Name_Id = بھریا_Name_Id_And_Flag.first;
    نتیجہ.m_nameParts = Log_ٹیگ_Manager::ونڈو_Name_Parts(نتیجہ.m_بھریا_Name);
    اندرونی_جوڑو_Or_Lookup_Name_Parts(نتیجہ.m_nameParts, نتیجہ.m_namePartIds);
    ثابت منطقي isNew = بھریا_Name_Id_And_Flag.second;
    إذا (isNew)
    {
        اندرونی_جوڑو_Cross_Reference(نتیجہ.m_بھریا_Name_Id, نتیجہ.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    نتیجہ.m_بھریا_Name_Info_Ptr = اندرونی_get_بھریا_Name_Info(نتیجہ.m_بھریا_Name_Id);
    إذا (نتیجہ.m_لبھو_Cross_References)
    {
        اندرونی_لبھو_Matching_Name_Parts_For_بھریا_Name(نتیجہ);
    }
}

فراغ Log_ٹیگ_Manager::Name_جدول::جوڑو_Or_Lookup_Name_Part(Name_Part_Lookup_نتیجہ& نتیجہ)
{
    نتیجہ.m_namePartId = اندرونی_جوڑو_Or_Lookup_Name_Part(نتیجہ.m_namePart);
    نتیجہ.m_namePartInfoPtr = اندرونی_get_Name_Part_Info(نتیجہ.m_namePartId);
    إذا (نتیجہ.m_لبھو_Cross_References)
    {
        اندرونی_لبھو_Matching_بھریا_ناں_For_Name_Part(نتیجہ);
    }
}

std::pair<size_t, منطقي> Log_ٹیگ_Manager::Name_جدول::اندرونی_جوڑو_Or_Lookup_بھریا_Name(ثابت std::string& بھریا_Name)
{
    ثابت خودکار بھریا_Name_Id_Iter = m_بھریا_Name_Ids.find(بھریا_Name);
    إذا (بھریا_Name_Id_Iter != m_بھریا_Name_Ids.end())
    {
        أرجع std::make_pair(بھریا_Name_Id_Iter->second, خطأ);
    }
    ثابت size_t بھریا_Name_Id = m_بھریا_Name_Infos.size();
    m_بھریا_Name_Infos.emplace_back(بھریا_Name_Info{});
    m_بھریا_Name_Ids.emplace(بھریا_Name, بھریا_Name_Id);
    أرجع std::make_pair(بھریا_Name_Id, سچ);
}

فراغ Log_ٹیگ_Manager::Name_جدول::اندرونی_جوڑو_Or_Lookup_Name_Parts(ثابت std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    ثابت size_t name_Part_گنتی = nameParts.size();
    namePartIds.resize(name_Part_گنتی, ~(size_t)0u);
    لكل (size_t name_Part_اشاریہ = 0u; name_Part_اشاریہ < name_Part_گنتی; ++name_Part_اشاریہ)
    {
        ثابت std::string& namePart = nameParts.at(name_Part_اشاریہ);
        ثابت size_t namePartId = اندرونی_جوڑو_Or_Lookup_Name_Part(namePart);
        namePartIds.at(name_Part_اشاریہ) = namePartId;
    }
}

size_t Log_ٹیگ_Manager::Name_جدول::اندرونی_جوڑو_Or_Lookup_Name_Part(ثابت std::string& namePart)
{
    ثابت خودکار namePartIter = m_namePartIds.find(namePart);
    إذا (namePartIter != m_namePartIds.end())
    {
        أرجع namePartIter->second;
    }
    ثابت size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    أرجع namePartId;
}

فراغ Log_ٹیگ_Manager::Name_جدول::اندرونی_جوڑو_Cross_Reference(size_t بھریا_Name_Id, ثابت std::vector<size_t>& namePartIds)
{
    ثابت size_t name_Part_گنتی = namePartIds.size();
    لكل (size_t namePartPos = 0u; namePartPos < name_Part_گنتی; ++namePartPos)
    {
        ثابت size_t namePartId = namePartIds.at(namePartPos);
        m_بھریا_Name_منزل_Name_Part_Ids.emplace(بھریا_Name_Id, std::make_pair(namePartId, namePartPos));
        m_name_Part_منزل_بھریا_Name_Ids.emplace(namePartId, std::make_pair(بھریا_Name_Id, namePartPos));
    }
}

Log_ٹیگ_Manager::بھریا_Name_Info* Log_ٹیگ_Manager::Name_جدول::get_بھریا_Name_Info(ثابت std::string& بھریا_Name)
{
    ثابت خودکار بھریا_Name_Id_Iter = m_بھریا_Name_Ids.find(بھریا_Name);
    إذا (بھریا_Name_Id_Iter == m_بھریا_Name_Ids.end())
    {
        أرجع خالی_سوچک;
    }
    ثابت size_t بھریا_Name_Id = بھریا_Name_Id_Iter->second;
    أرجع اندرونی_get_بھریا_Name_Info(بھریا_Name_Id);
}

Log_ٹیگ_Manager::بھریا_Name_Info* Log_ٹیگ_Manager::Name_جدول::اندرونی_get_بھریا_Name_Info(size_t بھریا_Name_Id)
{
    أرجع std::addressof(m_بھریا_Name_Infos.at(بھریا_Name_Id));
}

Log_ٹیگ_Manager::NamePartInfo* Log_ٹیگ_Manager::Name_جدول::اندرونی_get_Name_Part_Info(size_t namePartId)
{
    أرجع std::addressof(m_namePartInfos.at(namePartId));
}

فراغ Log_ٹیگ_Manager::Name_جدول::اندرونی_لبھو_Matching_Name_Parts_For_بھریا_Name(بھریا_Name_Lookup_نتیجہ& بھریا_Name_نتیجہ)
{
    ثابت size_t بھریا_Name_Id = بھریا_Name_نتیجہ.m_بھریا_Name_Id;
    بھریا_Name_Info* بھریا_Name_Info_2 = بھریا_Name_نتیجہ.m_بھریا_Name_Info_Ptr;
    ثابت خودکار& namePartIds = بھریا_Name_نتیجہ.m_namePartIds;
    ثابت size_t name_Part_گنتی = namePartIds.size();
    خودکار& crossReferences = بھریا_Name_نتیجہ.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_گنتی);
    لكل (size_t matchingPos = 0u; matchingPos < name_Part_گنتی; ++matchingPos)
    {
        ثابت size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = اندرونی_get_Name_Part_Info(namePartId);
        crossReferences.emplace_back(CrossReference(بھریا_Name_Id, namePartId, matchingPos, بھریا_Name_Info_2, namePartInfo));
    }
}

فراغ Log_ٹیگ_Manager::Name_جدول::اندرونی_لبھو_Matching_بھریا_ناں_For_Name_Part(Name_Part_Lookup_نتیجہ& نتیجہ)
{
    ثابت size_t namePartId = نتیجہ.m_namePartId;
    NamePartInfo* namePartInfo = نتیجہ.m_namePartInfoPtr;
    ثابت size_t matching_بھریا_Name_گنتی = m_name_Part_منزل_بھریا_Name_Ids.گنتی_2(namePartId);
    std::vector<CrossReference>& crossReferences = نتیجہ.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_بھریا_Name_گنتی);
    ثابت خودکار name_Part_منزل_بھریا_Name_Iter_Pair = m_name_Part_منزل_بھریا_Name_Ids.equal_range(نتیجہ.m_namePartId);
    ثابت خودکار iterBegin = name_Part_منزل_بھریا_Name_Iter_Pair.first;
    ثابت خودکار iter_سرا = name_Part_منزل_بھریا_Name_Iter_Pair.second;
    لكل (خودکار iter = iterBegin; iter != iter_سرا; ++iter)
    {
        ثابت size_t بھریا_Name_Id = iter->second.first;
        ثابت size_t matchingPos = iter->second.second;
        بھریا_Name_Info* بھریا_Name_Info_2 = اندرونی_get_بھریا_Name_Info(بھریا_Name_Id);
        crossReferences.emplace_back(CrossReference(بھریا_Name_Id, namePartId, matchingPos, بھریا_Name_Info_2, namePartInfo));
    }
}

}}} //namespace
