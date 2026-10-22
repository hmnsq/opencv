// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_LOGTAGMANAGER_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_LOGTAGMANAGER_HPP

#ਜੇ 1 // if not already in precompiled headers
#ਅਨ੍ਤਰ੍ਭਾਵ <string>
#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>
#ਅਨ੍ਤਰ੍ਭਾਵ <functional>
#ਅਨ੍ਤਰ੍ਭਾਵ <unordered_map>
#ਅਨ੍ਤਰ੍ਭਾਵ <mutex>
#ਅਨ੍ਤਰ੍ਭਾਵ <memory>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logtag.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ "logtagconfig.hpp"

ਨਾਮਾਕਾਸ਼ cv {
ਨਾਮਾਕਾਸ਼ utils {
ਨਾਮਾਕਾਸ਼ logging {

// forward declaration
ਵਰਗ Log_ਟੈਗ_Config_Parser;

// A lookup table of LogTags using full name, first part of name, and any part of name.
// The name parts of a LogTag is delimited by period.
//
// This class does not handle wildcard characters. The name-matching method can only be
// selected by calling the appropriate function.
//
ਵਰਗ Log_ਟੈਗ_Manager
{
ਨਿਜੀ:
    // Current implementation does not seem to require recursive mutex;
    // also, extensible functions (accepting user-provided callback) are not allowed
    // to call LogTagManger (to prevent iterator invalidation), which needs enforced
    // with a non-recursive mutex.
    ਵਰਤੋਂ MutexType = cv::Mutex;
    ਵਰਤੋਂ ਤਾਲਾ_Type = cv::Auto_ਤਾਲਾ;

    ਗਿਣਤੀ ਵਰਗ MatchingScope
    {
        None,
        ਭਰਿਆ_2,
        ਪਹਿਲਾ_Name_Part,
        AnyNamePart
    };

    ਬਣਤਰ ParsedLevel
    {
        LogLevel level;
        MatchingScope scope;

        ParsedLevel()
            : level()
            , scope(MatchingScope::None)
        {
        }
    };

    ਬਣਤਰ ਭਰਿਆ_Name_Info
    {
        Log_ਟੈਗ* log_ਟੈਗ_Ptr;
        ParsedLevel parsedLevel;
    };

    ਬਣਤਰ NamePartInfo
    {
        ParsedLevel parsedLevel;
    };

    ਬਣਤਰ CrossReference
    {
        size_t m_ਭਰਿਆ_Name_Id;
        size_t m_namePartId;
        size_t m_matchingPos;
        ਭਰਿਆ_Name_Info* m_ਭਰਿਆ_Name_Info;
        NamePartInfo* m_namePartInfo;

        ਸਪਸ਼ਟ CrossReference(size_t ਭਰਿਆ_Name_Id, size_t namePartId, size_t matchingPos,
            ਭਰਿਆ_Name_Info* ਭਰਿਆ_Name_Info_2, NamePartInfo* namePartInfo)
            : m_ਭਰਿਆ_Name_Id(ਭਰਿਆ_Name_Id)
            , m_namePartId(namePartId)
            , m_matchingPos(matchingPos)
            , m_ਭਰਿਆ_Name_Info(ਭਰਿਆ_Name_Info_2)
            , m_namePartInfo(namePartInfo)
        {
        }
    };

    ਬਣਤਰ ਭਰਿਆ_Name_Lookup_ਨਤੀਜਾ
    {
        // The full name being looked up
        std::string m_ਭਰਿਆ_Name;

        // The full name being broken down into name parts
        std::vector<std::string> m_nameParts;

        // The full name ID that is added or looked up from the table
        size_t m_ਭਰਿਆ_Name_Id;

        // The name part IDs that are added to or looked up from the table
        // listed in the same order as m_nameParts
        std::vector<size_t> m_namePartIds;

        // The information struct for the full name
        ਭਰਿਆ_Name_Info* m_ਭਰਿਆ_Name_Info_Ptr;

        // Specifies whether cross references (full names that match the name part)
        // should be computed.
        ਤਾਰਕਿਕ m_ਲੱਭੋ_Cross_References;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        ਸਪਸ਼ਟ ਭਰਿਆ_Name_Lookup_ਨਤੀਜਾ(ਅਚਲ std::string& ਭਰਿਆ_Name)
            : m_ਭਰਿਆ_Name(ਭਰਿਆ_Name)
            , m_nameParts()
            , m_ਭਰਿਆ_Name_Id()
            , m_namePartIds()
            , m_ਭਰਿਆ_Name_Info_Ptr()
            , m_ਲੱਭੋ_Cross_References()
            , m_crossReferences()
        {
        }
    };

    ਬਣਤਰ Name_Part_Lookup_ਨਤੀਜਾ
    {
        // The name part being looked up
        std::string m_namePart;

        // The name part ID that is added or looked up from the table
        size_t m_namePartId;

        // Information struct ptr for the name part
        NamePartInfo* m_namePartInfoPtr;

        // Specifies whether cross references (full names that match the name part) should be computed.
        ਤਾਰਕਿਕ m_ਲੱਭੋ_Cross_References;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        ਸਪਸ਼ਟ Name_Part_Lookup_ਨਤੀਜਾ(ਅਚਲ std::string& namePart)
            : m_namePart(namePart)
            , m_namePartId()
            , m_namePartInfoPtr()
            , m_ਲੱਭੋ_Cross_References()
            , m_crossReferences()
        {
        }
    };

    ਬਣਤਰ Name_ਸਾਰਣੀ
    {
        // All data structures in this class are append-only. The item count
        // is being used as an incrementing integer key.

    ਜਨਤਕ:
        // Full name information struct.
        std::vector<ਭਰਿਆ_Name_Info> m_ਭਰਿਆ_Name_Infos;

        // Name part information struct.
        std::vector<NamePartInfo> m_namePartInfos;

        // key: full name (string)
        // value: full name ID
        // .... (index into the vector of m_fullNameInfos)
        // .... (key into m_fullNameToNamePartIds)
        // .... (value.second in m_namePartToFullNameIds)
        std::unordered_map<std::string, size_t> m_ਭਰਿਆ_Name_Ids;

        // key: name part (string)
        // value: name part ID
        // .... (index into the vector of m_namePartInfos)
        // .... (key into m_namePartToFullNameIds)
        // .... (value.second in m_fullNameToNamePartIds)
        std::unordered_map<std::string, size_t> m_namePartIds;

        // key: full name ID
        // value.first: name part ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_ਭਰਿਆ_Name_ਮੰਜ਼ਿਲ_Name_Part_Ids;

        // key: name part ID
        // value.first: full name ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_name_Part_ਮੰਜ਼ਿਲ_ਭਰਿਆ_Name_Ids;

    ਜਨਤਕ:
        ਖਾਲੀ ਜੋੜੋ_Or_Lookup_ਭਰਿਆ_Name(ਭਰਿਆ_Name_Lookup_ਨਤੀਜਾ& ਨਤੀਜਾ);
        ਖਾਲੀ ਜੋੜੋ_Or_Lookup_Name_Part(Name_Part_Lookup_ਨਤੀਜਾ& ਨਤੀਜਾ);
        ਭਰਿਆ_Name_Info* get_ਭਰਿਆ_Name_Info(ਅਚਲ std::string& ਭਰਿਆ_Name);

    ਨਿਜੀ:
        // Add or get full name. Does not compute name parts or access them in the table.
        // Returns the full name ID, and a bool indicating if the full name is new.
        std::pair<size_t, ਤਾਰਕਿਕ> ਅੰਦਰੂਨੀ_ਜੋੜੋ_Or_Lookup_ਭਰਿਆ_Name(ਅਚਲ std::string& ਭਰਿਆ_Name);

        // Add or get multiple name parts. Saves name part IDs into a vector.
        ਖਾਲੀ ਅੰਦਰੂਨੀ_ਜੋੜੋ_Or_Lookup_Name_Parts(ਅਚਲ std::vector<std::string>& nameParts, std::vector<size_t>& namePartIds);

        // Add or get name part. Returns namePartId.
        size_t ਅੰਦਰੂਨੀ_ਜੋੜੋ_Or_Lookup_Name_Part(ਅਚਲ std::string& namePart);

        // For each name part ID, insert the tuples (full name, name part ID, name part position)
        // into the cross reference table
        ਖਾਲੀ ਅੰਦਰੂਨੀ_ਜੋੜੋ_Cross_Reference(size_t ਭਰਿਆ_Name_Id, ਅਚਲ std::vector<size_t>& namePartIds);

        // Gather pointer for full name info struct.
        // Note: The pointer is interior to the table vector. The pointers are invalidated
        // if the table is modified.
        ਭਰਿਆ_Name_Info* ਅੰਦਰੂਨੀ_get_ਭਰਿਆ_Name_Info(size_t ਭਰਿਆ_Name_Id);

        // Gather pointers for name part info struct.
        // Note: The pointers are interior to the table vector. The pointers are invalidated
        // if the table is modified.
        NamePartInfo* ਅੰਦਰੂਨੀ_get_Name_Part_Info(size_t namePartId);

        ਖਾਲੀ ਅੰਦਰੂਨੀ_ਲੱਭੋ_Matching_Name_Parts_For_ਭਰਿਆ_Name(ਭਰਿਆ_Name_Lookup_ਨਤੀਜਾ& ਭਰਿਆ_Name_ਨਤੀਜਾ);
        ਖਾਲੀ ਅੰਦਰੂਨੀ_ਲੱਭੋ_Matching_ਭਰਿਆ_ਨਾਂ_For_Name_Part(Name_Part_Lookup_ਨਤੀਜਾ& ਨਤੀਜਾ);
    };

ਜਨਤਕ:
    Log_ਟੈਗ_Manager(LogLevel defaultUnconfiguredGlobalLevel);
    ~Log_ਟੈਗ_Manager();

ਜਨਤਕ:
    // Parse and apply the config string.
    ਖਾਲੀ ਸੈੱਟ_Config_String(ਅਚਲ std::string& configString, ਤਾਰਕਿਕ apply = ਸੱਚ);

    // Gets the config parser. This is necessary to retrieve the list of malformed strings.
    Log_ਟੈਗ_Config_Parser& getConfigParser() ਅਚਲ;

    // Add (register) the log tag.
    // Note, passing in nullptr as value is equivalent to unassigning.
    ਖਾਲੀ assign(ਅਚਲ std::string& ਭਰਿਆ_Name, Log_ਟੈਗ* ptr);

    // Unassign the log tag. This is equivalent to calling assign with nullptr value.
    ਖਾਲੀ unassign(ਅਚਲ std::string& ਭਰਿਆ_Name);

    // Retrieve the log tag by exact name.
    Log_ਟੈਗ* get(ਅਚਲ std::string& ਭਰਿਆ_Name);

    // Changes the log level of the tag having the exact full name.
    ਖਾਲੀ ਸੈੱਟ_Level_By_ਭਰਿਆ_Name(ਅਚਲ std::string& ਭਰਿਆ_Name, LogLevel level);

    // Changes the log level of the tags matching the first part of the name.
    ਖਾਲੀ ਸੈੱਟ_Level_By_ਪਹਿਲਾ_Part(ਅਚਲ std::string& ਪਹਿਲਾ_Part, LogLevel level);

    // Changes the log level of the tags matching any part of the name.
    ਖਾਲੀ ਸੈੱਟ_Level_By_Any_Part(ਅਚਲ std::string& anyPart, LogLevel level);

    // Changes the log level of the tags with matching name part according
    // to the specified scope.
    ਖਾਲੀ ਸੈੱਟ_Level_By_Name_Part(ਅਚਲ std::string& namePart, LogLevel level, MatchingScope scope);

ਨਿਜੀ:
    ਤਾਰਕਿਕ ਅੰਦਰੂਨੀ_apply_ਭਰਿਆ_Name_Config_ਮੰਜ਼ਿਲ_ਟੈਗ(ਭਰਿਆ_Name_Info& ਭਰਿਆ_Name_Info_2);
    ਤਾਰਕਿਕ ਅੰਦਰੂਨੀ_apply_Name_Part_Config_ਮੰਜ਼ਿਲ_Specific_ਟੈਗ(ਭਰਿਆ_Name_Lookup_ਨਤੀਜਾ& ਭਰਿਆ_Name_ਨਤੀਜਾ);
    ਖਾਲੀ ਅੰਦਰੂਨੀ_apply_Name_Part_Config_ਮੰਜ਼ਿਲ_Matching_ਟੈਗ_ਸੂਚੀ(Name_Part_Lookup_ਨਤੀਜਾ& name_Part_ਨਤੀਜਾ);

ਨਿਜੀ:
    ਸਥਿਰ std::vector<std::string> ਵੰਡੋ_Name_Parts(ਅਚਲ std::string& ਭਰਿਆ_Name);
    ਸਥਿਰ ਤਾਰਕਿਕ ਅੰਦਰੂਨੀ_is_Name_Part_ਮੇਲ(MatchingScope scope, size_t matchingPos);

ਨਿਜੀ:
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ* m_globalName;

ਨਿਜੀ:
    ਬਦਲਣਯੋਗ MutexType m_mutex;
    std::unique_ptr<Log_ਟੈਗ> m_global_Log_ਟੈਗ;
    Name_ਸਾਰਣੀ m_name_ਸਾਰਣੀ;
    std::shared_ptr<Log_ਟੈਗ_Config_Parser> m_config;
};

}}} //namespace

#ਜੇ_ਅੰਤ //OPENCV_CORE_LOGTAGMANAGER_HPP
