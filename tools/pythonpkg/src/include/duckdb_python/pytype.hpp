#pragma once

#include "duckdb_python/pybind_wrapper.hpp"
#include "duckdb/common/types.hpp"

namespace duckdb {

class PyGenericAlias : public py::object {
public:
	using py::object::object;

public:
	static bool check_(const py::handle &object);
};

class PyUnionType : public py::object {
public:
	using py::object::object;

public:
	static bool check_(const py::handle &object);
};

class DuckDBPyType : public std::enable_shared_from_this<DuckDBPyType> {
public:
	explicit DuckDBPyType(LogicalType type);

public:
	static void Initialize(py::handle &m);

public:
	bool Equals(const shared_ptr<DuckDBPyType> &other) const;
	bool EqualsString(const string &type_str) const;
	shared_ptr<DuckDBPyType> GetAttribute(const string &name) const;
	string ToString() const;
	const LogicalType &Type() const;

private:
private:
	LogicalType type;
};

} // namespace duckdb
