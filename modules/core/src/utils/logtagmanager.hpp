// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_LOGTAGMANAGER_HPP
#define OPENCV_CORE_LOGTAGMANAGER_HPP

#जर 1 // if not already in precompiled headers
#अन्तर्भाव <string>
#अन्तर्भाव <algorithm>
#अन्तर्भाव <functional>
#अन्तर्भाव <unordered_map>
#अन्तर्भाव <mutex>
#अन्तर्भाव <memory>
#endif

#अन्तर्भाव <opencv2/core/utils/logtag.hpp>
#अन्तर्भाव "logtagconfig.hpp"

नामाकाश cv {
नामाकाश utils {
नामाकाश logging {

// forward declaration
वर्ग LogTagConfigParser;

// A lookup table of LogTags using full name, first part of name, and any part of name.
// The name parts of a LogTag is delimited by period.
//
// This class does not handle wildcard characters. The name-matching method can only be
// selected by calling the appropriate function.
//
वर्ग LogTagManager
{
खाजगी:
    // Current implementation does not seem to require recursive mutex;
    // also, extensible functions (accepting user-provided callback) are not allowed
    // to call LogTagManger (to prevent iterator invalidation), which needs enforced
    // with a non-recursive mutex.
    using MutexType = cv::Mutex;
    using LockType = cv::AutoLock;

    गणना वर्ग MatchingScope
    {
        None,
        Full,
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

    संरचना FullNameInfo
    {
        LogTag* logTagPtr;
        ParsedLevel parsedLevel;
    };

    संरचना NamePartInfo
    {
        ParsedLevel parsedLevel;
    };

    संरचना CrossReference
    {
        माप_प्रकार m_fullNameId;
        माप_प्रकार m_namePartId;
        माप_प्रकार m_matchingPos;
        FullNameInfo* m_fullNameInfo;
        NamePartInfo* m_namePartInfo;

        explicit CrossReference(माप_प्रकार fullNameId, माप_प्रकार namePartId, माप_प्रकार matchingPos,
            FullNameInfo* fullNameInfo, NamePartInfo* namePartInfo)
            : m_fullNameId(fullNameId)
            , m_namePartId(namePartId)
            , m_matchingPos(matchingPos)
            , m_fullNameInfo(fullNameInfo)
            , m_namePartInfo(namePartInfo)
        {
        }
    };

    संरचना FullNameLookupResult
    {
        // The full name being looked up
        std::string m_fullName;

        // The full name being broken down into name parts
        std::vector<std::string> m_nameParts;

        // The full name ID that is added or looked up from the table
        माप_प्रकार m_fullNameId;

        // The name part IDs that are added to or looked up from the table
        // listed in the same order as m_nameParts
        std::vector<माप_प्रकार> m_namePartIds;

        // The information struct for the full name
        FullNameInfo* m_fullNameInfoPtr;

        // Specifies whether cross references (full names that match the name part)
        // should be computed.
        तार्किक m_findCrossReferences;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        explicit FullNameLookupResult(स्थिरांक std::string& fullName)
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

    संरचना NamePartLookupResult
    {
        // The name part being looked up
        std::string m_namePart;

        // The name part ID that is added or looked up from the table
        माप_प्रकार m_namePartId;

        // Information struct ptr for the name part
        NamePartInfo* m_namePartInfoPtr;

        // Specifies whether cross references (full names that match the name part) should be computed.
        तार्किक m_findCrossReferences;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        explicit NamePartLookupResult(स्थिरांक std::string& namePart)
            : m_namePart(namePart)
            , m_namePartId()
            , m_namePartInfoPtr()
            , m_findCrossReferences()
            , m_crossReferences()
        {
        }
    };

    संरचना NameTable
    {
        // All data structures in this class are append-only. The item count
        // is being used as an incrementing integer key.

    सार्वजनिक:
        // Full name information struct.
        std::vector<FullNameInfo> m_fullNameInfos;

        // Name part information struct.
        std::vector<NamePartInfo> m_namePartInfos;

        // key: full name (string)
        // value: full name ID
        // .... (index into the vector of m_fullNameInfos)
        // .... (key into m_fullNameToNamePartIds)
        // .... (value.second in m_namePartToFullNameIds)
        std::unordered_map<std::string, माप_प्रकार> m_fullNameIds;

        // key: name part (string)
        // value: name part ID
        // .... (index into the vector of m_namePartInfos)
        // .... (key into m_namePartToFullNameIds)
        // .... (value.second in m_fullNameToNamePartIds)
        std::unordered_map<std::string, माप_प्रकार> m_namePartIds;

        // key: full name ID
        // value.first: name part ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<माप_प्रकार, std::pair<माप_प्रकार, माप_प्रकार>> m_fullNameToNamePartIds;

        // key: name part ID
        // value.first: full name ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<माप_प्रकार, std::pair<माप_प्रकार, माप_प्रकार>> m_namePartToFullNameIds;

    सार्वजनिक:
        शून्य addOrLookupFullName(FullNameLookupResult& result);
        शून्य addOrLookupNamePart(NamePartLookupResult& result);
        FullNameInfo* getFullNameInfo(स्थिरांक std::string& fullName);

    खाजगी:
        // Add or get full name. Does not compute name parts or access them in the table.
        // Returns the full name ID, and a bool indicating if the full name is new.
        std::pair<माप_प्रकार, तार्किक> internal_addOrLookupFullName(स्थिरांक std::string& fullName);

        // Add or get multiple name parts. Saves name part IDs into a vector.
        शून्य internal_addOrLookupNameParts(स्थिरांक std::vector<std::string>& nameParts, std::vector<माप_प्रकार>& namePartIds);

        // Add or get name part. Returns namePartId.
        माप_प्रकार internal_addOrLookupNamePart(स्थिरांक std::string& namePart);

        // For each name part ID, insert the tuples (full name, name part ID, name part position)
        // into the cross reference table
        शून्य internal_addCrossReference(माप_प्रकार fullNameId, स्थिरांक std::vector<माप_प्रकार>& namePartIds);

        // Gather pointer for full name info struct.
        // Note: The pointer is interior to the table vector. The pointers are invalidated
        // if the table is modified.
        FullNameInfo* internal_getFullNameInfo(माप_प्रकार fullNameId);

        // Gather pointers for name part info struct.
        // Note: The pointers are interior to the table vector. The pointers are invalidated
        // if the table is modified.
        NamePartInfo* internal_getNamePartInfo(माप_प्रकार namePartId);

        शून्य internal_findMatchingNamePartsForFullName(FullNameLookupResult& fullNameResult);
        शून्य internal_findMatchingFullNamesForNamePart(NamePartLookupResult& result);
    };

सार्वजनिक:
    LogTagManager(LogLevel defaultUnconfiguredGlobalLevel);
    ~LogTagManager();

सार्वजनिक:
    // Parse and apply the config string.
    शून्य setConfigString(स्थिरांक std::string& configString, तार्किक apply = सत्य);

    // Gets the config parser. This is necessary to retrieve the list of malformed strings.
    LogTagConfigParser& getConfigParser() स्थिरांक;

    // Add (register) the log tag.
    // Note, passing in nullptr as value is equivalent to unassigning.
    शून्य assign(स्थिरांक std::string& fullName, LogTag* ptr);

    // Unassign the log tag. This is equivalent to calling assign with nullptr value.
    शून्य unassign(स्थिरांक std::string& fullName);

    // Retrieve the log tag by exact name.
    LogTag* get(स्थिरांक std::string& fullName);

    // Changes the log level of the tag having the exact full name.
    शून्य setLevelByFullName(स्थिरांक std::string& fullName, LogLevel level);

    // Changes the log level of the tags matching the first part of the name.
    शून्य setLevelByFirstPart(स्थिरांक std::string& firstPart, LogLevel level);

    // Changes the log level of the tags matching any part of the name.
    शून्य setLevelByAnyPart(स्थिरांक std::string& anyPart, LogLevel level);

    // Changes the log level of the tags with matching name part according
    // to the specified scope.
    शून्य setLevelByNamePart(स्थिरांक std::string& namePart, LogLevel level, MatchingScope scope);

खाजगी:
    तार्किक internal_applyFullNameConfigToTag(FullNameInfo& fullNameInfo);
    तार्किक internal_applyNamePartConfigToSpecificTag(FullNameLookupResult& fullNameResult);
    शून्य internal_applyNamePartConfigToMatchingTags(NamePartLookupResult& namePartResult);

खाजगी:
    स्थिर std::vector<std::string> splitNameParts(स्थिरांक std::string& fullName);
    स्थिर तार्किक internal_isNamePartMatch(MatchingScope scope, माप_प्रकार matchingPos);

खाजगी:
    स्थिर स्थिरांक अक्षर* m_globalName;

खाजगी:
    mutable MutexType m_mutex;
    std::unique_ptr<LogTag> m_globalLogTag;
    NameTable m_nameTable;
    std::shared_ptr<LogTagConfigParser> m_config;
};

}}} //namespace

#endif //OPENCV_CORE_LOGTAGMANAGER_HPP
