
#ifndef DATA_SOURCE_INFO_H
#define DATA_SOURCE_INFO_H
 
#include <string>
#include <cstddef>
 
/**
 * @brief Metadata about a single Gegelati data source exposed by ArmLearnWrapper.
 *
 * Used by getDataSourcesInfo() to describe each array (name + size) so that
 * external tools (e.g. code-generation, header exporters) can interpret the
 * data layout without depending on the full wrapper.
 */
struct DataSourceInfo {
    std::string name;
    std::size_t size;
};
 
#endif // DATA_SOURCE_INFO_H