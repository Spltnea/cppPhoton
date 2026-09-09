#ifndef P_LOGGER_HPP
#   define P_LOGGER_HPP

#include <iostream>
#include <string>
#include <ctime>

namespace photon {

    enum class IdPrefix : int {
        PREPROCESSOR_LOG = 0x1000,
        LEXER_LOG        = 0x2000,
        PARSER_LOG       = 0x3000,
        LLVM_EMITTER_LOG = 0x4000
    };

    enum class SeverityPrefix : int {
        INFO = 0x100,
        WARN = 0x200,
        ERR  = 0x300,
    };

    struct LogFrame {
        int id;
        std::string message;
        time_t timestamp;
    };

    /** Provides small utilites to log compiler events when using the command line */
    class pLogger {
    private :
        static int getIdPrefixValueOf(IdPrefix prefix) {
            return static_cast<int>(prefix);
        }

        static int getSeverityPrefixValueOf(SeverityPrefix prefix) {
            return static_cast<int>(prefix);
        }

        static int buildID(IdPrefix id_prefix, SeverityPrefix sev_prefix, int eventCode) {
            return getIdPrefixValueOf(id_prefix) | getSeverityPrefixValueOf(sev_prefix) | eventCode;
        }

    public :
        /** Creates a log frame 
         * @param logType The type of log (PREPROCESSOR, LEXER, PARSER, LLVM_EMITTER)_LOG
         * @param severity The severity (INFO, WARN, ERR)
         * @param eventId The id of the event (0x01 to 0xFF)
         * @param msg The associated msg
         * @return A clean event log to emit using the lprint() function
        */
        static LogFrame 
        buildFrame(IdPrefix logType, SeverityPrefix severity, int eventId, std::initializer_list<std::string> msg);

        /** Prints a log frame to the console */
        static void lprint(const LogFrame& frame);
    };
}

#endif