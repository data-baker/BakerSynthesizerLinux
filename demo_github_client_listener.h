#include <sstream>
#include <cstdio>
#include "client_listener.h"

using namespace std;

#define SAVE_AUDIO 1

class MyClientListener: public tts_stream_sdk::ClientListener
{
public:
    MyClientListener()
    {
#if SAVE_AUDIO
        _fp = NULL;
#endif
    };
    virtual ~MyClientListener() {};
    void onTaskStarted()
    {
        onLog("Notice: onTaskStarted called. start task.");
#if SAVE_AUDIO
        struct timeval tv;
        gettimeofday(&tv, NULL);
        time_t sec = tv.tv_sec;
        tm tm = *localtime((const time_t*)&sec);
        ostringstream file_name("./audio/");
        file_name << (1900 + tm.tm_year) << "-"
                  << (1 + tm.tm_mon)     << "-"
                  << tm.tm_mday          << "_"
                  << tm.tm_hour          << "-"
                  << tm.tm_min           << "-"
                  << tm.tm_sec           << "."
                  << tv.tv_usec          << ".pcm";
        
        _fp = fopen(file_name.str().c_str(), "wb+");
        if(!_fp)
        {
            onLog("Error: open file aaa.pcm failed.");
        }
#endif
    }
    void onBinaryReceived(uint32_t idx, std::string audio_data, std::string audio_type, std::string interval)
    {
        std::ostringstream oss;
        oss << "Notice: onBinaryReceived called. audio frame info. "
            << "idx["               << idx
            << "], audio_data len[" << audio_data.length()
            << "], audio_type["     << audio_type
            << "], interval["       << interval
            << "]";
        onLog(oss.str());
#if SAVE_AUDIO
        if(_fp)
        {
            fwrite(audio_data.c_str(), 1, audio_data.length(), _fp);
        }
#endif
    }
    void onTaskCompleted()
    {
        onLog("Notice: onTaskCompleted called. task completed.");
#if SAVE_AUDIO
        if(_fp)
        {
            fclose(_fp);
            _fp = NULL;
        }
#endif
    }
    void onTaskFailed(uint32_t error_code, std::string info, std::string trace_id)
    {
        std::ostringstream oss;
        oss << "Notice: onTaskFailed called. task failed. "
            << "error_code:[" << error_code
            << "], info["     << info
            << "], trace_id[" << trace_id
            << "]";
        onLog(oss.str());
#if SAVE_AUDIO
        if(_fp)
        {
            fclose(_fp);
            _fp = NULL;
        }
#endif
    }
    void onLog(std::string log)
    {
        ostringstream oss;
        oss << __FILE__ << ":" << __LINE__ << "]";
        oss << log;
        fprintf(stderr, "%s\n", oss.str().c_str());
    }
#if SAVE_AUDIO
private:
    FILE*           _fp;
#endif

};


