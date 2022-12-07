//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/assert.hpp
//
//
//===----------------------------------------------------------------------===//

#include "duckdb/common/winapi.hpp"

#pragma once

#if (defined(DUCKDB_USE_STANDARD_ASSERT) || !defined(DEBUG)) && !defined(DUCKDB_FORCE_ASSERT)

#include <iostream>
#define D_ASSERT(condition) { if(!(condition)){ std::cerr << "ASSERT FAILED: " << #condition << " @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; exit(-1); } }

#else
namespace duckdb {
DUCKDB_API void DuckDBAssertInternal(bool condition, const char *condition_name, const char *file, int linenr);
}

#define D_ASSERT(condition) duckdb::DuckDBAssertInternal(bool(condition), #condition, __FILE__, __LINE__)

#endif
