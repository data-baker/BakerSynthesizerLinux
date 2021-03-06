#ifndef __WEBSOCKET_WRAPPER_H__
#define __WEBSOCKET_WRAPPER_H__
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include "pub_headers.h"
#include "client_listener.h"
#include "synthesizer_manager.h"

#define WSS_URL 1

namespace tts_stream_sdk {
#if WSS_URL
typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::config::asio_tls_client::message_type::ptr message_ptr;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;
#else
typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;
#endif
typedef client::connection_ptr connection_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
class SynthesizerManager;

class WSFrame
{
public:
    WSFrame(std::string version, 
            std::string& text,
            std::string domain,
            std::string language,
            std::string voice_name,
            std::string speed,
            std::string volume,
            std::string pitch,
            std::string audio_type,
            std::string rate,
            std::string spectrum,
            std::string spectrum_8k,
            std::string interval): _version(version),
                                   _text(text),
                                   _domain(domain),
                                   _language(language),
                                   _voice_name(voice_name),
                                   _speed(speed),
                                   _volume(volume),
                                   _pitch(pitch),
                                   _audio_type(audio_type),
                                   _rate(rate),
                                   _spectrum(spectrum),
                                   _spectrum_8k(spectrum_8k),
                                   _interval(interval) {};
    virtual ~WSFrame() {};

public:
    std::string      _version;
    std::string      _text;
    std::string      _domain;
    std::string      _language;
    std::string      _voice_name;
    std::string      _speed;
    std::string      _volume;
    std::string      _pitch;
    std::string      _audio_type;
    std::string      _rate;
    std::string      _spectrum;
    std::string      _spectrum_8k;
    std::string      _interval;
};

class WSFrameManager
{
public:
    WSFrameManager(SynthesizerManager* synthesizer_manager, boost::shared_ptr<ClientListener> client_listener, std::string client_id, std::string secret, std::string server_url);
    virtual ~WSFrameManager();
    std::string genJsonRequest(boost::shared_ptr<WSFrame> ws_request_frame);
    void sendRequestFrame(websocketpp::connection_hdl hdl, boost::shared_ptr<WSFrame> ws_request_frame);
    void procWSRequestFrame(boost::shared_ptr<WSFrame> ws_frame);
//    void procWSRequestFrameAgain(boost::shared_ptr<WSFrame> ws_frame);
    void recvAudioFrame(const std::string& payload);
    bool openConnection(std::string& uri);
    void closeConnection();
    void stopIOService();
//    void makeConnectionThread();
    void taskMonitorThread();
    void setToken(std::string& token)
    {
        boost::lock_guard<boost::mutex> lock(_token_mutex);
        _token = token;
    };
    void getToken(std::string& token)
    {
        boost::lock_guard<boost::mutex> lock(_token_mutex);
        token = _token;
    }
    bool isTokenEmpty()
    {
        boost::lock_guard<boost::mutex> lock(_token_mutex);
        return _token.empty();
    };
    void clearToken()
    {
        boost::lock_guard<boost::mutex> lock(_token_mutex);
        _token.clear();
    };
    void taskFailed(uint32_t error_code);
#if WSS_URL
    context_ptr on_tls_init(websocketpp::connection_hdl);
#endif
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_fail(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
private:
    void getToken();

public:
    std::string                        _client_id;
    std::string                        _secret;
    std::string                        _server_url;
    boost::shared_ptr<ClientListener>   _client_listener;
    client                              _ws_client;
    websocketpp::connection_hdl         _hdl;
    boost::mutex                        _hdl_mutex;

    boost::shared_ptr<WSFrame>          _ws_request_frame;
    boost::mutex                        _request_frame_mutex;

    std::list<WSFrame>                  _ws_audio_frame;
    boost::shared_mutex                 _frame_list_mutex;
    std::string                         _token;
    boost::mutex                        _token_mutex;

    volatile bool                       _is_task_running;
    volatile uint64_t                   _task_start_time_s;
    boost::mutex                        _update_time_mutex;
    boost::thread*                      _task_monitor_thread;
    volatile bool                       _keep_monitor_thread_running;
//    volatile bool                      _need_reprocess_request;
    volatile bool                      _stop_geting_token;
};

}
#endif
