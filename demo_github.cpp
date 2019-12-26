#include <cstdlib>
#include <sstream>
#include "synthesizer_manager.h"
#include "demo_github_client_listener.h"

using namespace std;
using namespace tts_stream_sdk;

string client_id = "e2d17fce-f69a-4b78-bbe5-7fef824a77c2";  //向标贝科技申请的client_id
string secret    = "ZTZlOTMyMzAtMThlZS00M2ZjLWJhMTktYTQ2NjBhZTE3Yzk0";  //向标贝科技申请的secret

int main(int argc, char* argv[])
{
    //while(true)  //每次合成请求都需要走一遍以下的流程，类似libcurl的使用。
    {
        boost::shared_ptr<ClientListener> client_listener(new(std::nothrow) MyClientListener());
        SynthesizerManager synthesizer_manager;
        synthesizer_manager.init(client_id, secret);  //client_id或secret不可为空
        synthesizer_manager.setListener(client_listener);

        SynthesizerParams synthesizer_param;
        synthesizer_param._text            = "欢迎使用标贝科技只能语音合成服务";  //text长度限制为250个汉字，且必须为utf-8编码格式
    //    synthesizer_param._speed           = 5.0f;                              //设置播放语速，0 ~ 9（支持浮点数），默认为5
        synthesizer_param._voice_name      = "标准合成_标准女声_瑶瑶";            //发音人
    //    synthesizer_param._pitch           = 5.0f;                              //设置音调，0 ~ 9（支持浮点数），默认为5
    //    synthesizer_param._audio_type      = 4;                                 //设置合成音频格式，4: 16k_pcm, 5:8k_pcm, 6&rate=0: 16k_wav, 6&rate=1: 8k_wav 默认为4
    //    synthesizer_param._rate            = 0;                                 //配合audio_type使用
    //    synthesizer_param._spectrum        = 1;                                 //频谱调整，默认为1。详细信息请参考文档
    //    synthesizer_param._spectrum_8k     = 0;                                 //频谱调整，默认为0。详细信息请参考文档
    //    synthesizer_param._need_interval   = false;                             //是否返回interval信息，默认为false

        client_listener->onLog("Notice: add new task.");
        synthesizer_manager.startTask(synthesizer_param);

        synthesizer_manager.uninit();
    }
}
