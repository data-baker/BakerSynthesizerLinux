#ifndef __CLIENT_LISTENER_H__
#define __CLIENT_LISTENER_H__

namespace tts_stream_sdk {
enum LogLevel
{
    LogLevel_notice   = 0,
    LogLevel_warning  = 1,
    LogLevel_error    = 2
};
class ClientListener
{

public:
    ClientListener() {};
    virtual ~ClientListener() {};
    virtual void onTaskStarted()
    {
        onLog(LogLevel_notice, "Notice: onTaskStarted called. start task.");
    }
    virtual void onBinaryReceived(uint32_t idx, std::string& audio_data, std::string& audio_type, std::string& interval)
    {
        std::ostringstream oss;
        oss << "Notice: onBinaryReceived called. audio frame info. "
            << "idx["               << idx
            << "], audio_data len[" << audio_data.length()
            << "], audio_type["     << audio_type
            << "], interval["       << interval
            << "]";
        onLog(LogLevel_notice, oss.str());
    }
    virtual void onTaskCompleted()
    {
        onLog(LogLevel_notice, "Notice: onTaskCompleted called. task completed.");
    }
    virtual void onTaskFailed(uint32_t error_code, std::string& info, std::string& trace_id)
    {
        std::ostringstream oss;
        oss << "Notice: onTaskFailed called. task failed. "
            << "error_code:[" << error_code
            << "], info["     << info
            << "], trace_id[" << trace_id
            << "]";
        onLog(LogLevel_notice, oss.str());
    }
    virtual void onLog(LogLevel log_level, std::string log)
    {
        fprintf(stderr, "%s\n", log.c_str());
    }
};
}

#endif
