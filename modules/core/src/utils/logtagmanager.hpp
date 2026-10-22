// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#جے_بنا_تعریف OPENCV_CORE_LOGTAGMANAGER_HPP
#تعریف_کرو OPENCV_CORE_LOGTAGMANAGER_HPP

#إذا 1 // if not already in precompiled headers
#ضمن <string>
#ضمن <algorithm>
#ضمن <functional>
#ضمن <unordered_map>
#ضمن <mutex>
#ضمن <memory>
#جے_انت

#ضمن <opencv2/core/utils/logtag.hpp>
#ضمن "logtagconfig.hpp"

نطاق cv {
نطاق utils {
نطاق logging {

// forward declaration
صنف Log_ٹیگ_Config_Parser;

// A lookup table of LogTags using full name, first part of name, and any part of name.
// The name parts of a LogTag is delimited by period.
//
// This class does not handle wildcard characters. The name-matching method can only be
// selected by calling the appropriate function.
//
صنف Log_ٹیگ_Manager
{
خاص:
    // Current implementation does not seem to require recursive mutex;
    // also, extensible functions (accepting user-provided callback) are not allowed
    // to call LogTagManger (to prevent iterator invalidation), which needs enforced
    // with a non-recursive mutex.
    ورتوں MutexType = cv::Mutex;
    ورتوں جندرا_Type = cv::Auto_جندرا;

    تعداد صنف MatchingScope
    {
        None,
        بھریا_2,
        پہلا_Name_Part,
        AnyNamePart
    };

    بنية ParsedLevel
    {
        LogLevel level;
        MatchingScope scope;

        ParsedLevel()
            : level()
            , scope(MatchingScope::None)
        {
        }
    };

    بنية بھریا_Name_Info
    {
        Log_ٹیگ* log_ٹیگ_Ptr;
        ParsedLevel parsedLevel;
    };

    بنية NamePartInfo
    {
        ParsedLevel parsedLevel;
    };

    بنية CrossReference
    {
        size_t m_بھریا_Name_Id;
        size_t m_namePartId;
        size_t m_matchingPos;
        بھریا_Name_Info* m_بھریا_Name_Info;
        NamePartInfo* m_namePartInfo;

        واضح CrossReference(size_t بھریا_Name_Id, size_t namePartId, size_t matchingPos,
            بھریا_Name_Info* بھریا_Name_Info_2, NamePartInfo* namePartInfo)
            : m_بھریا_Name_Id(بھریا_Name_Id)
            , m_namePartId(namePartId)
            , m_matchingPos(matchingPos)
            , m_بھریا_Name_Info(بھریا_Name_Info_2)
            , m_namePartInfo(namePartInfo)
        {
        }
    };

    بنية بھریا_Name_Lookup_نتیجہ
    {
        // The full name being looked up
        std::string m_بھریا_Name;

        // The full name being broken down into name parts
        std::vector<std::string> m_nameParts;

        // The full name ID that is added or looked up from the table
        size_t m_بھریا_Name_Id;

        // The name part IDs that are added to or looked up from the table
        // listed in the same order as m_nameParts
        std::vector<size_t> m_namePartIds;

        // The information struct for the full name
        بھریا_Name_Info* m_بھریا_Name_Info_Ptr;

        // Specifies whether cross references (full names that match the name part)
        // should be computed.
        منطقي m_لبھو_Cross_References;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        واضح بھریا_Name_Lookup_نتیجہ(ثابت std::string& بھریا_Name)
            : m_بھریا_Name(بھریا_Name)
            , m_nameParts()
            , m_بھریا_Name_Id()
            , m_namePartIds()
            , m_بھریا_Name_Info_Ptr()
            , m_لبھو_Cross_References()
            , m_crossReferences()
        {
        }
    };

    بنية Name_Part_Lookup_نتیجہ
    {
        // The name part being looked up
        std::string m_namePart;

        // The name part ID that is added or looked up from the table
        size_t m_namePartId;

        // Information struct ptr for the name part
        NamePartInfo* m_namePartInfoPtr;

        // Specifies whether cross references (full names that match the name part) should be computed.
        منطقي m_لبھو_Cross_References;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        واضح Name_Part_Lookup_نتیجہ(ثابت std::string& namePart)
            : m_namePart(namePart)
            , m_namePartId()
            , m_namePartInfoPtr()
            , m_لبھو_Cross_References()
            , m_crossReferences()
        {
        }
    };

    بنية Name_جدول
    {
        // All data structures in this class are append-only. The item count
        // is being used as an incrementing integer key.

    عام:
        // Full name information struct.
        std::vector<بھریا_Name_Info> m_بھریا_Name_Infos;

        // Name part information struct.
        std::vector<NamePartInfo> m_namePartInfos;

        // key: full name (string)
        // value: full name ID
        // .... (index into the vector of m_fullNameInfos)
        // .... (key into m_fullNameToNamePartIds)
        // .... (value.second in m_namePartToFullNameIds)
        std::unordered_map<std::string, size_t> m_بھریا_Name_Ids;

        // key: name part (string)
        // value: name part ID
        // .... (index into the vector of m_namePartInfos)
        // .... (key into m_namePartToFullNameIds)
        // .... (value.second in m_fullNameToNamePartIds)
        std::unordered_map<std::string, size_t> m_namePartIds;

        // key: full name ID
        // value.first: name part ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_بھریا_Name_منزل_Name_Part_Ids;

        // key: name part ID
        // value.first: full name ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_name_Part_منزل_بھریا_Name_Ids;

    عام:
        فراغ جوڑو_Or_Lookup_بھریا_Name(بھریا_Name_Lookup_نتیجہ& نتیجہ);
        فراغ جوڑو_Or_Lookup_Name_Part(Name_Part_Lookup_نتیجہ& نتیجہ);
        بھریا_Name_Info* get_بھریا_Name_Info(ثابت std::string& بھریا_Name);

    خاص:
        // Add or get full name. Does not compute name parts or access them in the table.
        // Returns the full name ID, and a bool indicating if the full name is new.
        std::pair<size_t, منطقي> اندرونی_جوڑو_Or_Lookup_بھریا_Name(ثابت std::string& بھریا_Name);

        // Add or get multiple name parts. Saves name part IDs into a vector.
        فراغ اندرونی_جوڑو_Or_Lookup_Name_Parts(ثابت std::vector<std::string>& nameParts, std::vector<size_t>& namePartIds);

        // Add or get name part. Returns namePartId.
        size_t اندرونی_جوڑو_Or_Lookup_Name_Part(ثابت std::string& namePart);

        // For each name part ID, insert the tuples (full name, name part ID, name part position)
        // into the cross reference table
        فراغ اندرونی_جوڑو_Cross_Reference(size_t بھریا_Name_Id, ثابت std::vector<size_t>& namePartIds);

        // Gather pointer for full name info struct.
        // Note: The pointer is interior to the table vector. The pointers are invalidated
        // if the table is modified.
        بھریا_Name_Info* اندرونی_get_بھریا_Name_Info(size_t بھریا_Name_Id);

        // Gather pointers for name part info struct.
        // Note: The pointers are interior to the table vector. The pointers are invalidated
        // if the table is modified.
        NamePartInfo* اندرونی_get_Name_Part_Info(size_t namePartId);

        فراغ اندرونی_لبھو_Matching_Name_Parts_For_بھریا_Name(بھریا_Name_Lookup_نتیجہ& بھریا_Name_نتیجہ);
        فراغ اندرونی_لبھو_Matching_بھریا_ناں_For_Name_Part(Name_Part_Lookup_نتیجہ& نتیجہ);
    };

عام:
    Log_ٹیگ_Manager(LogLevel defaultUnconfiguredGlobalLevel);
    ~Log_ٹیگ_Manager();

عام:
    // Parse and apply the config string.
    فراغ سیٹ_Config_String(ثابت std::string& configString, منطقي apply = سچ);

    // Gets the config parser. This is necessary to retrieve the list of malformed strings.
    Log_ٹیگ_Config_Parser& getConfigParser() ثابت;

    // Add (register) the log tag.
    // Note, passing in nullptr as value is equivalent to unassigning.
    فراغ assign(ثابت std::string& بھریا_Name, Log_ٹیگ* ptr);

    // Unassign the log tag. This is equivalent to calling assign with nullptr value.
    فراغ unassign(ثابت std::string& بھریا_Name);

    // Retrieve the log tag by exact name.
    Log_ٹیگ* get(ثابت std::string& بھریا_Name);

    // Changes the log level of the tag having the exact full name.
    فراغ سیٹ_Level_By_بھریا_Name(ثابت std::string& بھریا_Name, LogLevel level);

    // Changes the log level of the tags matching the first part of the name.
    فراغ سیٹ_Level_By_پہلا_Part(ثابت std::string& پہلا_Part, LogLevel level);

    // Changes the log level of the tags matching any part of the name.
    فراغ سیٹ_Level_By_Any_Part(ثابت std::string& anyPart, LogLevel level);

    // Changes the log level of the tags with matching name part according
    // to the specified scope.
    فراغ سیٹ_Level_By_Name_Part(ثابت std::string& namePart, LogLevel level, MatchingScope scope);

خاص:
    منطقي اندرونی_apply_بھریا_Name_Config_منزل_ٹیگ(بھریا_Name_Info& بھریا_Name_Info_2);
    منطقي اندرونی_apply_Name_Part_Config_منزل_Specific_ٹیگ(بھریا_Name_Lookup_نتیجہ& بھریا_Name_نتیجہ);
    فراغ اندرونی_apply_Name_Part_Config_منزل_Matching_ٹیگ_سوچی(Name_Part_Lookup_نتیجہ& name_Part_نتیجہ);

خاص:
    ساكن std::vector<std::string> ونڈو_Name_Parts(ثابت std::string& بھریا_Name);
    ساكن منطقي اندرونی_is_Name_Part_میل(MatchingScope scope, size_t matchingPos);

خاص:
    ساكن ثابت حرف* m_globalName;

خاص:
    بدلنجوگ MutexType m_mutex;
    std::unique_ptr<Log_ٹیگ> m_global_Log_ٹیگ;
    Name_جدول m_name_جدول;
    std::shared_ptr<Log_ٹیگ_Config_Parser> m_config;
};

}}} //namespace

#جے_انت //OPENCV_CORE_LOGTAGMANAGER_HPP
