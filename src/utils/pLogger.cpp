#include "utils/pLogger.hpp"

namespace photon {
    LogFrame pLogger::buildFrame(IdPrefix logType, SeverityPrefix severity, int eventId, std::initializer_list<std::string> msg) {
        int fullId = buildID(logType, severity, eventId);
        std::string message = "";
        for (auto str : msg) {
            message += str;
        }
        return LogFrame {fullId, message, time(nullptr)};
    }

    void pLogger::lprint(const LogFrame& frame) {
        std::cout << "[ " 
                  <<  ctime(&frame.timestamp) 
                  << "- (ID: 0x" << std::hex << frame.id << ") ] : " 
                  << std::dec 
                  << frame.message 
                  << '\n';
    }
} // namespace photon