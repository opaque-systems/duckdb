#include "duckdb/function/scalar/string_functions.hpp"
#include "duckdb/function/scalar/blob_functions.hpp"
#include "duckdb/function/scalar/uuid_functions.hpp"
#include "duckdb/function/scalar/bit_functions.hpp"

namespace duckdb {

void BuiltinFunctions::RegisterStringFunctions() {
	Register<ReverseFun>();
	Register<LowerFun>();
	Register<UpperFun>();
	Register<StripAccentsFun>();
	Register<ConcatFun>();
	Register<StartsWithFun>();
	Register<ContainsFun>();
	Register<LengthFun>();
	Register<LikeFun>();
	Register<LikeEscapeFun>();
	Register<LpadFun>();
	Register<LeftFun>();
	Register<MD5Fun>();
	Register<RightFun>();
	Register<PrintfFun>();
	Register<RegexpFun>();
	Register<SubstringFun>();
	Register<InstrFun>();
	Register<PrefixFun>();
	Register<RepeatFun>();
	Register<ReplaceFun>();
	Register<RpadFun>();
	Register<TranslateFun>();
	Register<SuffixFun>();
	Register<TrimFun>();
	Register<UnicodeFun>();
	Register<NFCNormalizeFun>();
	Register<StringSplitFun>();
	Register<BarFun>();
	Register<ASCII>();
	Register<CHR>();
	Register<MismatchesFun>();
	Register<LevenshteinFun>();
	Register<DamerauLevenshteinFun>();
	Register<JaccardFun>();
	Register<JaroWinklerFun>();

	// blob functions
	Register<Base64Fun>();
	Register<EncodeFun>();
	Register<HexFun>();

	// bit functions
	Register<GetBitFun>();
	Register<SetBitFun>();
	Register<BitPositionFun>();
	Register<BitStringFun>();

	// uuid functions
	Register<UUIDFun>();
}

} // namespace duckdb
