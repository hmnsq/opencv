// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"
#ضمن "logtagmanager.hpp"
#ضمن "logtagconfigparser.hpp"

نطاق cv {
نطاق utils {
نطاق logging {


ثابت حرف* Log_ٽيگ_Manager::m_globalName = "global";


Log_ٽيگ_Manager::Log_ٽيگ_Manager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_global_Log_ٽيگ(جديد Log_ٽيگ(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<Log_ٽيگ_Config_Parser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_global_Log_ٽيگ.get());
}

Log_ٽيگ_Manager::~Log_ٽيگ_Manager()
{
}

فراغ Log_ٽيگ_Manager::سيٽ_Config_String(ثابت std::string& configString, منطقي apply /*true*/)
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
    ثابت خودڪار& globalConfig = m_config->getGlobalConfig();
    m_global_Log_ٽيگ->level = globalConfig.level;
    لكل (ثابت خودڪار& config : m_config->get_پهريون_Part_Configs())
    {
        سيٽ_Level_By_پهريون_Part(config.namePart, config.level);
    }
    لكل (ثابت خودڪار& config : m_config->getAnyPartConfigs())
    {
        سيٽ_Level_By_Any_Part(config.namePart, config.level);
    }
    لكل (ثابت خودڪار& config : m_config->get_ڀريل_Name_Configs())
    {
        سيٽ_Level_By_ڀريل_Name(config.namePart, config.level);
    }
}

Log_ٽيگ_Config_Parser& Log_ٽيگ_Manager::getConfigParser() ثابت
{
    أرجع *m_config;
}

فراغ Log_ٽيگ_Manager::assign(ثابت std::string& ڀريل_Name, Log_ٽيگ* ptr)
{
    CV_TRACE_FUNCTION();
    تالو_Type lock(m_mutex);
    ڀريل_Name_Lookup_نتيجو نتيجو(ڀريل_Name);
    نتيجو.m_ڳوليو_Cross_References = سچ;
    m_name_جدول.جوڙيو_Or_Lookup_ڀريل_Name(نتيجو);
    ڀريل_Name_Info& ڀريل_Name_Info_2 = *نتيجو.m_ڀريل_Name_Info_Ptr;
    ثابت منطقي is_Ptr_بدليو = (ڀريل_Name_Info_2.log_ٽيگ_Ptr != ptr);
    إذا (!is_Ptr_بدليو)
    {
        أرجع;
    }
    ڀريل_Name_Info_2.log_ٽيگ_Ptr = ptr;
    إذا (!ptr)
    {
        أرجع;
    }
    ثابت منطقي has_Applied_ڀريل_Name_Config = اندروني_apply_ڀريل_Name_Config_منزل_ٽيگ(ڀريل_Name_Info_2);
    إذا (has_Applied_ڀريل_Name_Config)
    {
        أرجع;
    }
    اندروني_apply_Name_Part_Config_منزل_Specific_ٽيگ(نتيجو);
}

فراغ Log_ٽيگ_Manager::unassign(ثابت std::string& ڀريل_Name)
{
    // Lock is inside assign() method.
    assign(ڀريل_Name, خالي_اشارو);
}

Log_ٽيگ* Log_ٽيگ_Manager::get(ثابت std::string& ڀريل_Name)
{
    CV_TRACE_FUNCTION();
    تالو_Type lock(m_mutex);
    ڀريل_Name_Info* ڀريل_Name_Info_Ptr = m_name_جدول.get_ڀريل_Name_Info(ڀريل_Name);
    إذا (ڀريل_Name_Info_Ptr && ڀريل_Name_Info_Ptr->log_ٽيگ_Ptr)
    {
        أرجع ڀريل_Name_Info_Ptr->log_ٽيگ_Ptr;
    }
    أرجع خالي_اشارو;
}

فراغ Log_ٽيگ_Manager::سيٽ_Level_By_ڀريل_Name(ثابت std::string& ڀريل_Name, LogLevel level)
{
    CV_TRACE_FUNCTION();
    تالو_Type lock(m_mutex);
    ڀريل_Name_Lookup_نتيجو نتيجو(ڀريل_Name);
    نتيجو.m_ڳوليو_Cross_References = خطأ;
    m_name_جدول.جوڙيو_Or_Lookup_ڀريل_Name(نتيجو);
    ڀريل_Name_Info& ڀريل_Name_Info_2 = *نتيجو.m_ڀريل_Name_Info_Ptr;
    إذا (ڀريل_Name_Info_2.parsedLevel.scope == MatchingScope::ڀريل_2 &&
        ڀريل_Name_Info_2.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        أرجع;
    }
    // update the cached configured value.
    ڀريل_Name_Info_2.parsedLevel.scope = MatchingScope::ڀريل_2;
    ڀريل_Name_Info_2.parsedLevel.level = level;
    // update the actual tag, if already registered.
    Log_ٽيگ* log_ٽيگ_Ptr = ڀريل_Name_Info_2.log_ٽيگ_Ptr;
    إذا (log_ٽيگ_Ptr)
    {
        log_ٽيگ_Ptr->level = level;
    }
}

فراغ Log_ٽيگ_Manager::سيٽ_Level_By_پهريون_Part(ثابت std::string& پهريون_Part, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    سيٽ_Level_By_Name_Part(پهريون_Part, level, MatchingScope::پهريون_Name_Part);
}

فراغ Log_ٽيگ_Manager::سيٽ_Level_By_Any_Part(ثابت std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    سيٽ_Level_By_Name_Part(anyPart, level, MatchingScope::AnyNamePart);
}

فراغ Log_ٽيگ_Manager::سيٽ_Level_By_Name_Part(ثابت std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    تالو_Type lock(m_mutex);
    Name_Part_Lookup_نتيجو نتيجو(namePart);
    نتيجو.m_ڳوليو_Cross_References = سچ;
    m_name_جدول.جوڙيو_Or_Lookup_Name_Part(نتيجو);
    NamePartInfo& namePartInfo = *نتيجو.m_namePartInfoPtr;
    إذا (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        أرجع;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    اندروني_apply_Name_Part_Config_منزل_Matching_ٽيگ_فهرست(نتيجو);
}

std::vector<std::string> Log_ٽيگ_Manager::ورهايو_Name_Parts(ثابت std::string& ڀريل_Name)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = ڀريل_Name.length();
    std::vector<std::string> nameParts;
    size_t شروعات = 0u;
    طالما (شروعات < len)
    {
        size_t ايندڙ_Period = ڀريل_Name.find('.', شروعات);
        إذا (ايندڙ_Period == npos)
        {
            ايندڙ_Period = len;
        }
        إذا (ايندڙ_Period >= شروعات + 1u)
        {
            nameParts.emplace_back(ڀريل_Name.substr(شروعات, ايندڙ_Period - شروعات));
        }
        شروعات = ايندڙ_Period + 1u;
    }
    أرجع nameParts;
}

منطقي Log_ٽيگ_Manager::اندروني_is_Name_Part_ميلاپ(MatchingScope scope, size_t matchingPos)
{
    بدل (scope)
    {
    حالة MatchingScope::پهريون_Name_Part:
        أرجع (matchingPos == 0u);
    حالة MatchingScope::AnyNamePart:
        أرجع سچ;
    حالة MatchingScope::None:
    حالة MatchingScope::ڀريل_2:
    افتراضي:
        أرجع خطأ;
    }
}

منطقي Log_ٽيگ_Manager::اندروني_apply_ڀريل_Name_Config_منزل_ٽيگ(ڀريل_Name_Info& ڀريل_Name_Info_2)
{
    إذا (!ڀريل_Name_Info_2.log_ٽيگ_Ptr)
    {
        أرجع خطأ;
    }
    إذا (ڀريل_Name_Info_2.parsedLevel.scope == MatchingScope::ڀريل_2)
    {
        ڀريل_Name_Info_2.log_ٽيگ_Ptr->level = ڀريل_Name_Info_2.parsedLevel.level;
        أرجع سچ;
    }
    أرجع خطأ;
}

منطقي Log_ٽيگ_Manager::اندروني_apply_Name_Part_Config_منزل_Specific_ٽيگ(ڀريل_Name_Lookup_نتيجو& ڀريل_Name_نتيجو)
{
    ثابت ڀريل_Name_Info& ڀريل_Name_Info_2 = *ڀريل_Name_نتيجو.m_ڀريل_Name_Info_Ptr;
    Log_ٽيگ* ثابت log_ٽيگ = ڀريل_Name_Info_2.log_ٽيگ_Ptr;
    إذا (!log_ٽيگ)
    {
        أرجع خطأ;
    }
    CV_Assert(ڀريل_Name_نتيجو.m_ڳوليو_Cross_References);
    ثابت خودڪار& crossReferences = ڀريل_Name_نتيجو.m_crossReferences;
    ثابت size_t matching_Name_Part_ڳڻپ = crossReferences.size();
    لكل (size_t k = 0u; k < matching_Name_Part_ڳڻپ; ++k)
    {
        ثابت خودڪار& ميلاپ = crossReferences.at(k);
        ثابت خودڪار& namePartInfo = *ميلاپ.m_namePartInfo;
        ثابت خودڪار& parsedLevel = namePartInfo.parsedLevel;
        ثابت خودڪار scope = parsedLevel.scope;
        ثابت LogLevel level = parsedLevel.level;
        ثابت size_t matchingPos = ميلاپ.m_matchingPos;
        ثابت منطقي is_ميلاپ = اندروني_is_Name_Part_ميلاپ(scope, matchingPos);
        إذا (is_ميلاپ)
        {
            log_ٽيگ->level = level;
            أرجع سچ;
        }
    }
    أرجع خطأ;
}

فراغ Log_ٽيگ_Manager::اندروني_apply_Name_Part_Config_منزل_Matching_ٽيگ_فهرست(Name_Part_Lookup_نتيجو& name_Part_نتيجو)
{
    CV_Assert(name_Part_نتيجو.m_ڳوليو_Cross_References);
    ثابت خودڪار& crossReferences = name_Part_نتيجو.m_crossReferences;
    ثابت size_t matching_ڀريل_Name_ڳڻپ = crossReferences.size();
    NamePartInfo& namePartInfo = *name_Part_نتيجو.m_namePartInfoPtr;
    ثابت MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::ڀريل_2);
    إذا (scope == MatchingScope::None)
    {
        أرجع;
    }
    ثابت LogLevel level = namePartInfo.parsedLevel.level;
    لكل (size_t k = 0u; k < matching_ڀريل_Name_ڳڻپ; ++k)
    {
        ثابت خودڪار& ميلاپ = crossReferences.at(k);
        ڀريل_Name_Info& ڀريل_Name_Info_2 = *ميلاپ.m_ڀريل_Name_Info;
        Log_ٽيگ* log_ٽيگ_Ptr = ڀريل_Name_Info_2.log_ٽيگ_Ptr;
        إذا (!log_ٽيگ_Ptr)
        {
            استمر;
        }
        إذا (ڀريل_Name_Info_2.parsedLevel.scope == MatchingScope::ڀريل_2)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            استمر;
        }
        ثابت size_t matchingPos = ميلاپ.m_matchingPos;
        ثابت منطقي is_ميلاپ = اندروني_is_Name_Part_ميلاپ(scope, matchingPos);
        إذا (!is_ميلاپ)
        {
            استمر;
        }
        log_ٽيگ_Ptr->level = level;
    }
}

