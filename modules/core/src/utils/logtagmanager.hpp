// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_CORE_LOGTAGMANAGER_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_LOGTAGMANAGER_HPP

#ಆದರೆ 1 // if not already in precompiled headers
#ಅನ್ತರ್ಭಾವ <string>
#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <functional>
#ಅನ್ತರ್ಭಾವ <unordered_map>
#ಅನ್ತರ್ಭಾವ <mutex>
#ಅನ್ತರ್ಭಾವ <memory>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logtag.hpp>
#ಅನ್ತರ್ಭಾವ "logtagconfig.hpp"

ನಾಮಾಕಾಶ cv {
ನಾಮಾಕಾಶ utils {
ನಾಮಾಕಾಶ logging {

// forward declaration
ವರ್ಗ Log_ಟ್ಯಾಗ್_Config_Parser;

// A lookup table of LogTags using full name, first part of name, and any part of name.
// The name parts of a LogTag is delimited by period.
//
// This class does not handle wildcard characters. The name-matching method can only be
// selected by calling the appropriate function.
//
ವರ್ಗ Log_ಟ್ಯಾಗ್_Manager
{
ಖಾಸಗಿ:
    // Current implementation does not seem to require recursive mutex;
    // also, extensible functions (accepting user-provided callback) are not allowed
    // to call LogTagManger (to prevent iterator invalidation), which needs enforced
    // with a non-recursive mutex.
    ಬಳಸಿ MutexType = cv::Mutex;
    ಬಳಸಿ ಬೀಗ_Type = cv::Auto_ಬೀಗ;

    ಎಣಿಕೆ ವರ್ಗ MatchingScope
    {
        None,
        ತುಂಬಿದ_2,
        ಮೊದಲ_Name_Part,
        AnyNamePart
    };

    ರಚನೆ ParsedLevel
    {
        LogLevel level;
        MatchingScope scope;

        ParsedLevel()
            : level()
            , scope(MatchingScope::None)
        {
        }
    };

    ರಚನೆ ತುಂಬಿದ_Name_Info
    {
        Log_ಟ್ಯಾಗ್* log_ಟ್ಯಾಗ್_Ptr;
        ParsedLevel parsedLevel;
    };

    ರಚನೆ NamePartInfo
    {
        ParsedLevel parsedLevel;
    };

    ರಚನೆ CrossReference
    {
        size_t m_ತುಂಬಿದ_Name_Id;
        size_t m_namePartId;
        size_t m_matchingPos;
        ತುಂಬಿದ_Name_Info* m_ತುಂಬಿದ_Name_Info;
        NamePartInfo* m_namePartInfo;

        ಸ್ಪಷ್ಟ CrossReference(size_t ತುಂಬಿದ_Name_Id, size_t namePartId, size_t matchingPos,
            ತುಂಬಿದ_Name_Info* ತುಂಬಿದ_Name_Info_2, NamePartInfo* namePartInfo)
            : m_ತುಂಬಿದ_Name_Id(ತುಂಬಿದ_Name_Id)
            , m_namePartId(namePartId)
            , m_matchingPos(matchingPos)
            , m_ತುಂಬಿದ_Name_Info(ತುಂಬಿದ_Name_Info_2)
            , m_namePartInfo(namePartInfo)
        {
        }
    };

    ರಚನೆ ತುಂಬಿದ_Name_Lookup_ಫಲಿತಾಂಶ
    {
        // The full name being looked up
        std::string m_ತುಂಬಿದ_Name;

        // The full name being broken down into name parts
        std::vector<std::string> m_nameParts;

        // The full name ID that is added or looked up from the table
        size_t m_ತುಂಬಿದ_Name_Id;

        // The name part IDs that are added to or looked up from the table
        // listed in the same order as m_nameParts
        std::vector<size_t> m_namePartIds;

        // The information struct for the full name
        ತುಂಬಿದ_Name_Info* m_ತುಂಬಿದ_Name_Info_Ptr;

        // Specifies whether cross references (full names that match the name part)
        // should be computed.
        ತಾರ್ಕಿಕ m_ಕಂಡುಹಿಡಿ_Cross_References;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        ಸ್ಪಷ್ಟ ತುಂಬಿದ_Name_Lookup_ಫಲಿತಾಂಶ(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name)
            : m_ತುಂಬಿದ_Name(ತುಂಬಿದ_Name)
            , m_nameParts()
            , m_ತುಂಬಿದ_Name_Id()
            , m_namePartIds()
            , m_ತುಂಬಿದ_Name_Info_Ptr()
            , m_ಕಂಡುಹಿಡಿ_Cross_References()
            , m_crossReferences()
        {
        }
    };

    ರಚನೆ Name_Part_Lookup_ಫಲಿತಾಂಶ
    {
        // The name part being looked up
        std::string m_namePart;

        // The name part ID that is added or looked up from the table
        size_t m_namePartId;

        // Information struct ptr for the name part
        NamePartInfo* m_namePartInfoPtr;

        // Specifies whether cross references (full names that match the name part) should be computed.
        ತಾರ್ಕಿಕ m_ಕಂಡುಹಿಡಿ_Cross_References;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        ಸ್ಪಷ್ಟ Name_Part_Lookup_ಫಲಿತಾಂಶ(ಸ್ಥಿರಾಂಕ std::string& namePart)
            : m_namePart(namePart)
            , m_namePartId()
            , m_namePartInfoPtr()
            , m_ಕಂಡುಹಿಡಿ_Cross_References()
            , m_crossReferences()
        {
        }
    };

    ರಚನೆ Name_ಕೋಷ್ಟಕ
    {
        // All data structures in this class are append-only. The item count
        // is being used as an incrementing integer key.

    ಸಾರ್ವಜನಿಕ:
        // Full name information struct.
        std::vector<ತುಂಬಿದ_Name_Info> m_ತುಂಬಿದ_Name_Infos;

        // Name part information struct.
        std::vector<NamePartInfo> m_namePartInfos;

        // key: full name (string)
        // value: full name ID
        // .... (index into the vector of m_fullNameInfos)
        // .... (key into m_fullNameToNamePartIds)
        // .... (value.second in m_namePartToFullNameIds)
        std::unordered_map<std::string, size_t> m_ತುಂಬಿದ_Name_Ids;

        // key: name part (string)
        // value: name part ID
        // .... (index into the vector of m_namePartInfos)
        // .... (key into m_namePartToFullNameIds)
        // .... (value.second in m_fullNameToNamePartIds)
        std::unordered_map<std::string, size_t> m_namePartIds;

        // key: full name ID
        // value.first: name part ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_ತುಂಬಿದ_Name_ಗಮ್ಯ_Name_Part_Ids;

        // key: name part ID
        // value.first: full name ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_name_Part_ಗಮ್ಯ_ತುಂಬಿದ_Name_Ids;

    ಸಾರ್ವಜನಿಕ:
        ಶೂನ್ಯ ಕೂಡಿಸು_Or_Lookup_ತುಂಬಿದ_Name(ತುಂಬಿದ_Name_Lookup_ಫಲಿತಾಂಶ& ಫಲಿತಾಂಶ);
        ಶೂನ್ಯ ಕೂಡಿಸು_Or_Lookup_Name_Part(Name_Part_Lookup_ಫಲಿತಾಂಶ& ಫಲಿತಾಂಶ);
        ತುಂಬಿದ_Name_Info* get_ತುಂಬಿದ_Name_Info(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name);

    ಖಾಸಗಿ:
        // Add or get full name. Does not compute name parts or access them in the table.
        // Returns the full name ID, and a bool indicating if the full name is new.
        std::pair<size_t, ತಾರ್ಕಿಕ> ಆಂತರಿಕ_ಕೂಡಿಸು_Or_Lookup_ತುಂಬಿದ_Name(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name);

        // Add or get multiple name parts. Saves name part IDs into a vector.
        ಶೂನ್ಯ ಆಂತರಿಕ_ಕೂಡಿಸು_Or_Lookup_Name_Parts(ಸ್ಥಿರಾಂಕ std::vector<std::string>& nameParts, std::vector<size_t>& namePartIds);

        // Add or get name part. Returns namePartId.
        size_t ಆಂತರಿಕ_ಕೂಡಿಸು_Or_Lookup_Name_Part(ಸ್ಥಿರಾಂಕ std::string& namePart);

        // For each name part ID, insert the tuples (full name, name part ID, name part position)
        // into the cross reference table
        ಶೂನ್ಯ ಆಂತರಿಕ_ಕೂಡಿಸು_Cross_Reference(size_t ತುಂಬಿದ_Name_Id, ಸ್ಥಿರಾಂಕ std::vector<size_t>& namePartIds);

        // Gather pointer for full name info struct.
        // Note: The pointer is interior to the table vector. The pointers are invalidated
        // if the table is modified.
        ತುಂಬಿದ_Name_Info* ಆಂತರಿಕ_get_ತುಂಬಿದ_Name_Info(size_t ತುಂಬಿದ_Name_Id);

        // Gather pointers for name part info struct.
        // Note: The pointers are interior to the table vector. The pointers are invalidated
        // if the table is modified.
        NamePartInfo* ಆಂತರಿಕ_get_Name_Part_Info(size_t namePartId);

        ಶೂನ್ಯ ಆಂತರಿಕ_ಕಂಡುಹಿಡಿ_Matching_Name_Parts_For_ತುಂಬಿದ_Name(ತುಂಬಿದ_Name_Lookup_ಫಲಿತಾಂಶ& ತುಂಬಿದ_Name_ಫಲಿತಾಂಶ);
        ಶೂನ್ಯ ಆಂತರಿಕ_ಕಂಡುಹಿಡಿ_Matching_ತುಂಬಿದ_ಹೆಸರುಗಳು_For_Name_Part(Name_Part_Lookup_ಫಲಿತಾಂಶ& ಫಲಿತಾಂಶ);
    };

ಸಾರ್ವಜನಿಕ:
    Log_ಟ್ಯಾಗ್_Manager(LogLevel defaultUnconfiguredGlobalLevel);
    ~Log_ಟ್ಯಾಗ್_Manager();

ಸಾರ್ವಜನಿಕ:
    // Parse and apply the config string.
    ಶೂನ್ಯ ಗಣ_Config_String(ಸ್ಥಿರಾಂಕ std::string& configString, ತಾರ್ಕಿಕ apply = ನಿಜ);

    // Gets the config parser. This is necessary to retrieve the list of malformed strings.
    Log_ಟ್ಯಾಗ್_Config_Parser& getConfigParser() ಸ್ಥಿರಾಂಕ;

    // Add (register) the log tag.
    // Note, passing in nullptr as value is equivalent to unassigning.
    ಶೂನ್ಯ assign(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name, Log_ಟ್ಯಾಗ್* ptr);

    // Unassign the log tag. This is equivalent to calling assign with nullptr value.
    ಶೂನ್ಯ unassign(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name);

    // Retrieve the log tag by exact name.
    Log_ಟ್ಯಾಗ್* get(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name);

    // Changes the log level of the tag having the exact full name.
    ಶೂನ್ಯ ಗಣ_Level_By_ತುಂಬಿದ_Name(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name, LogLevel level);

    // Changes the log level of the tags matching the first part of the name.
    ಶೂನ್ಯ ಗಣ_Level_By_ಮೊದಲ_Part(ಸ್ಥಿರಾಂಕ std::string& ಮೊದಲ_Part, LogLevel level);

    // Changes the log level of the tags matching any part of the name.
    ಶೂನ್ಯ ಗಣ_Level_By_Any_Part(ಸ್ಥಿರಾಂಕ std::string& anyPart, LogLevel level);

    // Changes the log level of the tags with matching name part according
    // to the specified scope.
    ಶೂನ್ಯ ಗಣ_Level_By_Name_Part(ಸ್ಥಿರಾಂಕ std::string& namePart, LogLevel level, MatchingScope scope);

ಖಾಸಗಿ:
    ತಾರ್ಕಿಕ ಆಂತರಿಕ_apply_ತುಂಬಿದ_Name_Config_ಗಮ್ಯ_ಟ್ಯಾಗ್(ತುಂಬಿದ_Name_Info& ತುಂಬಿದ_Name_Info_2);
    ತಾರ್ಕಿಕ ಆಂತರಿಕ_apply_Name_Part_Config_ಗಮ್ಯ_Specific_ಟ್ಯಾಗ್(ತುಂಬಿದ_Name_Lookup_ಫಲಿತಾಂಶ& ತುಂಬಿದ_Name_ಫಲಿತಾಂಶ);
    ಶೂನ್ಯ ಆಂತರಿಕ_apply_Name_Part_Config_ಗಮ್ಯ_Matching_ಟ್ಯಾಗುಗಳು(Name_Part_Lookup_ಫಲಿತಾಂಶ& name_Part_ಫಲಿತಾಂಶ);

ಖಾಸಗಿ:
    ಸ್ಥಿರ std::vector<std::string> ವಿಭಜಿಸು_Name_Parts(ಸ್ಥಿರಾಂಕ std::string& ತುಂಬಿದ_Name);
    ಸ್ಥಿರ ತಾರ್ಕಿಕ ಆಂತರಿಕ_is_Name_Part_ಹೊಂದಿಕೆ(MatchingScope scope, size_t matchingPos);

ಖಾಸಗಿ:
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* m_globalName;

ಖಾಸಗಿ:
    ಬದಲಾಯಿಸಬಹುದಾದ MutexType m_mutex;
    std::unique_ptr<Log_ಟ್ಯಾಗ್> m_global_Log_ಟ್ಯಾಗ್;
    Name_ಕೋಷ್ಟಕ m_name_ಕೋಷ್ಟಕ;
    std::shared_ptr<Log_ಟ್ಯಾಗ್_Config_Parser> m_config;
};

}}} //namespace

#ಆದರೆ_ಅಂತ್ಯ //OPENCV_CORE_LOGTAGMANAGER_HPP
