// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"
#ضمن "logtagmanager.hpp"
#ضمن "logtagconfigparser.hpp"

نطاق cv {
نطاق utils {
نطاق logging {


ثابت حرف* Log_برچسب_Manager::m_globalName = "global";


Log_برچسب_Manager::Log_برچسب_Manager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_global_Log_برچسب(جديد Log_برچسب(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<Log_برچسب_Config_Parser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_global_Log_برچسب.get());
}

Log_برچسب_Manager::~Log_برچسب_Manager()
{
}

فراغ Log_برچسب_Manager::مجموعه_Config_String(ثابت std::string& configString, منطقي apply /*true*/)
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
    m_global_Log_برچسب->level = globalConfig.level;
    لكل (ثابت خودکار& config : m_config->get_اولی_Part_Configs())
    {
        مجموعه_Level_By_اولی_Part(config.namePart, config.level);
    }
    لكل (ثابت خودکار& config : m_config->getAnyPartConfigs())
    {
        مجموعه_Level_By_Any_Part(config.namePart, config.level);
    }
    لكل (ثابت خودکار& config : m_config->get_پر_Name_Configs())
    {
        مجموعه_Level_By_پر_Name(config.namePart, config.level);
    }
}

Log_برچسب_Config_Parser& Log_برچسب_Manager::getConfigParser() ثابت
{
    أرجع *m_config;
}

فراغ Log_برچسب_Manager::assign(ثابت std::string& پر_Name, Log_برچسب* ptr)
{
    CV_TRACE_FUNCTION();
    قفل_Type lock(m_mutex);
    پر_Name_Lookup_نتیجه نتیجه(پر_Name);
    نتیجه.m_پیدا_کن_Cross_References = درست;
    m_name_جدول.جمع_کن_Or_Lookup_پر_Name(نتیجه);
    پر_Name_Info& پر_Name_Info_2 = *نتیجه.m_پر_Name_Info_Ptr;
    ثابت منطقي is_Ptr_تغییریافته = (پر_Name_Info_2.log_برچسب_Ptr != ptr);
    إذا (!is_Ptr_تغییریافته)
    {
        أرجع;
    }
    پر_Name_Info_2.log_برچسب_Ptr = ptr;
    إذا (!ptr)
    {
        أرجع;
    }
    ثابت منطقي has_Applied_پر_Name_Config = داخلی_apply_پر_Name_Config_مقصد_برچسب(پر_Name_Info_2);
    إذا (has_Applied_پر_Name_Config)
    {
        أرجع;
    }
    داخلی_apply_Name_Part_Config_مقصد_Specific_برچسب(نتیجه);
}

فراغ Log_برچسب_Manager::unassign(ثابت std::string& پر_Name)
{
    // Lock is inside assign() method.
    assign(پر_Name, اشاره_تهی);
}

Log_برچسب* Log_برچسب_Manager::get(ثابت std::string& پر_Name)
{
    CV_TRACE_FUNCTION();
    قفل_Type lock(m_mutex);
    پر_Name_Info* پر_Name_Info_Ptr = m_name_جدول.get_پر_Name_Info(پر_Name);
    إذا (پر_Name_Info_Ptr && پر_Name_Info_Ptr->log_برچسب_Ptr)
    {
        أرجع پر_Name_Info_Ptr->log_برچسب_Ptr;
    }
    أرجع اشاره_تهی;
}

فراغ Log_برچسب_Manager::مجموعه_Level_By_پر_Name(ثابت std::string& پر_Name, LogLevel level)
{
    CV_TRACE_FUNCTION();
    قفل_Type lock(m_mutex);
    پر_Name_Lookup_نتیجه نتیجه(پر_Name);
    نتیجه.m_پیدا_کن_Cross_References = خطأ;
    m_name_جدول.جمع_کن_Or_Lookup_پر_Name(نتیجه);
    پر_Name_Info& پر_Name_Info_2 = *نتیجه.m_پر_Name_Info_Ptr;
    إذا (پر_Name_Info_2.parsedLevel.scope == MatchingScope::پر_2 &&
        پر_Name_Info_2.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        أرجع;
    }
    // update the cached configured value.
    پر_Name_Info_2.parsedLevel.scope = MatchingScope::پر_2;
    پر_Name_Info_2.parsedLevel.level = level;
    // update the actual tag, if already registered.
    Log_برچسب* log_برچسب_Ptr = پر_Name_Info_2.log_برچسب_Ptr;
    إذا (log_برچسب_Ptr)
    {
        log_برچسب_Ptr->level = level;
    }
}

فراغ Log_برچسب_Manager::مجموعه_Level_By_اولی_Part(ثابت std::string& اولی_Part, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    مجموعه_Level_By_Name_Part(اولی_Part, level, MatchingScope::اولی_Name_Part);
}

فراغ Log_برچسب_Manager::مجموعه_Level_By_Any_Part(ثابت std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    مجموعه_Level_By_Name_Part(anyPart, level, MatchingScope::AnyNamePart);
}

فراغ Log_برچسب_Manager::مجموعه_Level_By_Name_Part(ثابت std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    قفل_Type lock(m_mutex);
    Name_Part_Lookup_نتیجه نتیجه(namePart);
    نتیجه.m_پیدا_کن_Cross_References = درست;
    m_name_جدول.جمع_کن_Or_Lookup_Name_Part(نتیجه);
    NamePartInfo& namePartInfo = *نتیجه.m_namePartInfoPtr;
    إذا (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        أرجع;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    داخلی_apply_Name_Part_Config_مقصد_Matching_برچسبها(نتیجه);
}

std::vector<std::string> Log_برچسب_Manager::تقسیم_Name_Parts(ثابت std::string& پر_Name)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = پر_Name.length();
    std::vector<std::string> nameParts;
    size_t شروع = 0u;
    طالما (شروع < len)
    {
        size_t بعدی_Period = پر_Name.find('.', شروع);
        إذا (بعدی_Period == npos)
        {
            بعدی_Period = len;
        }
        إذا (بعدی_Period >= شروع + 1u)
        {
            nameParts.emplace_back(پر_Name.substr(شروع, بعدی_Period - شروع));
        }
        شروع = بعدی_Period + 1u;
    }
    أرجع nameParts;
}

منطقي Log_برچسب_Manager::داخلی_is_Name_Part_تطابق(MatchingScope scope, size_t matchingPos)
{
    بدل (scope)
    {
    حالة MatchingScope::اولی_Name_Part:
        أرجع (matchingPos == 0u);
    حالة MatchingScope::AnyNamePart:
        أرجع درست;
    حالة MatchingScope::None:
    حالة MatchingScope::پر_2:
    افتراضي:
        أرجع خطأ;
    }
}

منطقي Log_برچسب_Manager::داخلی_apply_پر_Name_Config_مقصد_برچسب(پر_Name_Info& پر_Name_Info_2)
{
    إذا (!پر_Name_Info_2.log_برچسب_Ptr)
    {
        أرجع خطأ;
    }
    إذا (پر_Name_Info_2.parsedLevel.scope == MatchingScope::پر_2)
    {
        پر_Name_Info_2.log_برچسب_Ptr->level = پر_Name_Info_2.parsedLevel.level;
        أرجع درست;
    }
    أرجع خطأ;
}

منطقي Log_برچسب_Manager::داخلی_apply_Name_Part_Config_مقصد_Specific_برچسب(پر_Name_Lookup_نتیجه& پر_Name_نتیجه)
{
    ثابت پر_Name_Info& پر_Name_Info_2 = *پر_Name_نتیجه.m_پر_Name_Info_Ptr;
    Log_برچسب* ثابت log_برچسب = پر_Name_Info_2.log_برچسب_Ptr;
    إذا (!log_برچسب)
    {
        أرجع خطأ;
    }
    CV_Assert(پر_Name_نتیجه.m_پیدا_کن_Cross_References);
    ثابت خودکار& crossReferences = پر_Name_نتیجه.m_crossReferences;
    ثابت size_t matching_Name_Part_شمار = crossReferences.size();
    لكل (size_t k = 0u; k < matching_Name_Part_شمار; ++k)
    {
        ثابت خودکار& تطابق = crossReferences.at(k);
        ثابت خودکار& namePartInfo = *تطابق.m_namePartInfo;
        ثابت خودکار& parsedLevel = namePartInfo.parsedLevel;
        ثابت خودکار scope = parsedLevel.scope;
        ثابت LogLevel level = parsedLevel.level;
        ثابت size_t matchingPos = تطابق.m_matchingPos;
        ثابت منطقي is_تطابق = داخلی_is_Name_Part_تطابق(scope, matchingPos);
        إذا (is_تطابق)
        {
            log_برچسب->level = level;
            أرجع درست;
        }
    }
    أرجع خطأ;
}

فراغ Log_برچسب_Manager::داخلی_apply_Name_Part_Config_مقصد_Matching_برچسبها(Name_Part_Lookup_نتیجه& name_Part_نتیجه)
{
    CV_Assert(name_Part_نتیجه.m_پیدا_کن_Cross_References);
    ثابت خودکار& crossReferences = name_Part_نتیجه.m_crossReferences;
    ثابت size_t matching_پر_Name_شمار = crossReferences.size();
    NamePartInfo& namePartInfo = *name_Part_نتیجه.m_namePartInfoPtr;
    ثابت MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::پر_2);
    إذا (scope == MatchingScope::None)
    {
        أرجع;
    }
    ثابت LogLevel level = namePartInfo.parsedLevel.level;
    لكل (size_t k = 0u; k < matching_پر_Name_شمار; ++k)
    {
        ثابت خودکار& تطابق = crossReferences.at(k);
        پر_Name_Info& پر_Name_Info_2 = *تطابق.m_پر_Name_Info;
        Log_برچسب* log_برچسب_Ptr = پر_Name_Info_2.log_برچسب_Ptr;
        إذا (!log_برچسب_Ptr)
        {
            استمر;
        }
        إذا (پر_Name_Info_2.parsedLevel.scope == MatchingScope::پر_2)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            استمر;
        }
        ثابت size_t matchingPos = تطابق.m_matchingPos;
        ثابت منطقي is_تطابق = داخلی_is_Name_Part_تطابق(scope, matchingPos);
        إذا (!is_تطابق)
        {
            استمر;
        }
        log_برچسب_Ptr->level = level;
    }
}

