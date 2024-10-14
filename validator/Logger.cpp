#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

class Logger {
public:
    Logger(const std::string &filename) {
        log_file.open(filename, std::ios_base::app);
    }

    ~Logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void log_action(const std::string &event, const std::string &message) {
        log("ACTION", event, message);
    }

    void log_event(const std::string &event, const std::string &message) {
        log("EVENT", event, message);
    }

    void log_condition(const std::string &event, const std::string &message) {
        log("CONDITION", event, message);
    }

// private:
    std::ofstream log_file;

    void log(const std::string &type, const std::string &event, const std::string &message) {
        std::string timestamp = get_current_timestamp();
        log_file << "timestamp=" << timestamp
                 << ", type=" << type
                 << ", event=" << event
                 << ", message='" << message << "'"
                 << std::endl;
    }

    std::string get_current_timestamp() {
        std::time_t now = std::time(nullptr);
        char buf[100];
        std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", std::gmtime(&now));
        return buf;
    }

    void log(const std::string &type, const std::string &scenario, const std::string &event, const std::string &message) {
    std::string timestamp = get_current_timestamp();
    log_file << "timestamp=" << timestamp 
             << ", scenario=" << scenario 
             << ", type=" << type 
             << ", event=" << event 
             << ", message='" << message << "'" 
             << std::endl;
}
};

int main() {
    Logger logger("game_logs.txt");

// Example usage
logger.log("ACTION", "BONUS_GAME", "BONUS_TRIGGER", "Bonus game triggered by player");
logger.log("EVENT", "BONUS_GAME", "BONUS_START", "Bonus game started");
    // Simulate a game scenario
    logger.log_action("BET_PLACED", "Player placed a bet of 100 credits");
    logger.log_condition("SUFFICIENT_BET_CHECK", "Sufficient bet placed: true");
    logger.log_action("BUTTON_PRESS", "Deal button pressed by player");
    logger.log_event("STATE_CHANGE", "State changed from BETTING to DEALING");
    logger.log_action("CREDIT_UPDATE", "Player credits decreased by 100, new total: 900");
    logger.log_condition("GLOBAL_SETTING_CHECK", "Sounds enabled: true");
    logger.log_event("SOUND_PLAY", "Sound played: card_deal.wav");
    logger.log_condition("PREVIOUS_ROUND_CHECK", "Player won previous round: true");
    logger.log_event("ANIMATION_START", "Animation started: deal_cards_animation");

    return 0;
}

