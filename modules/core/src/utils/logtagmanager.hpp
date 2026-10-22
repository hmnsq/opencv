// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#నిర్వచితం_కాకపోతే OPENCV_CORE_LOGTAGMANAGER_HPP
#నిర్వచించు OPENCV_CORE_LOGTAGMANAGER_HPP

#ఒకవేళ 1 // if not already in precompiled headers
#అన్తర్భావ <string>
#అన్తర్భావ <algorithm>
#అన్తర్భావ <functional>
#అన్తర్భావ <unordered_map>
#అన్తర్భావ <mutex>
#అన్తర్భావ <memory>
#అయితే_ముగింపు

#అన్తర్భావ <opencv2/core/utils/logtag.hpp>
#అన్తర్భావ "logtagconfig.hpp"

నామాకాశ cv {
నామాకాశ utils {
నామాకాశ logging {

// forward declaration
తరగతి Log_ట్యాగ్_Config_Parser;

// A lookup table of LogTags using full name, first part of name, and any part of name.
// The name parts of a LogTag is delimited by period.
//
// This class does not handle wildcard characters. The name-matching method can only be
// selected by calling the appropriate function.
//
తరగతి Log_ట్యాగ్_Manager
{
అంతరంగ:
    // Current implementation does not seem to require recursive mutex;
    // also, extensible functions (accepting user-provided callback) are not allowed
    // to call LogTagManger (to prevent iterator invalidation), which needs enforced
    // with a non-recursive mutex.
    ఉపయోగించి MutexType = cv::Mutex;
    ఉపయోగించి తాళం_Type = cv::Auto_తాళం;

    గణన తరగతి MatchingScope
    {
        None,
        నిండిన_2,
        మొదటి_Name_Part,
        AnyNamePart
    };

    నిర్మాణం ParsedLevel
    {
        LogLevel level;
        MatchingScope scope;

        ParsedLevel()
            : level()
            , scope(MatchingScope::None)
        {
        }
    };

    నిర్మాణం నిండిన_Name_Info
    {
        Log_ట్యాగ్* log_ట్యాగ్_Ptr;
        ParsedLevel parsedLevel;
    };

    నిర్మాణం NamePartInfo
    {
        ParsedLevel parsedLevel;
    };

    నిర్మాణం CrossReference
    {
        size_t m_నిండిన_Name_Id;
        size_t m_namePartId;
        size_t m_matchingPos;
        నిండిన_Name_Info* m_నిండిన_Name_Info;
        NamePartInfo* m_namePartInfo;

        స్పష్టమైన CrossReference(size_t నిండిన_Name_Id, size_t namePartId, size_t matchingPos,
            నిండిన_Name_Info* నిండిన_Name_Info_2, NamePartInfo* namePartInfo)
            : m_నిండిన_Name_Id(నిండిన_Name_Id)
            , m_namePartId(namePartId)
            , m_matchingPos(matchingPos)
            , m_నిండిన_Name_Info(నిండిన_Name_Info_2)
            , m_namePartInfo(namePartInfo)
        {
        }
    };

    నిర్మాణం నిండిన_Name_Lookup_ఫలితం
    {
        // The full name being looked up
        std::string m_నిండిన_Name;

        // The full name being broken down into name parts
        std::vector<std::string> m_nameParts;

        // The full name ID that is added or looked up from the table
        size_t m_నిండిన_Name_Id;

        // The name part IDs that are added to or looked up from the table
        // listed in the same order as m_nameParts
        std::vector<size_t> m_namePartIds;

        // The information struct for the full name
        నిండిన_Name_Info* m_నిండిన_Name_Info_Ptr;

        // Specifies whether cross references (full names that match the name part)
        // should be computed.
        తార్కిక m_కనుగొను_Cross_References;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        స్పష్టమైన నిండిన_Name_Lookup_ఫలితం(స్థిరరాశి std::string& నిండిన_Name)
            : m_నిండిన_Name(నిండిన_Name)
            , m_nameParts()
            , m_నిండిన_Name_Id()
            , m_namePartIds()
            , m_నిండిన_Name_Info_Ptr()
            , m_కనుగొను_Cross_References()
            , m_crossReferences()
        {
        }
    };

    నిర్మాణం Name_Part_Lookup_ఫలితం
    {
        // The name part being looked up
        std::string m_namePart;

        // The name part ID that is added or looked up from the table
        size_t m_namePartId;

        // Information struct ptr for the name part
        NamePartInfo* m_namePartInfoPtr;

        // Specifies whether cross references (full names that match the name part) should be computed.
        తార్కిక m_కనుగొను_Cross_References;

        // List of all full names that match the given name part.
        // This field is computed only if m_findCrossReferences is true.
        std::vector<CrossReference> m_crossReferences;

        స్పష్టమైన Name_Part_Lookup_ఫలితం(స్థిరరాశి std::string& namePart)
            : m_namePart(namePart)
            , m_namePartId()
            , m_namePartInfoPtr()
            , m_కనుగొను_Cross_References()
            , m_crossReferences()
        {
        }
    };

    నిర్మాణం Name_పట్టిక
    {
        // All data structures in this class are append-only. The item count
        // is being used as an incrementing integer key.

    బహిరంగ:
        // Full name information struct.
        std::vector<నిండిన_Name_Info> m_నిండిన_Name_Infos;

        // Name part information struct.
        std::vector<NamePartInfo> m_namePartInfos;

        // key: full name (string)
        // value: full name ID
        // .... (index into the vector of m_fullNameInfos)
        // .... (key into m_fullNameToNamePartIds)
        // .... (value.second in m_namePartToFullNameIds)
        std::unordered_map<std::string, size_t> m_నిండిన_Name_Ids;

        // key: name part (string)
        // value: name part ID
        // .... (index into the vector of m_namePartInfos)
        // .... (key into m_namePartToFullNameIds)
        // .... (value.second in m_fullNameToNamePartIds)
        std::unordered_map<std::string, size_t> m_namePartIds;

        // key: full name ID
        // value.first: name part ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_నిండిన_Name_గమ్యం_Name_Part_Ids;

        // key: name part ID
        // value.first: full name ID
        // value.second: occurrence position of name part in the full name
        std::unordered_multimap<size_t, std::pair<size_t, size_t>> m_name_Part_గమ్యం_నిండిన_Name_Ids;

    బహిరంగ:
        శూన్య కూడు_Or_Lookup_నిండిన_Name(నిండిన_Name_Lookup_ఫలితం& ఫలితం);
        శూన్య కూడు_Or_Lookup_Name_Part(Name_Part_Lookup_ఫలితం& ఫలితం);
        నిండిన_Name_Info* get_నిండిన_Name_Info(స్థిరరాశి std::string& నిండిన_Name);

    అంతరంగ:
        // Add or get full name. Does not compute name parts or access them in the table.
        // Returns the full name ID, and a bool indicating if the full name is new.
        std::pair<size_t, తార్కిక> అంతర్గత_కూడు_Or_Lookup_నిండిన_Name(స్థిరరాశి std::string& నిండిన_Name);

        // Add or get multiple name parts. Saves name part IDs into a vector.
        శూన్య అంతర్గత_కూడు_Or_Lookup_Name_Parts(స్థిరరాశి std::vector<std::string>& nameParts, std::vector<size_t>& namePartIds);

        // Add or get name part. Returns namePartId.
        size_t అంతర్గత_కూడు_Or_Lookup_Name_Part(స్థిరరాశి std::string& namePart);

        // For each name part ID, insert the tuples (full name, name part ID, name part position)
        // into the cross reference table
        శూన్య అంతర్గత_కూడు_Cross_Reference(size_t నిండిన_Name_Id, స్థిరరాశి std::vector<size_t>& namePartIds);

        // Gather pointer for full name info struct.
        // Note: The pointer is interior to the table vector. The pointers are invalidated
        // if the table is modified.
        నిండిన_Name_Info* అంతర్గత_get_నిండిన_Name_Info(size_t నిండిన_Name_Id);

        // Gather pointers for name part info struct.
        // Note: The pointers are interior to the table vector. The pointers are invalidated
        // if the table is modified.
        NamePartInfo* అంతర్గత_get_Name_Part_Info(size_t namePartId);

        శూన్య అంతర్గత_కనుగొను_Matching_Name_Parts_For_నిండిన_Name(నిండిన_Name_Lookup_ఫలితం& నిండిన_Name_ఫలితం);
        శూన్య అంతర్గత_కనుగొను_Matching_నిండిన_పేర్లు_For_Name_Part(Name_Part_Lookup_ఫలితం& ఫలితం);
    };

బహిరంగ:
    Log_ట్యాగ్_Manager(LogLevel defaultUnconfiguredGlobalLevel);
    ~Log_ట్యాగ్_Manager();

బహిరంగ:
    // Parse and apply the config string.
    శూన్య సమితి_Config_String(స్థిరరాశి std::string& configString, తార్కిక apply = నిజం);

    // Gets the config parser. This is necessary to retrieve the list of malformed strings.
    Log_ట్యాగ్_Config_Parser& getConfigParser() స్థిరరాశి;

    // Add (register) the log tag.
    // Note, passing in nullptr as value is equivalent to unassigning.
    శూన్య assign(స్థిరరాశి std::string& నిండిన_Name, Log_ట్యాగ్* ptr);

    // Unassign the log tag. This is equivalent to calling assign with nullptr value.
    శూన్య unassign(స్థిరరాశి std::string& నిండిన_Name);

    // Retrieve the log tag by exact name.
    Log_ట్యాగ్* get(స్థిరరాశి std::string& నిండిన_Name);

    // Changes the log level of the tag having the exact full name.
    శూన్య సమితి_Level_By_నిండిన_Name(స్థిరరాశి std::string& నిండిన_Name, LogLevel level);

    // Changes the log level of the tags matching the first part of the name.
    శూన్య సమితి_Level_By_మొదటి_Part(స్థిరరాశి std::string& మొదటి_Part, LogLevel level);

    // Changes the log level of the tags matching any part of the name.
    శూన్య సమితి_Level_By_Any_Part(స్థిరరాశి std::string& anyPart, LogLevel level);

    // Changes the log level of the tags with matching name part according
    // to the specified scope.
    శూన్య సమితి_Level_By_Name_Part(స్థిరరాశి std::string& namePart, LogLevel level, MatchingScope scope);

అంతరంగ:
    తార్కిక అంతర్గత_apply_నిండిన_Name_Config_గమ్యం_ట్యాగ్(నిండిన_Name_Info& నిండిన_Name_Info_2);
    తార్కిక అంతర్గత_apply_Name_Part_Config_గమ్యం_Specific_ట్యాగ్(నిండిన_Name_Lookup_ఫలితం& నిండిన_Name_ఫలితం);
    శూన్య అంతర్గత_apply_Name_Part_Config_గమ్యం_Matching_ట్యాగులు(Name_Part_Lookup_ఫలితం& name_Part_ఫలితం);

అంతరంగ:
    స్థిర std::vector<std::string> విభజించు_Name_Parts(స్థిరరాశి std::string& నిండిన_Name);
    స్థిర తార్కిక అంతర్గత_is_Name_Part_సరిపోలిక(MatchingScope scope, size_t matchingPos);

అంతరంగ:
    స్థిర స్థిరరాశి అక్షరం* m_globalName;

అంతరంగ:
    మార్చదగిన MutexType m_mutex;
    std::unique_ptr<Log_ట్యాగ్> m_global_Log_ట్యాగ్;
    Name_పట్టిక m_name_పట్టిక;
    std::shared_ptr<Log_ట్యాగ్_Config_Parser> m_config;
};

}}} //namespace

#అయితే_ముగింపు //OPENCV_CORE_LOGTAGMANAGER_HPP
