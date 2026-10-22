// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#यदि_अपरिभाषितम् OPENCV_CORE_LOGTAGMANAGER_HPP
#परिभाषय OPENCV_CORE_LOGTAGMANAGER_HPP

#यदि 1 // if not already in precompiled headers
#अन्तर्भाव <string>
#अन्तर्भाव <algorithm>
#अन्तर्भाव <functional>
#अन्तर्भाव <unordered_map>
#अन्तर्भाव <mutex>
#अन्तर्भाव <memory>
#यदि_अन्तः

#अन्तर्भाव <opencv2/core/utils/logtag.hpp>
#अन्तर्भाव "logtagconfig.hpp"

नामाकाश cv {
नामाकाश utils {
नामाकाश logging {

// forward declaration
वर्ग Log_चिह्नकम्_Config_Parser;

// A lookup table of LogTags using full name, first part of name, and any part of name.
// The name parts of a LogTag is delimited by period.
//
// This class does not handle wildcard characters. The name-matching method can only be
// selected by calling the appropriate function.
//
वर्ग Log_चिह्नकम्_Manager
{
गुप्त:
    // Current implementation does not seem to require recursive mutex;
    // also, extensible functions (accepting user-provided callback) are not allowed
    // to call LogTagManger (to prevent iterator invalidation), which needs enforced
    // with a non-recursive mutex.
    उपयुज्य MutexType = cv::Mutex;
    उपयुज्य तालकम्_Type = cv::Auto_तालकम्;

    गणना वर्ग MatchingScope
    {
        None,
        पूर्णम्_2,
        FirstNamePart,
        AnyNamePart
    };

    संरचना ParsedLevel
    {
        LogLevel level;
        MatchingScope scope;

        ParsedLevel()
            : level()
            , scope(MatchingScope::None)
        {
        }
    };

    संरचना पूर्णम्_Name_Info
    {
        Log_चिह्नकम्* log_चिह्नकम्_Ptr;
        ParsedLevel parsedLevel;
    };

    संरचना NamePartInfo
    {
        ParsedLevel parsedLevel;
    };

    संरचना CrossReference
    {
        size_t m_पूर्णम्_Name_Id;
        size_t m_namePartId;
        size_t m_matchingPos;
        पूर्णम्_Name_Info* m_पूर्णम्_Name_Info;
        NamePartInfo* m_namePartInfo;

        स्पष्ट CrossReference(size_t पूर्णम्_Name_Id, size_t namePartId, size_t matchingPos,
            पूर्णम्_Name_Info* पूर्णम्_Name_Info_2, NamePartInfo* namePartInfo)
            : m_पूर्णम्_Name_Id(पूर्णम्_Name_Id)
            , m_namePartId(namePartId)
            , m_matchingPos(matchingPos)
            , m_पूर्णम्_Name_Info(पूर्णम्_Name_Info_2)
            , m_namePartInfo(namePartInfo)
        {
        }
    };

    संरचना पूर्णम्_Name_Lookup_परिणाम
    {
        // The full name being looked up
        std::string m_पूर्णम्_Name;

        // The full name being broken down into name parts
        std::vector<std::string> m_nameParts;

        // The full name ID that is added or looked up from the table
        size_t m_पूर्णम्_Name_Id;

        // The name part IDs that are added to or looked up from the table
        // listed in the same order as m_nameParts
        std::vector<size_t> m_namePartIds;

        // The information struct for the full name
        पूर्णम्_Name_Info* m_पूर्णम्_Name_Info_Ptr;

        // Specifies whether cross references (full names that match the name part)
        // should be computed.
        तार्किक m_findCrossReferences;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        स्पष्ट पूर्णम्_Name_Lookup_परिणाम(ध्रुव std::string& पूर्णम्_Name)
            : m_पूर्णम्_Name(पूर्णम्_Name)
            , m_nameParts()
            , m_पूर्णम्_Name_Id()
            , m_namePartIds()
            , m_पूर्णम्_Name_Info_Ptr()
            , m_findCrossReferences()
            , m_crossReferences()
        {
        }
    };

    संरचना Name_Part_Lookup_परिणाम
    {
        // The name part being looked up
        std::string m_namePart;

        // The name part ID that is added or looked up from the table
        size_t m_namePartId;

        // Information struct ptr for the name part
        NamePartInfo* m_namePartInfoPtr;

        // Specifies whether cross references (full names that match the name part) should be computed.
        तार्किक m_findCrossReferences;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        स्पष्ट Name_Part_Lookup_परिणाम(ध्रुव std::string& namePart)
            : m_namePart(namePart)
            , m_namePartId()
            , m_namePartInfoPtr()
            , m_findCrossReferences()
            , m_crossReferences()
        {
        }
    };

    संरचना Name_सारणी
    {
        // All data structures in this class are append-only. The item count
        // is being used as an incrementing integer key.

    सार्वजनिक:
        // Full name information struct.
        std::vector<पूर्णम्_Name_Info> m_पूर्णम्_Name_Infos;

        // Name part information struct.
        std::vector<NamePartInfo> m_namePartInfos;

        // key: full name (string)
        // value: full name ID
        // .... (index into the vector of m_fullNameInfos)
        // .... (key into m_fullNameToNamePartIds)
        // .... (value.second in m_namePartToFullNameIds)
        std::unordered_map<std::string, size_t> m_पूर्णम्_Name_Ids;

        // key: name part (string)
        // value: name part ID
        // .... (index into the vector of m_namePartInfos)
        // .... (key into m_namePartToFullNameIds)
        // .... (value.second in m_fullNameToNamePartIds)
        std::unordered_map<std::string, size_t> m_namePartIds;

        // key: full name ID
        // value.first: name part ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_पूर्णम्_Name_गन्तव्यम्_Name_Part_Ids;

        // key: name part ID
        // value.first: full name ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_name_Part_गन्तव्यम्_पूर्णम्_Name_Ids;

    सार्वजनिक:
        शून्य सङ्कलय_Or_Lookup_पूर्णम्_Name(पूर्णम्_Name_Lookup_परिणाम& परिणाम);
        शून्य सङ्कलय_Or_Lookup_Name_Part(Name_Part_Lookup_परिणाम& परिणाम);
        पूर्णम्_Name_Info* get_पूर्णम्_Name_Info(ध्रुव std::string& पूर्णम्_Name);

    गुप्त:
        // Add or get full name. Does not compute name parts or access them in the table.
        // Returns the full name ID, and a bool indicating if the full name is new.
        std::pair<size_t, तार्किक> आन्तरिकम्_सङ्कलय_Or_Lookup_पूर्णम्_Name(ध्रुव std::string& पूर्णम्_Name);

        // Add or get multiple name parts. Saves name part IDs into a vector.
        शून्य आन्तरिकम्_सङ्कलय_Or_Lookup_Name_Parts(ध्रुव std::vector<std::string>& nameParts, std::vector<size_t>& namePartIds);

        // Add or get name part. Returns namePartId.
        size_t आन्तरिकम्_सङ्कलय_Or_Lookup_Name_Part(ध्रुव std::string& namePart);

        // For each name part ID, insert the tuples (full name, name part ID, name part position)
        // into the cross reference table
        शून्य आन्तरिकम्_सङ्कलय_Cross_Reference(size_t पूर्णम्_Name_Id, ध्रुव std::vector<size_t>& namePartIds);

        // Gather pointer for full name info struct.
        // Note: The pointer is interior to the table vector. The pointers are invalidated
        // if the table is modified.
        पूर्णम्_Name_Info* आन्तरिकम्_get_पूर्णम्_Name_Info(size_t पूर्णम्_Name_Id);

        // Gather pointers for name part info struct.
        // Note: The pointers are interior to the table vector. The pointers are invalidated
        // if the table is modified.
        NamePartInfo* आन्तरिकम्_get_Name_Part_Info(size_t namePartId);

        शून्य आन्तरिकम्_find_Matching_Name_Parts_For_पूर्णम्_Name(पूर्णम्_Name_Lookup_परिणाम& पूर्णम्_Name_परिणाम);
        शून्य आन्तरिकम्_find_Matching_पूर्णम्_नामानि_For_Name_Part(Name_Part_Lookup_परिणाम& परिणाम);
    };

सार्वजनिक:
    Log_चिह्नकम्_Manager(LogLevel defaultUnconfiguredGlobalLevel);
    ~Log_चिह्नकम्_Manager();

सार्वजनिक:
    // Parse and apply the config string.
    शून्य समुच्चयः_Config_String(ध्रुव std::string& configString, तार्किक apply = सत्य);

    // Gets the config parser. This is necessary to retrieve the list of malformed strings.
    Log_चिह्नकम्_Config_Parser& getConfigParser() ध्रुव;

    // Add (register) the log tag.
    // Note, passing in nullptr as value is equivalent to unassigning.
    शून्य assign(ध्रुव std::string& पूर्णम्_Name, Log_चिह्नकम्* ptr);

    // Unassign the log tag. This is equivalent to calling assign with nullptr value.
    शून्य unassign(ध्रुव std::string& पूर्णम्_Name);

    // Retrieve the log tag by exact name.
    Log_चिह्नकम्* get(ध्रुव std::string& पूर्णम्_Name);

    // Changes the log level of the tag having the exact full name.
    शून्य समुच्चयः_Level_By_पूर्णम्_Name(ध्रुव std::string& पूर्णम्_Name, LogLevel level);

    // Changes the log level of the tags matching the first part of the name.
    शून्य समुच्चयः_Level_By_First_Part(ध्रुव std::string& firstPart, LogLevel level);

    // Changes the log level of the tags matching any part of the name.
    शून्य समुच्चयः_Level_By_Any_Part(ध्रुव std::string& anyPart, LogLevel level);

    // Changes the log level of the tags with matching name part according
    // to the specified scope.
    शून्य समुच्चयः_Level_By_Name_Part(ध्रुव std::string& namePart, LogLevel level, MatchingScope scope);

गुप्त:
    तार्किक आन्तरिकम्_apply_पूर्णम्_Name_Config_गन्तव्यम्_चिह्नकम्(पूर्णम्_Name_Info& पूर्णम्_Name_Info_2);
    तार्किक आन्तरिकम्_apply_Name_Part_Config_गन्तव्यम्_Specific_चिह्नकम्(पूर्णम्_Name_Lookup_परिणाम& पूर्णम्_Name_परिणाम);
    शून्य आन्तरिकम्_apply_Name_Part_Config_गन्तव्यम्_Matching_चिह्नकानि(Name_Part_Lookup_परिणाम& name_Part_परिणाम);

गुप्त:
    स्थिर std::vector<std::string> विभज_Name_Parts(ध्रुव std::string& पूर्णम्_Name);
    स्थिर तार्किक आन्तरिकम्_is_Name_Part_मेलनम्(MatchingScope scope, size_t matchingPos);

गुप्त:
    स्थिर ध्रुव वर्ण* m_globalName;

गुप्त:
    परिवर्तनीय MutexType m_mutex;
    std::unique_ptr<Log_चिह्नकम्> m_global_Log_चिह्नकम्;
    Name_सारणी m_name_सारणी;
    std::shared_ptr<Log_चिह्नकम्_Config_Parser> m_config;
};

}}} //namespace

#यदि_अन्तः //OPENCV_CORE_LOGTAGMANAGER_HPP
