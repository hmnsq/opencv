// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGMANAGER_HPP
#define OPENCV_CORE_LOGTAGMANAGER_HPP

#ܐܢ 1 // if not already in precompiled headers
#ضمن <string>
#ضمن <algorithm>
#ضمن <functional>
#ضمن <unordered_map>
#ضمن <mutex>
#ضمن <memory>
#endif

#ضمن <opencv2/core/utils/logtag.hpp>
#ضمن "logtagconfig.hpp"

نطاق cv {
نطاق utils {
نطاق logging {

// forward declaration
صنف LogTagConfigParser;

// A lookup table of LogTags using full name, first part of name, and any part of name.
// The name parts of a LogTag is delimited by period.
//
// This class does not handle wildcard characters. The name-matching method can only be
// selected by calling the appropriate function.
//
صنف LogTagManager
{
خاص:
    // Current implementation does not seem to require recursive mutex;
    // also, extensible functions (accepting user-provided callback) are not allowed
    // to call LogTagManger (to prevent iterator invalidation), which needs enforced
    // with a non-recursive mutex.
    using MutexType = cv::Mutex;
    using LockType = cv::AutoLock;

    تعداد صنف MatchingScope
    {
        None,
        Full,
        FirstNamePart,
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

    بنية FullNameInfo
    {
        LogTag* logTagPtr;
        ParsedLevel parsedLevel;
    };

    بنية NamePartInfo
    {
        ParsedLevel parsedLevel;
    };

    بنية CrossReference
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

    بنية FullNameLookupResult
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
        منطقي m_findCrossReferences;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        explicit FullNameLookupResult(ثابت std::string& fullName)
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

    بنية NamePartLookupResult
    {
        // The name part being looked up
        std::string m_namePart;

        // The name part ID that is added or looked up from the table
        size_t m_namePartId;

        // Information struct ptr for the name part
        NamePartInfo* m_namePartInfoPtr;

        // Specifies whether cross references (full names that match the name part) should be computed.
        منطقي m_findCrossReferences;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        explicit NamePartLookupResult(ثابت std::string& namePart)
            : m_namePart(namePart)
            , m_namePartId()
            , m_namePartInfoPtr()
            , m_findCrossReferences()
            , m_crossReferences()
        {
        }
    };

    بنية NameTable
    {
        // All data structures in this class are append-only. The item count
        // is being used as an incrementing integer key.

    عام:
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

    عام:
        فراغ addOrLookupFullName(FullNameLookupResult& result);
        فراغ addOrLookupNamePart(NamePartLookupResult& result);
        FullNameInfo* getFullNameInfo(ثابت std::string& fullName);

    خاص:
        // Add or get full name. Does not compute name parts or access them in the table.
        // Returns the full name ID, and a bool indicating if the full name is new.
        std::pair<size_t, منطقي> internal_addOrLookupFullName(ثابت std::string& fullName);

        // Add or get multiple name parts. Saves name part IDs into a vector.
        فراغ internal_addOrLookupNameParts(ثابت std::vector<std::string>& nameParts, std::vector<size_t>& namePartIds);

        // Add or get name part. Returns namePartId.
        size_t internal_addOrLookupNamePart(ثابت std::string& namePart);

        // For each name part ID, insert the tuples (full name, name part ID, name part position)
        // into the cross reference table
        فراغ internal_addCrossReference(size_t fullNameId, ثابت std::vector<size_t>& namePartIds);

        // Gather pointer for full name info struct.
        // Note: The pointer is interior to the table vector. The pointers are invalidated
        // if the table is modified.
        FullNameInfo* internal_getFullNameInfo(size_t fullNameId);

        // Gather pointers for name part info struct.
        // Note: The pointers are interior to the table vector. The pointers are invalidated
        // if the table is modified.
        NamePartInfo* internal_getNamePartInfo(size_t namePartId);

        فراغ internal_findMatchingNamePartsForFullName(FullNameLookupResult& fullNameResult);
        فراغ internal_findMatchingFullNamesForNamePart(NamePartLookupResult& result);
    };

عام:
    LogTagManager(LogLevel defaultUnconfiguredGlobalLevel);
    ~LogTagManager();

عام:
    // Parse and apply the config string.
    فراغ setConfigString(ثابت std::string& configString, منطقي apply = ܫܪܪܐ);

    // Gets the config parser. This is necessary to retrieve the list of malformed strings.
    LogTagConfigParser& getConfigParser() ثابت;

    // Add (register) the log tag.
    // Note, passing in nullptr as value is equivalent to unassigning.
    فراغ assign(ثابت std::string& fullName, LogTag* ptr);

    // Unassign the log tag. This is equivalent to calling assign with nullptr value.
    فراغ unassign(ثابت std::string& fullName);

    // Retrieve the log tag by exact name.
    LogTag* get(ثابت std::string& fullName);

    // Changes the log level of the tag having the exact full name.
    فراغ setLevelByFullName(ثابت std::string& fullName, LogLevel level);

    // Changes the log level of the tags matching the first part of the name.
    فراغ setLevelByFirstPart(ثابت std::string& firstPart, LogLevel level);

    // Changes the log level of the tags matching any part of the name.
    فراغ setLevelByAnyPart(ثابت std::string& anyPart, LogLevel level);

    // Changes the log level of the tags with matching name part according
    // to the specified scope.
    فراغ setLevelByNamePart(ثابت std::string& namePart, LogLevel level, MatchingScope scope);

خاص:
    منطقي internal_applyFullNameConfigToTag(FullNameInfo& fullNameInfo);
    منطقي internal_applyNamePartConfigToSpecificTag(FullNameLookupResult& fullNameResult);
    فراغ internal_applyNamePartConfigToMatchingTags(NamePartLookupResult& namePartResult);

خاص:
    ساكن std::vector<std::string> splitNameParts(ثابت std::string& fullName);
    ساكن منطقي internal_isNamePartMatch(MatchingScope scope, size_t matchingPos);

خاص:
    ساكن ثابت حرف* m_globalName;

خاص:
    mutable MutexType m_mutex;
    std::unique_ptr<LogTag> m_globalLogTag;
    NameTable m_nameTable;
    std::shared_ptr<LogTagConfigParser> m_config;
};

}}} //namespace

#endif //OPENCV_CORE_LOGTAGMANAGER_HPP
