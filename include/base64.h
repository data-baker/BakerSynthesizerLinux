#include <string>

namespace tts_stream_sdk {

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

}

