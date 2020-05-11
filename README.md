# 标贝科技语音合成服务Linux SDK使用文档（2.0）

## 链接lib（参考demo_github） 

## 关键类及方法

所有涉及的字符均使用utf-8编码

### SynthesizerManager：语音合成业务处理类。

类成员方法：

| 方法                | 参数说明                      | 说明                    |
| ------------------- | ----------------------------- | ----------------------- |
| int  init(…)        | string  client_id             | 从标贝获取的client id   |
|                     | string secret                 | 从标贝获取的secret      |
|                     | string server_url             | 合成服务的WebSocket地址 |
| int  setListener(…) | ClientListener*  listener     | 设置回调类              |
| int  startTask(…)   | SynthesizerParams&  tts_param | 设置合成参数，进行合成  |
| int  stopTask()     | 无参数                        | 停止当前合成任务        |
| void  uninit()      | 无参数                        | 反初始化                |

 

### ClientListener：合成结果回调类。

​    在获得合成音频数据，发生错误等事件发生时会触发回调。您应当实现该类，在回调方法中加入自己的处理逻辑。

类成员方法：

| 方法                      | 参数                   | 说明                                     |
| ------------------------- | ---------------------- | ---------------------------------------- |
| void  onTaskStarted()     | 无参数                 |                                          |
| void  onBinaryReceived(…) | uint32_t  idx          | 音频数据包的序号，从1开始递增            |
|                           | string      audio_data | 二进制音频数据                           |
|                           | string      audio_type | 音频格式                                 |
|                           | string      interval   | 时间戳信息                               |
| void  onTaskCompleted()   | 无参数                 |                                          |
| void  onTaskFailed()      | uint32_t  code         | 错误码                                   |
|                           | string      info       | 提示信息                                 |
|                           | string      trace_id   | 跟踪id，偶现忽略，重复出现反馈给开发人员 |
| void  onLog               | string  log            | sdk内部生成的日志，默认打印到终端        |

 

### SynthesizerParams合成参数类。

类成员变量：

| 变量名        | 类型     | 是否必须赋值 | 说明                                                         |
| ------------- | -------- | ------------ | ------------------------------------------------------------ |
| text          | string   | 是           | 合成的文本，utf-8编码，长度限制为250个汉字，即750字节。      |
| voice_name    | string   | 是           | 合成发音人，例如：静静，邻家女声  等，utf-8编码<br />默认：标准合成_模仿儿童_果子 |
| language      | string   | 否           | 合成请求文本的语言，目前支持 <br />ZH(中文和中英混)<br />ENG(纯英文，中文部分不会合成)<br />默认：ZH |
| speed         | string   | 否           | 设置播放的语速，在0～9之间（支持浮点值），<br />默认值为”5”  |
| volume        | uint32_t | 否           | 设置语音的音量，在0～9之间（只支持整型值），<br />默认值为5  |
| pitch         | string   | 否           | 设置语音的音调，取值0-9（支持浮点值），<br />默认值为”5”     |
| audio_type    | uint32_t | 否           | 设置合成音频的格式<br />audiotype=4 ：返回16K采样率的pcm格式<br />audiotype=5 ：返回8K采样率的pcm格式<br />audiotype=6 ：返回16K采样率的wav格式<br />audiotype=6&rate=1  ：返回8K的wav格式<br />默认值为4 |
| rate          | uint32_t | 否           | 配合audio_type使用                                           |
| spectrum      | uint32_t | 否           | 取值范围0~20，<br />0代表使用配置文件tts_attention.conf中spec_adjust_d的值；<br />1代表不调整频谱；<br />1以上的值代表高频能量增加幅度，值越大声音的高频部分增强越多，听起来更亮和尖细<br />默认为1，不调整频谱； |
| spectrum_8k   | uint32_t | 否           | 取值范围0~20，<br />默认为0，仅针对8K音频频谱的调整。        |
| need_interval | bool     | 否           | 设置是否返回interval，<br />true：需要返回;<br />false：不需要返回。<br />默认为false |

 

## 调用顺序

### 顺序描述

1 初始化

2 设置回调handler

3 创建一个参数类变量

4 开始合成任务

5 回调返回音频数据或错误信息

6 反初始化（注意：该版本为非阻塞版本，请确保业务已完全处理完毕，否则任务会被终止）

如果需要开始新的合成请求，重复1-6步骤即可；

### 方法调用顺序

 SynthesizerManager::getInstance()->init(string client_id, string secret);

 SynthesizerManager::getInstance()->setListener(ClientListener* listener);

 SynthesizerParams tts_param();

 tts_param.text = “…”;

 tts_param.voice_name = “…”; //如不设置，则使用默认值

 … //其他合成参数设置

 SynthesizerManager::getInstance()->startTask(tts_param);

 … //触发回调

 SynthesizerManager::getInstance()->uninit();（注意：该版本为非阻塞版本，请确保业务已完全处理完毕，否则任务会被终止）

 

## 错误码

### 合成失败code码：

| 错误码 | 含义                                       |
| ------ | ------------------------------------------ |
| 90000  | 合成成功                                   |
| 90001  | 任务失败，网络连接失败                     |
| 90002  | 任务失败，返回json解析失败                 |
| 90003  | 任务失败，返回结果缺少必要字段             |
| 90004  | 任务失败，任务超时                         |
| 90005  | 任务失败，其他错误                         |
|        |                                            |
| 10001  | access_token参数获取失败或未传输           |
| 10002  | domain参数值错误                           |
| 10003  | language参数错误                           |
| 10004  | voice_name参数错误                         |
| 10005  | audiotype参数错误                          |
| 10006  | rate参数错误                               |
| 10007  | idx错误                                    |
| 10008  | single错误                                 |
| 10009  | text参数错误                               |
| 10010  | 文本太长                                   |
| 20000  | 取资源错误                                 |
| 20001  | 断句失败                                   |
| 20002  | 分段数错误                                 |
| 20003  | 分段后的文本长度错误                       |
| 20004  | 获取引擎链接错误                           |
| 20005  | RPC链接失败错误                            |
| 20006  | 引擎内部错误                               |
| 20007  | 操作redis错误                              |
| 20008  | 音频编码错误                               |
| 30000  | 鉴权错误（access_token值不正确或已经失效） |
| 30001  | 并发错误                                   |
| 30002  | 内部配置错误                               |
| 30003  | json串解析错误                             |
| 30004  | 获取url失败                                |
| 30005  | 获取客户IP地址失败                         |
| 30006  | 任务队列错误                               |
| 40001  | 请求不是json结构                           |
| 40002  | 缺少必须字段                               |
| 40003  | 版本错误                                   |
| 40004  | 字段值类型错误                             |
| 40005  | 参数错误                                   |
| 50001  | 处理超时                                   |
| 50002  | 内部rpc调用失败                            |
| 50004  | 其他内部错误                               |