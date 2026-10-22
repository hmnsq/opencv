// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#اگر_غیر_تعریف_شدہ OPENCV_CORE_LOGTAGMANAGER_HPP
#تعریف_کرو OPENCV_CORE_LOGTAGMANAGER_HPP

#اگر 1 // if not already in precompiled headers
#شامل <string>
#شامل <algorithm>
#شامل <functional>
#شامل <unordered_map>
#شامل <mutex>
#شامل <memory>
#اگر_ختم

#شامل <opencv2/core/utils/logtag.hpp>
#شامل "logtagconfig.hpp"

نامخلا cv {
نامخلا utils {
نامخلا logging {

// forward declaration
جماعت Log_لیبل_Config_Parser;

// A lookup table of LogTags using full name, first part of name, and any part of name.
// The name parts of a LogTag is delimited by period.
//
// This class does not handle wildcard characters. The name-matching method can only be
// selected by calling the appropriate function.
//
جماعت Log_لیبل_Manager
{
نجی:
    // Current implementation does not seem to require recursive mutex;
    // also, extensible functions (accepting user-provided callback) are not allowed
    // to call LogTagManger (to prevent iterator invalidation), which needs enforced
    // with a non-recursive mutex.
    استعمال MutexType = cv::Mutex;
    استعمال تالا_Type = cv::Auto_تالا;

    شمار جماعت MatchingScope
    {
        None,
        بھرا_2,
        پہلا_Name_Part,
        AnyNamePart
    };

    ساخت ParsedLevel
    {
        LogLevel level;
        MatchingScope scope;

        ParsedLevel()
            : level()
            , scope(MatchingScope::None)
        {
        }
    };

    ساخت بھرا_Name_Info
    {
        Log_لیبل* log_لیبل_Ptr;
        ParsedLevel parsedLevel;
    };

    ساخت NamePartInfo
    {
        ParsedLevel parsedLevel;
    };

    ساخت CrossReference
    {
        size_t m_بھرا_Name_Id;
        size_t m_namePartId;
        size_t m_matchingPos;
        بھرا_Name_Info* m_بھرا_Name_Info;
        NamePartInfo* m_namePartInfo;

        صریح CrossReference(size_t بھرا_Name_Id, size_t namePartId, size_t matchingPos,
            بھرا_Name_Info* بھرا_Name_Info_2, NamePartInfo* namePartInfo)
            : m_بھرا_Name_Id(بھرا_Name_Id)
            , m_namePartId(namePartId)
            , m_matchingPos(matchingPos)
            , m_بھرا_Name_Info(بھرا_Name_Info_2)
            , m_namePartInfo(namePartInfo)
        {
        }
    };

    ساخت بھرا_Name_Lookup_نتیجہ
    {
        // The full name being looked up
        std::string m_بھرا_Name;

        // The full name being broken down into name parts
        std::vector<std::string> m_nameParts;

        // The full name ID that is added or looked up from the table
        size_t m_بھرا_Name_Id;

        // The name part IDs that are added to or looked up from the table
        // listed in the same order as m_nameParts
        std::vector<size_t> m_namePartIds;

        // The information struct for the full name
        بھرا_Name_Info* m_بھرا_Name_Info_Ptr;

        // Specifies whether cross references (full names that match the name part)
        // should be computed.
        منطقی m_ڈھونڈو_Cross_References;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        صریح بھرا_Name_Lookup_نتیجہ(ثابت std::string& بھرا_Name)
            : m_بھرا_Name(بھرا_Name)
            , m_nameParts()
            , m_بھرا_Name_Id()
            , m_namePartIds()
            , m_بھرا_Name_Info_Ptr()
            , m_ڈھونڈو_Cross_References()
            , m_crossReferences()
        {
        }
    };

    ساخت Name_Part_Lookup_نتیجہ
    {
        // The name part being looked up
        std::string m_namePart;

        // The name part ID that is added or looked up from the table
        size_t m_namePartId;

        // Information struct ptr for the name part
        NamePartInfo* m_namePartInfoPtr;

        // Specifies whether cross references (full names that match the name part) should be computed.
        منطقی m_ڈھونڈو_Cross_References;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        صریح Name_Part_Lookup_نتیجہ(ثابت std::string& namePart)
            : m_namePart(namePart)
            , m_namePartId()
            , m_namePartInfoPtr()
            , m_ڈھونڈو_Cross_References()
            , m_crossReferences()
        {
        }
    };

    ساخت Name_جدول
    {
        // All data structures in this class are append-only. The item count
        // is being used as an incrementing integer key.

    عوامی:
        // Full name information struct.
        std::vector<بھرا_Name_Info> m_بھرا_Name_Infos;

        // Name part information struct.
        std::vector<NamePartInfo> m_namePartInfos;

        // key: full name (string)
        // value: full name ID
        // .... (index into the vector of m_fullNameInfos)
        // .... (key into m_fullNameToNamePartIds)
        // .... (value.second in m_namePartToFullNameIds)
        std::unordered_map<std::string, size_t> m_بھرا_Name_Ids;

        // key: name part (string)
        // value: name part ID
        // .... (index into the vector of m_namePartInfos)
        // .... (key into m_namePartToFullNameIds)
        // .... (value.second in m_fullNameToNamePartIds)
        std::unordered_map<std::string, size_t> m_namePartIds;

        // key: full name ID
        // value.first: name part ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_بھرا_Name_مقصد_Name_Part_Ids;

        // key: name part ID
        // value.first: full name ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_name_Part_مقصد_بھرا_Name_Ids;

    عوامی:
        خالی جوڑو_Or_Lookup_بھرا_Name(بھرا_Name_Lookup_نتیجہ& نتیجہ);
        خالی جوڑو_Or_Lookup_Name_Part(Name_Part_Lookup_نتیجہ& نتیجہ);
        بھرا_Name_Info* get_بھرا_Name_Info(ثابت std::string& بھرا_Name);

    نجی:
        // Add or get full name. Does not compute name parts or access them in the table.
        // Returns the full name ID, and a bool indicating if the full name is new.
        std::pair<size_t, منطقی> اندرونی_جوڑو_Or_Lookup_بھرا_Name(ثابت std::string& بھرا_Name);

        // Add or get multiple name parts. Saves name part IDs into a vector.
        خالی اندرونی_جوڑو_Or_Lookup_Name_Parts(ثابت std::vector<std::string>& nameParts, std::vector<size_t>& namePartIds);

        // Add or get name part. Returns namePartId.
        size_t اندرونی_جوڑو_Or_Lookup_Name_Part(ثابت std::string& namePart);

        // For each name part ID, insert the tuples (full name, name part ID, name part position)
        // into the cross reference table
        خالی اندرونی_جوڑو_Cross_Reference(size_t بھرا_Name_Id, ثابت std::vector<size_t>& namePartIds);

        // Gather pointer for full name info struct.
        // Note: The pointer is interior to the table vector. The pointers are invalidated
        // if the table is modified.
        بھرا_Name_Info* اندرونی_get_بھرا_Name_Info(size_t بھرا_Name_Id);

        // Gather pointers for name part info struct.
        // Note: The pointers are interior to the table vector. The pointers are invalidated
        // if the table is modified.
        NamePartInfo* اندرونی_get_Name_Part_Info(size_t namePartId);

        خالی اندرونی_ڈھونڈو_Matching_Name_Parts_For_بھرا_Name(بھرا_Name_Lookup_نتیجہ& بھرا_Name_نتیجہ);
        خالی اندرونی_ڈھونڈو_Matching_بھرا_نام_For_Name_Part(Name_Part_Lookup_نتیجہ& نتیجہ);
    };

عوامی:
    Log_لیبل_Manager(LogLevel defaultUnconfiguredGlobalLevel);
    ~Log_لیبل_Manager();

عوامی:
    // Parse and apply the config string.
    خالی سیٹ_Config_String(ثابت std::string& configString, منطقی apply = سچ);

    // Gets the config parser. This is necessary to retrieve the list of malformed strings.
    Log_لیبل_Config_Parser& getConfigParser() ثابت;

    // Add (register) the log tag.
    // Note, passing in nullptr as value is equivalent to unassigning.
    خالی assign(ثابت std::string& بھرا_Name, Log_لیبل* ptr);

    // Unassign the log tag. This is equivalent to calling assign with nullptr value.
    خالی unassign(ثابت std::string& بھرا_Name);

    // Retrieve the log tag by exact name.
    Log_لیبل* get(ثابت std::string& بھرا_Name);

    // Changes the log level of the tag having the exact full name.
    خالی سیٹ_Level_By_بھرا_Name(ثابت std::string& بھرا_Name, LogLevel level);

    // Changes the log level of the tags matching the first part of the name.
    خالی سیٹ_Level_By_پہلا_Part(ثابت std::string& پہلا_Part, LogLevel level);

    // Changes the log level of the tags matching any part of the name.
    خالی سیٹ_Level_By_Any_Part(ثابت std::string& anyPart, LogLevel level);

    // Changes the log level of the tags with matching name part according
    // to the specified scope.
    خالی سیٹ_Level_By_Name_Part(ثابت std::string& namePart, LogLevel level, MatchingScope scope);

نجی:
    منطقی اندرونی_apply_بھرا_Name_Config_مقصد_لیبل(بھرا_Name_Info& بھرا_Name_Info_2);
    منطقی اندرونی_apply_Name_Part_Config_مقصد_Specific_لیبل(بھرا_Name_Lookup_نتیجہ& بھرا_Name_نتیجہ);
    خالی اندرونی_apply_Name_Part_Config_مقصد_Matching_لیبل_فہرست(Name_Part_Lookup_نتیجہ& name_Part_نتیجہ);

نجی:
    ساکن std::vector<std::string> بانٹو_Name_Parts(ثابت std::string& بھرا_Name);
    ساکن منطقی اندرونی_is_Name_Part_مطابقت(MatchingScope scope, size_t matchingPos);

نجی:
    ساکن ثابت حرف* m_globalName;

نجی:
    تغیرپذیر MutexType m_mutex;
    std::unique_ptr<Log_لیبل> m_global_Log_لیبل;
    Name_جدول m_name_جدول;
    std::shared_ptr<Log_لیبل_Config_Parser> m_config;
};

}}} //namespace

#اگر_ختم //OPENCV_CORE_LOGTAGMANAGER_HPP
