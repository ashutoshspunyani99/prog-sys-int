#ifndef OatppUtils_hpp
#define OatppUtils_hpp

#include "dto/ResponseDTO.hpp"
#include "oatpp/Types.hpp"
#include "oatpp/web/protocol/http/Http.hpp"
#include <vector>

namespace OatppUtils {
oatpp::Vector<oatpp::Int32> toOatppVector(const std::vector<int>& input);
oatpp::web::protocol::http::Status getHttpStatus(int code);

}

template <typename T>
using ResponseWrapper = std::pair<oatpp::Object<ResponseDataDto<T>>,
                                  oatpp::web::protocol::http::Status>;

#endif // OatppUtils_hpp
