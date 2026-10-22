// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGMANAGER_HPP
#define OPENCV_CORE_LOGTAGMANAGER_HPP

#ಯದಿ 1 // if not already in precompiled headers
#ಅನ್ತರ್ಭಾವ <string>
#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <functional>
#ಅನ್ತರ್ಭಾವ <unordered_map>
#ಅನ್ತರ್ಭಾವ <mutex>
#ಅನ್ತರ್ಭಾವ <memory>
#endif

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logtag.hpp>
#ಅನ್ತರ್ಭಾವ "logtagconfig.hpp"

ನಾಮಾಕಾಶ cv {
ನಾಮಾಕಾಶ utils {
ನಾಮಾಕಾಶ logging {

// forward declaration
ವರ್ಗ LogTagConfigParser;

// A lookup table of LogTags using full name, first part of name, and any part of name.
// The name parts of a LogTag is delimited by period.
//
// This class does not handle wildcard characters. The name-matching method can only be
// selected by calling the appropriate function.
//
ವರ್ಗ LogTagManager
{
ಗುಪ್ತ:
    // Current implementation does not seem to require recursive mutex;
    // also, extensible functions (accepting user-provided callback) are not allowed
    // to call LogTagManger (to prevent iterator invalidation), which needs enforced
    // with a non-recursive mutex.
    using MutexType = cv::Mutex;
    using LockType = cv::AutoLock;

    ಗಣನಾ ವರ್ಗ MatchingScope
    {
        None,
        Full,
        FirstNamePart,
        AnyNamePart
    };

    ಸಂರಚನಾ ParsedLevel
    {
        LogLevel level;
        MatchingScope scope;

        ParsedLevel()
            : level()
            , scope(MatchingScope::None)
        {
        }
    };

    ಸಂರಚನಾ FullNameInfo
    {
        LogTag* logTagPtr;
        ParsedLevel parsedLevel;
    };

    ಸಂರಚನಾ NamePartInfo
    {
        ParsedLevel parsedLevel;
    };

    ಸಂರಚನಾ CrossReference
    {
        size_t m_fullNameId;
        size_t m_namePartId;
        size_t m_matchingPos;
        FullNameInfo* m_fullNameInfo;
        NamePartInfo* m_namePartInfo;

        explicit CrossReference(size_t fullNameId, size_t namePartId, size_t matchingPos,
            FullNameInfo* fullNameInfo, NamePartInfo* namePartInfo)
            : m_fullNameId(fullNameId)
            , m_namePartId(namePartId)
            , m_matchingPos(matchingPos)
            , m_fullNameInfo(fullNameInfo)
            , m_namePartInfo(namePartInfo)
        {
        }
    };

    ಸಂರಚನಾ FullNameLookupResult
    {
        // The full name being looked up
        std::string m_fullName;

        // The full name being broken down into name parts
        std::vector<std::string> m_nameParts;

        // The full name ID that is added or looked up from the table
        size_t m_fullNameId;

        // The name part IDs that are added to or looked up from the table
        // listed in the same order as m_nameParts
        std::vector<size_t> m_namePartIds;

        // The information struct for the full name
        FullNameInfo* m_fullNameInfoPtr;

        // Specifies whether cross references (full names that match the name part)
        // should be computed.
        ತಾರ್ಕಿಕ m_findCrossReferences;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        explicit FullNameLookupResult(ಧ್ರುವ std::string& fullName)
            : m_fullName(fullName)
            , m_nameParts()
            , m_fullNameId()
            , m_namePartIds()
            , m_fullNameInfoPtr()
            , m_findCrossReferences()
            , m_crossReferences()
        {
        }
    };

    ಸಂರಚನಾ NamePartLookupResult
    {
        // The name part being looked up
        std::string m_namePart;

        // The name part ID that is added or looked up from the table
        size_t m_namePartId;

        // Information struct ptr for the name part
        NamePartInfo* m_namePartInfoPtr;

        // Specifies whether cross references (full names that match the name part) should be computed.
        ತಾರ್ಕಿಕ m_findCrossReferences;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        explicit NamePartLookupResult(ಧ್ರುವ std::string& namePart)
            : m_namePart(namePart)
            , m_namePartId()
            , m_namePartInfoPtr()
            , m_findCrossReferences()
            , m_crossReferences()
        {
        }
    };

    ಸಂರಚನಾ NameTable
    {
        // All data structures in this class are append-only. The item count
        // is being used as an incrementing integer key.

    ಸಾರ್ವಜನಿಕ:
        // Full name information struct.
        std::vector<FullNameInfo> m_fullNameInfos;

        // Name part information struct.
        std::vector<NamePartInfo> m_namePartInfos;

        // key: full name (string)
        // value: full name ID
        // .... (index into the vector of m_fullNameInfos)
        // .... (key into m_fullNameToNamePartIds)
        // .... (value.second in m_namePartToFullNameIds)
        std::unordered_map<std::string, size_t> m_fullNameIds;

        // key: name part (string)
        // value: name part ID
        // .... (index into the vector of m_namePartInfos)
        // .... (key into m_namePartToFullNameIds)
        // .... (value.second in m_fullNameToNamePartIds)
        std::unordered_map<std::string, size_t> m_namePartIds;

        // key: full name ID
        // value.first: name part ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_fullNameToNamePartIds;

        // key: name part ID
        // value.first: full name ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_namePartToFullNameIds;

    ಸಾರ್ವಜನಿಕ:
        ಶೂನ್ಯ addOrLookupFullName(FullNameLookupResult& result);
        ಶೂನ್ಯ addOrLookupNamePart(NamePartLookupResult& result);
        FullNameInfo* getFullNameInfo(ಧ್ರುವ std::string& fullName);

    ಗುಪ್ತ:
        // Add or get full name. Does not compute name parts or access them in the table.
        // Returns the full name ID, and a bool indicating if the full name is new.
        std::pair<size_t, ತಾರ್ಕಿಕ> internal_addOrLookupFullName(ಧ್ರುವ std::string& fullName);

        // Add or get multiple name parts. Saves name part IDs into a vector.
        ಶೂನ್ಯ internal_addOrLookupNameParts(ಧ್ರುವ std::vector<std::string>& nameParts, std::vector<size_t>& namePartIds);

        // Add or get name part. Returns namePartId.
        size_t internal_addOrLookupNamePart(ಧ್ರುವ std::string& namePart);

        // For each name part ID, insert the tuples (full name, name part ID, name part position)
        // into the cross reference table
        ಶೂನ್ಯ internal_addCrossReference(size_t fullNameId, ಧ್ರುವ std::vector<size_t>& namePartIds);

        // Gather pointer for full name info struct.
        // Note: The pointer is interior to the table vector. The pointers are invalidated
        // if the table is modified.
        FullNameInfo* internal_getFullNameInfo(size_t fullNameId);

        // Gather pointers for name part info struct.
        // Note: The pointers are interior to the table vector. The pointers are invalidated
        // if the table is modified.
        NamePartInfo* internal_getNamePartInfo(size_t namePartId);

        ಶೂನ್ಯ internal_findMatchingNamePartsForFullName(FullNameLookupResult& fullNameResult);
        ಶೂನ್ಯ internal_findMatchingFullNamesForNamePart(NamePartLookupResult& result);
    };

ಸಾರ್ವಜನಿಕ:
    LogTagManager(LogLevel defaultUnconfiguredGlobalLevel);
    ~LogTagManager();

ಸಾರ್ವಜನಿಕ:
    // Parse and apply the config string.
    ಶೂನ್ಯ setConfigString(ಧ್ರುವ std::string& configString, ತಾರ್ಕಿಕ apply = ಸತ್ಯ);

    // Gets the config parser. This is necessary to retrieve the list of malformed strings.
    LogTagConfigParser& getConfigParser() ಧ್ರುವ;

    // Add (register) the log tag.
    // Note, passing in nullptr as value is equivalent to unassigning.
    ಶೂನ್ಯ assign(ಧ್ರುವ std::string& fullName, LogTag* ptr);

    // Unassign the log tag. This is equivalent to calling assign with nullptr value.
    ಶೂನ್ಯ unassign(ಧ್ರುವ std::string& fullName);

    // Retrieve the log tag by exact name.
    LogTag* get(ಧ್ರುವ std::string& fullName);

    // Changes the log level of the tag having the exact full name.
    ಶೂನ್ಯ setLevelByFullName(ಧ್ರುವ std::string& fullName, LogLevel level);

    // Changes the log level of the tags matching the first part of the name.
    ಶೂನ್ಯ setLevelByFirstPart(ಧ್ರುವ std::string& firstPart, LogLevel level);

    // Changes the log level of the tags matching any part of the name.
    ಶೂನ್ಯ setLevelByAnyPart(ಧ್ರುವ std::string& anyPart, LogLevel level);

    // Changes the log level of the tags with matching name part according
    // to the specified scope.
    ಶೂನ್ಯ setLevelByNamePart(ಧ್ರುವ std::string& namePart, LogLevel level, MatchingScope scope);

ಗುಪ್ತ:
    ತಾರ್ಕಿಕ internal_applyFullNameConfigToTag(FullNameInfo& fullNameInfo);
    ತಾರ್ಕಿಕ internal_applyNamePartConfigToSpecificTag(FullNameLookupResult& fullNameResult);
    ಶೂನ್ಯ internal_applyNamePartConfigToMatchingTags(NamePartLookupResult& namePartResult);

ಗುಪ್ತ:
    ಸ್ಥಿರ std::vector<std::string> splitNameParts(ಧ್ರುವ std::string& fullName);
    ಸ್ಥಿರ ತಾರ್ಕಿಕ internal_isNamePartMatch(MatchingScope scope, size_t matchingPos);

ಗುಪ್ತ:
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* m_globalName;

ಗುಪ್ತ:
    mutable MutexType m_mutex;
    std::unique_ptr<LogTag> m_globalLogTag;
    NameTable m_nameTable;
    std::shared_ptr<LogTagConfigParser> m_config;
};

}}} //namespace

#endif //OPENCV_CORE_LOGTAGMANAGER_HPP
