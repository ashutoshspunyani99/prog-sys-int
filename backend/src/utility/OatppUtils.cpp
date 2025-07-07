#include "OatppUtils.hpp"

namespace OatppUtils {
/// @brief Converts a vector of integers to an oatpp Vector of Int32.
oatpp::Vector<oatpp::Int32> toOatppVector(const std::vector<int>& input) {
  auto output = oatpp::Vector<oatpp::Int32>::createShared();
  for (int val : input) {
    output->push_back(val);
  }
  return output;
}

/// @brief Converts an oatpp Vector of Int32 to a vector of integers.
oatpp::web::protocol::http::Status getHttpStatus(int code) {
    using oatpp::web::protocol::http::Status;
    switch(code) {
        case 200: return Status::CODE_200;
        case 201: return Status::CODE_201;
        case 204: return Status::CODE_204;
        case 400: return Status::CODE_400;
        case 401: return Status::CODE_401;
        case 403: return Status::CODE_403;
        case 404: return Status::CODE_404;
        case 409: return Status::CODE_409;
        case 500: return Status::CODE_500;
        default:  return Status::CODE_500; 
    }
}

}



