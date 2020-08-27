#include <cstdlib>
#include <sstream>
#include "synthesizer_manager.h"
#include "demo_github_client_listener.h"

using namespace std;
using namespace tts_stream_sdk;

string server_url = "wss://openapi.data-baker.com/wss";
//string server_url = "ws://192.168.1.21:9003";

string client_id = "";  //向标贝科技申请的client_id
string secret    = "";  //向标贝科技申请的secret

//这是一个非阻塞版本的测试程序
int main(int argc, char* argv[])
{
    //while(true)  //每次合成请求都需要走一遍以下的流程，类似libcurl的使用。
    {
        boost::shared_ptr<ClientListener> client_listener(new(std::nothrow) MyClientListener());
        SynthesizerManager synthesizer_manager;
        synthesizer_manager.init(client_id, secret, server_url);  //client_id, secret和server_url均不可为空
        synthesizer_manager.setListener(client_listener);

        SynthesizerParams synthesizer_param;
 //       synthesizer_param._text            = "欢迎使用标贝科技只能语音合成服务";                  //text长度限制为250个汉字，且必须为utf-8编码格式
        synthesizer_param._text            = "新闻，在中国古代又称新文，近代有时泛指报纸，在日语及韩语汉字中则只有报纸一义。通常指新闻机构发布的最近发生事件的消息报道。";                  //text长度限制为250个汉字，且必须为utf-8编码格式
        synthesizer_param._speed           = 5.0f;                              //设置播放语速，0 ~ 9（支持浮点数），默认为5
        synthesizer_param._voice_name      = "标准合成_标准女声_瑶瑶";                      //发音人，请确保指定的clientid支持该voice_name
        synthesizer_param._pitch           = 5.0f;                              //设置音调，0 ~ 9（支持浮点数），默认为5
        synthesizer_param._audio_type      = 5;                                 //设置合成音频格式，4: 16k_pcm, 5:8k_pcm
        synthesizer_param._volume          = 5;                                 //设置音量
    //    synthesizer_param._rate            = 0;                                 //配合audio_type使用
    //    synthesizer_param._spectrum        = 1;                                 //频谱调整，默认为1。详细信息请参考文档
    //    synthesizer_param._spectrum_8k     = 0;                                 //频谱调整，默认为0。详细信息请参考文档
    //    synthesizer_param._need_interval   = false;                             //是否返回interval信息，默认为false

        client_listener->onLog(LogLevel_notice, "Notice: add new task.");
        synthesizer_manager.startTask(synthesizer_param);
        getchar();  //阻塞一下，不然程序就退出了
        synthesizer_manager.uninit();
    }
}
