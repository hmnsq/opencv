// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "../precomp.hpp"
#ضمن "logtagmanager.hpp"
#ضمن "logtagconfigparser.hpp"

نطاق cv {
نطاق utils {
نطاق logging {


ثابت حرف* Log_نښان_Manager::m_globalName = "global";


Log_نښان_Manager::Log_نښان_Manager(LogLevel defaultUnconfiguredGlobalLevel)
    : m_mutex()
    , m_global_Log_نښان(جديد Log_نښان(m_globalName, defaultUnconfiguredGlobalLevel))
    , m_config(std::make_shared<Log_نښان_Config_Parser>(defaultUnconfiguredGlobalLevel))
{
    assign(m_globalName, m_global_Log_نښان.get());
}

Log_نښان_Manager::~Log_نښان_Manager()
{
}

فراغ Log_نښان_Manager::سېټ_Config_String(ثابت std::string& configString, منطقي apply /*true*/)
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
    ثابت خپلکاری& globalConfig = m_config->getGlobalConfig();
    m_global_Log_نښان->level = globalConfig.level;
    لكل (ثابت خپلکاری& config : m_config->get_لومړی_Part_Configs())
    {
        سېټ_Level_By_لومړی_Part(config.namePart, config.level);
    }
    لكل (ثابت خپلکاری& config : m_config->getAnyPartConfigs())
    {
        سېټ_Level_By_Any_Part(config.namePart, config.level);
    }
    لكل (ثابت خپلکاری& config : m_config->get_ډک_Name_Configs())
    {
        سېټ_Level_By_ډک_Name(config.namePart, config.level);
    }
}

Log_نښان_Config_Parser& Log_نښان_Manager::getConfigParser() ثابت
{
    أرجع *m_config;
}

فراغ Log_نښان_Manager::assign(ثابت std::string& ډک_Name, Log_نښان* ptr)
{
    CV_TRACE_FUNCTION();
    قلف_Type lock(m_mutex);
    ډک_Name_Lookup_پایله پایله(ډک_Name);
    پایله.m_پیدا_کړه_Cross_References = ریښتیا;
    m_name_جدول.جمع_کړه_Or_Lookup_ډک_Name(پایله);
    ډک_Name_Info& ډک_Name_Info_2 = *پایله.m_ډک_Name_Info_Ptr;
    ثابت منطقي is_Ptr_بدل_شوی = (ډک_Name_Info_2.log_نښان_Ptr != ptr);
    إذا (!is_Ptr_بدل_شوی)
    {
        أرجع;
    }
    ډک_Name_Info_2.log_نښان_Ptr = ptr;
    إذا (!ptr)
    {
        أرجع;
    }
    ثابت منطقي has_Applied_ډک_Name_Config = دننی_apply_ډک_Name_Config_منزل_نښان(ډک_Name_Info_2);
    إذا (has_Applied_ډک_Name_Config)
    {
        أرجع;
    }
    دننی_apply_Name_Part_Config_منزل_Specific_نښان(پایله);
}

فراغ Log_نښان_Manager::unassign(ثابت std::string& ډک_Name)
{
    // Lock is inside assign() method.
    assign(ډک_Name, تش_اشاره);
}

Log_نښان* Log_نښان_Manager::get(ثابت std::string& ډک_Name)
{
    CV_TRACE_FUNCTION();
    قلف_Type lock(m_mutex);
    ډک_Name_Info* ډک_Name_Info_Ptr = m_name_جدول.get_ډک_Name_Info(ډک_Name);
    إذا (ډک_Name_Info_Ptr && ډک_Name_Info_Ptr->log_نښان_Ptr)
    {
        أرجع ډک_Name_Info_Ptr->log_نښان_Ptr;
    }
    أرجع تش_اشاره;
}

فراغ Log_نښان_Manager::سېټ_Level_By_ډک_Name(ثابت std::string& ډک_Name, LogLevel level)
{
    CV_TRACE_FUNCTION();
    قلف_Type lock(m_mutex);
    ډک_Name_Lookup_پایله پایله(ډک_Name);
    پایله.m_پیدا_کړه_Cross_References = خطأ;
    m_name_جدول.جمع_کړه_Or_Lookup_ډک_Name(پایله);
    ډک_Name_Info& ډک_Name_Info_2 = *پایله.m_ډک_Name_Info_Ptr;
    إذا (ډک_Name_Info_2.parsedLevel.scope == MatchingScope::ډک_2 &&
        ډک_Name_Info_2.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        أرجع;
    }
    // update the cached configured value.
    ډک_Name_Info_2.parsedLevel.scope = MatchingScope::ډک_2;
    ډک_Name_Info_2.parsedLevel.level = level;
    // update the actual tag, if already registered.
    Log_نښان* log_نښان_Ptr = ډک_Name_Info_2.log_نښان_Ptr;
    إذا (log_نښان_Ptr)
    {
        log_نښان_Ptr->level = level;
    }
}

فراغ Log_نښان_Manager::سېټ_Level_By_لومړی_Part(ثابت std::string& لومړی_Part, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    سېټ_Level_By_Name_Part(لومړی_Part, level, MatchingScope::لومړی_Name_Part);
}

فراغ Log_نښان_Manager::سېټ_Level_By_Any_Part(ثابت std::string& anyPart, LogLevel level)
{
    // Lock is inside setLevelByNamePart() method.
    سېټ_Level_By_Name_Part(anyPart, level, MatchingScope::AnyNamePart);
}

فراغ Log_نښان_Manager::سېټ_Level_By_Name_Part(ثابت std::string& namePart, LogLevel level, MatchingScope scope)
{
    CV_TRACE_FUNCTION();
    قلف_Type lock(m_mutex);
    Name_Part_Lookup_پایله پایله(namePart);
    پایله.m_پیدا_کړه_Cross_References = ریښتیا;
    m_name_جدول.جمع_کړه_Or_Lookup_Name_Part(پایله);
    NamePartInfo& namePartInfo = *پایله.m_namePartInfoPtr;
    إذا (namePartInfo.parsedLevel.scope == scope &&
        namePartInfo.parsedLevel.level == level)
    {
        // skip additional processing if nothing changes.
        أرجع;
    }
    namePartInfo.parsedLevel.scope = scope;
    namePartInfo.parsedLevel.level = level;
    دننی_apply_Name_Part_Config_منزل_Matching_نښانونه(پایله);
}

std::vector<std::string> Log_نښان_Manager::وېشل_Name_Parts(ثابت std::string& ډک_Name)
{
    ثابت size_t npos = std::string::npos;
    ثابت size_t len = ډک_Name.length();
    std::vector<std::string> nameParts;
    size_t پیل = 0u;
    طالما (پیل < len)
    {
        size_t بل_Period = ډک_Name.find('.', پیل);
        إذا (بل_Period == npos)
        {
            بل_Period = len;
        }
        إذا (بل_Period >= پیل + 1u)
        {
            nameParts.emplace_back(ډک_Name.substr(پیل, بل_Period - پیل));
        }
        پیل = بل_Period + 1u;
    }
    أرجع nameParts;
}

منطقي Log_نښان_Manager::دننی_is_Name_Part_سمون(MatchingScope scope, size_t matchingPos)
{
    بدل (scope)
    {
    حالة MatchingScope::لومړی_Name_Part:
        أرجع (matchingPos == 0u);
    حالة MatchingScope::AnyNamePart:
        أرجع ریښتیا;
    حالة MatchingScope::None:
    حالة MatchingScope::ډک_2:
    افتراضي:
        أرجع خطأ;
    }
}

منطقي Log_نښان_Manager::دننی_apply_ډک_Name_Config_منزل_نښان(ډک_Name_Info& ډک_Name_Info_2)
{
    إذا (!ډک_Name_Info_2.log_نښان_Ptr)
    {
        أرجع خطأ;
    }
    إذا (ډک_Name_Info_2.parsedLevel.scope == MatchingScope::ډک_2)
    {
        ډک_Name_Info_2.log_نښان_Ptr->level = ډک_Name_Info_2.parsedLevel.level;
        أرجع ریښتیا;
    }
    أرجع خطأ;
}

منطقي Log_نښان_Manager::دننی_apply_Name_Part_Config_منزل_Specific_نښان(ډک_Name_Lookup_پایله& ډک_Name_پایله)
{
    ثابت ډک_Name_Info& ډک_Name_Info_2 = *ډک_Name_پایله.m_ډک_Name_Info_Ptr;
    Log_نښان* ثابت log_نښان = ډک_Name_Info_2.log_نښان_Ptr;
    إذا (!log_نښان)
    {
        أرجع خطأ;
    }
    CV_Assert(ډک_Name_پایله.m_پیدا_کړه_Cross_References);
    ثابت خپلکاری& crossReferences = ډک_Name_پایله.m_crossReferences;
    ثابت size_t matching_Name_Part_شمېر = crossReferences.size();
    لكل (size_t k = 0u; k < matching_Name_Part_شمېر; ++k)
    {
        ثابت خپلکاری& سمون = crossReferences.at(k);
        ثابت خپلکاری& namePartInfo = *سمون.m_namePartInfo;
        ثابت خپلکاری& parsedLevel = namePartInfo.parsedLevel;
        ثابت خپلکاری scope = parsedLevel.scope;
        ثابت LogLevel level = parsedLevel.level;
        ثابت size_t matchingPos = سمون.m_matchingPos;
        ثابت منطقي is_سمون = دننی_is_Name_Part_سمون(scope, matchingPos);
        إذا (is_سمون)
        {
            log_نښان->level = level;
            أرجع ریښتیا;
        }
    }
    أرجع خطأ;
}

فراغ Log_نښان_Manager::دننی_apply_Name_Part_Config_منزل_Matching_نښانونه(Name_Part_Lookup_پایله& name_Part_پایله)
{
    CV_Assert(name_Part_پایله.m_پیدا_کړه_Cross_References);
    ثابت خپلکاری& crossReferences = name_Part_پایله.m_crossReferences;
    ثابت size_t matching_ډک_Name_شمېر = crossReferences.size();
    NamePartInfo& namePartInfo = *name_Part_پایله.m_namePartInfoPtr;
    ثابت MatchingScope scope = namePartInfo.parsedLevel.scope;
    CV_Assert(scope != MatchingScope::ډک_2);
    إذا (scope == MatchingScope::None)
    {
        أرجع;
    }
    ثابت LogLevel level = namePartInfo.parsedLevel.level;
    لكل (size_t k = 0u; k < matching_ډک_Name_شمېر; ++k)
    {
        ثابت خپلکاری& سمون = crossReferences.at(k);
        ډک_Name_Info& ډک_Name_Info_2 = *سمون.m_ډک_Name_Info;
        Log_نښان* log_نښان_Ptr = ډک_Name_Info_2.log_نښان_Ptr;
        إذا (!log_نښان_Ptr)
        {
            استمر;
        }
        إذا (ډک_Name_Info_2.parsedLevel.scope == MatchingScope::ډک_2)
        {
            // If the full name already has valid config, that full name config
            // has precedence over name part config.
            استمر;
        }
        ثابت size_t matchingPos = سمون.m_matchingPos;
        ثابت منطقي is_سمون = دننی_is_Name_Part_سمون(scope, matchingPos);
        إذا (!is_سمون)
        {
            استمر;
        }
        log_نښان_Ptr->level = level;
    }
}

فراغ Log_نښان_Manager::Name_جدول::جمع_کړه_Or_Lookup_ډک_Name(ډک_Name_Lookup_پایله& پایله)
{
    ثابت خپلکاری ډک_Name_Id_And_Flag = دننی_جمع_کړه_Or_Lookup_ډک_Name(پایله.m_ډک_Name);
    پایله.m_ډک_Name_Id = ډک_Name_Id_And_Flag.first;
    پایله.m_nameParts = Log_نښان_Manager::وېشل_Name_Parts(پایله.m_ډک_Name);
    دننی_جمع_کړه_Or_Lookup_Name_Parts(پایله.m_nameParts, پایله.m_namePartIds);
    ثابت منطقي isNew = ډک_Name_Id_And_Flag.second;
    إذا (isNew)
    {
        دننی_جمع_کړه_Cross_Reference(پایله.m_ډک_Name_Id, پایله.m_namePartIds);
    }
    // ====== IMPORTANT ====== Critical order-of-operation ======
    // The gathering of the pointers of FullNameInfo and NamePartInfo are performed
    // as the last step of the operation, so that these pointer are not invalidated
    // by the vector append operations earlier in this function.
    // ======
    پایله.m_ډک_Name_Info_Ptr = دننی_get_ډک_Name_Info(پایله.m_ډک_Name_Id);
    إذا (پایله.m_پیدا_کړه_Cross_References)
    {
        دننی_پیدا_کړه_Matching_Name_Parts_For_ډک_Name(پایله);
    }
}

فراغ Log_نښان_Manager::Name_جدول::جمع_کړه_Or_Lookup_Name_Part(Name_Part_Lookup_پایله& پایله)
{
    پایله.m_namePartId = دننی_جمع_کړه_Or_Lookup_Name_Part(پایله.m_namePart);
    پایله.m_namePartInfoPtr = دننی_get_Name_Part_Info(پایله.m_namePartId);
    إذا (پایله.m_پیدا_کړه_Cross_References)
    {
        دننی_پیدا_کړه_Matching_ډک_نومونه_For_Name_Part(پایله);
    }
}

std::pair<size_t, منطقي> Log_نښان_Manager::Name_جدول::دننی_جمع_کړه_Or_Lookup_ډک_Name(ثابت std::string& ډک_Name)
{
    ثابت خپلکاری ډک_Name_Id_Iter = m_ډک_Name_Ids.find(ډک_Name);
    إذا (ډک_Name_Id_Iter != m_ډک_Name_Ids.end())
    {
        أرجع std::make_pair(ډک_Name_Id_Iter->second, خطأ);
    }
    ثابت size_t ډک_Name_Id = m_ډک_Name_Infos.size();
    m_ډک_Name_Infos.emplace_back(ډک_Name_Info{});
    m_ډک_Name_Ids.emplace(ډک_Name, ډک_Name_Id);
    أرجع std::make_pair(ډک_Name_Id, ریښتیا);
}

فراغ Log_نښان_Manager::Name_جدول::دننی_جمع_کړه_Or_Lookup_Name_Parts(ثابت std::vector<std::string>& nameParts,
    std::vector<size_t>& namePartIds)
{
    ثابت size_t name_Part_شمېر = nameParts.size();
    namePartIds.resize(name_Part_شمېر, ~(size_t)0u);
    لكل (size_t name_Part_شاخص = 0u; name_Part_شاخص < name_Part_شمېر; ++name_Part_شاخص)
    {
        ثابت std::string& namePart = nameParts.at(name_Part_شاخص);
        ثابت size_t namePartId = دننی_جمع_کړه_Or_Lookup_Name_Part(namePart);
        namePartIds.at(name_Part_شاخص) = namePartId;
    }
}

size_t Log_نښان_Manager::Name_جدول::دننی_جمع_کړه_Or_Lookup_Name_Part(ثابت std::string& namePart)
{
    ثابت خپلکاری namePartIter = m_namePartIds.find(namePart);
    إذا (namePartIter != m_namePartIds.end())
    {
        أرجع namePartIter->second;
    }
    ثابت size_t namePartId = m_namePartInfos.size();
    m_namePartInfos.emplace_back(NamePartInfo{});
    m_namePartIds.emplace(namePart, namePartId);
    أرجع namePartId;
}

فراغ Log_نښان_Manager::Name_جدول::دننی_جمع_کړه_Cross_Reference(size_t ډک_Name_Id, ثابت std::vector<size_t>& namePartIds)
{
    ثابت size_t name_Part_شمېر = namePartIds.size();
    لكل (size_t namePartPos = 0u; namePartPos < name_Part_شمېر; ++namePartPos)
    {
        ثابت size_t namePartId = namePartIds.at(namePartPos);
        m_ډک_Name_منزل_Name_Part_Ids.emplace(ډک_Name_Id, std::make_pair(namePartId, namePartPos));
        m_name_Part_منزل_ډک_Name_Ids.emplace(namePartId, std::make_pair(ډک_Name_Id, namePartPos));
    }
}

Log_نښان_Manager::ډک_Name_Info* Log_نښان_Manager::Name_جدول::get_ډک_Name_Info(ثابت std::string& ډک_Name)
{
    ثابت خپلکاری ډک_Name_Id_Iter = m_ډک_Name_Ids.find(ډک_Name);
    إذا (ډک_Name_Id_Iter == m_ډک_Name_Ids.end())
    {
        أرجع تش_اشاره;
    }
    ثابت size_t ډک_Name_Id = ډک_Name_Id_Iter->second;
    أرجع دننی_get_ډک_Name_Info(ډک_Name_Id);
}

Log_نښان_Manager::ډک_Name_Info* Log_نښان_Manager::Name_جدول::دننی_get_ډک_Name_Info(size_t ډک_Name_Id)
{
    أرجع std::addressof(m_ډک_Name_Infos.at(ډک_Name_Id));
}

Log_نښان_Manager::NamePartInfo* Log_نښان_Manager::Name_جدول::دننی_get_Name_Part_Info(size_t namePartId)
{
    أرجع std::addressof(m_namePartInfos.at(namePartId));
}

فراغ Log_نښان_Manager::Name_جدول::دننی_پیدا_کړه_Matching_Name_Parts_For_ډک_Name(ډک_Name_Lookup_پایله& ډک_Name_پایله)
{
    ثابت size_t ډک_Name_Id = ډک_Name_پایله.m_ډک_Name_Id;
    ډک_Name_Info* ډک_Name_Info_2 = ډک_Name_پایله.m_ډک_Name_Info_Ptr;
    ثابت خپلکاری& namePartIds = ډک_Name_پایله.m_namePartIds;
    ثابت size_t name_Part_شمېر = namePartIds.size();
    خپلکاری& crossReferences = ډک_Name_پایله.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(name_Part_شمېر);
    لكل (size_t matchingPos = 0u; matchingPos < name_Part_شمېر; ++matchingPos)
    {
        ثابت size_t namePartId = namePartIds.at(matchingPos);
        NamePartInfo* namePartInfo = دننی_get_Name_Part_Info(namePartId);
        crossReferences.emplace_back(CrossReference(ډک_Name_Id, namePartId, matchingPos, ډک_Name_Info_2, namePartInfo));
    }
}

فراغ Log_نښان_Manager::Name_جدول::دننی_پیدا_کړه_Matching_ډک_نومونه_For_Name_Part(Name_Part_Lookup_پایله& پایله)
{
    ثابت size_t namePartId = پایله.m_namePartId;
    NamePartInfo* namePartInfo = پایله.m_namePartInfoPtr;
    ثابت size_t matching_ډک_Name_شمېر = m_name_Part_منزل_ډک_Name_Ids.شمېر_2(namePartId);
    std::vector<CrossReference>& crossReferences = پایله.m_crossReferences;
    crossReferences.clear();
    crossReferences.reserve(matching_ډک_Name_شمېر);
    ثابت خپلکاری name_Part_منزل_ډک_Name_Iter_Pair = m_name_Part_منزل_ډک_Name_Ids.equal_range(پایله.m_namePartId);
    ثابت خپلکاری iterBegin = name_Part_منزل_ډک_Name_Iter_Pair.first;
    ثابت خپلکاری iter_پای_ټکی = name_Part_منزل_ډک_Name_Iter_Pair.second;
    لكل (خپلکاری iter = iterBegin; iter != iter_پای_ټکی; ++iter)
    {
        ثابت size_t ډک_Name_Id = iter->second.first;
        ثابت size_t matchingPos = iter->second.second;
        ډک_Name_Info* ډک_Name_Info_2 = دننی_get_ډک_Name_Info(ډک_Name_Id);
        crossReferences.emplace_back(CrossReference(ډک_Name_Id, namePartId, matchingPos, ډک_Name_Info_2, namePartInfo));
    }
}

}}} //namespace
