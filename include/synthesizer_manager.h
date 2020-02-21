#ifndef __SYNTHESIZER_MANAGER_H__
#define __SYNTHESIZER_MANAGER_H__
#include "pub_headers.h"
#include "client_listener.h"
#include "websocket_wrapper.h"

namespace tts_stream_sdk {

enum SdkCallbackErrorCode
{
    Success_audio_data         = 90000,
    Failed_connection_error    = 90001,
    Failed_parse_json_error    = 90002,
    Failed_json_incomplete     = 90003,
    Failed_task_timeout        = 90004,
    Failed_other_error         = 90005
};

class SynthesizerParams
{
public:
    SynthesizerParams():_domain(1),
                        _language("ZH"),
                        _speed(5.0f),
                        _volume(5),
                        _pitch(5.0f),
                        _audio_type(4),
                        _rate(0),
                        _spectrum(1),
                        _spectrum_8k(0),
                        _need_interval(false) {};
    virtual ~SynthesizerParams() {};

public:
    std::string      _text;
    uint32_t         _domain;
    std::string      _language;
    std::string      _voice_name;
    float            _speed;
    uint32_t         _volume;
    float            _pitch;
    uint32_t         _audio_type;
    uint32_t         _rate;
    uint32_t         _spectrum;
    uint32_t         _spectrum_8k;
    bool             _need_interval;
};

class WSFrame;
class WSFrameManager;

class SynthesizerManager
{
public:
    SynthesizerManager(): _inited(false) {};
    virtual ~SynthesizerManager() { _client_listener.reset(); };

    /********************************************************************************************
	**	@func 初始化，只需一次
	**
	**	@param client_id 从标贝申请的client_id
	**	@param secret    从标贝申请的secret
	**
	**	@return 成功返回0，失败返回-1
	********************************************************************************************/
    int init(std::string client_id, std::string secret, std::string server_url);

    /********************************************************************************************
	**	@func 设置回调指针
	**
	**	@param client_listener 
	**
	**	@return 成功返回0，失败返回-1
	********************************************************************************************/
    int setListener(boost::shared_ptr<ClientListener> client_listener);

    /********************************************************************************************
	**	@func 开始一个合成任务，函数会一直阻塞到任务成功或失败
	**
	**	@param tts_params 合成参数
	**
	**	@return 成功返回0，失败返回-1
	********************************************************************************************/
    int startTask(SynthesizerParams& tts_params);

    /********************************************************************************************
	**	@func 结束当前的合成任务
	**
	**	@param 无
	**
	**	@return 成功返回0，失败返回-1
	********************************************************************************************/
    int stopTask();

    /********************************************************************************************
	**	@func 反初始化，只需一次
	**
	**	@param 无
	**
	**	@return 无
	********************************************************************************************/
    void uninit();

    void setReprocessRequest(boost::shared_ptr<WSFrame> reprocess_request);

public:
    boost::shared_ptr<WSFrameManager>  _ws_frame_manager;
    boost::shared_ptr<ClientListener>  _client_listener;
    boost::mutex                       _api_mutex;
    std::string                        _client_id;
    std::string                        _secret;
    std::string                        _server_url;

private:
    volatile bool                      _inited;
    boost::shared_ptr<WSFrame>         _reprocess_request;
};

}
#endif