فراغ Log_برچسب_Manager::Name_جدول::جمع_کن_Or_Lookup_پر_Name(پر_Name_Lookup_نتیجه& نتیجه)
{
    ثابت خودکار پر_Name_Id_And_Flag = داخلی_جمع_کن_Or_Lookup_پر_Name(نتیجه.m_پر_Name);
    نتیجه.m_پر_Name_Id = پر_Name_Id_And_Flag.first;
    نتیجه.m_nameParts = Log_برچسب_Manager::تقسیم_Name_Parts(نتیجه.m_پر_Name);
    داخلی_جمع_کن_Or_Lookup_Name_Parts(نتیجه.m_nameParts, نتیجه.m_namePartIds);
    ثابت منطقي isNew = پر_Name_Id_And_Flag.second;
    إذا (isNew)
    {
        داخلی_جمع_کن_Cross_Reference(نتیجه.m_پر_Name_Id, نتیجه.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    نتیجه.m_پر_Name_Info_Ptr = داخلی_get_پر_Name_Info(نتیجه.m_پر_Name_Id);
    إذا (نتیجه.m_پیدا_کن_Cross_References)
    {
        داخلی_پیدا_کن_Matching_Name_Parts_For_پر_Name(نتیجه);
    }
}

فراغ Log_برچسب_Manager::Name_جدول::جمع_کن_Or_Lookup_Name_Part(Name_Part_Lookup_نتیجه& نتیجه)
{
    نتیجه.m_namePartId = داخلی_جمع_کن_Or_Lookup_Name_Part(نتیجه.m_namePart);
    نتیجه.m_namePartInfoPtr = داخلی_get_Name_Part_Info(نتیجه.m_namePartId);
    إذا (نتیجه.m_پیدا_کن_Cross_References)
    {
        داخلی_پیدا_کن_Matching_پر_نامها_For_Name_Part(نتیجه);
    }
}

std::pair<size_t, منطقي> Log_برچسب_Manager::Name_جدول::داخلی_جمع_کن_Or_Lookup_پر_Name(ثابت std::string& پر_Name)
{
    ثابت خودکار پر_Name_Id_Iter = m_پر_Name_Ids.find(پر_Name);
    إذا (پر_Name_Id_Iter != m_پر_Name_Ids.end())
    {
        أرجع std::make_pair(پر_Name_Id_Iter->second, خطأ);
    }
    ثابت size_t پر_Name_Id = m_پر_Name_Infos.size();
    m_پر_Name_Infos.emplace_back(پر_Name_Info{});
    m_پر_Name_Ids.emplace(پر_Name, پر_Name_Id);
    أرجع std::make_pair(پر_Name_Id, درست);
}

فراغ Log_برچسب_Manager::Name_جدول::داخلی_جمع_کن_Or_Lookup_Name_Parts(ثابت std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    ثابت size_t name_Part_شمار = nameParts.size();
    namePartIds.resize(name_Part_شمار, ~(size_t)0u);
    لكل (size_t name_Part_شاخص = 0u; name_Part_شاخص < name_Part_شمار; ++name_Part_شاخص)
    {
        ثابت std::string& namePart = nameParts.at(name_Part_شاخص);
        ثابت size_t namePartId = داخلی_جمع_کن_Or_Lookup_Name_Part(namePart);
        namePartIds.at(name_Part_شاخص) = namePartId;
    }
}

size_t Log_برچسب_Manager::Name_جدول::داخلی_جمع_کن_Or_Lookup_Name_Part(ثابت std::string& namePart)
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

فراغ Log_برچسب_Manager::Name_جدول::داخلی_جمع_کن_Cross_Reference(size_t پر_Name_Id, ثابت std::vector<size_t>& namePartIds)
{
    ثابت size_t name_Part_شمار = namePartIds.size();
    لكل (size_t namePartPos = 0u; namePartPos < name_Part_شمار; ++namePartPos)
    {
        ثابت size_t namePartId = namePartIds.at(namePartPos);
        m_پر_Name_مقصد_Name_Part_Ids.emplace(پر_Name_Id, std::make_pair(namePartId, namePartPos));
        m_name_Part_مقصد_پر_Name_Ids.emplace(namePartId, std::make_pair(پر_Name_Id, namePartPos));
    }
}

Log_برچسب_Manager::پر_Name_Info* Log_برچسب_Manager::Name_جدول::get_پر_Name_Info(ثابت std::string& پر_Name)
{
    ثابت خودکار پر_Name_Id_Iter = m_پر_Name_Ids.find(پر_Name);
    إذا (پر_Name_Id_Iter == m_پر_Name_Ids.end())
    {
        أرجع اشاره_تهی;
    }
    ثابت size_t پر_Name_Id = پر_Name_Id_Iter->second;
    أرجع داخلی_get_پر_Name_Info(پر_Name_Id);
}

Log_برچسب_Manager::پر_Name_Info* Log_برچسب_Manager::Name_جدول::داخلی_get_پر_Name_Info(size_t پر_Name_Id)
{
    أرجع std::addressof(m_پر_Name_Infos.at(پر_Name_Id));
}

Log_برچسب_Manager::NamePartInfo* Log_برچسب_Manager::Name_جدول::داخلی_get_Name_Part_Info(size_t namePartId)
{
    أرجع std::addressof(m_namePartInfos.at(namePartId));
}

فراغ Log_برچسب_Manager::Name_جدول::داخلی_پیدا_کن_Matching_Name_Parts_For_پر_Name(پر_Name_Lookup_نتیجه& پر_Name_نتیجه)
{
    ثابت size_t پر_Name_Id = پر_Name_نتیجه.m_پر_Name_Id;
    پر_Name_Info* پر_Name_Info_2 = پر_Name_نتیجه.m_پر_Name_Info_Ptr;
    ثابت خودکار& namePartIds = پر_Name_نتیجه.m_namePartIds;
    ثابت size_t name_Part_شمار = namePartIds.size();
    خودکار& crossReferences = پر_Name_نتیجه.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_شمار);
    لكل (size_t matchingPos = 0u; matchingPos < name_Part_شمار; ++matchingPos)
    {
        ثابت size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = داخلی_get_Name_Part_Info(namePartId);
        crossReferences.emplace_back(CrossReference(پر_Name_Id, namePartId, matchingPos, پر_Name_Info_2, namePartInfo));
    }
}

فراغ Log_برچسب_Manager::Name_جدول::داخلی_پیدا_کن_Matching_پر_نامها_For_Name_Part(Name_Part_Lookup_نتیجه& نتیجه)
{
    ثابت size_t namePartId = نتیجه.m_namePartId;
    NamePartInfo* namePartInfo = نتیجه.m_namePartInfoPtr;
    ثابت size_t matching_پر_Name_شمار = m_name_Part_مقصد_پر_Name_Ids.شمار_2(namePartId);
    std::vector<CrossReference>& crossReferences = نتیجه.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_پر_Name_شمار);
    ثابت خودکار name_Part_مقصد_پر_Name_Iter_Pair = m_name_Part_مقصد_پر_Name_Ids.equal_range(نتیجه.m_namePartId);
    ثابت خودکار iterBegin = name_Part_مقصد_پر_Name_Iter_Pair.first;
    ثابت خودکار iter_انتها = name_Part_مقصد_پر_Name_Iter_Pair.second;
    لكل (خودکار iter = iterBegin; iter != iter_انتها; ++iter)
    {
        ثابت size_t پر_Name_Id = iter->second.first;
        ثابت size_t matchingPos = iter->second.second;
        پر_Name_Info* پر_Name_Info_2 = داخلی_get_پر_Name_Info(پر_Name_Id);
        crossReferences.emplace_back(CrossReference(پر_Name_Id, namePartId, matchingPos, پر_Name_Info_2, namePartInfo));
    }
}

}}} //namespace