فراغ Log_ٽيگ_Manager::Name_جدول::جوڙيو_Or_Lookup_ڀريل_Name(ڀريل_Name_Lookup_نتيجو& نتيجو)
{
    ثابت خودڪار ڀريل_Name_Id_And_Flag = اندروني_جوڙيو_Or_Lookup_ڀريل_Name(نتيجو.m_ڀريل_Name);
    نتيجو.m_ڀريل_Name_Id = ڀريل_Name_Id_And_Flag.first;
    نتيجو.m_nameParts = Log_ٽيگ_Manager::ورهايو_Name_Parts(نتيجو.m_ڀريل_Name);
    اندروني_جوڙيو_Or_Lookup_Name_Parts(نتيجو.m_nameParts, نتيجو.m_namePartIds);
    ثابت منطقي isNew = ڀريل_Name_Id_And_Flag.second;
    إذا (isNew)
    {
        اندروني_جوڙيو_Cross_Reference(نتيجو.m_ڀريل_Name_Id, نتيجو.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    نتيجو.m_ڀريل_Name_Info_Ptr = اندروني_get_ڀريل_Name_Info(نتيجو.m_ڀريل_Name_Id);
    إذا (نتيجو.m_ڳوليو_Cross_References)
    {
        اندروني_ڳوليو_Matching_Name_Parts_For_ڀريل_Name(نتيجو);
    }
}

فراغ Log_ٽيگ_Manager::Name_جدول::جوڙيو_Or_Lookup_Name_Part(Name_Part_Lookup_نتيجو& نتيجو)
{
    نتيجو.m_namePartId = اندروني_جوڙيو_Or_Lookup_Name_Part(نتيجو.m_namePart);
    نتيجو.m_namePartInfoPtr = اندروني_get_Name_Part_Info(نتيجو.m_namePartId);
    إذا (نتيجو.m_ڳوليو_Cross_References)
    {
        اندروني_ڳوليو_Matching_ڀريل_نالا_For_Name_Part(نتيجو);
    }
}

std::pair<size_t, منطقي> Log_ٽيگ_Manager::Name_جدول::اندروني_جوڙيو_Or_Lookup_ڀريل_Name(ثابت std::string& ڀريل_Name)
{
    ثابت خودڪار ڀريل_Name_Id_Iter = m_ڀريل_Name_Ids.find(ڀريل_Name);
    إذا (ڀريل_Name_Id_Iter != m_ڀريل_Name_Ids.end())
    {
        أرجع std::make_pair(ڀريل_Name_Id_Iter->second, خطأ);
    }
    ثابت size_t ڀريل_Name_Id = m_ڀريل_Name_Infos.size();
    m_ڀريل_Name_Infos.emplace_back(ڀريل_Name_Info{});
    m_ڀريل_Name_Ids.emplace(ڀريل_Name, ڀريل_Name_Id);
    أرجع std::make_pair(ڀريل_Name_Id, سچ);
}

فراغ Log_ٽيگ_Manager::Name_جدول::اندروني_جوڙيو_Or_Lookup_Name_Parts(ثابت std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    ثابت size_t name_Part_ڳڻپ = nameParts.size();
    namePartIds.resize(name_Part_ڳڻپ, ~(size_t)0u);
    لكل (size_t name_Part_اشاريو = 0u; name_Part_اشاريو < name_Part_ڳڻپ; ++name_Part_اشاريو)
    {
        ثابت std::string& namePart = nameParts.at(name_Part_اشاريو);
        ثابت size_t namePartId = اندروني_جوڙيو_Or_Lookup_Name_Part(namePart);
        namePartIds.at(name_Part_اشاريو) = namePartId;
    }
}

size_t Log_ٽيگ_Manager::Name_جدول::اندروني_جوڙيو_Or_Lookup_Name_Part(ثابت std::string& namePart)
{
    ثابت خودڪار namePartIter = m_namePartIds.find(namePart);
    إذا (namePartIter != m_namePartIds.end())
    {
        أرجع namePartIter->second;
    }
    ثابت size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    أرجع namePartId;
}

فراغ Log_ٽيگ_Manager::Name_جدول::اندروني_جوڙيو_Cross_Reference(size_t ڀريل_Name_Id, ثابت std::vector<size_t>& namePartIds)
{
    ثابت size_t name_Part_ڳڻپ = namePartIds.size();
    لكل (size_t namePartPos = 0u; namePartPos < name_Part_ڳڻپ; ++namePartPos)
    {
        ثابت size_t namePartId = namePartIds.at(namePartPos);
        m_ڀريل_Name_منزل_Name_Part_Ids.emplace(ڀريل_Name_Id, std::make_pair(namePartId, namePartPos));
        m_name_Part_منزل_ڀريل_Name_Ids.emplace(namePartId, std::make_pair(ڀريل_Name_Id, namePartPos));
    }
}

Log_ٽيگ_Manager::ڀريل_Name_Info* Log_ٽيگ_Manager::Name_جدول::get_ڀريل_Name_Info(ثابت std::string& ڀريل_Name)
{
    ثابت خودڪار ڀريل_Name_Id_Iter = m_ڀريل_Name_Ids.find(ڀريل_Name);
    إذا (ڀريل_Name_Id_Iter == m_ڀريل_Name_Ids.end())
    {
        أرجع خالي_اشارو;
    }
    ثابت size_t ڀريل_Name_Id = ڀريل_Name_Id_Iter->second;
    أرجع اندروني_get_ڀريل_Name_Info(ڀريل_Name_Id);
}

Log_ٽيگ_Manager::ڀريل_Name_Info* Log_ٽيگ_Manager::Name_جدول::اندروني_get_ڀريل_Name_Info(size_t ڀريل_Name_Id)
{
    أرجع std::addressof(m_ڀريل_Name_Infos.at(ڀريل_Name_Id));
}

Log_ٽيگ_Manager::NamePartInfo* Log_ٽيگ_Manager::Name_جدول::اندروني_get_Name_Part_Info(size_t namePartId)
{
    أرجع std::addressof(m_namePartInfos.at(namePartId));
}

فراغ Log_ٽيگ_Manager::Name_جدول::اندروني_ڳوليو_Matching_Name_Parts_For_ڀريل_Name(ڀريل_Name_Lookup_نتيجو& ڀريل_Name_نتيجو)
{
    ثابت size_t ڀريل_Name_Id = ڀريل_Name_نتيجو.m_ڀريل_Name_Id;
    ڀريل_Name_Info* ڀريل_Name_Info_2 = ڀريل_Name_نتيجو.m_ڀريل_Name_Info_Ptr;
    ثابت خودڪار& namePartIds = ڀريل_Name_نتيجو.m_namePartIds;
    ثابت size_t name_Part_ڳڻپ = namePartIds.size();
    خودڪار& crossReferences = ڀريل_Name_نتيجو.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_ڳڻپ);
    لكل (size_t matchingPos = 0u; matchingPos < name_Part_ڳڻپ; ++matchingPos)
    {
        ثابت size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = اندروني_get_Name_Part_Info(namePartId);
        crossReferences.emplace_back(CrossReference(ڀريل_Name_Id, namePartId, matchingPos, ڀريل_Name_Info_2, namePartInfo));
    }
}

فراغ Log_ٽيگ_Manager::Name_جدول::اندروني_ڳوليو_Matching_ڀريل_نالا_For_Name_Part(Name_Part_Lookup_نتيجو& نتيجو)
{
    ثابت size_t namePartId = نتيجو.m_namePartId;
    NamePartInfo* namePartInfo = نتيجو.m_namePartInfoPtr;
    ثابت size_t matching_ڀريل_Name_ڳڻپ = m_name_Part_منزل_ڀريل_Name_Ids.ڳڻپ_2(namePartId);
    std::vector<CrossReference>& crossReferences = نتيجو.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_ڀريل_Name_ڳڻپ);
    ثابت خودڪار name_Part_منزل_ڀريل_Name_Iter_Pair = m_name_Part_منزل_ڀريل_Name_Ids.equal_range(نتيجو.m_namePartId);
    ثابت خودڪار iterBegin = name_Part_منزل_ڀريل_Name_Iter_Pair.first;
    ثابت خودڪار iter_پڇاڙي = name_Part_منزل_ڀريل_Name_Iter_Pair.second;
    لكل (خودڪار iter = iterBegin; iter != iter_پڇاڙي; ++iter)
    {
        ثابت size_t ڀريل_Name_Id = iter->second.first;
        ثابت size_t matchingPos = iter->second.second;
        ڀريل_Name_Info* ڀريل_Name_Info_2 = اندروني_get_ڀريل_Name_Info(ڀريل_Name_Id);
        crossReferences.emplace_back(CrossReference(ڀريل_Name_Id, namePartId, matchingPos, ڀريل_Name_Info_2, namePartInfo));
    }
}

}}} //namespace
