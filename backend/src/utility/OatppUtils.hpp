#ifndef OatppUtils_hpp
#define OatppUtils_hpp

#include "oatpp/Types.hpp"
#include <vector>

namespace OatppUtils {
oatpp::Vector<oatpp::Int32> toOatppVector(const std::vector<int>& input);
oatpp::web::protocol::http::Status getHttpStatus(int code);

}

#endif // OatppUtils_hpp
