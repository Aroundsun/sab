#ifndef __SAB_LOG_H_
#define __SAB_LOG_H_
#include<iostream>
#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include<fstream>
namespace sab
{
    // 日志事件
    class LogEvent
    {
    public:
        typedef std::shared_ptr<LogEvent> ptr;
        LogEvent();

    private:
        const char *m_file = nullptr; // 文件名
        int32_t m_line = 0;           // 行号
        u_int32_t m_elapse = 0;       // 程序启动开始到现在的毫秒数
        uint32_t m_threadId = 0;      // 线程号
        uint32_t m_fiberId = 0;       // 协程号
        uint32_t m_time;              // 时间戳
        std::string m_content;        //
    };

    // 日志级别
    class LogLevel
    {
    public:
        enum Level
        {
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5

        };

    private:
    };

    // 日志格式器
    class LogFormatter
    {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        std::string format(LogEvent::ptr event);

    private:  
    };

    // 日志输出地
    class LogAppender
    {
    public:
        typedef std::shared_ptr<LogAppender> ptr;
        virtual ~LogAppender() {};
        virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;
        void setFormatter(LogFormatter::ptr val){m_formatter = val;}
        LogFormatter::ptr getFormatter(){return m_formatter;}

    protected:
        LogLevel::Level m_level;
        LogFormatter::ptr m_formatter;
    };
    // 日志器
    class Logger
    {
    public:
        typedef std::shared_ptr<Logger> ptr;
        Logger(const std::string &name = "root");

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);

        void log(LogLevel::Level level, LogEvent::ptr event);

        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void error(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);

        void setLevel(LogLevel::Level val){m_level =val;}
        LogLevel::Level getLevel()const{return m_level;}

    private:
        std::string m_name;                      // 日志名称
        LogLevel::Level m_level;                 // 日志级别
        std::list<LogAppender::ptr> m_appenders; // appender集合
    };
    // 输出到控制台
    class StdoutLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;

        virtual void log(LogLevel::Level level, LogEvent::ptr event) override;
    };
    // 输出到文件
    class FileLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;
        FileLogAppender(const std::string& name);
        virtual void log(LogLevel::Level level, LogEvent::ptr event) override;

        //重新打开文件，文件打开成功返回true
        bool reopen();
    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };

};

#endif