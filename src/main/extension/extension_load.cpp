#include "duckdb/common/dl.hpp"
#include "duckdb/common/virtual_file_system.hpp"
#include "duckdb/main/extension_helper.hpp"
#include "duckdb/main/error_manager.hpp"
#include "mbedtls_wrapper.hpp"

namespace duckdb {

//===--------------------------------------------------------------------===//
// Load External Extension
//===--------------------------------------------------------------------===//
typedef void (*ext_init_fun_t)(DatabaseInstance &);
typedef const char *(*ext_version_fun_t)(void);
typedef void (*ext_storage_init_t)(DBConfig &);

template <class T>
static T LoadFunctionFromDLL(void *dll, const string &function_name, const string &filename) {
	throw std::runtime_error("FUNCTION NOT SUPPORTED BY OE");

	// auto function = dlsym(dll, function_name.c_str());
	// if (!function) {
	// 	throw IOException("File \"%s\" did not contain function \"%s\": %s", filename, function_name, GetDLError());
	// }
	// return (T)function;
}

bool ExtensionHelper::TryInitialLoad(DBConfig &config, FileOpener *opener, const string &extension,
                                     ExtensionInitResult &result, string &error) {
	throw std::runtime_error("duckdb::ExtensionHelper::TryInitialLoad() is not supported by Open Enclave");
}

ExtensionInitResult ExtensionHelper::InitialLoad(DBConfig &config, FileOpener *opener, const string &extension) {
	string error;
	ExtensionInitResult result;
	if (!TryInitialLoad(config, opener, extension, result, error)) {
		throw IOException(error);
	}
	return result;
}

bool ExtensionHelper::IsFullPath(const string &extension) {
	return StringUtil::Contains(extension, ".") || StringUtil::Contains(extension, "/") ||
	       StringUtil::Contains(extension, "\\");
}

string ExtensionHelper::GetExtensionName(const string &extension) {
	if (!IsFullPath(extension)) {
		return extension;
	}
	auto splits = StringUtil::Split(StringUtil::Replace(extension, "\\", "/"), '/');
	if (splits.empty()) {
		return extension;
	}
	splits = StringUtil::Split(splits.back(), '.');
	if (splits.empty()) {
		return extension;
	}
	return StringUtil::Lower(splits.front());
}

void ExtensionHelper::LoadExternalExtension(DatabaseInstance &db, FileOpener *opener, const string &extension) {
	if (db.ExtensionIsLoaded(extension)) {
		return;
	}

	auto res = InitialLoad(DBConfig::GetConfig(db), opener, extension);
	auto init_fun_name = res.basename + "_init";

	ext_init_fun_t init_fun;
	init_fun = LoadFunctionFromDLL<ext_init_fun_t>(res.lib_hdl, init_fun_name, res.filename);

	try {
		(*init_fun)(db);
	} catch (std::exception &e) {
		throw InvalidInputException("Initialization function \"%s\" from file \"%s\" threw an exception: \"%s\"",
		                            init_fun_name, res.filename, e.what());
	}

	db.SetExtensionLoaded(extension);
}

void ExtensionHelper::LoadExternalExtension(ClientContext &context, const string &extension) {
	LoadExternalExtension(DatabaseInstance::GetDatabase(context), FileSystem::GetFileOpener(context), extension);
}

void ExtensionHelper::StorageInit(string &extension, DBConfig &config) {
	extension = ExtensionHelper::ApplyExtensionAlias(extension);
	ExtensionInitResult res;
	string error;
	if (!TryInitialLoad(config, nullptr, extension, res, error)) {
		if (!ExtensionHelper::AllowAutoInstall(extension)) {
			throw IOException(error);
		}
		// the extension load failed - try installing the extension
		if (!config.file_system) {
			throw InternalException("Attempting to install an extension without a file system");
		}
		ExtensionHelper::InstallExtension(config, *config.file_system, extension, false);
		// try loading again
		if (!TryInitialLoad(config, nullptr, extension, res, error)) {
			throw IOException(error);
		}
	}
	auto storage_fun_name = res.basename + "_storage_init";

	ext_storage_init_t storage_init_fun;
	storage_init_fun = LoadFunctionFromDLL<ext_storage_init_t>(res.lib_hdl, storage_fun_name, res.filename);

	try {
		(*storage_init_fun)(config);
	} catch (std::exception &e) {
		throw InvalidInputException(
		    "Storage initialization function \"%s\" from file \"%s\" threw an exception: \"%s\"", storage_fun_name,
		    res.filename, e.what());
	}
}

string ExtensionHelper::ExtractExtensionPrefixFromPath(const string &path) {
	auto first_colon = path.find(':');
	if (first_colon == string::npos || first_colon < 2) { // needs to be at least two characters because windows c: ...
		return "";
	}
	auto extension = path.substr(0, first_colon);

	if (path.substr(first_colon, 3) == "://") {
		// these are not extensions
		return "";
	}

	D_ASSERT(extension.size() > 1);
	// needs to be alphanumeric
	for (auto &ch : extension) {
		if (!isalnum(ch) && ch != '_') {
			return "";
		}
	}
	return extension;
}

} // namespace duckdb
