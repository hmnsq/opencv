// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGMANAGER_HPP
#define OPENCV_CORE_LOGTAGMANAGER_HPP

#যদি 1 // if not already in precompiled headers
#অন্তর্ভাব <string>
#অন্তর্ভাব <algorithm>
#অন্তর্ভাব <functional>
#অন্তর্ভাব <unordered_map>
#অন্তর্ভাব <mutex>
#অন্তর্ভাব <memory>
#endif

#অন্তর্ভাব <opencv2/core/utils/logtag.hpp>
#অন্তর্ভাব "logtagconfig.hpp"

নামাকাশ cv {
নামাকাশ utils {
নামাকাশ logging {

// forward declaration
বর্গ LogTagConfigParser;

// A lookup table of LogTags using full name, first part of name, and any part of name.
// The name parts of a LogTag is delimited by period.
//
// This class does not handle wildcard characters. The name-matching method can only be
// selected by calling the appropriate function.
//
বর্গ LogTagManager
{
গুপ্ত:
    // Current implementation does not seem to require recursive mutex;
    // also, extensible functions (accepting user-provided callback) are not allowed
    // to call LogTagManger (to prevent iterator invalidation), which needs enforced
    // with a non-recursive mutex.
    using MutexType = cv::Mutex;
    using LockType = cv::AutoLock;

    গণনা বর্গ MatchingScope
    {
        None,
        Full,
        FirstNamePart,
        AnyNamePart
    };

    সংরচনা ParsedLevel
    {
        LogLevel level;
        MatchingScope scope;

        ParsedLevel()
            : level()
            , scope(MatchingScope::None)
        {
        }
    };

    সংরচনা FullNameInfo
    {
        LogTag* logTagPtr;
        ParsedLevel parsedLevel;
    };

    সংরচনা NamePartInfo
    {
        ParsedLevel parsedLevel;
    };

    সংরচনা CrossReference
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

    সংরচনা FullNameLookupResult
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
        তার্কিক m_findCrossReferences;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        explicit FullNameLookupResult(ধ্রুব std::string& fullName)
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

    সংরচনা NamePartLookupResult
    {
        // The name part being looked up
        std::string m_namePart;

        // The name part ID that is added or looked up from the table
        size_t m_namePartId;

        // Information struct ptr for the name part
        NamePartInfo* m_namePartInfoPtr;

        // Specifies whether cross references (full names that match the name part) should be computed.
        তার্কিক m_findCrossReferences;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        explicit NamePartLookupResult(ধ্রুব std::string& namePart)
            : m_namePart(namePart)
            , m_namePartId()
            , m_namePartInfoPtr()
            , m_findCrossReferences()
            , m_crossReferences()
        {
        }
    };

    সংরচনা NameTable
    {
        // All data structures in this class are append-only. The item count
        // is being used as an incrementing integer key.

    সার্বজনিক:
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

    সার্বজনিক:
        শূন্য addOrLookupFullName(FullNameLookupResult& result);
        শূন্য addOrLookupNamePart(NamePartLookupResult& result);
        FullNameInfo* getFullNameInfo(ধ্রুব std::string& fullName);

    গুপ্ত:
        // Add or get full name. Does not compute name parts or access them in the table.
        // Returns the full name ID, and a bool indicating if the full name is new.
        std::pair<size_t, তার্কিক> internal_addOrLookupFullName(ধ্রুব std::string& fullName);

        // Add or get multiple name parts. Saves name part IDs into a vector.
        শূন্য internal_addOrLookupNameParts(ধ্রুব std::vector<std::string>& nameParts, std::vector<size_t>& namePartIds);

        // Add or get name part. Returns namePartId.
        size_t internal_addOrLookupNamePart(ধ্রুব std::string& namePart);

        // For each name part ID, insert the tuples (full name, name part ID, name part position)
        // into the cross reference table
        শূন্য internal_addCrossReference(size_t fullNameId, ধ্রুব std::vector<size_t>& namePartIds);

        // Gather pointer for full name info struct.
        // Note: The pointer is interior to the table vector. The pointers are invalidated
        // if the table is modified.
        FullNameInfo* internal_getFullNameInfo(size_t fullNameId);

        // Gather pointers for name part info struct.
        // Note: The pointers are interior to the table vector. The pointers are invalidated
        // if the table is modified.
        NamePartInfo* internal_getNamePartInfo(size_t namePartId);

        শূন্য internal_findMatchingNamePartsForFullName(FullNameLookupResult& fullNameResult);
        শূন্য internal_findMatchingFullNamesForNamePart(NamePartLookupResult& result);
    };

সার্বজনিক:
    LogTagManager(LogLevel defaultUnconfiguredGlobalLevel);
    ~LogTagManager();

সার্বজনিক:
    // Parse and apply the config string.
    শূন্য setConfigString(ধ্রুব std::string& configString, তার্কিক apply = সত্য);

    // Gets the config parser. This is necessary to retrieve the list of malformed strings.
    LogTagConfigParser& getConfigParser() ধ্রুব;

    // Add (register) the log tag.
    // Note, passing in nullptr as value is equivalent to unassigning.
    শূন্য assign(ধ্রুব std::string& fullName, LogTag* ptr);

    // Unassign the log tag. This is equivalent to calling assign with nullptr value.
    শূন্য unassign(ধ্রুব std::string& fullName);

    // Retrieve the log tag by exact name.
    LogTag* get(ধ্রুব std::string& fullName);

    // Changes the log level of the tag having the exact full name.
    শূন্য setLevelByFullName(ধ্রুব std::string& fullName, LogLevel level);

    // Changes the log level of the tags matching the first part of the name.
    শূন্য setLevelByFirstPart(ধ্রুব std::string& firstPart, LogLevel level);

    // Changes the log level of the tags matching any part of the name.
    শূন্য setLevelByAnyPart(ধ্রুব std::string& anyPart, LogLevel level);

    // Changes the log level of the tags with matching name part according
    // to the specified scope.
    শূন্য setLevelByNamePart(ধ্রুব std::string& namePart, LogLevel level, MatchingScope scope);

গুপ্ত:
    তার্কিক internal_applyFullNameConfigToTag(FullNameInfo& fullNameInfo);
    তার্কিক internal_applyNamePartConfigToSpecificTag(FullNameLookupResult& fullNameResult);
    শূন্য internal_applyNamePartConfigToMatchingTags(NamePartLookupResult& namePartResult);

গুপ্ত:
    স্থির std::vector<std::string> splitNameParts(ধ্রুব std::string& fullName);
    স্থির তার্কিক internal_isNamePartMatch(MatchingScope scope, size_t matchingPos);

গুপ্ত:
    স্থির ধ্রুব বর্ণ* m_globalName;

গুপ্ত:
    mutable MutexType m_mutex;
    std::unique_ptr<LogTag> m_globalLogTag;
    NameTable m_nameTable;
    std::shared_ptr<LogTagConfigParser> m_config;
};

}}} //namespace

#endif //OPENCV_CORE_LOGTAGMANAGER_HPP
